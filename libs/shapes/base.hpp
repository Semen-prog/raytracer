#ifndef BASE_SHAPES_H
#define BASE_SHAPES_H

#include "../common.hpp"
#include "../vec3.hpp"
#include "../ray.hpp"

struct shape_record {
    point3 p;
    vec3 normal;
    double t;
    bool in_face;
    double x;
    double y;
    CUDA_PREFIX void set_normal(const vec3 &n, const ray &r, bool ifc) {
        if (dot(n, r.dir) >= 0) {
            normal = -n.unit();
            in_face = !ifc;
        } else {
            normal = n.unit();
            in_face = ifc;
        }
    }
};

CUDA_PREFIX vec3 random_out(vec3 normal) {
    vec3 random = random_unit();
    if (dot(normal, random) < 0) return -random;
    return random;
}

CUDA_PREFIX vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - n * (2 * dot(v, n));
}

CUDA_PREFIX bool refract(const vec3& v, const vec3 &n, double index, vec3 &result) {
    vec3 vu = v.unit();
    vec3 parru = n * dot(vu, n);
    vec3 perpu = vu - parru;
    double sins = perpu.len() * index;
    if (sins >= 1.0 - eps) return false;
    double coss = std::sqrt(1.0 - sq(sins));
    result = perpu.unit() * sins + parru.unit() * coss;
    return true;
}

class Shape {
 public:
    vec3 mov;
    CUDA_PREFIX virtual ~Shape() = default;
    CUDA_PREFIX virtual bool intersect(const ray& r, interval zone, shape_record& rec) const = 0;
};

CUDA_PREFIX bool check_angle(const point3 &p, const point3 &p1, const point3 &p2, const point3 &p3) {
    return dot((p2 - p1) ^ (p3 - p1), (p2 - p1) ^ (p - p1)) > eps && 
            dot((p2 - p1) ^ (p3 - p1), (p - p1) ^ (p3 - p1)) > eps;
}

class Plane {
 private:
    int plen;
    point3 *parr, center;
    vec3 norm, dx, dy, in;
 public:
    CUDA_PREFIX Plane() = default;
    CUDA_PREFIX Plane(int n, point3 *ps, const vec3 &dir_x, const vec3 &dir_y, const vec3 &infc) {
        plen = n;
        parr = new point3[plen];
        for (int i = 0; i < plen; ++i) parr[i] = ps[i];
        norm = ((parr[1] - parr[0]) ^ (parr[2] - parr[1]));
        norm = (norm * dot(norm, infc)).unit();
        interval x(inf, -inf), y(inf, -inf);
        center = point3(0, 0, 0);
        dx = dir_x.unit();
        dy = dir_y.unit();
        for (int i = 0; i < plen; ++i) {
            double cx = dot(parr[i] - center, dx), cy = dot(parr[i] - center, dy);
            x = x | cx;
            y = y | cy;
        }
        center = center + dx * x.left + dy * y.left;
        dx = dx * x.size();
        dy = dy * y.size();
        in = infc;
    }
    CUDA_PREFIX bool intersect(const ray &r, const interval &zone, shape_record &rec, const vec3 &mov) {
        double c1 = dot(parr[0] + mov - r.orig, norm), c2 = dot(r.dir, norm);
        if (std::abs(c2) <= eps) return false;
        point3 it = r.orig + r.dir * c1 / c2;
        double tim = c1 / c2;
        if (!zone.surrounds(tim)) return false;
        bool in_pol = true;
        for (int i = 0; i < plen; ++i) {
            if (!check_angle(it, parr[(i + 1) % plen] + mov, parr[i] + mov, parr[(i + 2) % plen] + mov)) {
                in_pol = false;
                break;
            }
        }
        if (in_pol) {
            rec.p = it;
            rec.t = tim;
            rec.set_normal(norm, r, true);
            rec.x = dot(dx, it - center) / dx.len();
            rec.y = dot(dy, it - center) / dy.len();
            return true;
        }
        return false;
    }
};

#endif