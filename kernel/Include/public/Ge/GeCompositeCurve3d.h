#ifndef GECOMPOSITECURVE3D_H
#define GECOMPOSITECURVE3D_H

#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"
#include "GeCurve3dPtrArray.h"
#include "GeDoubleArray.h"
#include "GeVoidPointerArray.h"

class GeCompositeCurve3d : public GeCurve3d
{
public:
    GeCompositeCurve3d();
    GeCompositeCurve3d(const GeCompositeCurve3d& source);
    GeCompositeCurve3d(const GeCurve3dPtrArray& curveList);
    ~GeCompositeCurve3d();

    GEENTITY3D_METHODS(GeCompositeCurve3d);

    void getCurveList(GeCurve3dPtrArray& curveList) const;
    const GeCurve3dPtrArray& getCurveList() const;
    GeCompositeCurve3d& setCurveList(const GeCurve3dPtrArray& curveList);

    double globalToLocalParam(double param, int& crvNum) const;
    double localToGlobalParam(double param, int crvNum) const;

    GeCompositeCurve3d& operator = (const GeCompositeCurve3d& compCurve);

    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const override;
    GeCurve3d& reverseParam() override;

    double distanceTo(const GePoint3d& pnt) const override;
    double distanceTo(const GePoint3d& pnt, const GeTol& tol) const override;
    double distanceTo(const GeCurve3d& curve) const override;
    double distanceTo(const GeCurve3d& curve, const GeTol& tol) const override;

    GePoint3d closestPointTo(const GePoint3d& pnt) const override;
    GePoint3d closestPointTo(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv) const override;
    GePoint3d closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const override;

    double paramOf(const GePoint3d& pnt) const override;
    double paramOf(const GePoint3d& pnt, const GeTol& tol) const override;

    bool isClosed() const override;
    bool isClosed(const GeTol& tol) const override;

    double length() const override;
    double length(double fromParam, double toParam) const override;
    double length(double fromParam, double toParam, double tol) const override;
    double paramAtLength(double datumParam, double length) const override;
    double paramAtLength(double datumParam, double length, double tol) const override;

    bool hasStartPoint(GePoint3d& startPoint) const override;
    bool hasEndPoint(GePoint3d& endPoint) const override;
    GePoint3d evalPoint(double param) const override;

private:
    void clearOwnedCurves();
    void appendOwnedCurve(const GeCurve3d& curve);
    void ensureDefaultCurve();
    void rebuildParamMap();
    int findCurveIndex(double globalParam) const;

private:
    GeCurve3dPtrArray m_curveList;
    GeVoidPointerArray m_curves;
    GeDoubleArray m_curveStarts;
    GeDoubleArray m_curveSpans;
    double m_totalLength;
};

#endif
