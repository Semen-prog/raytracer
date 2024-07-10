#include "LibTrace/textures/checker.hpp"

Color Checker::value(const shape_record &rd) const {
    int ix = (int)(rd.p.x / dx);
    int iy = (int)(rd.p.y / dy);
    int iz = (int)(rd.p.z / dz);
    return (ix + iy + iz) % 2 == 0 ? even->value(rd) : odd->value(rd);
}