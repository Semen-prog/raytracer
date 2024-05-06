#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cmath>

#ifdef ENABLE_CUDA
#define CUDA_PREFIX __device__
#include <cuda_runtime.h>
#include <curand_kernel.h>
#else
#define CUDA_PREFIX
#endif

#define sq(x) ((x)*(x))
CUDA_PREFIX constexpr double eps = 1e-5;
CUDA_PREFIX constexpr double inf = 1'000'000'000'000'000'000;

#define pi M_PI
#define rad(d) ((double)(d) / 180 * pi)

#ifdef ENABLE_CUDA
CUDA_PREFIX double random_01() {
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

CUDA_PREFIX double random_double(double min, double max) {
    return random_01() * (max - min) + min;
}

CUDA_PREFIX int sign(double val) {
    if (val < -eps) return -1;
    if (-eps <= val && val <= eps) return 0;
    return 1;
}

struct interval {
    double left, right;
    CUDA_PREFIX interval(): left(0), right(inf) {}
    CUDA_PREFIX interval(double l, double r): left(l), right(r) {}
    CUDA_PREFIX double size() const {
        return right - left;
    }
    CUDA_PREFIX bool contains(double x) const {
        return left - eps <= x && x <= right - eps;
    }
    CUDA_PREFIX bool surrounds(double x) const {
        return left + eps < x && x < right - eps;
    }
};

CUDA_PREFIX double cuda_min(double a, double b) {
    return a < b ? a: b;
}

CUDA_PREFIX double cuda_max(double a, double b) {
    return a > b ? a: b;
}

#endif