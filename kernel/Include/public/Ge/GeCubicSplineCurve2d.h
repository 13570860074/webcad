#ifndef GECUBICSPLINECURVE2D_H
#define GECUBICSPLINECURVE2D_H

#include "GeSplineEnt2d.h"
#include "GeVector2dArray.h"

class GeCubicSplineCurve2d : public GeSplineEnt2d
{
public:
    GeCubicSplineCurve2d();
    GeCubicSplineCurve2d(const GeCubicSplineCurve2d& source);
    GeCubicSplineCurve2d(const GePoint2dArray& fitPnts, const GeTol& tol = GeContext::gTol);
    GeCubicSplineCurve2d(const GePoint2dArray& fitPnts, const GeVector2d& startDeriv, const GeVector2d& endDeriv, const GeTol& tol = GeContext::gTol);
    GeCubicSplineCurve2d(const GeCurve2d& curve, double tol = GeContext::gTol.equalPoint());
    GeCubicSplineCurve2d(const GeKnotVector& knots, const GePoint2dArray& fitPnts, const GeVector2dArray& firstDerivs, bool isPeriodic = false);

    GEENTITY2D_METHODS(GeCubicSplineCurve2d);

    int numFitPoints() const;
    GePoint2d fitPointAt(int fitPointIndex) const;
    GeCubicSplineCurve2d& setFitPointAt(int fitPointIndex, const GePoint2d& point);

    GeVector2d firstDerivAt(int fitPointIndex) const;
    GeCubicSplineCurve2d& setFirstDerivAt(int fitPointIndex, const GeVector2d& deriv);

    GeCubicSplineCurve2d& operator = (const GeCubicSplineCurve2d& spline);

private:
    void rebuildBaseData();

private:
    GePoint2dArray m_fitPoints;
    GeVector2dArray m_firstDerivs;
    bool m_isPeriodic;
};

#endif