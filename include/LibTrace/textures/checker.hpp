#pragma once

#include "LibTrace/textures.hpp"
#include "LibTrace/parse.hpp"

class Checker : public Texture {
 private:
    Color odd, even;
    long double dx, dy, dz;
 public:
    Checker() = default;
    Checker(const Color &o, const Color &e, long double x, long double y, long double z): odd(o), even(e), dx(x), dy(y), dz(z) {}
    Color value(const shape_record &rd) const override;
    static QSharedPointer<Texture> parse_json(const QJsonObject &json);
    static verdict check_json(const QJsonObject &json);
};