#pragma once

#include "LibTrace/materials.hpp"
#include "LibTrace/parse.hpp"

class Lamberitan : public Material {
 public:
    Lamberitan() = default;
    bool scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const override;
    static QSharedPointer<Material> parse_json(const QJsonObject& json);
    static verdict check_json(const QJsonObject& json);
};