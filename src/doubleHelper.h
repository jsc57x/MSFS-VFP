#pragma once

#include <math.h>

#define DOUBLE_PRECISION 0.0000000001

inline bool areDoubleEqual(double a, double b) {
    return fabs(a - b) < DOUBLE_PRECISION;
}

inline bool isDoubleInRange(double value, double min, double max)
{
    if (min > max) std::swap(min, max);

    return !(value < min - DOUBLE_PRECISION || value > max + DOUBLE_PRECISION);
}