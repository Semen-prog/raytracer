#include "LibTrace/materials/lamberitan.hpp"

bool Lamberitan::scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const {
    scattered = Ray(rd.p, random_unit() - rd.n, r_in.time);
    return true;
}