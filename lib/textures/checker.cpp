#include "LibTrace/textures/checker.hpp"

Color Checker::value(const shape_record &rd) const {
    int ix = (int)(rd.p.x / dx);
    int iy = (int)(rd.p.y / dy);
    int iz = (int)(rd.p.z / dz);
    return (ix + iy + iz) % 2 == 0 ? even : odd;
}

std::shared_ptr<Texture> Checker::parse_json(const QJsonObject &json) {
    return std::make_shared<Checker>(parse_json_color(json, "odd"), parse_json_color(json, "even"), parse_json_double(json, "dx", eps), parse_json_double(json, "dy", eps), parse_json_double(json, "dz", eps));
}