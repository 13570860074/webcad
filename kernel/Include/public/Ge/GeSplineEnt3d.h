#ifndef GESPLINEENT3D_H
#define GESPLINEENT3D_H

#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"
#include "GeKnotVector.h"

class GeSplineEnt3d : public GeCurve3d
{
public:
    GEENTITY3D_METHODS(GeSplineEnt3d);

    bool isRational() const;
    int degree() const;
    int order() const;
    int numKnots() const;
    const GeKnotVector& knots() const;
    int numControlPoints() const;
    int continuityAtKnot(int knotIndex, const GeTol& tol = GeContext::gTol) const;
    double startParam() const;
    double endParam() const;
    GePoint3d startPoint() const;
    GePoint3d endPoint() const;
    bool hasFitData() const;
    double knotAt(int knotIndex) const;
    GeSplineEnt3d& setKnotAt(int knotIndex, double val);
    GePoint3d controlPointAt(int controlPointIndex) const;
    GeSplineEnt3d& setControlPointAt(int controlPointIndex, const GePoint3d& point);

    GeSplineEnt3d& operator = (const GeSplineEnt3d& spline);

em_protected:
    GeSplineEnt3d();
    GeSplineEnt3d(const GeSplineEnt3d& spline);

protected:
    bool m_isRational;
    int m_degree;
    bool m_hasFitData;
    GeKnotVector m_knots;
    GePoint3dArray m_controlPoints;
};

#endif
