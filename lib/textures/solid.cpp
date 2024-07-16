#include "LibTrace/textures/solid.hpp"

Color Solid::value(__attribute_maybe_unused__ const shape_record &rd) const {
    return albedo;
}

std::shared_ptr<Texture> Solid::parse_json(const QJsonObject &json) {
    return std::make_shared<Solid>(parse_json_color(json, "albedo"));
}