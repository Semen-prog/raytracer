#ifndef LAMBERITAN_H
#define LAMBERITAN_H

#include "LibTrace/tracing.hpp"

class Lamberitan : public Material {
 public:
    Lamberitan() = default;
    bool scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const override;
};

#endif // LAMBERITAN_H