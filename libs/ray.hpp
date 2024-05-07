#ifndef RAY_H
#define RAY_H

#include "common.hpp"
#include "vec3.hpp"

struct ray {
    point3 orig;
    double tim;
    vec3 dir;
    CUDA_PREFIX ray() = default;
    CUDA_PREFIX ray(point3 p, vec3 v): orig(p), dir(v.unit()), tim(random_01()) {}
};

#endif