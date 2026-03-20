#ifndef GESURFSURFINT_H
#define GESURFSURFINT_H

#include "GeCurve2d.h"
#include "GeCurve3d.h"
#include "GeEntity3d.h"
#include "GeEntity3dMethods.h"
#include "GePoint2dArray.h"
#include "GePoint3dArray.h"
#include "GeSurface.h"

class GeSurfSurfInt : public GeEntity3d
{
public:
    GeSurfSurfInt();
    GeSurfSurfInt(const GeSurface& srf1, const GeSurface& srf2, const GeTol& tol = GeContext::gTol);
    GeSurfSurfInt(const GeSurfSurfInt& source);

    GEENTITY3D_METHODS(GeSurfSurfInt);

    const GeSurface* surface1() const;
    const GeSurface* surface2() const;
    GeTol tolerance() const;

    int numResults(Ge::GeIntersectError& status) const;
    int numIntPoints(Ge::GeIntersectError& status) const;
    GeCurve3d* intCurve(int intNum, bool isExternal, Ge::GeIntersectError& status) const;
    GeCurve2d* intParamCurve(int intNum, bool isExternal, bool isFirst, Ge::GeIntersectError& status) const;
    GePoint3d intPoint(int intNum, Ge::GeIntersectError& status) const;
    void getIntPointParams(int intNum, GePoint2d& param1, GePoint2d& param2, Ge::GeIntersectError& status) const;

    void getIntConfigs(
        int intNum,
        Ge::ssiConfig& surf1Left,
        Ge::ssiConfig& surf1Right,
        Ge::ssiConfig& surf2Left,
        Ge::ssiConfig& surf2Right,
        Ge::ssiType& intType,
        int& dim,
        Ge::GeIntersectError& status) const;

    int numIntCurves(Ge::GeIntersectError& status) const;
    int getDimension(int intNum, Ge::GeIntersectError& status) const;
    Ge::ssiType getType(int intNum, Ge::GeIntersectError& status) const;
    bool haveOverlap(Ge::GeIntersectError& status) const;

    GeSurfSurfInt& set(const GeSurface& srf1, const GeSurface& srf2, const GeTol& tol = GeContext::gTol);
    GeSurfSurfInt& operator = (const GeSurfSurfInt& surfSurfInt);

private:
    void recalcIntersections();
    void appendIntersection(const GePoint3d& point, const GePoint2d& param1, const GePoint2d& param2);

private:
    const GeSurface* m_surface1;
    const GeSurface* m_surface2;
    GeTol m_tol;

    GePoint3dArray m_intPoints;
    GePoint2dArray m_params1;
    GePoint2dArray m_params2;
};

#endif