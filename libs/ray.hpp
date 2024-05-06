#ifndef RAY_H
#define RAY_H

#include "common.hpp"
#include "vec3.hpp"

struct ray {
    point3 orig;
    vec3 dir;
    CUDA_PREFIX ray() = default;
    CUDA_PREFIX ray(point3 p, vec3 v): orig(p), dir(v.unit()) {}
    CUDA_PREFIX point3 operator()(double t) const {
        return orig + dir * t;
    }
    CUDA_PREFIX double time(const point3 &p) const {
        vec3 dist = p - orig;
        if (dot(dist, dir) < 0) return -1;
        return dist.len();
    }
};

#endif