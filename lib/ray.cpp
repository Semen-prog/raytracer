#include "LibTrace/ray.hpp"

long double Ray::ptime(const Point &p) const {
    return Vector::dotProduct(p - start, dir);
}
Point Ray::operator()(long double t) const {
    return start + dir * t;
}