#ifndef VEC3_H
#define VEC3_H

#include "common.hpp"

struct vec3 {
    double x, y, z;
    CUDA_PREFIX vec3() = default;
    CUDA_PREFIX vec3(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
    inline CUDA_PREFIX vec3 operator^(const vec3 &v) {
        return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    inline CUDA_PREFIX vec3 operator*(const vec3 &v) const {
        return vec3(x * v.x, y * v.y, z * v.z);
    }
    inline CUDA_PREFIX vec3 operator*(double c) const {
        return { x * c, y * c, z * c };
    }
    inline CUDA_PREFIX vec3 operator/(double c) const {
        return { x / c, y / c, z / c };
    }
    inline CUDA_PREFIX vec3 operator+(const vec3 &v) const {
        return { x + v.x, y + v.y, z + v.z };
    }
    inline CUDA_PREFIX vec3 operator-(const vec3 &v) const {
        return { x - v.x, y - v.y, z - v.z };
    }
    inline CUDA_PREFIX vec3 operator-() const {
        return { -x, -y, -z };
    }
    inline CUDA_PREFIX double len() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    inline CUDA_PREFIX double len_sq() const {
        return x * x + y * y + z * z;
    }
    inline CUDA_PREFIX vec3 unit() const {
        return (*this) / len();
    }
};

inline CUDA_PREFIX double dot(const vec3 &v1, const vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline CUDA_PREFIX bool almost_zero(const vec3 &v) {
    return v.len() <= eps;
}

inline CUDA_PREFIX vec3 random_unit() {
    return vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1)).unit();
}

using point3 = vec3;

#endif