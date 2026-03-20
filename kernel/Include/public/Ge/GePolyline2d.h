#ifndef GEPOLYLINE2D_H
#define GEPOLYLINE2D_H

#include "GeCurve2d.h"
#include "GeEntity2dMethods.h"
#include "GePoint2dArray.h"
#include "GeKnotVector.h"

class GePolyline2d : public GeCurve2d
{
public:
    GePolyline2d();
    GePolyline2d(const GePolyline2d& source);
    GePolyline2d(const GePoint2dArray& fitPoints);
    GePolyline2d(const GeKnotVector& knots, const GePoint2dArray& points);
    GePolyline2d(const GeCurve2d& crv, double approxEps);

    GEENTITY2D_METHODS(GePolyline2d);

    int numFitPoints() const;
    GePoint2d fitPointAt(int fitPointIndex) const;
    GePolyline2d& setFitPointAt(int fitPointIndex, const GePoint2d& point);

    GePolyline2d& set(const GePoint2dArray& fitPoints);
    GePolyline2d& set(const GeKnotVector& knots, const GePoint2dArray& points);

    GePolyline2d& operator = (const GePolyline2d& polyline);

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
    void setDefault();
    void rebuildKnotsFromPoints();
    int segmentIndexAtParam(double param) const;
    GePoint2d closestPointOnSegment(const GePoint2d& pnt, int segIndex, double& segParam) const;

private:
    GePoint2dArray m_points;
    GeKnotVector m_knots;
};

#endif
