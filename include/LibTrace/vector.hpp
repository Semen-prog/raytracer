#pragma once

#include "LibTrace/base.hpp"

class Vector {
   public:
    long double x, y, z;
    Vector() = default;
    Vector(long double _x, long double _y, long double _z) : x(_x), y(_y), z(_z) {}
    Vector normalized() const;
    long double length() const;
    static long double dotProduct(const Vector &v, const Vector &u);
    static Vector crossProduct(const Vector &v, const Vector &u);
    static Vector normal(const Vector &v, const Vector &u);
};

Vector operator+(const Vector &v, const Vector &u);
Vector operator*(const Vector &v, const Vector &u);
Vector operator*(const Vector &v, long double x);
Vector operator*(double x, const Vector &v);
Vector operator-(const Vector &v);
Vector operator-(const Vector &v, const Vector &u);
Vector operator/(const Vector &v, long double x);

using Point = Vector;
using Color = Vector;

Vector random_unit();