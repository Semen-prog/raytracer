#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "common.h"

struct ray {
    point3 orig;
    vec3 dir;
    ray() = default;
    ray(point3 p, vec3 v): orig(p), dir(v.unit()) {}
    point3 operator()(double t) const {
        return orig + dir * t;
    }
    double time(const point3 &p) const {
        vec3 dist = p - orig;
        if (dist * dir < 0) return -1;
        return dist.len();
    }
};

#endif