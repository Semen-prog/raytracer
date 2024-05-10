#ifndef PYRAMID_H
#define PYRAMID_H

#include "base.hpp"

class Pyramid : public Shape {
 private:
    point3 ps[4];
    CUDA_PREFIX bool check_angl(const point3 &p, const point3 &p1, const point3 &p2, const point3 &p3) const {
        return dot((p2 - p1) ^ (p3 - p1), (p2 - p1) ^ (p - p1)) > eps && 
               dot((p2 - p1) ^ (p3 - p1), (p - p1) ^ (p3 - p1)) > eps;
    }
    CUDA_PREFIX bool check_pale(const ray &r, int dlt, shape_record &rec) const {
        int cnt = 0;
        point3 pp[3]; point3 dt = ps[dlt] + mov * r.tim;
        for (int i = 0; i < 4; ++i) {
            if (i != dlt) pp[cnt++] = ps[i] + mov * r.tim;
        }
        vec3 v1 = pp[1] - pp[0], v2 = pp[2] - pp[0];
        vec3 n = (v1 ^ v2).unit();
        double c1 = dot(pp[0] - r.orig, n), c2 = dot(r.dir, n);
        if (std::abs(c2) <= eps) return false;
        point3 it = r.orig + r.dir * c1 / c2;
        double tim = c1 / c2;
        if (check_angl(it, pp[0], pp[1], pp[2]) && check_angl(it, pp[1], pp[0], pp[2]) && check_angl(it, pp[2], pp[0], pp[1])) {
            rec.p = it;
            rec.t = tim;
            rec.set_normal(n * dot(dt - pp[0], n), r, true);
            rec.sphere_project(it - (pp[0] + pp[1] + pp[2] + dt) / 4);
            return true;
        }
        return false;
    }
 public:
    CUDA_PREFIX Pyramid(const point3 &p1, const point3 &p2, const point3 &p3, const point3 &p4) {
        ps[0] = p1;
        ps[1] = p2;
        ps[2] = p3;
        ps[3] = p4;
        mov = vec3(0, 0, 0);
    }
    CUDA_PREFIX Pyramid(const point3 &p1, const point3 &p2, const point3 &p3, const point3 &p4, const vec3 &v) {
        ps[0] = p1;
        ps[1] = p2;
        ps[2] = p3;
        ps[3] = p4;
        mov = v;
    }
    CUDA_PREFIX bool intersect(const ray& r, interval zone, shape_record& rec) const override {
        shape_record temp_rec;
        bool good = false;
        for (int i = 0; i < 4; ++i) {
            if (check_pale(r, i, temp_rec)) {
                if (zone.surrounds(temp_rec.t)) {
                    good = true;
                    rec = temp_rec;
                    zone.right = rec.t;
                }
            }
        }
        return good;
    }
};

#endif