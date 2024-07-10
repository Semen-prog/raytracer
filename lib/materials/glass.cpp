#include "LibTrace/materials/glass.hpp"

bool Glass::scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const {
    scattered = Ray(rd.p, refract(r_in.dir, rd.n, rd.ifc ? refraction_index : 1 / refraction_index));
    return true;
}