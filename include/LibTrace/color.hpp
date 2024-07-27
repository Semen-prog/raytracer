#pragma once

#include <QColor>
#include "LibTrace/vector.hpp"

Color gamma_correction(const Color &col, long double gamma);
QColor correct(const Color &col);
bool almost_black(const Color &col);