#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "common.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "list.hpp"

class Viewport {
 private:
    int pixel_width, pixel_height;
    vec3 viewport_delta_x, viewport_delta_y;
    point3 upper_left;
    point3 center;
    int samples_per_pixel;
    double offset_x, offset_y;
    double defocus_rad;
    int max_depth;
    CUDA_PREFIX vec3 random_unit_disk() const {
        return vec3(random_double(-1, 1), random_double(-1, 1), 0.0).unit();
    }
    CUDA_PREFIX point3 random_disk() const {
        auto rv = random_unit_disk();
        return center + viewport_delta_x.unit() * defocus_rad * rv.x + viewport_delta_y.unit() * defocus_rad * rv.y;
    }
 public:
    ObjectList world;
    CUDA_PREFIX Viewport() = default;
    CUDA_PREFIX Viewport(
             int pw, // pixel width
             int ph, // pixel height
             double fov, // vertical field of view 
             int samples, // samples per pixel
             double ox, // offset for OX
             double oy, // offset for OY
             int md, // max depth
             point3 lookfrom, // look from (camera position)
             point3 lookat, // look at (image center)
             vec3 dir_up, // up from camera
             double da, // defocus angle
             int max_fig_cnt // max fig cnt
    ) {
        world.allocate(max_fig_cnt);
        pixel_width = pw;
        pixel_height = ph;
        center = lookfrom;
        double focal_length = (lookat - lookfrom).len();
        double viewport_height = tan(fov / 2) * focal_length * 2;
        double viewport_width = viewport_height * (double)pixel_width / (double)pixel_height;
        vec3 front = (lookat - lookfrom).unit();
        vec3 right = (front ^ dir_up).unit();
        vec3 up = (right ^ front).unit();
        vec3 viewport_x = right * viewport_width;
        vec3 viewport_y = (-up) * viewport_height;
        viewport_delta_x = viewport_x / pixel_width;
        viewport_delta_y = viewport_y / pixel_height;
        upper_left = lookat - viewport_x / 2 - viewport_y / 2;
        samples_per_pixel = samples;
        offset_x = ox;
        offset_y = oy;
        max_depth = md;
        defocus_rad = focal_length * tan(da / 2);
    }
    CUDA_PREFIX point3 get_pixel(int x, int y) const {
        return upper_left + viewport_delta_x * (x + 0.5) + viewport_delta_y * (y + 0.5);
    }
    CUDA_PREFIX ray get_ray(int x, int y) const {
        double shift_ox = random_double(-offset_x, offset_x);
        double shift_oy = random_double(-offset_y, offset_y);
        point3 orig = (defocus_rad <= eps) ? center: random_disk();
        point3 pixel = get_pixel(x, y) + viewport_delta_x * shift_ox + viewport_delta_y * shift_oy;
        return ray(orig, pixel - orig);
    }
    CUDA_PREFIX color get_ray_color(const ray &r, int depth) const {
        color attenuation_all(1, 1, 1);
        ray temp_ray = r;
        for (int i = 0; i < depth; ++i) {
            if (almost_zero(attenuation_all)) return color(0, 0, 0);
            bool issc;
            color attenuation;
            ray scattered;
            if (world.intersect(temp_ray, interval(0, inf), issc, attenuation, scattered)) {
                attenuation_all = attenuation_all * attenuation;
                temp_ray = scattered;
                if (!issc) return attenuation_all;
            } else {
                auto a = 0.5 * (r.dir.y + 1);
                return (color(1.0, 1.0, 1.0) * (1 - a) + color(0.5, 0.7, 1.0) * a) * attenuation_all;
            }
        }
        return color(0, 0, 0);
    }
    CUDA_PREFIX color get_pixel_color(int x, int y) const {
        color fin(0, 0, 0);
        for (int _ = 0; _ < samples_per_pixel; ++_) {
            fin = fin + get_ray_color(get_ray(x, y), max_depth);
        }
        return fin / samples_per_pixel;
    }
    
};

#endif