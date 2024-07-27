#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QImageWriter>
#include <QJsonDocument>
#include <QTextStream>

#include "LibTrace/tracing.hpp"

QTextStream es(stderr);

QJsonObject open_config(const QString &config_name) {
    QFile file;
    file.setFileName(config_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        es << "Error: cannot open file " << config_name << '\n';
        exit(127);
    }
    QString val = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError) {
        es << "Error: parse error: " << err.errorString() << '\n';
        exit(127);
    }
    if (!doc.isObject()) {
        es << "Error: doc is not an object\n";
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
                                     QCoreApplication::translate("main", QString("Output file name (suffix must be one of %1, default img.png)").arg(pf).toStdString().c_str()),
                                     QCoreApplication::translate("main", "Output file"));
    parser.addOption(format_option);

    parser.process(app);
    
    QStringList args = parser.positionalArguments();
    if (args.empty()) {
        es << "Error: no config provided\n";
        exit(127);
    }

    QString output_name = "img.png";
    if (parser.isSet(format_option)) {
        output_name = parser.value(format_option);
    }

    QString config_name = args.at(0);
    auto config = open_config(config_name);

    auto v = check_json_scene(config);
    if (!v.ok) {
        es << v.msg;
        exit(127);
    }

    auto [scene, ht] = parse_json_scene(config);
    if (!scene->render((Hittype)ht).save(output_name)) {
        es << "Error: failed to save result to " << output_name << " (maybe wrong suffix?)\n";
        exit(127);
    }
}