#pragma once

#include "LibTrace/textures.hpp"
#include "LibTrace/parse.hpp"

class Solid : public Texture {
 private:
    Color albedo;
 public:
    Solid() = default;
    Solid(const Color &a): albedo(a) {}
    Color value(const shape_record &rd) const override;
    static QSharedPointer<Texture> parse_json(const QJsonObject &json);
    static verdict check_json(const QJsonObject &json);
};