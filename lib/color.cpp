#include "LibTrace/tracing.hpp"

Color gamma_correction(const Color &col, long double gamma) {
    return Color(powl(col.x, 1 / gamma), powl(col.y, 1 / gamma), powl(col.z, 1 / gamma));
}

int form(long double x) {
    return (int)(x * 255.0);
}

std::tuple<int, int, int> correct(const Color &col) {
    auto corr = gamma_correction(col, 2);
    return { form(corr.x), form(corr.y), form(corr.z) };
}

bool almost_black(const Color &col) {
    return abs(col.length()) < eps;
}