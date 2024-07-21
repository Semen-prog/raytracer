#ifndef CUBE_H
#define CUBE_H

#include "LibTrace/tracing.hpp"


class Cube : public Shape {
   private:
    Point center;
    long double rx, ry, rz;
    void set_record(const Ray &r, long double tim, const Vector &n, shape_record &rd) const;
   protected:
    Bbox base_bbox() const override;
   public:
    Cube() = default;
    Cube(const Point &c, double x, double y, double z) : center(c), rx(x), ry(y), rz(z) {
        move = {0, 0, 0};
    }
    Cube(const Point &c, double x, double y, double z, const Vector &m) : Cube(c, x, y, z) {
        move = m;
    }
    bool hit(const Ray &r, Interval &zone, shape_record &rd) const override;
    static QSharedPointer<Shape> parse_json(const QJsonObject &json);
};

#endif // CUBE_H