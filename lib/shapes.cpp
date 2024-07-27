#include "LibTrace/shapes.hpp"

Bbox Shape::bbox() const {
    Bbox b = base_bbox();
    return b | (b + move);
}