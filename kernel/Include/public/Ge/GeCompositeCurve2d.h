#ifndef GECOMPOSITECURVE2D_H
#define GECOMPOSITECURVE2D_H

#include "GeCurve2d.h"
#include "GeEntity2dMethods.h"
#include "GeCurve2dPtrArray.h"
#include "GeDoubleArray.h"
#include "GeVoidPointerArray.h"

class GeCompositeCurve2d : public GeCurve2d
{
public:
    GeCompositeCurve2d();
    GeCompositeCurve2d(const GeCompositeCurve2d& source);
    GeCompositeCurve2d(const GeCurve2dPtrArray& curveList);
    ~GeCompositeCurve2d();

    GEENTITY2D_METHODS(GeCompositeCurve2d);

    void getCurveList(GeCurve2dPtrArray& curveList) const;
    const GeCurve2dPtrArray& getCurveList() const;
    GeCompositeCurve2d& setCurveList(const GeCurve2dPtrArray& curveList);

    double globalToLocalParam(double param, int& crvNum) const;
    double localToGlobalParam(double param, int crvNum) const;

    GeCompositeCurve2d& operator = (const GeCompositeCurve2d& compCurve);

    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const override;
    GeCurve2d& reverseParam() override;

    double distanceTo(const GePoint2d& pnt) const override;
    double distanceTo(const GePoint2d& pnt, const GeTol& tol) const override;
    double distanceTo(const GeCurve2d& curve) const override;
    double distanceTo(const GeCurve2d& curve, const GeTol& tol) const override;

    GePoint2d closestPointTo(const GePoint2d& pnt) const override;
    GePoint2d closestPointTo(const GePoint2d& pnt, const GeTol& tol) const override;
    GePoint2d closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const override;
    GePoint2d closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const override;

    double paramOf(const GePoint2d& pnt) const override;
    double paramOf(const GePoint2d& pnt, const GeTol& tol) const override;

    bool isClosed() const override;
    bool isClosed(const GeTol& tol) const override;

    double length() const override;
    double length(double fromParam, double toParam) const override;
    double length(double fromParam, double toParam, double tol) const override;
    double paramAtLength(double datumParam, double length) const override;
    double paramAtLength(double datumParam, double length, double tol) const override;

    bool hasStartPoint(GePoint2d& startPoint) const override;
    bool hasEndPoint(GePoint2d& endPoint) const override;
    GePoint2d evalPoint(double param) const override;

private:
    void clearOwnedCurves();
    void appendOwnedCurve(const GeCurve2d& curve);
    void ensureDefaultCurve();
    void rebuildParamMap();
    int findCurveIndex(double globalParam) const;

private:
    GeCurve2dPtrArray m_curveList;
    GeVoidPointerArray m_curves;
    GeDoubleArray m_curveStarts;
    GeDoubleArray m_curveSpans;
    double m_totalLength;
};

#endif
