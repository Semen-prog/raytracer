#pragma once

#include "LibTrace/shapes.hpp"

class Material {
   public:
    virtual bool scatter(const Ray &r_in, const shape_record &rd, Ray &scattered) const = 0;
};

Vector reflect(const Vector &d, const Vector &n);
Vector refract(const Vector &d, const Vector &n, long double ref);