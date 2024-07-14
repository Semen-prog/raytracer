#include "LibTrace/tracing.hpp"

std::mt19937 rnd(65536);
std::uniform_real_distribution<long double> ud;

int randint(int l, int r) {
    return (int)(rnd() % (unsigned int)(r - l)) + l;
}

long double uniform(long double l, long double r) {
    return l + ud(rnd) * (r - l);
}