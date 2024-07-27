#pragma once

#include <QRandomGenerator>
#include <cmath>

int randint(int l, int r);
long double uniform(long double l, long double r);

constexpr long double eps = 1e-6;
constexpr long double inf = 1'000'000'000'000'000'000;

#define radian(x) ((x) / 180.0 * (long double)M_PI)