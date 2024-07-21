#include "LibTrace/tracing.hpp"

int randint(int l, int r) {
    return (int)(QRandomGenerator::global()->generate() % (quint32)(r - l)) + l;
}

long double uniform(long double l, long double r) {
    return l + QRandomGenerator::global()->generateDouble() * (r - l);
}