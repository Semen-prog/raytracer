#include "LibTrace/tracing.hpp"

Interval Interval::operator&(const Interval &i) const {
    return Interval(std::max(left, i.left), std::min(right, i.right));
}
Interval Interval::operator|(const Interval &i) const {
    return Interval(std::min(left, i.left), std::max(right, i.right));
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
    if (abs(dir) < eps) return surrounds(start) ? Interval(-inf, inf): Interval(inf, -inf);
    Interval h((left - start) / dir, (right - start) / dir);
    if (h.left > h.right) std::swap(h.left, h.right);
    return h;
}
bool Interval::empty() const {
    return left - eps > right + eps;
}