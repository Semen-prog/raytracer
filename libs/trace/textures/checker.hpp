#ifndef CHECKER_H
#define CHECKER_H

#include "base.hpp"

class Checker : public Texture {
 private:
    color even, odd;
    double scale;
 public:
    CUDA_PREFIX Checker(double s, color e, color o): scale(s), even(e), odd(o) {}
    CUDA_PREFIX color value(const shape_record &rec) const override {
        int x = (int)(rec.p.x);
        int y = (int)(rec.p.y * scale);
        int z = (int)(rec.p.z * scale);
        if ((x + y + z) % 2 == 0) return even;
        return odd; 
    };
};

#endif