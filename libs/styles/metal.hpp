#ifndef METAL_H
#define METAL_H

#include "base.hpp"

class Metal: public Style {
 private:
    color albedo;
    double fizz; // radius of random circle
    Texture *texture;
 public:
    CUDA_PREFIX Metal(double f, Texture *t): fizz(f), texture(t) {} 
    CUDA_PREFIX void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        vec3 s_dir = reflect(r_in.dir, rec.normal);
        s_dir = s_dir.unit() + random_unit() * fizz;
        scattered = ray(rec.p, s_dir);
        scattered.tim = r_in.tim;
        attenuation = texture->value(rec);
    }
};

#endif