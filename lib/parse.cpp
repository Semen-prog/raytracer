#include "LibTrace/parse.hpp"

verdict verdict::operator+(const verdict &v) const {
    return verdict(ok && v.ok, msg + v.msg);
}

int parse_json_int(const QJsonObject &json, const QString &name) {
    return json[name].toInt();
}
verdict check_json_int(const QJsonObject &json, const QString &name, int left, int right) {
    if (!json.contains(name) || json[name].isNull()) return verdict(false, QString("Error: no such int field: %1\n").arg(name));
    bool ok = false;
    int res = json[name].toVariant().toInt(&ok);
    if (!ok) return verdict(false, QString("Error: no such int field: %1\n").arg(name));
    if (left > res || right < res) return verdict(false, QString("Error: int field %1 violates the range [%2;%3]\n").arg(name).arg(left).arg(right));
    return verdict();
}

long double parse_json_double(const QJsonObject &json, const QString &name) {
    return json[name].toDouble();
}
verdict check_json_double(const QJsonObject &json, const QString &name, long double left, long double right) {
    if (!json.contains(name) || !json[name].isDouble()) return verdict(false, QString("Error: no such double field: %1\n").arg(name));
    long double res = json[name].toDouble();
    if (left > res || right < res) return verdict(false, QString("Error: double field %1 violates the range [%2;%3]\n").arg(name).arg(QString::number((double)left)).arg(QString::number((double)right)));
    return verdict();
}

QString parse_json_string(const QJsonObject &json, const QString &name) {
    return json[name].toString();
}
verdict check_json_string(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isString()) return verdict(false, QString("Error: no such string field: %1\n").arg(name));
    return verdict();
}

Vector parse_json_pv(const QJsonObject &json, const QString &name) {
    QJsonObject obj = json[name].toObject();
    return Vector(parse_json_double(obj, "x"), parse_json_double(obj, "y"), parse_json_double(obj, "z"));
}
verdict check_json_pv(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) return verdict(false, QString("Error: no such pv field: %1\n").arg(name));
    QJsonObject obj = json[name].toObject();
    return check_json_double(obj, "x") + check_json_double(obj, "y") + check_json_double(obj, "z");
}

Color parse_json_color(const QJsonObject &json, const QString &name) {
    QJsonObject obj = json[name].toObject();
    return Color(parse_json_int(obj, "r") / (double)255, parse_json_int(obj, "g") / (double)255, parse_json_int(obj, "b") / (double)255);
}
verdict check_json_color(const QJsonObject &json, const QString &name) {
    if (!json.contains(name) || !json[name].isObject()) return verdict(false, QString("Error: no such color field: %1\n").arg(name));
    QJsonObject obj = json[name].toObject();
    return check_json_int(obj, "r", 0, 255) + check_json_int(obj, "g", 0, 255) + check_json_int(obj, "b", 0, 255);
}