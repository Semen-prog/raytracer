#pragma once

#include "LibTrace/shapes.hpp"
#include "LibTrace/parse.hpp"

class Sphere : public Shape {
   private:
    Point center;
    long double rad;
   protected:
    Bbox base_bbox() const override;
   public:
    Sphere() = default;
    Sphere(const Point &c, long double r) : center(c), rad(r) {
        move = {0, 0, 0};
    }
    Sphere(const Point &c, long double r, const Vector &m) : Sphere(c, r) {
        move = m;
    }
    bool hit(const Ray &r, Interval &zone, shape_record &rd) const override;
    static QSharedPointer<Shape> parse_json(const QJsonObject &json);
    static verdict check_json(const QJsonObject &json);
};