#ifndef COLOR_H
#define COLOR_H

#include "common.hpp"
#include "vec3.hpp"

using color = vec3;

CUDA_PREFIX color prepare_color(const color &pixel) {
    return pixel * 255.0;
}

CUDA_PREFIX color random_color(double min, double max) {
    return color(random_double(min, max), random_double(min, max), random_double(min, max));
}

#endif