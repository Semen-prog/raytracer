#include "LibTrace/vector.hpp"

Vector Vector::normalized() const {
    long double len = length();
    return Vector(x / len, y / len, z / len);
}
long double Vector::length() const {
    return sqrtl(x * x + y * y + z * z);
}
long double Vector::dotProduct(const Vector &v, const Vector &u) {
    return v.x * u.x + v.y * u.y + v.z * u.z;
}
Vector Vector::crossProduct(const Vector &v, const Vector &u) {
    return Vector(v.y * u.z - u.y * v.z, v.z * u.x - u.z * v.x, v.x * u.y - v.y * u.x);
}
Vector Vector::normal(const Vector &v, const Vector &u) {
    return crossProduct(v, u).normalized();
}

Vector operator+(const Vector &v, const Vector &u) {
    return Vector(v.x + u.x, v.y + u.y, v.z + u.z);
}
Vector operator*(const Vector &v, const Vector &u) {
    return Vector(v.x * u.x, v.y * u.y, v.z * u.z);
}
Vector operator*(const Vector &v, long double x) {
    return Vector(v.x * x, v.y * x, v.z * x);
}
Vector operator*(double x, const Vector &v) {
    return Vector(v.x * x, v.y * x, v.z * x);
}
Vector operator-(const Vector &v) {
    return Vector(-v.x, -v.y, -v.z);
}
Vector operator-(const Vector &v, const Vector &u) {
    return Vector(v.x - u.x, v.y - u.y, v.z - u.z);
}
Vector operator/(const Vector &v, long double x) {
    return Vector(v.x / x, v.y / x, v.z / x);
}

Vector random_unit() {
    return Vector(uniform(-1, 1), uniform(-1, 1), uniform(-1, 1)).normalized();
}