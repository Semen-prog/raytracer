#ifndef OBJECT_H
#define OBJECT_H

#include "shapes.hpp"
#include "styles.hpp"

struct Object {
    Shape *shape;
    Style *style;
    CUDA_PREFIX Object() = default;
    CUDA_PREFIX Object(Shape *sh, Style *st): shape(sh), style(st) {}
};

class ObjectList {
 private:
    int len;
    Object *lib;
 public:
    CUDA_PREFIX ObjectList() = default;
    CUDA_PREFIX void allocate(int size) {
        lib = new Object[size];
        len = 0;
    }
    CUDA_PREFIX void add(Shape *sh, Style *st) {
        lib[len++] = {sh, st};
    }
    CUDA_PREFIX bool intersect(const ray& r, interval zone, color& attenuation, ray& scattered) const {
        shape_record temp_rec;
        bool good = false;
        for (int i = 0; i < len; ++i) {
            if (lib[i].shape->intersect(r, zone, temp_rec)) {
                zone.right = temp_rec.t;
                good = true;
                lib[i].style->scatter(r, temp_rec, attenuation, scattered);
            }
        }
        return good;
    }
};

#endif