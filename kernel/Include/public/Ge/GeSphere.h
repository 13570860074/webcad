#ifndef GESPHERE_H
#define GESPHERE_H

#include "adesk.h"
#include "GeSurface.h"

class GeSphere : public GeSurface
{
public:
    GeSphere();
    GeSphere(double radius, const GePoint3d &center);
    GeSphere(double radius, const GePoint3d &center,
             const GeVector3d &northAxis, const GeVector3d &refAxis,
             double startAngleU, double endAngleU,
             double startAngleV, double endAngleV);
    GeSphere(const GeSphere &sphere);

    // Geometric properties.
    //
    double radius() const;
    GePoint3d center() const;
    void getAnglesInU(double &start, double &end) const;
    void getAnglesInV(double &start, double &end) const;
    GeVector3d northAxis() const;
    GeVector3d refAxis() const;
    GePoint3d northPole() const;
    GePoint3d southPole() const;
    Adesk::Boolean isOuterNormal() const;
    Adesk::Boolean isClosed(const GeTol &tol = GeContext::gTol) const;

    GeSphere &setRadius(double);
    GeSphere &setAnglesInU(double start, double end);
    GeSphere &setAnglesInV(double start, double end);
    GeSphere &set(double radius, const GePoint3d &center);
    GeSphere &set(double radius, const GePoint3d &center,
                  const GeVector3d &northAxis,
                  const GeVector3d &refAxis,
                  double startAngleU,
                  double endAngleU,
                  double startAngleV,
                  double endAngleV);
    // Assignment operator.
    //
    GeSphere &operator=(const GeSphere &sphere);

    // Intersection with a linear entity
    //
    Adesk::Boolean intersectWith(const GeLinearEnt3d &, int &intn,
                                 GePoint3d &p1, GePoint3d &p2,
                                 const GeTol &tol = GeContext::gTol) const;
};

#endif
