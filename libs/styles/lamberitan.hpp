#ifndef LAMBERITAN_H
#define LAMBERITAN_H

#include "base.hpp"

class Lamberitan: public Style {
 private:
    color albedo;
 public:
    CUDA_PREFIX Lamberitan(const color &a): albedo(a) {}
    CUDA_PREFIX void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir = rec.normal + random_unit();
        if (almost_zero(s_dir)) s_dir = rec.normal;
        scattered = ray(rec.p, s_dir);
        scattered.tim = r_in.tim;
        attenuation = albedo;
    }
};

#endif