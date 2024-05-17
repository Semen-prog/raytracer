#ifndef LAMP_H
#define LAMP_H

#include "base.hpp"

class Lamp : public Style {
 private:
    color albedo;
 public:
    CUDA_PREFIX Lamp(const color &a): albedo(a) {}
    CUDA_PREFIX bool scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = albedo;
        return false;
    }
};

#endif