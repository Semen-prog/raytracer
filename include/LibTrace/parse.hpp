#pragma once

#include <QJsonObject>
#include "LibTrace/vector.hpp"

struct verdict {
    bool ok;
    QString msg;
    verdict(): ok(true), msg("") {}
    verdict(bool _o, const QString &_s): ok(_o), msg(_s) {}
    verdict operator+(const verdict &v) const;
};

int parse_json_int(const QJsonObject &json, const QString &name);
verdict check_json_int(const QJsonObject &json, const QString &name, int left = INT32_MIN, int right = INT32_MAX);

long double parse_json_double(const QJsonObject &json, const QString &name);
verdict check_json_double(const QJsonObject &json, const QString &name, long double left = -inf, long double right = inf);

QString parse_json_string(const QJsonObject &json, const QString &name);
verdict check_json_string(const QJsonObject &json, const QString &name);

Vector parse_json_pv(const QJsonObject &json, const QString &name);
verdict check_json_pv(const QJsonObject &json, const QString &name);

Color parse_json_color(const QJsonObject &json, const QString &name);
verdict check_json_color(const QJsonObject &json, const QString &name);