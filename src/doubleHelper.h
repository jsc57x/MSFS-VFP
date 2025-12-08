#pragma once

#include <math.h>

#define DOUBLE_PRECISION 0.0000000001

bool areDoubleEqual(double a, double b) {
    return fabs(a - b) < DOUBLE_PRECISION;
}

bool isDoubleEqualOrMore(double a, double b) {
    if(!areDoubleEqual(a, b))
    {
        double diff = b - a;
        return fabs(diff) >= DOUBLE_PRECISION;
    }

    return true;
}

bool isDoubleInRange(double value, double min, double max)
{
    return isDoubleEqualOrMore(value, min) && isDoubleEqualOrMore(max, value);
}