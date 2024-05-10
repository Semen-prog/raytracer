#ifndef DIELECTRIC_H
#define DIELECTRIC_h

#include "base.hpp"

CUDA_PREFIX double schlick_reflection(double coss, double ri) {
    auto r0 = (1 - ri) / (1 + ri);
    r0 = r0 * r0;
    return r0 + (1-r0) * pow(1 - coss, 5);
}

class Dielectric: public Style {
 private:
    double refractive_index;
    
 public:
    CUDA_PREFIX Dielectric(double ri): refractive_index(ri) {}
    CUDA_PREFIX void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir;
        double ri = rec.in_face ? refractive_index: 1 / refractive_index;
        if (!refract(r_in.dir, rec.normal, ri, s_dir) || schlick_reflection(cuda_min(dot(-r_in.dir, rec.normal), 1 - eps), ri) > random_01()) {
            s_dir = reflect(r_in.dir, rec.normal);
        }
        scattered = ray(rec.p, s_dir);
        scattered.tim = r_in.tim;
        attenuation = color(1, 1, 1);
    }
};

#endif