#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vec3.h"
#include "ray.h"
#include "common.h"

struct shape_record {
    point3 p;
    vec3 normal;
    double t;
    bool in_face;
    void set_normal(const vec3 &n, const ray &r, bool ifc) {
        if (n * r.dir >= 0) {
            normal = -n.unit();
            in_face = !ifc;
        } else {
            normal = n.unit();
            in_face = ifc;
        }
    }
};

vec3 random_vec() {
    return vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1)).unit();
}

vec3 random_out(vec3 normal) {
    vec3 random = random_vec();
    if (normal * random < 0) return -random;
    return random;
}

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - n * (2 * (v * n));
}

bool refract(const vec3& v, const vec3 &n, double index, vec3 &result) {
    vec3 perp = (v - n * (v * n)) * index;
    double sins = perp.len();
    if (sins >= 1.0 - eps) return false;
    double coss = std::sqrt(1.0 - sq(sins));
    if ((v * n) < eps) coss = -coss;
    result = perp + n * coss;
    return true;
}

class Shape {
 public:
    virtual ~Shape() = default;
    virtual bool intersect(const ray& r, interval zone, shape_record& rec) const = 0;
};

class Sphere : public Shape {
 public:
    point3 center;
    double radius;
    Sphere(point3 _c, double _r): center(_c), radius(_r) {}
    bool intersect(const ray& r, interval zone, shape_record& rec) const override {
        vec3 orig_to_center = center - r.orig;
        double dist_to_pro = orig_to_center * r.dir;
        double dist_to_cnt = std::abs(orig_to_center ^ r.dir);
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