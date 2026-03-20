#ifndef GEPOLYLINE3D_H
#define GEPOLYLINE3D_H

#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"
#include "GePoint3dArray.h"
#include "GeKnotVector.h"

class GePolyline3d : public GeCurve3d
{
public:
    GePolyline3d();
    GePolyline3d(const GePolyline3d& source);
    GePolyline3d(const GePoint3dArray& fitPoints);
    GePolyline3d(const GeKnotVector& knots, const GePoint3dArray& points);
    GePolyline3d(const GeCurve3d& crv, double approxEps);
    GePolyline3d(int numPoints, const GePoint3d* pPoints);

    GEENTITY3D_METHODS(GePolyline3d);

    int numFitPoints() const;
    GePoint3d fitPointAt(int fitPointIndex) const;
    GePolyline3d& setFitPointAt(int fitPointIndex, const GePoint3d& point);

    GePolyline3d& set(const GePoint3dArray& fitPoints);
    GePolyline3d& set(const GeKnotVector& knots, const GePoint3dArray& points);

    GePolyline3d& operator = (const GePolyline3d& polyline);

    GePoint3d evalPointSeg(double param, int& numSeg) const;

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
    void setDefault();
    void rebuildKnotsFromPoints();
    int segmentIndexAtParam(double param) const;
    GePoint3d closestPointOnSegment(const GePoint3d& pnt, int segIndex, double& segParam) const;

private:
    GePoint3dArray m_points;
    GeKnotVector m_knots;
};

#endif
