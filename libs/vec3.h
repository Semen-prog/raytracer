#ifndef VEC3_H
#define VEC3_H

#include "common.h"

struct vec3 {
    double x, y, z;
    vec3() = default;
    vec3(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
    double operator*(const vec3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    vec3 operator|(const vec3 &v) {
        return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    vec3 operator&(const vec3 &v) const {
        return vec3(x * v.x, y * v.y, z * v.z);
    }
    vec3 operator*(double c) const {
        return { x * c, y * c, z * c };
    }
    vec3 operator/(double c) const {
        return { x / c, y / c, z / c };
    }
    vec3 operator+(const vec3 &v) const {
        return { x + v.x, y + v.y, z + v.z };
    }
    vec3 operator-(const vec3 &v) const {
        return { x - v.x, y - v.y, z - v.z };
    }
    vec3 operator-() const {
        return { -x, -y, -z };
    }
    double len() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    double len_sq() const {
        return x * x + y * y + z * z;
    }
    vec3 unit() const {
        return (*this) / len();
    }
};

bool almost_zero(const vec3 &v) {
    return v.len() <= eps;
}

using point3 = vec3;

#endif