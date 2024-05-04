#ifndef SPHERE_H
#define SPHERE_H

#include "../base.h"

class Sphere : public Shape {
 private:
    point3 center;
    double radius;
 public:
    Sphere(point3 _c, double _r): center(_c), radius(_r) {}
    bool intersect(const ray& r, interval zone, shape_record& rec) const override {
        vec3 orig_to_center = center - r.orig;
        double dist_to_pro = orig_to_center * r.dir;
        double dist_to_cnt = (orig_to_center | r.dir).len();
        if (dist_to_cnt >= radius - eps) return false;
        point3 pro = r.orig + r.dir * dist_to_pro;
        double dist_to_sph = std::sqrt(sq(radius) - sq(dist_to_cnt));
        double t = r.time(pro - r.dir * dist_to_sph);
        point3 p = pro - r.dir * dist_to_sph;
        if (!zone.surrounds(t)) {
            t = r.time(pro + r.dir * dist_to_sph);
            p = pro + r.dir * dist_to_sph;
        }
        if (zone.surrounds(t)) {
            rec.p = p, rec.t = t;
            rec.set_normal((p - center).unit(), r, false);
            return true;
        }
        return false;
    }
};

#endif