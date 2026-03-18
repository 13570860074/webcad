
#ifndef GEPOINTONCURVE2D_H
#define GEPOINTONCURVE2D_H 

#include "GePointEnt2d.h"
#include "GeEntity2dMethods.h"


class GeCurve2d;
class GePointOnCurve2d : public GePointEnt2d
{
public:
    GePointOnCurve2d();
    GePointOnCurve2d(const GeCurve2d& crv);
    GePointOnCurve2d(const GeCurve2d& crv, double param);
    GePointOnCurve2d(const GePointOnCurve2d& src);
    GEENTITY2D_METHODS(GePointOnCurve2d);
    GEPOINTENT2D_METHODS(GePointOnCurve2d);


    // Assignment operator.
    //
    GePointOnCurve2d& operator = (const GePointOnCurve2d& src);

    // Query functions.
    //
    const GeCurve2d* curve() const;
    double parameter() const;

    // Functions to evaluate a point.
    //
    GePoint2d point();
    GePoint2d point(double param);
    GePoint2d point(const GeCurve2d& crv, double param);


    // Set functions.
    //
    GePointOnCurve2d& setCurve(const GeCurve2d& crv);
    GePointOnCurve2d& setParameter(double param);
};



#endif