#ifndef CHECKER_H
#define CHECKER_H

#include "LibTrace/tracing.hpp"

class Checker : public Texture {
 private:
    std::shared_ptr<Texture> odd, even;
    long double dx, dy, dz;
 public:
    Checker() = default;
    Checker(const std::shared_ptr<Texture> &o, const std::shared_ptr<Texture> &e, long double x, long double y, long double z): odd(o), even(e), dx(x), dy(y), dz(z) {}
    Color value(const shape_record &rd) const override;
};

#endif // CHECKER_H