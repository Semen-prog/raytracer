#ifndef BASE_TEXTURES_H
#define BASE_TEXTURES_H

#include "../shapes/shapes.hpp"
#include "../color.hpp"

class Texture {
 public:
    CUDA_PREFIX virtual ~Texture() = default;
    CUDA_PREFIX virtual color value(const shape_record &rec) const = 0;
};

#endif