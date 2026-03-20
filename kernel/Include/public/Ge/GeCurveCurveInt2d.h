#ifndef GECURVECURVEINT2D_H
#define GECURVECURVEINT2D_H

#include "GeEntity2d.h"
#include "GeEntity2dMethods.h"
#include "GeCurve2d.h"
#include "GeInterval.h"
#include "GePoint2dArray.h"
#include "GeDoubleArray.h"
#include "GePointOnCurve2d.h"

class GeCurveCurveInt2d : public GeEntity2d
{
public:
    GeCurveCurveInt2d();
    GeCurveCurveInt2d(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeTol& tol = GeContext::gTol);
    GeCurveCurveInt2d(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeInterval& range1, const GeInterval& range2, const GeTol& tol = GeContext::gTol);
    GeCurveCurveInt2d(const GeCurveCurveInt2d& source);

    GEENTITY2D_METHODS(GeCurveCurveInt2d);

    const GeCurve2d* curve1() const;
    const GeCurve2d* curve2() const;
    void getIntRanges(GeInterval& range1, GeInterval& range2) const;
    GeTol tolerance() const;

    int numIntPoints() const;
    GePoint2d intPoint(int intNum) const;
    void getIntParams(int intNum, double& param1, double& param2) const;
    void getPointOnCurve1(int intNum, GePointOnCurve2d& intPnt) const;
    void getPointOnCurve2(int intNum, GePointOnCurve2d& intPnt) const;
    void getIntConfigs(int intNum, Ge::GeXConfig& config1wrt2, Ge::GeXConfig& config2wrt1) const;
    bool isTangential(int intNum) const;
    bool isTransversal(int intNum) const;
    double intPointTol(int intNum) const;

    int overlapCount() const;
    bool overlapDirection(int overlapNum) const;
    void getOverlapRanges(int overlapNum, GeInterval& range1, GeInterval& range2) const;

    void changeCurveOrder();
    GeCurveCurveInt2d& orderWrt1();
    GeCurveCurveInt2d& orderWrt2();

    GeCurveCurveInt2d& set(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeTol& tol = GeContext::gTol);
    GeCurveCurveInt2d& set(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeInterval& range1, const GeInterval& range2, const GeTol& tol = GeContext::gTol);

    GeCurveCurveInt2d& operator = (const GeCurveCurveInt2d& source);

private:
    void recalcIntersections();
    void appendIntersection(double param1, double param2, const GePoint2d& pnt);

private:
    const GeCurve2d* m_curve1;
    const GeCurve2d* m_curve2;
    GeInterval m_range1;
    GeInterval m_range2;
    GeTol m_tol;

    GePoint2dArray m_intPoints;
    GeDoubleArray m_params1;
    GeDoubleArray m_params2;
};

#endif
