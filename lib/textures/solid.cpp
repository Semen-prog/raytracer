#include "LibTrace/textures/solid.hpp"

Color Solid::value(__attribute_maybe_unused__ const shape_record &rd) const {
    return albedo;
}