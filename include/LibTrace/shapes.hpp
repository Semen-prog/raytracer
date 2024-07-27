#pragma once

#include "LibTrace/bbox.hpp"

struct shape_record {
    Point p;
    Vector n;
    long double t;
    bool ifc;
    long double x, y;
};

class Shape {
   protected:
    Vector move;
    virtual Bbox base_bbox() const = 0;

   public:
    Bbox bbox() const;
    virtual bool hit(const Ray &r, Interval &zone, shape_record &rd) const = 0;
};