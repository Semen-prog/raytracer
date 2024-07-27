#pragma once

#include "LibTrace/shapes.hpp"

class Texture {
   public:
    virtual Color value(const shape_record &rd) const = 0;
};