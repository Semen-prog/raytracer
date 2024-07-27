#pragma once

#include "LibTrace/ray.hpp"

Vector random_unit_disk();

class Viewport {
   private:
    Vector viewport_delta_x, viewport_delta_y;
    Point upper_left;
    Point center;
    long double offset_x, offset_y;
    long double defocus_rad;
    Point random_orig() const;
    Point get_pixel(int x, int y) const;

   public:
    int pixel_width, pixel_height;
    Viewport() = default;
    Viewport(
        int pw,           // pixel width
        int ph,           // pixel height
        long double fov,  // vertical field of view
        long double ox,   // offset for OX
        long double oy,   // offset for OY
        Point lookfrom,   // look from (camera position)
        Point lookat,     // look at (image center)
        Vector dir_up,    // up from camera
        long double da    // defocus angle
    ) {
        pixel_width = pw;
        pixel_height = ph;
        center = lookfrom;
        long double focal_length = (lookat - lookfrom).length();
        long double viewport_height = tanl(fov / 2) * focal_length * 2;
        long double viewport_width = viewport_height * (double)pixel_width / (double)pixel_height;
        Vector front = (lookat - lookfrom).normalized();
        Vector right = Vector::crossProduct(front, dir_up).normalized();
        Vector up = Vector::crossProduct(right, front).normalized();
        Vector viewport_x = right * viewport_width;
        Vector viewport_y = (-up) * viewport_height;
        viewport_delta_x = viewport_x / pixel_width;
        viewport_delta_y = viewport_y / pixel_height;
        upper_left = lookat - viewport_x / 2 - viewport_y / 2;
        offset_x = ox;
        offset_y = oy;
        defocus_rad = focal_length * tanl(da / 2);
    }
    Ray get_ray(int x, int y) const;
};