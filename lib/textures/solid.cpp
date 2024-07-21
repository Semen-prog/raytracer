#include "LibTrace/textures/solid.hpp"

Color Solid::value(__attribute_maybe_unused__ const shape_record &rd) const {
    return albedo;
}

QSharedPointer<Texture> Solid::parse_json(const QJsonObject &json) {
    return QSharedPointer<Solid>::create(parse_json_color(json, "albedo"));
}