#ifndef GLASS_H
#define GLASS_H

#include "LibTrace/tracing.hpp"

class Glass : public Material {
 private:
    long double refraction_index;
 public:
    Glass() = default;
    Glass(long double ri): refraction_index(ri) {}
    bool scatter(const Ray& r_in, const shape_record& rd, Ray& scattered) const override;
};

#endif // GLASS_H