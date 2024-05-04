#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "common.h"

using color = vec3;

void write_color(std::ostream& out, const color &pixel) {
    auto pixel_formed = pixel * 255.0;
    out << std::round(pixel_formed.x) << ' ' << std::round(pixel_formed.y) << ' ' << std::round(pixel_formed.z) << '\n';
}

color random_color() {
    return color(random_double(0, 1), random_double(0, 1), random_double(0, 1));
}

#endif