#include "LibTrace/shapes/cube.hpp"

void Cube::set_record(const Ray &r, long double tim, const Vector &n, shape_record &rd) const {
    Vector tc = center + move * r.time;
    rd.t = tim;
    rd.p = r(tim);
    rd.n = n;
    rd.ifc = Vector::dotProduct(n, tc - rd.p) > 0;
    Point cds = rd.p - tc + Vector(rx, ry, rz);
    cds = Point(cds.x / (2 * rx), cds.y / (2 * ry), cds.z / (2 * rz));
    if (abs(n.x) > 0.5) {
        rd.x = n.y;
        rd.y = n.z;
    } else if (abs(n.y) > 0.5) {
        rd.x = n.x;
        rd.y = n.z;
    } else {
        rd.x = n.x;
        rd.y = n.y;
    }
    if (Vector::dotProduct(r.dir, rd.n) < 0) {
        rd.n = -rd.n;
        rd.ifc = !rd.ifc;
    }
}

Bbox Cube::base_bbox() const {
    return Bbox(Interval(center.x - rx, center.x + rx), Interval(center.y - ry, center.y + ry), Interval(center.z - rz, center.z + rz));
}

bool Cube::hit(const Ray &r, Interval &zone, shape_record &rd) const {
    Vector tc = center + move * r.time;
    Interval x = Interval(tc.x - rx, tc.x + rx).hit(r.start.x, r.dir.x);
    Interval y = Interval(tc.y - ry, tc.y + ry).hit(r.start.y, r.dir.y);
    Interval z = Interval(tc.z - rz, tc.z + rz).hit(r.start.z, r.dir.z);
    long double vl = x.left, vr = x.right;
    Vector nl = Vector(1, 0, 0), nr = Vector(1, 0, 0);
    if (y.left > vl) {
        vl = y.left;
        nl = Vector(0, 1, 0);
    }
    if (y.right < vr) {
        vr = y.right;
        nr = Vector(0, 1, 0);
    }
    if (z.left > vl) {
        vl = z.left;
        nl = Vector(0, 0, 1);
    }
    if (z.right < vr) {
        vr = z.right;
        nr = Vector(0, 0, 1);
    }
    if (Interval(vl, vr).empty()) return false;
    if (zone.surrounds(vl)) {
        set_record(r, vl, nl, rd);
        zone.right = rd.t;
        return true;
    } else if (zone.surrounds(vr)) {
        set_record(r, vr, nr, rd);
        zone.right = rd.t;
        return true;
    }
    return false;
}

QSharedPointer<Shape> Cube::parse_json(const QJsonObject &json) {
    return QSharedPointer<Cube>::create(parse_json_pv(json, "center"), parse_json_double(json, "rx", eps), parse_json_double(json, "ry", eps), parse_json_double(json, "rz", eps));
}