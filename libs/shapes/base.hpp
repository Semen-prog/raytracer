#ifndef BASE_SHAPES_H
#define BASE_SHAPES_H

#include "../common.hpp"
#include "../vec3.hpp"
#include "../ray.hpp"

struct shape_record {
    point3 p;
    vec3 normal;
    double t;
    bool in_face;
    CUDA_PREFIX void set_normal(const vec3 &n, const ray &r, bool ifc) {
        if (dot(n, r.dir) >= 0) {
            normal = -n.unit();
            in_face = !ifc;
        } else {
            normal = n.unit();
            in_face = ifc;
        }
    }
};

CUDA_PREFIX vec3 random_out(vec3 normal) {
    vec3 random = random_unit();
    if (dot(normal, random) < 0) return -random;
    return random;
}

CUDA_PREFIX vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - n * (2 * dot(v, n));
}

CUDA_PREFIX bool refract(const vec3& v, const vec3 &n, double index, vec3 &result) {
    vec3 perp = (v - n * (v * n)) * index;
    double sins = perp.len();
    if (sins >= 1.0 - eps) return false;
    double coss = std::sqrt(1.0 - sq(sins));
    if (dot(v, n) < eps) coss = -coss;
    result = perp + n * coss;
    return true;
}

class Shape {
 public:
    CUDA_PREFIX virtual ~Shape() = default;
    CUDA_PREFIX virtual bool intersect(const ray& r, interval zone, shape_record& rec) const = 0;
};

#endif