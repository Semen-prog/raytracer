#include "LibTrace/tracing.hpp"

#include "LibTrace/shapes/sphere.hpp"
#include "LibTrace/shapes/cube.hpp"

#include "LibTrace/materials/lamberitan.hpp"
#include "LibTrace/materials/metal.hpp"
#include "LibTrace/materials/light.hpp"
#include "LibTrace/materials/glass.hpp"

#include "LibTrace/textures/solid.hpp"
#include "LibTrace/textures/checker.hpp"

#include <QJsonArray>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QJsonDocument>
#include <QImageWriter>

QSharedPointer<Shape> (*shape_parsers[])(const QJsonObject&) = {
    Sphere::parse_json,
    Cube::parse_json
};

QSharedPointer<Material> (*material_parsers[])(const QJsonObject&) {
    Lamberitan::parse_json,
    Metal::parse_json,
    Light::parse_json,
    Glass::parse_json
};

QSharedPointer<Texture> (*texture_parsers[])(const QJsonObject&) {
    Solid::parse_json,
    Checker::parse_json
};

QSharedPointer<Shape> parse_json_shape(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type", 0, 1);
    return shape_parsers[type](obj);
}

QSharedPointer<Material> parse_json_material(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type", 0, 3);
    return material_parsers[type](obj);
}

QSharedPointer<Texture> parse_json_texture(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type", 0, 1);
    return texture_parsers[type](obj);
}

QSharedPointer<FigureList> parse_json_figurelist(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isArray()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonArray arr = json[name].toArray();
    auto fl = QSharedPointer<FigureList>::create();
    for (int i = 0; i < arr.size(); ++i) {
        if (!arr.at(i).isObject()) {
            std::cerr << "Error: figure is not an object\n";
            exit(127);
        }
        QJsonObject obj = arr.at(i).toObject();
        fl->add(parse_json_shape(obj, "shape"), parse_json_material(obj, "material"), parse_json_texture(obj, "texture"));
    }
    return fl;
}

QSharedPointer<Viewport> parse_json_viewport(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    return QSharedPointer<Viewport>::create(
        parse_json_int(obj, "pixel_width", 1),
        parse_json_int(obj, "pixel_height", 1),
        radian(parse_json_double(obj, "field_of_view", 0, 180)),
        parse_json_double(obj, "x_offset", 0),
        parse_json_double(obj, "y_offset", 0),
        parse_json_pv(obj, "lookfrom"),
        parse_json_pv(obj, "lookat"),
        parse_json_pv(obj, "dir_up"),
        radian(parse_json_double(obj, "defocus_angle", 0, 180)));
}

QPair<QSharedPointer<Scene>, int> parse_json_scene(const QJsonObject &json) {
    return QPair(
        QSharedPointer<Scene>::create(parse_json_figurelist(json, "figure_list"),
                              parse_json_viewport(json, "viewport"),
                              parse_json_int(json, "samples_per_pixel", 1),
                              parse_json_int(json, "max_depth", 1)),
        parse_json_int(json, "hittype", 0, 1));
}

QJsonObject open_config(const QString &config_name) {
    QFile file;
    file.setFileName(config_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: cannot open file " << config_name.toStdString() << '\n';
        exit(127);
    }
    QString val = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError) {
        std::cerr << "Error: parse error: " << err.errorString().toStdString() << '\n';
        exit(127);
    }
    if (!doc.isObject()) {
        std::cerr << "Error: doc is not an object\n";
        exit(127);
    }
    return doc.object();
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("cmd-raytracer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Build pucture from config using raytracer engine");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("config", QCoreApplication::translate("main", "Config file with scene structure."));

    QString pf = QImageWriter::supportedImageFormats().join("/");
    QCommandLineOption format_option(QStringList() << "o" << "output",
                                     QCoreApplication::translate("main", QString("Output file name (suffix must be one of %1, default img.png)").arg(pf).toStdString().c_str()));
    parser.addOption(format_option);

    parser.process(app);
    
    QStringList args = parser.positionalArguments();
    if (args.empty()) {
        std::cerr << "Error: no config provided\n";
        exit(127);
    }

    QString output_name = "img.png";
    if (parser.isSet(format_option)) {
        output_name = parser.value(format_option);
    }

    QString config_name = args.at(0);
    auto [scene, ht] = parse_json_scene(open_config(config_name));
    if (!scene->render((Hittype)ht).save(output_name)) {
        std::cerr << "Error: failed to save result to " << output_name.toStdString() << " (maybe wrong suffix?)\n";
        exit(127);
    }
}