#ifndef GELINEARENT2D_H
#define GELINEARENT2D_H /*!DOM*/


#include "GeCurve2d.h"
#include "GeEntity2dMethods.h"


class GeLinearEnt2d : public GeCurve2d
{
public:
    static GePoint2d vertical(const GePoint2d& pt, const GeLinearEnt2d& line);
    static GePoint2d vertical(const GePoint2d& pt, const GeLinearEnt2d& line, const GeTol& tol);
    static void generalFormula(const GeLinearEnt2d& line, double& a, double& b, double& c);
    // Intersection with other geometric objects.
    //
    virtual bool intersectWith(const GeLinearEnt2d& line, GePoint2d& intPnt) const;
    virtual bool intersectWith(const GeLinearEnt2d& line, GePoint2d& intPnt, const GeTol& tol) const;

    // Direction tests.
    //
    bool isParallelTo(const GeLinearEnt2d& line) const;
    bool isParallelTo(const GeLinearEnt2d& line, const GeTol& tol) const;
    bool isPerpendicularTo(const GeLinearEnt2d& line) const;
    bool isPerpendicularTo(const GeLinearEnt2d& line, const GeTol& tol) const;
    // Test if two lines are colinear.
    //
    bool isColinearTo(const GeLinearEnt2d& line) const;
    bool isColinearTo(const GeLinearEnt2d& line, const GeTol& tol) const;
    // Perpendicular through a given point
    //
    void getPerpLine(const GePoint2d& pnt, GeLine2d& perpLine) const;

    // Definition of line.
    //
    GePoint2d pointOnLine() const;
    GeVector2d direction() const;
    void getLine(GeLine2d& line) const;

    // Assignment operator.
    //
    GeLinearEnt2d& operator = (const GeLinearEnt2d& line);

em_protected:
    GeLinearEnt2d();
    GeLinearEnt2d(const GeLinearEnt2d&);
};


#endif

