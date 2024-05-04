#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cmath>
#include <vector>
#include <random>

#define sq(x) ((x)*(x))
constexpr double eps = 1e-5;
constexpr double inf = std::numeric_limits<double>::infinity();

#define pi M_PI
#define rad(d) ((double)(d) / 180 * pi)

std::mt19937 rnd(192168);
std::uniform_real_distribution<double> ud(0.0, 1.0);

double random_01() {
    return ud(rnd);
}

double random_double(double min, double max) {
    return random_01() * (max - min) + min;
}

struct interval {
    double left, right;
    interval(): left(0), right(inf) {}
    interval(double l, double r): left(l), right(r) {}
    double size() const {
        return right - left;
    }
    bool contains(double x) const {
        return left - eps <= x && x <= right - eps;
    }
    bool surrounds(double x) const {
        return left + eps < x && x < right - eps;
    }
};

const interval empty(inf, 0);
const interval universe(0, inf);

#endif