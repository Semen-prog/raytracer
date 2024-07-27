#pragma once

#include "LibTrace/interval.hpp"
#include "LibTrace/ray.hpp"

class Bbox {
   public:
    Interval x, y, z;
    Bbox() = default;
    Bbox(const Interval &_x, const Interval &_y, const Interval &_z) : x(_x), y(_y), z(_z) {}
    Bbox operator+(const Vector &v) const;
    Bbox operator|(const Bbox &b) const;
    bool hit(const Ray &r, const Interval &zone) const;
};