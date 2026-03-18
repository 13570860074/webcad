
#ifndef GEPOINTONCURVE3D_H
#define GEPOINTONCURVE3D_H 

#include "GePointEnt3d.h"
#include "GeEntity3dMethods.h"


class GeCurve3d;
class GePointOnCurve3d : public GePointEnt3d
{
public:
    GePointOnCurve3d();
    GePointOnCurve3d(const GeCurve3d& crv);
    GePointOnCurve3d(const GeCurve3d& crv, double param);
    GePointOnCurve3d(const GePointOnCurve3d& src);
    GEENTITY3D_METHODS(GePointOnCurve3d);
    GEPOINTENT3D_METHODS(GePointOnCurve3d);


    // Assignment operator.
    //
    GePointOnCurve3d& operator = (const GePointOnCurve3d& src);

    // Query functions.
    //
    const GeCurve3d* curve() const;
    double parameter() const;

    // Functions to evaluate a point.
    //
    GePoint3d point();
    GePoint3d point(double param);
    GePoint3d point(const GeCurve3d& crv, double param);


    // Set functions.
    //
    GePointOnCurve3d& setCurve(const GeCurve3d& crv);
    GePointOnCurve3d& setParameter(double param);
};



#endif