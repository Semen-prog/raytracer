#ifndef METAL_H
#define METAL_H

#include "LibTrace/tracing.hpp"

class Metal : public Material {
 private:
    long double fizz;
 public:
    Metal(): fizz(0) {}
    Metal(long double f): fizz(f) {}
    bool scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const override;
    static QSharedPointer<Material> parse_json(const QJsonObject& json);
};

#endif // METAL_H