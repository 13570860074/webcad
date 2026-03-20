#ifndef GEELLIPARC_H
#define GEELLIPARC_H

#include "Ge.h"
#include <cmath>

namespace GeEllipArc
{
inline double calibrateAngle(double val, double input)
{
    const double twoPi = PI * 2.0;

    if (input > val) {
        if ((input - val) < PI) {
            return val;
        }
        return val + std::round((input - val) / twoPi) * twoPi;
    }

    if ((val - input) < PI) {
        return val;
    }
    return val - std::round((val - input) / twoPi) * twoPi;
}

inline double angleFromParam(double param, double radiusRatio)
{
    const double twoPi = PI * 2.0;

    if (param == 0.0) {
        return 0.0;
    }
    if (param == twoPi) {
        return twoPi;
    }

    return calibrateAngle(std::atan2(radiusRatio * std::sin(param), std::cos(param)), param);
}

inline double paramFromAngle(double angle, double radiusRatio)
{
    const double twoPi = PI * 2.0;

    if (angle == 0.0) {
        return 0.0;
    }
    if (angle == twoPi) {
        return twoPi;
    }

    return calibrateAngle(std::atan2(std::sin(angle), radiusRatio * std::cos(angle)), angle);
}
}

#endif