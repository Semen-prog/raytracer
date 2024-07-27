#include "LibTrace/textures/checker.hpp"

Color Checker::value(const shape_record &rd) const {
    int ix = (int)(rd.p.x / dx);
    int iy = (int)(rd.p.y / dy);
    int iz = (int)(rd.p.z / dz);
    return (ix + iy + iz) % 2 == 0 ? even : odd;
}

QSharedPointer<Texture> Checker::parse_json(const QJsonObject &json) {
    return QSharedPointer<Checker>::create(parse_json_color(json, "odd"), parse_json_color(json, "even"), parse_json_double(json, "dx"), parse_json_double(json, "dy"), parse_json_double(json, "dz"));
}

verdict Checker::check_json(const QJsonObject &json) {
    return check_json_color(json, "odd") + check_json_color(json, "even") + check_json_double(json, "dx", eps) + check_json_double(json, "dy", eps) + check_json_double(json, "dz", eps);
}