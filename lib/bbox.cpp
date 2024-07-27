#include "LibTrace/bbox.hpp"

Bbox Bbox::operator+(const Vector &v) const {
    return Bbox(x + v.x, y + v.y, z + v.z);
}
Bbox Bbox::operator|(const Bbox &b) const {
    return Bbox(x | b.x, y | b.y, z | b.z);
}
bool Bbox::hit(const Ray &r, const Interval &zone) const {
    return !(x.hit(r.start.x, r.dir.x) & y.hit(r.start.y, r.dir.y) & z.hit(r.start.z, r.dir.z) & zone).empty();
}