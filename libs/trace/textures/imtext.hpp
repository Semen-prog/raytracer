#ifndef IMTEXT_H
#define IMTEXT_H

#include "base.hpp"

class Imtext : public Texture {
 private:
    int w, h;
    color *img;
 public:
    CUDA_PREFIX Imtext(int _w, int _h, color *arr): w(_w), h(_h) {
        img = arr;
        for (int i = 0; i < _w * _h; ++i) img[i] = gamma_correction(img[i], 1.0 / GAMMA);
    }
    CUDA_PREFIX color pixel(double x, double y) const {
        int j = w - 1 - cuda_min(w - 1, cuda_max(0, (int)(x * w)));
        int i = h - 1 - cuda_min(h - 1, cuda_max(0, (int)(y * h)));
        return img[i * w + j];
    }
    CUDA_PREFIX color value(const shape_record &rec) const override {
        return pixel(rec.x, rec.y);
    };
};

#endif