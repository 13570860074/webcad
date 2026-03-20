#ifndef GECURVECURVEINT3D_H
#define GECURVECURVEINT3D_H

#include "GeEntity3d.h"
#include "GeEntity3dMethods.h"
#include "GeCurve3d.h"
#include "GeInterval.h"
#include "GePoint3dArray.h"
#include "GeDoubleArray.h"
#include "GePointOnCurve3d.h"

class GeCurveCurveInt3d : public GeEntity3d
{
public:
    GeCurveCurveInt3d();
    GeCurveCurveInt3d(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeVector3d& planeNormal = GeVector3d::kIdentity, const GeTol& tol = GeContext::gTol);
    GeCurveCurveInt3d(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeInterval& range1, const GeInterval& range2, const GeVector3d& planeNormal = GeVector3d::kIdentity, const GeTol& tol = GeContext::gTol);
    GeCurveCurveInt3d(const GeCurveCurveInt3d& source);

    GEENTITY3D_METHODS(GeCurveCurveInt3d);

    const GeCurve3d* curve1() const;
    const GeCurve3d* curve2() const;
    void getIntRanges(GeInterval& range1, GeInterval& range2) const;
    GeVector3d planeNormal() const;
    GeTol tolerance() const;

    int numIntPoints() const;
    GePoint3d intPoint(int intNum) const;
    void getIntParams(int intNum, double& param1, double& param2) const;
    void getPointOnCurve1(int intNum, GePointOnCurve3d& intPnt) const;
    void getPointOnCurve2(int intNum, GePointOnCurve3d& intPnt) const;
    void getIntConfigs(int intNum, Ge::GeXConfig& config1wrt2, Ge::GeXConfig& config2wrt1) const;
    bool isTangential(int intNum) const;
    bool isTransversal(int intNum) const;
    double intPointTol(int intNum) const;

    int overlapCount() const;
    bool overlapDirection(int overlapNum) const;
    void getOverlapRanges(int overlapNum, GeInterval& range1, GeInterval& range2) const;

    void changeCurveOrder();
    GeCurveCurveInt3d& orderWrt1();
    GeCurveCurveInt3d& orderWrt2();

    GeCurveCurveInt3d& set(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeVector3d& planeNormal = GeVector3d::kIdentity, const GeTol& tol = GeContext::gTol);
    GeCurveCurveInt3d& set(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeInterval& range1, const GeInterval& range2, const GeVector3d& planeNormal = GeVector3d::kIdentity, const GeTol& tol = GeContext::gTol);

    GeCurveCurveInt3d& operator = (const GeCurveCurveInt3d& source);

private:
    void recalcIntersections();
    void appendIntersection(double param1, double param2, const GePoint3d& pnt);

private:
    const GeCurve3d* m_curve1;
    const GeCurve3d* m_curve2;
    GeInterval m_range1;
    GeInterval m_range2;
    GeVector3d m_planeNormal;
    GeTol m_tol;

    GePoint3dArray m_intPoints;
    GeDoubleArray m_params1;
    GeDoubleArray m_params2;
};

#endif
