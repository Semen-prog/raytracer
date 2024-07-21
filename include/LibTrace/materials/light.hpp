#ifndef LIGHT_H
#define LIGHT_H

#include "LibTrace/tracing.hpp"

class Light : public Material {
 public:
    Light() = default;
    bool scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const override;
    static QSharedPointer<Material> parse_json(const QJsonObject& json);
};

#endif // LIGHT_H