#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cmath>
#include <fstream>

#ifdef ENABLE_CUDA
#define CUDA_PREFIX __device__
#include <cuda_runtime.h>
#include <curand_kernel.h>
#else
#define CUDA_PREFIX
#endif

#define sq(x) ((x)*(x))
#define GAMMA 2
CUDA_PREFIX constexpr double eps = 1e-5;
CUDA_PREFIX constexpr double inf = 1'000'000'000'000'000'000;

#define pi M_PI
#define rad(d) ((double)(d) / 180 * pi)

#ifdef ENABLE_CUDA
inline CUDA_PREFIX double random_01() {
    curandState state;
    curand_init(clock64(), threadIdx.x, 0, &state);
    return curand_uniform_double(&state);
}
#else
#include <random>
std::mt19937 rnd(192168);
std::uniform_real_distribution<double> ud(0.0, 1.0);
double random_01() {
    return ud(rnd);
}
#endif

inline CUDA_PREFIX double random_double(double min, double max) {
    return random_01() * (max - min) + min;
}

inline CUDA_PREFIX int sign(double val) {
    if (val < -eps) return -1;
    if (-eps <= val && val <= eps) return 0;
    return 1;
}

template<typename T>
inline CUDA_PREFIX T cuda_min(T a, T b) {
    return a < b ? a: b;
}

template<typename T>
inline CUDA_PREFIX T cuda_max(T a, T b) {
    return a > b ? a: b;
}

struct interval {
    double left, right;
    CUDA_PREFIX interval(): left(0), right(inf) {}
    CUDA_PREFIX interval(double l, double r): left(l), right(r) {}
    CUDA_PREFIX double size() const {
        return right - left;
    }
    inline CUDA_PREFIX bool contains(double x) const {
        return left - eps <= x && x <= right - eps;
    }
    inline CUDA_PREFIX bool surrounds(double x) const {
        return left + eps < x && x < right - eps;
    }
    inline CUDA_PREFIX interval operator|(const interval &i) const {
        return interval(cuda_min(i.left, left), cuda_max(i.right, right));
    }
    inline CUDA_PREFIX interval operator|(double v) const {
        return interval(cuda_min(v, left), cuda_max(v, right));
    }
    inline CUDA_PREFIX interval operator&(const interval &i) const {
        return interval(cuda_max(i.left, left), cuda_min(i.right, right));
    }
};

#endif