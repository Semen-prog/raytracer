#ifndef BASE_STYLES_H
#define BASE_STYLES_H

#include "../color.hpp"
#include "../ray.hpp"
#include "../shapes/shapes.hpp"
#include "../textures/textures.hpp"

class Style {
 public:
    CUDA_PREFIX virtual ~Style() = default;
    CUDA_PREFIX virtual void scatter(const ray& r_in, const shape_record& rec, color& attenuation, ray& scattered) const {
        return;
    }
};

#endif