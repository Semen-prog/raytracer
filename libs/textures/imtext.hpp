#ifndef IMTEXT_H
#define IMTEXT_H

#include "base.hpp"

void parse_image(int &wa, int &ha, color *&arr, std::string path) {
    std::ifstream ifs(path);
    std::string tmp;
    int w, h, mxt;
    ifs >> tmp >> w >> h >> mxt;
    arr = new color[w * h];
    wa = w, ha = h;
    for (int i = 0; i < w * h; ++i) {
        int r, g, b;
        ifs >> r >> g >> b;
        arr[i].x = (double)r / mxt;
        arr[i].y = (double)g / mxt;
        arr[i].z = (double)b / mxt;
    }
}

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