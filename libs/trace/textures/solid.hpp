#ifndef SOLID_H
#define SOLID_H

#include "base.hpp"

class Solid : public Texture {
 private:
    color albedo;
 public:
    CUDA_PREFIX Solid(const color &a): albedo(a) {}
    CUDA_PREFIX color value(const shape_record &rec) const override {
        return albedo;
    };
};

#endif