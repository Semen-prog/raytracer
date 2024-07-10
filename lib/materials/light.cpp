#include "LibTrace/materials/light.hpp"

bool Light::scatter(__attribute_maybe_unused__ const Ray& r_in, __attribute_maybe_unused__ const shape_record& rd, __attribute_maybe_unused__ Ray& scattered) const {
    return false;
}