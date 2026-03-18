#ifndef GESURF_H
#define GESURF_H


#include "GeEntity3d.h"
#include "GeVector3d.h"
#include "GeVector3dArray.h"


class GePoint2d;
class GeCurve3d;
class GePointOnCurve3d;
class GePointOnSurface;
class GePointOnSurfaceData;
class GeInterval;
class GeSurface : public GeEntity3d
{
public:
    // Parameter related.
    //
    virtual GePoint2d paramOf(const GePoint3d& pnt) const;
    virtual GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const;

    // Point containment
    //
    virtual bool isOn(const GePoint3d& pnt) const;
    virtual bool isOn(const GePoint3d& pnt, const GeTol& tol) const;
    virtual bool isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const;
    virtual bool isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const;
    // Operations.
    //
    virtual GePoint3d closestPointTo(const GePoint3d& pnt) const;
    virtual GePoint3d closestPointTo(const GePoint3d& pnt, const GeTol& tol) const;

    virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const;
    virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const;

    virtual double distanceTo(const GePoint3d& pnt) const;
    virtual double distanceTo(const GePoint3d& pnt, const GeTol& tol) const;

    virtual bool isNormalReversed() const;
    virtual bool isLeftHanded() const;
    virtual GeSurface& reverseNormal();
    virtual void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const;

    // Assignment operator.
    //
    virtual GeSurface& operator = (const GeSurface& otherSurface);

    // Evaluators.
    // Derivative arrays are indexed partialU, partialV followed by
    // the mixed partial.
    //
    virtual GePoint3d evalPoint(const GePoint2d& param) const;

em_protected:
    GeSurface();
    GeSurface(const GeSurface&);
};



#endif

