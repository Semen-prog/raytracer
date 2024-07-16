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

std::shared_ptr<Shape> (*shape_parsers[])(const QJsonObject&) = {
    Sphere::parse_json,
    Cube::parse_json
};

std::shared_ptr<Material> (*material_parsers[])(const QJsonObject&) {
    Lamberitan::parse_json,
    Metal::parse_json,
    Light::parse_json,
    Glass::parse_json
};

std::shared_ptr<Texture> (*texture_parsers[])(const QJsonObject&) {
    Solid::parse_json,
    Checker::parse_json
};

std::shared_ptr<Shape> parse_json_shape(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type", 0, 1);
    return shape_parsers[type](obj);
}

std::shared_ptr<Material> parse_json_material(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type", 0, 3);
    return material_parsers[type](obj);
}

std::shared_ptr<Texture> parse_json_texture(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    int type = parse_json_int(obj, "type", 0, 1);
    return texture_parsers[type](obj);
}

std::shared_ptr<FigureList> parse_json_figurelist(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isArray()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonArray arr = json[name].toArray();
    auto fl = std::make_shared<FigureList>();
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

std::shared_ptr<Viewport> parse_json_viewport(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    return std::make_shared<Viewport>(
        parse_json_int(obj, "pixel_width", 1),
        parse_json_int(obj, "pixel_height", 1),
        radian(parse_json_double(obj, "field_of_view", 0, 180)),
        parse_json_double(obj, "x_offset", 0),
        parse_json_double(obj, "y_offset", 0),
        parse_json_pv(obj, "lookfrom"),
        parse_json_pv(obj, "lookat"),
        parse_json_pv(obj, "dir_up"),
        radian(parse_json_double(obj, "defocus_angle", 0, 180))
    );
}

std::pair<std::shared_ptr<Scene>, int> parse_json_scene(const QJsonObject &json) {
    return std::make_pair(
        std::make_shared<Scene>(parse_json_figurelist(json, "figure_list"),
                                parse_json_viewport(json, "viewport"),
                                parse_json_int(json, "samples_per_pixel", 1),
                                parse_json_int(json, "max_depth", 1)),
        parse_json_int(json, "hittype", 0, 1)
    );
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

    parser.process(app);
    
    QStringList args = parser.positionalArguments();
    if (args.empty()) {
        std::cerr << "Error: no config provided\n";
        exit(127);
    }

    QString config_name = args.at(0);
    auto [scene, ht] = parse_json_scene(open_config(config_name));
    auto result = scene->render((Hittype)ht);

    int pw = scene->viewport->pixel_width;
    int ph = scene->viewport->pixel_height;

    std::cout << "P3\n" << pw << ' ' << ph << "\n255\n";
    for (int i = 0; i < ph; ++i) {
        for (int j = 0; j < pw; ++j) {
            auto [r, g, b] = result[i][j];
            std::cout << r << ' ' << g << ' ' << b << '\n';
        }
    }
}