#ifndef OBJECT_H
#define OBJECT_H

#include "shapes/shapes.hpp"
#include "styles/styles.hpp"

struct Object {
    Shape *shape;
    Style *style;
    CUDA_PREFIX Object() = default;
    CUDA_PREFIX Object(Shape *sh, Style *st): shape(sh), style(st) {}
};

class ObjectList {
 private:
    int len, maxlen;
    Object *lib;
 public:
    CUDA_PREFIX ObjectList() = default;
    CUDA_PREFIX void allocate(int size) {
        lib = new Object[size];
        maxlen = size;
        len = 0;
    }
    inline CUDA_PREFIX bool add(Shape *sh, Style *st) {
        if (len == maxlen) return false;
        lib[len++] = {sh, st};
        return true;
    }
    CUDA_PREFIX bool intersect(const ray& r, interval zone, bool &issc, color &attenuation, ray &scattered) const {
        shape_record temp_rec;
        bool good = false;
        for (int i = 0; i < len; ++i) {
            if (lib[i].shape->intersect(r, zone, temp_rec)) {
                zone.right = temp_rec.t;
                good = true;
                issc = lib[i].style->scatter(r, temp_rec, attenuation, scattered);
            }
        }
        return good;
    }
};

#endif