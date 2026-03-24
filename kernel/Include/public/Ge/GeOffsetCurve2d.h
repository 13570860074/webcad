#ifndef GEOFFSETCURVE2D_H
#define GEOFFSETCURVE2D_H

#include "GeCurve2d.h"
#include "GeEntity2dMethods.h"
#include "GeInterval.h"
#include "GeMatrix2d.h"

class GeOffsetCurve2d : public GeCurve2d
{
public:
    GeOffsetCurve2d();
    GeOffsetCurve2d(const GeCurve2d& baseCurve, double offsetDistance, bool makeCopy = false);
    GeOffsetCurve2d(const GeOffsetCurve2d& source);
    ~GeOffsetCurve2d();

    GEENTITY2D_METHODS(GeOffsetCurve2d);

    const GeCurve2d* curve() const;
    double offsetDistance() const;
    bool paramDirection() const;
    GeMatrix2d transformation() const;

    GeOffsetCurve2d& setCurve(const GeCurve2d& baseCurve, bool makeCopy = false);
    GeOffsetCurve2d& setOffsetDistance(double distance);

    GeOffsetCurve2d& operator = (const GeOffsetCurve2d& offsetCurve);

    bool setInterval(const GeInterval& range) override;
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
    void getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const override;

private:
    void clearOwnedCurve();
    GeCurve2d* mutableCurve();
    double mapParam(double param) const;
    GePoint2d evalLocalPoint(double param, const GeTol& tol) const;
    GeVector2d tangentAt(double param, const GeTol& tol) const;

private:
    GeCurve2d* m_pCurve;
    bool m_isOwner;
    bool m_paramDir;
    double m_offsetDist;
    GeMatrix2d m_transform;
    GeInterval m_interval;
    bool m_hasCustomInterval;
};

#endif
