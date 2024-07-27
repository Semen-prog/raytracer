#include "LibTrace/interval.hpp"

Interval Interval::operator&(const Interval &i) const {
    return Interval(qMax(left, i.left), qMin(right, i.right));
}
Interval Interval::operator|(const Interval &i) const {
    return Interval(qMin(left, i.left), qMax(right, i.right));
}
Interval Interval::operator+(long double x) const {
    return Interval(left + x, right + x);
}
bool Interval::contains(long double x) const {
    return left - eps <= x && x <= right + eps;
}
bool Interval::surrounds(long double x) const {
    return left + eps < x && x < right - eps;
}
Interval Interval::hit(long double start, long double dir) const {
    if (qAbs(dir) < eps) return surrounds(start) ? Interval(-inf, inf): Interval(inf, -inf);
    Interval h((left - start) / dir, (right - start) / dir);
    if (h.left > h.right) qSwap(h.left, h.right);
    return h;
}
bool Interval::empty() const {
    return left - eps > right + eps;
}