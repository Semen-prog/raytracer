#include "LibTrace/shapes/sphere.hpp"

Bbox Sphere::base_bbox() const {
    return Bbox(Interval(center.x - rad, center.x + rad), Interval(center.y - rad, center.y + rad), Interval(center.z - rad, center.z + rad));
}

bool Sphere::hit(const Ray &r, Interval &zone, shape_record &rd) const {
    Point tc = center + move * r.time;
    Vector ptoc = tc - r.start;
    long double dtol = Vector::crossProduct(ptoc, r.dir).length();
    if (dtol >= rad - eps) return false;
    Vector lp = tc + Vector::normal(Vector::normal(ptoc, r.dir), r.dir) * dtol;
    long double ltop = sqrtl(rad * rad - dtol * dtol);
    Point p = lp - r.dir * ltop;
    if (!zone.surrounds(r.ptime(p))) {
        p = lp + r.dir * ltop;
    }
    if (!zone.surrounds(r.ptime(p))) return false;
    rd.p = p;
    rd.t = r.ptime(p);
    rd.n = (p - tc).normalized();
    rd.ifc = false;
    rd.x = (atan2l(rd.n.y, rd.n.x) + M_PI) / (2 * M_PI);
    rd.y = (atan2l(rd.n.z, sqrtl(rd.n.x * rd.n.x + rd.n.y * rd.n.y)) + M_PI / 2) / M_PI;
    if (Vector::dotProduct(rd.n, r.dir) < 0) {
        rd.n = -rd.n;
        rd.ifc = !rd.ifc;
    }
    zone.right = rd.t;
    return true;
}

std::shared_ptr<Shape> Sphere::parse_json(const QJsonObject &json) {
    return std::make_shared<Sphere>(parse_json_pv(json, "center"), parse_json_double(json, "radius", eps));
}