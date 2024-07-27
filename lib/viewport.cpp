#include "LibTrace/viewport.hpp"

Vector random_unit_disk() {
    return Vector(uniform(-1, 1), uniform(-1, 1), 0.0).normalized();
}

Point Viewport::random_orig() const {
    auto rv = random_unit_disk();
    return center + viewport_delta_x.normalized() * rv.x + viewport_delta_y.normalized() * rv.y;
}
Point Viewport::get_pixel(int x, int y) const {
    return upper_left + viewport_delta_x * (x + 0.5) + viewport_delta_y * (y + 0.5);
}
Ray Viewport::get_ray(int x, int y) const {
    long double shift_ox = uniform(-offset_x, offset_x);
    long double shift_oy = uniform(-offset_y, offset_y);
    Point orig = (defocus_rad <= eps) ? center : random_orig();
    Point pixel = get_pixel(x, y) + viewport_delta_x * shift_ox + viewport_delta_y * shift_oy;
    return Ray(orig, pixel - orig);
}