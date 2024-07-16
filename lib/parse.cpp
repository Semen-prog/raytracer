#include "LibTrace/tracing.hpp"

int parse_json_int(const QJsonObject &json, const QString &name, int left, int right) {
    if (!json.contains(name) || json[name].isNull()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    bool ok = false;
    int res = json[name].toVariant().toInt(&ok);
    if (!ok) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    if (left > res || right < res) {
        std::cerr << "Error: field " << name.toStdString() << " violates the range [" << left << ";" << right << "]\n";
        exit(127);
    }
    return res;
}

long double parse_json_double(const QJsonObject &json, const QString &name, long double left, long double right) {
    if (!json.contains(name) || !json[name].isDouble()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    long double res = json[name].toDouble();
    if (left > res || right < res) {
        std::cerr << "Error: field " << name.toStdString() << " violates the range [" << left << ";" << right << "]\n";
        exit(127);
    }
    return res;
}

QString parse_json_string(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isString()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    return json[name].toString();
}

Vector parse_json_pv(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    return Vector(parse_json_double(obj, "x"), parse_json_double(obj, "y"), parse_json_double(obj, "z"));
}

Color parse_json_color(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) {
        std::cerr << "Error: no such field: " << name.toStdString() << '\n';
        exit(127);
    }
    QJsonObject obj = json[name].toObject();
    return Color(parse_json_int(obj, "r", 0, 255) / (double)255, parse_json_int(obj, "g", 0, 255) / (double)255, parse_json_int(obj, "b", 0, 255) / (double)255);
}