#ifndef GESPLINEENT2D_H
#define GESPLINEENT2D_H

#include "GeCurve2d.h"
#include "GeEntity2dMethods.h"
#include "GeKnotVector.h"

class GeSplineEnt2d : public GeCurve2d
{
public:
    GEENTITY2D_METHODS(GeSplineEnt2d);

    bool isRational() const;
    int degree() const;
    int order() const;
    int numKnots() const;
    const GeKnotVector& knots() const;
    int numControlPoints() const;
    int continuityAtKnot(int knotIndex, const GeTol& tol = GeContext::gTol) const;
    double startParam() const;
    double endParam() const;
    GePoint2d startPoint() const;
    GePoint2d endPoint() const;
    bool hasFitData() const;
    double knotAt(int knotIndex) const;
    GeSplineEnt2d& setKnotAt(int knotIndex, double val);
    GePoint2d controlPointAt(int controlPointIndex) const;
    GeSplineEnt2d& setControlPointAt(int controlPointIndex, const GePoint2d& point);

    GeSplineEnt2d& operator = (const GeSplineEnt2d& spline);

em_protected:
    GeSplineEnt2d();
    GeSplineEnt2d(const GeSplineEnt2d& spline);

protected:
    bool m_isRational;
    int m_degree;
    bool m_hasFitData;
    GeKnotVector m_knots;
    GePoint2dArray m_controlPoints;
};

#endif
