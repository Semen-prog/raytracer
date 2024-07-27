#pragma once

#include "LibTrace/vector.hpp"

class Ray {
   public:
    Point start;
    Vector dir;
    long double time;
    Ray() = default;
    Ray(const Point &s, const Vector &d) : start(s), dir(d.normalized()), time(uniform(0, 1)) {}
    Ray(const Point &s, const Vector &d, long double t) : start(s), dir(d.normalized()), time(t) {}
    long double ptime(const Point &p) const;
    Point operator()(long double t) const;
};