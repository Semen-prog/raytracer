#ifndef SOLID_H
#define SOLID_H

#include "LibTrace/tracing.hpp"

class Solid : public Texture {
 private:
    Color albedo;
 public:
    Solid() = default;
    Solid(const Color &a): albedo(a) {}
    Color value(const shape_record &rd) const override;
    static QSharedPointer<Texture> parse_json(const QJsonObject &json);
};

#endif // SOLID_H