#ifndef METAL_H
#define METAL_H

#include "base.hpp"

class Metal: public Style {
 private:
    color albedo;
    double fizz; // radius of random circle
 public:
    CUDA_PREFIX Metal(const color &a, double f): albedo(a), fizz(f) {} 
    CUDA_PREFIX void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir = reflect(r_in.dir, rec.normal);
        s_dir = s_dir.unit() + random_unit() * fizz;
        scattered = ray(rec.p, s_dir);
        scattered.tim = r_in.tim;
        attenuation = albedo;
    }
};

#endif