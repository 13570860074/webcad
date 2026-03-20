#ifndef GECURVESURFINT_H
#define GECURVESURFINT_H

#include "GeCurve3d.h"
#include "GeDoubleArray.h"
#include "GeEntity3d.h"
#include "GeEntity3dMethods.h"
#include "GePoint2dArray.h"
#include "GePoint3dArray.h"
#include "GeSurface.h"

class GePointOnCurve3d;
class GePointOnSurface;

class GeCurveSurfInt : public GeEntity3d
{
public:
    GeCurveSurfInt();
    GeCurveSurfInt(const GeCurve3d& curve, const GeSurface& surf, const GeTol& tol = GeContext::gTol);
    GeCurveSurfInt(const GeCurveSurfInt& source);

    GEENTITY3D_METHODS(GeCurveSurfInt);

    const GeCurve3d* curve() const;
    const GeSurface* surface() const;
    GeTol tolerance() const;

    int numResults(Ge::GeIntersectError& status) const;
    int getDimension(int intNum, Ge::GeIntersectError& status) const;

    int numIntPoints(Ge::GeIntersectError& status) const;
    GePoint3d intPoint(int intNum, Ge::GeIntersectError& status) const;

    void getIntParams(int intNum, double& param1, GePoint2d& param2, Ge::GeIntersectError& status) const;
    void getPointOnCurve(int intNum, GePointOnCurve3d& intPnt, Ge::GeIntersectError& status) const;
    void getPointOnSurface(int intNum, GePointOnSurface& intPnt, Ge::GeIntersectError& status) const;

    void getIntConfigs(int intNum, Ge::csiConfig& lower, Ge::csiConfig& higher, bool& smallAngle, Ge::GeIntersectError& status) const;

    int overlapCount(Ge::GeIntersectError& status) const;
    void getOverlapRange(int intNum, GeInterval& range, Ge::GeIntersectError& status) const;

    GeCurve2d* intParamCurve(int intNum, bool isExternal, Ge::GeIntersectError& status) const;

    GeCurveSurfInt& set(const GeCurve3d& crv, const GeSurface& surface, const GeTol& tol = GeContext::gTol);
    GeCurveSurfInt& operator = (const GeCurveSurfInt& crvSurfInt);

private:
    void recalcIntersections();
    void appendIntersection(double curveParam, const GePoint2d& surfParam, const GePoint3d& point);

private:
    const GeCurve3d* m_curve;
    const GeSurface* m_surface;
    GeTol m_tol;

    GePoint3dArray m_intPoints;
    GeDoubleArray m_curveParams;
    GePoint2dArray m_surfaceParams;
};

#endif