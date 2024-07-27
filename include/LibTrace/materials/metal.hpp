#pragma once

#include "LibTrace/materials.hpp"
#include "LibTrace/parse.hpp"

class Metal : public Material {
 private:
    long double fizz;
 public:
    Metal(): fizz(0) {}
    Metal(long double f): fizz(f) {}
    bool scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const override;
    static QSharedPointer<Material> parse_json(const QJsonObject& json);
    static verdict check_json(const QJsonObject& json);
};