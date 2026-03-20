#ifndef GEUTILS_H
#define GEUTILS_H

#include "Ge.h"
#include <algorithm>
#include <cmath>

inline void moveIntoValidInter(const double& startAngleInV, const double& endAngleInV, double& v, const double period = PI * 2.0)
{
    if (v < startAngleInV) {
        v = startAngleInV - std::fmod(startAngleInV - v, period);
        if (v < startAngleInV) {
            v += period;
        }
    }
    if (v > endAngleInV) {
        v = endAngleInV + std::fmod(v - endAngleInV, period);
        if (v > endAngleInV) {
            v -= period;
        }
    }
    if (v < startAngleInV) {
        double d1 = startAngleInV - v;
        double d2 = std::fabs(endAngleInV - v - period);
        if (d2 < d1) {
            v += period;
        }
    }
}

inline void makeInterValid(double& startAngle, double& endAngle, const double period = PI * 2.0)
{
    if (startAngle > endAngle) {
        std::swap(startAngle, endAngle);
    }
    if ((endAngle - startAngle) > period) {
        startAngle = -period * 0.5;
        endAngle = period * 0.5;
    }
}

#endif