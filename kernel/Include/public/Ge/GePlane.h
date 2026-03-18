#ifndef GEPLANE_H
#define GEPLANE_H

#include "GePlanarEnt.h"
#include "GeEntity3dMethods.h"

class GeBoundedPlane;
class GeLine3d;

class GePlane : public GePlanarEnt
{
public:
    // Global plane objects.
    //
    GE_STATIC_EXPORT static const GePlane kXYPlane;
    GE_STATIC_EXPORT static const GePlane kYZPlane;
    GE_STATIC_EXPORT static const GePlane kZXPlane;

    GePlane();
    GePlane(const GePlane& src);
    GePlane(const GePoint3d& origin, const GeVector3d& normal);
    GePlane(const GePoint3d& pntU, const GePoint3d& org, const GePoint3d& pntV);
    GePlane(const GePoint3d& org, const GeVector3d& uAxis, const GeVector3d& vAxis);
    GePlane(double a, double b, double c, double d);
    GEENTITY3D_METHODS(GePlane);
    GESURFACE_METHODS(GePlane);
    GEPLANARENT_METHODS(GePlane);

    // Signed distance from a point to a plane.
    //
    double signedDistanceTo(const GePoint3d& pnt) const;
    bool isLeftHanded() const;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const;

    // Intersection
    //
    bool intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt) const;
    bool intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt, const GeTol& tol) const;
    bool intersectWith(const GePlane& otherPln, GeLine3d& resultLine) const;
    bool intersectWith(const GePlane& otherPln, GeLine3d& resultLine, const GeTol& tol) const;
    bool intersectWith(const GeBoundedPlane& bndPln, GeLineSeg3d& resultLineSeg) const;
    bool intersectWith(const GeBoundedPlane& bndPln, GeLineSeg3d& resultLineSeg, const GeTol& tol) const;

    // Geometry redefinition.
    //
    GePlane& set(const GePoint3d& pnt, const GeVector3d& normal);
    GePlane& set(const GePoint3d& pntU, const GePoint3d& org, const GePoint3d& pntV);
    GePlane& set(double a, double b, double c, double d);
    GePlane& set(const GePoint3d& org, const GeVector3d& uAxis, const GeVector3d& vAxis);
    // Assignment operator.
    //
    GePlane& operator = (const GePlane& src);
};


#endif 

