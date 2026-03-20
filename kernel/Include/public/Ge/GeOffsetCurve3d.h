#ifndef GEOFFSETCURVE3D_H
#define GEOFFSETCURVE3D_H

#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"

class GeOffsetCurve3d : public GeCurve3d
{
public:
    GeOffsetCurve3d();
    GeOffsetCurve3d(const GeCurve3d& baseCurve, const GeVector3d& planeNormal, double offsetDistance, bool makeCopy = false);
    GeOffsetCurve3d(const GeOffsetCurve3d& source);
    ~GeOffsetCurve3d();

    GEENTITY3D_METHODS(GeOffsetCurve3d);

    const GeCurve3d* curve() const;
    GeVector3d normal() const;
    double offsetDistance() const;
    bool paramDirection() const;
    GeMatrix3d transformation() const;

    GeOffsetCurve3d& setCurve(const GeCurve3d& baseCurve, bool makeCopy = false);
    GeOffsetCurve3d& setNormal(const GeVector3d& planeNormal);
    GeOffsetCurve3d& setOffsetDistance(double distance);

    GeOffsetCurve3d& operator = (const GeOffsetCurve3d& offsetCurve);

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
    void clearOwnedCurve();
    GeCurve3d* mutableCurve();
    double mapParam(double param) const;
    GePoint3d evalLocalPoint(double param, const GeTol& tol) const;
    GeVector3d tangentAt(double param, const GeTol& tol) const;

private:
    GeCurve3d* m_pCurve;
    bool m_isOwner;
    bool m_paramDir;
    double m_offsetDist;
    GeVector3d m_planeNormal;
    GeMatrix3d m_transform;
};

#endif
