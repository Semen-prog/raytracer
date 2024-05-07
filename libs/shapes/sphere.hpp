#ifndef SPHERE_H
#define SPHERE_H

#include "base.hpp"

class Sphere : public Shape {
 private:
    point3 center;
    double radius;
 public:
    CUDA_PREFIX Sphere(point3 _c, double _r): center(_c), radius(_r) {
        mov = vec3(0, 0, 0);
    }
    CUDA_PREFIX Sphere(point3 _c, double _r, const vec3 &v): center(_c), radius(_r) {
        mov = v;
    }
    CUDA_PREFIX bool intersect(const ray& r, interval zone, shape_record& rec) const override {
        vec3 orig_to_center = center + mov * r.tim - r.orig;
        double dist_to_pro = dot(orig_to_center, r.dir);
        double dist_to_cnt = (orig_to_center ^ r.dir).len();
        if (dist_to_cnt >= radius - eps) return false;
        point3 pro = r.orig + r.dir * dist_to_pro;
        double dist_to_sph = std::sqrt(sq(radius) - sq(dist_to_cnt));
        double t = dot(pro - r.dir * dist_to_sph - r.orig, r.dir);
        point3 p = pro - r.dir * dist_to_sph;
        if (!zone.surrounds(t)) {
            t = dot(pro + r.dir * dist_to_sph - r.orig, r.dir);
            p = pro + r.dir * dist_to_sph;
        }
        if (zone.surrounds(t)) {
            rec.p = p, rec.t = t;
            rec.set_normal((p - center - mov * r.tim).unit(), r, false);
            return true;
        }
        return false;
    }
};

#endif