#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include "base.hpp"

class Parallelepiped : public Shape {
 private:
    Plane *arr;
 public:
    CUDA_PREFIX Parallelepiped(const point3 &a, const point3 &b, const point3 &c, const point3 &d,
                    const point3 &a1, const point3 &b1, const point3 &c1, const point3 &d1) {
        arr = new Plane[6]; point3 *ps = new point3[4];
        ps[0] = a, ps[1] = a1, ps[2] = b1, ps[3] = b;
        arr[0] = Plane(4, ps, b - a, a1 - a, d - a);
        ps[0] = b, ps[1] = b1, ps[2] = c1, ps[3] = c;
        arr[1] = Plane(4, ps, c - b, b1 - b, a - b);
        ps[0] = c, ps[1] = c1, ps[2] = d1, ps[3] = d;
        arr[2] = Plane(4, ps, d - c, c1 - c, b - c);
        ps[0] = d, ps[1] = d1, ps[2] = a1, ps[3] = a;
        arr[3] = Plane(4, ps, a - d, d1 - d, c - d);
        ps[0] = a1, ps[1] = b1, ps[2] = c1, ps[3] = d1;
        arr[4] = Plane(4, ps, b1 - a1, d1 - a1, a - a1);
        ps[0] = d, ps[1] = c, ps[2] = b, ps[3] = a;
        arr[5] = Plane(4, ps, c - d, a - d, a1 - a);
    }
    CUDA_PREFIX Parallelepiped(const point3 &a, const point3 &b, const point3 &c, const point3 &d,
                    const point3 &a1, const point3 &b1, const point3 &c1, const point3 &d1, const vec3 &v): Parallelepiped(a, b, c, d, a1, b1, c1, d1) {
        mov = v;
    }
    CUDA_PREFIX bool intersect(const ray& r, interval zone, shape_record& rec) const override {
        bool good = false;
        for (int i = 0; i < 6; ++i) {
            if (arr[i].intersect(r, zone, rec, mov * r.tim)) {
               good = true;
               zone.right = rec.t;
            }
        }
        return good;
    };
};

#endif