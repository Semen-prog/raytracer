#include "LibTrace/materials/metal.hpp"

bool Metal::scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const {
    scattered = Ray(rd.p, reflect(r_in.dir, rd.n) + random_unit() * fizz, r_in.time);
    return true;
}