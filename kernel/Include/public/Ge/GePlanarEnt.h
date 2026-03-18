#ifndef GEPLANAR_H
#define GEPLANAR_H

#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"



class GeLineSeg3d;
class GeLinearEnt3d;
class GeCircArc3d;
class GePlanarEnt : public GeSurface
{
public:
    // Intersection
    //
    bool intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& pnt) const;
    bool intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& pnt, const GeTol& tol) const;
    // Closest point
    //
    GePoint3d closestPointToLinearEnt(const GeLinearEnt3d& line, GePoint3d& pointOnLine) const;
    GePoint3d closestPointToLinearEnt(const GeLinearEnt3d& line, GePoint3d& pointOnLine, const GeTol& tol) const;
    GePoint3d closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln) const;
    GePoint3d closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const;
    // Direction tests.
    //
    bool isParallelTo(const GeLinearEnt3d& linEnt) const;
    bool isParallelTo(const GeLinearEnt3d& linEnt, const GeTol& tol) const;
    bool isParallelTo(const GePlanarEnt& otherPlnEnt) const;
    bool isParallelTo(const GePlanarEnt& otherPlnEnt, const GeTol& tol) const;
    bool isPerpendicularTo(const GeLinearEnt3d& linEnt) const;
    bool isPerpendicularTo(const GeLinearEnt3d& linEnt, const GeTol& tol) const;
    bool isPerpendicularTo(const GePlanarEnt& linEnt) const;
    bool isPerpendicularTo(const GePlanarEnt& linEnt, const GeTol& tol) const;

    // Point set equality.
    //
    bool isCoplanarTo(const GePlanarEnt& otherPlnEnt) const;
    bool isCoplanarTo(const GePlanarEnt& otherPlnEnt, const GeTol& tol) const;

    // Get methods.
    //
    void get(GePoint3d& origin, GeVector3d& uVec, GeVector3d& vVec) const;

    // Geometric properties.
    //
    GePoint3d pointOnPlane() const;
    GeVector3d normal() const;
    void getCoefficients(double& a, double& b, double& c, double& d) const;
    void getCoordSystem(GePoint3d& origin, GeVector3d& axis1,GeVector3d& axis2) const;
    // Assignment operator.
    //
    GePlanarEnt& operator = (const GePlanarEnt& src);

protected:
    GePlanarEnt();
    GePlanarEnt(const GePlanarEnt&);
};



#endif 

