#ifndef GECUBICSPLINECURVE3D_H
#define GECUBICSPLINECURVE3D_H

#include "GeSplineEnt3d.h"
#include "GeVector3dArray.h"

class GeCubicSplineCurve3d : public GeSplineEnt3d
{
public:
    GeCubicSplineCurve3d();
    GeCubicSplineCurve3d(const GeCubicSplineCurve3d& source);
    GeCubicSplineCurve3d(const GePoint3dArray& fitPnts, const GeTol& tol = GeContext::gTol);
    GeCubicSplineCurve3d(const GePoint3dArray& fitPnts, const GeVector3d& startDeriv, const GeVector3d& endDeriv, const GeTol& tol = GeContext::gTol);
    GeCubicSplineCurve3d(const GeCurve3d& curve, double epsilon = GeContext::gTol.equalPoint());
    GeCubicSplineCurve3d(const GeKnotVector& knots, const GePoint3dArray& fitPnts, const GeVector3dArray& firstDerivs, bool isPeriodic = false);

    GEENTITY3D_METHODS(GeCubicSplineCurve3d);

    int numFitPoints() const;
    GePoint3d fitPointAt(int fitPointIndex) const;
    GeCubicSplineCurve3d& setFitPointAt(int fitPointIndex, const GePoint3d& point);

    GeVector3d firstDerivAt(int fitPointIndex) const;
    GeCubicSplineCurve3d& setFirstDerivAt(int fitPointIndex, const GeVector3d& deriv);

    GeCubicSplineCurve3d& operator = (const GeCubicSplineCurve3d& spline);

private:
    void rebuildBaseData();

private:
    GePoint3dArray m_fitPoints;
    GeVector3dArray m_firstDerivs;
    bool m_isPeriodic;
};

#endif