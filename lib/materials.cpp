#include "LibTrace/materials.hpp"

Vector reflect(const Vector &d, const Vector &n) {
    return d - 2 * n;
}

Vector refract(const Vector &d, const Vector &n, long double ref) {
    long double r0 = powl((1 - ref) / (1 + ref), 2);
    long double sf = r0 + (1 - r0) * powl(1 - qMin(1 - eps, Vector::dotProduct(-d, n)), 5);
    if (sf < uniform(0, 1)) return reflect(d, n);
    Vector beta = d - n * Vector::dotProduct(d, n);
    long double sina = beta.length();
    long double sinb = ref * sina;
    if (!Interval(-1, 1).surrounds(sinb)) return reflect(d, n);
    long double cosb = sqrtl(1 - sinb * sinb);
    return n * cosb + beta.normalized() * sinb;
}