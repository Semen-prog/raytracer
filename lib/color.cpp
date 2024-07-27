#include "LibTrace/color.hpp"

Color gamma_correction(const Color &col, long double gamma) {
    return Color(powl(col.x, 1 / gamma), powl(col.y, 1 / gamma), powl(col.z, 1 / gamma));
}

int form(long double x) {
    return (int)(x * 255.0);
}

QColor correct(const Color &col) {
    auto corr = gamma_correction(col, 2);
    return QColor(form(corr.x), form(corr.y), form(corr.z));
}

bool almost_black(const Color &col) {
    return qAbs(col.length()) < eps;
}