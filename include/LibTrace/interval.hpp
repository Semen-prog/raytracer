#pragma once

#include "LibTrace/base.hpp"

class Interval {
   public:
    long double left, right;
    Interval() = default;
    Interval(long double l, long double r) : left(l), right(r) {}
    Interval operator&(const Interval &i) const;
    Interval operator|(const Interval &i) const;
    Interval operator+(long double x) const;
    bool contains(long double x) const;
    bool surrounds(long double x) const;
    Interval hit(long double start, long double dir) const;
    bool empty() const;
};