#ifndef COLOR_H
#define COLOR_H

#include "common.hpp"
#include "vec3.hpp"

using color = vec3;

inline CUDA_PREFIX color gamma_correction(const color &pixel, double gamma) {
    return color(pow(pixel.x, 1 / gamma), pow(pixel.y, 1 / gamma), pow(pixel.z, 1 / gamma));
}

inline CUDA_PREFIX color prepare_color(const color &pixel) {
    return gamma_correction(pixel, 2) * 255.0;
}

inline CUDA_PREFIX color random_color(double min, double max) {
    return color(random_double(min, max), random_double(min, max), random_double(min, max));
}

#endif