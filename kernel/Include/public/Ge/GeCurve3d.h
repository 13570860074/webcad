#ifndef GECURVE3D_H
#define GECURVE3D_H



#include "GeIntArray.h"
#include "GeDoubleArray.h"
#include "GePoint3dArray.h"
#include "GeVector3dArray.h"
#include "GeVoidPointerArray.h"
#include "GeEntity3d.h"


class GeCurve2d;
class GeSurface;
class GePoint3d;
class GePlane;
class GeVector3d;
class GeLinearEnt3d;
class GeLine3d;
class GePointOnCurve3d;
class GePointOnSurface;
class GeInterval;
class GeMatrix3d;
class GePointOnCurve3dData;
class GeBoundBlock3d;
class GeCurve3d : public GeEntity3d
{
public:

    // Distance to other geometric objects.
    //
    virtual double distanceTo(const GePoint3d& pnt) const;
    virtual double distanceTo(const GePoint3d& pnt, const GeTol& tol) const;
    virtual double distanceTo(const GeCurve3d& curve) const;
    virtual double distanceTo(const GeCurve3d& curve, const GeTol& tol) const;

    // Return the point on this object that is closest to the other object.
    // These methods return point on this curve as a simple 3d point.
    //
    virtual GePoint3d closestPointTo(const GePoint3d& pnt) const;
    virtual GePoint3d closestPointTo(const GePoint3d& pnt, const GeTol& tol) const;
    virtual GePoint3d closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv) const;
    virtual GePoint3d closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;

    // Alternate signatures for above functions.  These methods return point
    // on this curve as an GePointOnCurve3d.
    //
    virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const;
    virtual void getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const;
    virtual void getClosestPointTo(const GeCurve3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;
    virtual void getClosestPointTo(const GeCurve3d& curve3d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;

    // Return closest points when projected in a given direction.
    // These methods return point on this curve as a simple 3d point.
    //
    virtual GePoint3d projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const;
    virtual GePoint3d projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const;
    virtual GePoint3d projClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const;
    virtual GePoint3d projClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const;

    // Alternate signatures for above functions.  These methods return point
    // on this curve as an GePointOnCurve3d.
    //
    virtual void getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const;
    virtual void getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const;
    virtual void getProjClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const;
    virtual void getProjClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const;

    // Return point on curve whose normal vector passes thru input point.
    // Second parameter contains initial guess value and also 
    // contains output point.

    // Returns true or false depending on whether a normal point was found.
        //
    virtual bool getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const;
    virtual bool getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const;

    // Return oriented bounding box of curve.
    //
    virtual GeBoundBlock3d boundBlock() const;
    virtual GeBoundBlock3d boundBlock(const GeInterval& range) const;

    // Return bounding box whose sides are parallel to coordinate axes.
    //
    virtual GeBoundBlock3d orthoBoundBlock() const;
    virtual GeBoundBlock3d orthoBoundBlock(const GeInterval& range) const;

    // Project methods.
    //
    virtual GeEntity3d* project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const;
    virtual GeEntity3d* project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const;
    virtual GeEntity3d* orthoProject(const GePlane& projectionPlane) const;
    virtual GeEntity3d* orthoProject(const GePlane& projectionPlane, const GeTol& tol) const;

    // Tests if point is on curve.
    //
    virtual bool isOn(const GePoint3d& pnt) const;
    virtual bool isOn(const GePoint3d& pnt, const GeTol& tol) const;
    virtual bool isOn(const GePoint3d& pnt, double& param) const;
    virtual bool isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const;
    virtual bool isOn(double param) const;
    virtual bool isOn(double param, const GeTol& tol) const;

    // Parameter of the point on curve.  Contract: point IS on curve
    //
    virtual double paramOf(const GePoint3d& pnt)const;
    virtual double paramOf(const GePoint3d& pnt, const GeTol& tol)const;

    // Return the offset of the curve.
    //
    virtual void getTrimmedOffset(double distance,const GeVector3d& planeNormal,GeVoidPointerArray& offsetCurveList) const;
    virtual void getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const;
    virtual void getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const;

    // Geometric inquiry methods.
    //
    virtual bool isClosed() const;
    virtual bool isClosed(const GeTol& tol) const;
    virtual bool isPlanar(GePlane& plane) const;
    virtual bool isPlanar(GePlane& plane, const GeTol& tol) const;
    virtual bool isLinear(GeLine3d& line) const;
    virtual bool isLinear(GeLine3d& line, const GeTol& tol) const;
    virtual bool isCoplanarWith(const GeCurve3d& curve3d, GePlane& plane) const;
    virtual bool isCoplanarWith(const GeCurve3d& curve3d, GePlane& plane, const GeTol& tol) const;

    // Length based methods.
    //
    virtual double length() const;
    virtual double length(double fromParam, double toParam) const;
    virtual double length(double fromParam, double toParam, double tol) const;
    virtual double paramAtLength(double datumParam, double length) const;
    virtual double paramAtLength(double datumParam, double length, double tol) const;
    virtual double area() const;
    virtual double area(const GeTol& tol) const;

    // Modify methods.
    //
    virtual void getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const;

    // Explode curve into its component sub-curves.
    //
    virtual bool explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurves) const;

    // Return start and end points.
    //
    virtual bool hasStartPoint(GePoint3d& startPnt) const;
    virtual bool hasEndPoint(GePoint3d& endPnt) const;

    // Evaluate methods.
    //
    virtual GePoint3d evalPoint(double param) const;

    // Polygonize curve to within a specified tolerance.
    // Note: forceResampling will make sure that the actual error is
    //       as close to approxEps as possible
    virtual void getSamplePoints(double fromParam,
        double toParam,
        double approxEps,
        GePoint3dArray& pointArray,
        GeDoubleArray& paramArray) const;
    virtual void getSamplePoints(double fromParam,
        double toParam, 
        double approxEps, 
        GePoint3dArray& pointArray,
        GeDoubleArray& paramArray, 
        bool forceResampling) const;
    virtual void getSamplePoints(int numSample, GePoint3dArray& pointArray) const;
    virtual void getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const;

    // Assignment operator.
    //
    GeCurve3d& operator = (const GeCurve3d& curve);

em_protected:

    // Private constructors so that no object of this class can be instantiated.
    GeCurve3d();
    GeCurve3d(const GeCurve3d&);

};


#endif

