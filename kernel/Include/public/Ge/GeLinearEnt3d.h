#ifndef GELINEARENT3D_H
#define GELINEARENT3D_H


#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"


class GeLinearEnt3d : public GeCurve3d
{
public:
    static GePoint3d vertical(const GePoint3d& pt, const GeLinearEnt3d& line);
    static GePoint3d vertical(const GePoint3d& pt, const GeLinearEnt3d& line, const GeTol& tol);

    // Intersection with other geometric objects.
    //
    virtual bool intersectWith(const GeLinearEnt3d& line, GePoint3d& intPnt) const;
    virtual bool intersectWith(const GeLinearEnt3d& line, GePoint3d& intPnt, const GeTol& tol) const;
    virtual bool intersectWith(const GePlanarEnt& plane, GePoint3d& intPnt) const;
    virtual bool intersectWith(const GePlanarEnt& plane, GePoint3d& intPnt, const GeTol& tol) const;

    // Projection-intersection with other geometric objects.
    //
    virtual bool projIntersectWith(const GeLinearEnt3d& line, const GeVector3d& projDir, GePoint3d& pntOnThisLine, GePoint3d& pntOnOtherLine) const;
    virtual bool projIntersectWith(const GeLinearEnt3d& line, const GeVector3d& projDir, GePoint3d& pntOnThisLine, GePoint3d& pntOnOtherLine, const GeTol& tol) const;

    // Containment tests.
    //
    virtual bool isOn(const GePoint3d& pnt) const;
    virtual bool isOn(const GePoint3d& pnt, const GeTol& tol) const;
    virtual bool isOn(double param) const;
    virtual bool isOn(double param, const GeTol& tol) const;
    virtual bool isOn(const GePlane& plane) const;
    virtual bool isOn(const GePlane& plane, const GeTol& tol) const;

    // Direction tests.
    //
    virtual bool isParallelTo(const GeLinearEnt3d& line) const;
    virtual bool isParallelTo(const GeLinearEnt3d& line, const GeTol& tol) const;
    virtual bool isParallelTo(const GePlanarEnt& plane) const;
    virtual bool isParallelTo(const GePlanarEnt& plane, const GeTol& tol) const;
    virtual bool isPerpendicularTo(const GeLinearEnt3d& line) const;
    virtual bool isPerpendicularTo(const GeLinearEnt3d& line, const GeTol& tol) const;
    virtual bool isPerpendicularTo(const GePlanarEnt& plane) const;
    virtual bool isPerpendicularTo(const GePlanarEnt& plane, const GeTol& tol) const;

    // Test if two lines are colinear.
    //
    virtual bool isColinearTo(const GeLinearEnt3d& line) const;
    virtual bool isColinearTo(const GeLinearEnt3d& line, const GeTol& tol) const;
    // Perpendicular through a given point
    //
    virtual void getPerpPlane(const GePoint3d& pnt, GePlane& plane) const;

    // Definition of line.
    //
    GePoint3d pointOnLine() const;
    GeVector3d direction() const;
    void getLine(GeLine3d& line) const;

    // Assignment operator.
    //
    GeLinearEnt3d& operator = (const GeLinearEnt3d& line);

em_protected:
    GeLinearEnt3d();
    GeLinearEnt3d(const GeLinearEnt3d&);
};


#endif

