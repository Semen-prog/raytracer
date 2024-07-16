#include "LibTrace/materials/light.hpp"

bool Light::scatter(__attribute_maybe_unused__ const Ray& r_in, __attribute_maybe_unused__ const shape_record& rd, __attribute_maybe_unused__ Ray& scattered) const {
    return false;
}

std::shared_ptr<Material> Light::parse_json(__attribute_maybe_unused__ const QJsonObject& json) {
    return std::make_shared<Light>();
}