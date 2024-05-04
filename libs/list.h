#ifndef OBJECT_H
#define OBJECT_H

#include "geometry.h"
#include "styles.h"

class ObjectList {
 private:
    std::vector<std::pair<Shape*, Style*>> lib;
 public:
    ObjectList() {
        lib = {};
    }
    void add(Shape *sh, Style *st) {
        lib.emplace_back(sh, st);
    }
    bool intersect(const ray& r, interval zone, color& attenuation, ray& scattered) const {
        shape_record temp_rec;
        bool good = false;
        for (const auto &s: lib) {
            if (s.first->intersect(r, zone, temp_rec)) {
                zone.right = temp_rec.t;
                good = true;
                s.second->scatter(r, temp_rec, attenuation, scattered);
            }
        }
        return good;
    }
};

#endif