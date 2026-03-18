#ifndef GECURVE2D_H
#define GECURVE2D_H 

#include "GeEntity2d.h"
#include "GeVoidPointerArray.h"
#include "GeIntArray.h"
#include "GeVector2dArray.h"
#include "GePoint2dArray.h"
#include "GeDoubleArray.h"



class GePoint2d;
class GeVector2d;
class GePointOnCurve2d;
class GeInterval;
class GeMatrix2d;
class GeLine2d;
class GePointOnCurve2dData;
class GeBoundBlock2d;

class GeCurve2d : public GeEntity2d
{
public:

    // Distance to other geometric objects.
    //
    virtual double distanceTo(const GePoint2d& pnt) const;
    virtual double distanceTo(const GePoint2d& pnt, const GeTol& tol) const;
    virtual double distanceTo(const GeCurve2d& entity) const;
    virtual double distanceTo(const GeCurve2d&, const GeTol& tol) const;

    // Return the point on this object that is closest to the other object.
    // These methods return point on this curve as a simple 2d point.
    //
    virtual GePoint2d closestPointTo(const GePoint2d& pnt) const;
    virtual GePoint2d closestPointTo(const GePoint2d& pnt, const GeTol& tol) const;
    virtual GePoint2d closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const;
    virtual GePoint2d closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const;


    // Alternate signatures for above functions.  These methods return point
    // on this curve as an GePointOnCurve2d.
    //
    virtual void getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv) const;
    virtual void getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv, const GeTol& tol) const;
    virtual void getClosestPointTo(const GeCurve2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const;
    virtual void getClosestPointTo(const GeCurve2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const;

#if 0
    // Return point on curve whose normal vector passes thru input point.
    // Second parameter contains initial guess value and also contains output point.
    // Returns true or false depending on whether a normal point was found.
    //
    bool getNormalPoint(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv, const GeTol& tol = GeContext::gTol) const;
#endif

    // Tests if point is on curve.
    //
    virtual bool isOn(const GePoint2d& pnt) const;
    virtual bool isOn(const GePoint2d& pnt, const GeTol& tol) const;

    // Parameter of the point on curve.  Contract: point IS on curve
    //
    virtual double paramOf(const GePoint2d& pnt) const;
    virtual double paramOf(const GePoint2d& pnt, const GeTol& tol) const;

    // Return the offset of the curve.
    //
    virtual void getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const;
    virtual void getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const;
    virtual void getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const;



    // Geometric inquiry methods.
    //
    virtual bool isClosed() const;
    virtual bool isClosed(const GeTol& tol) const;

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
    virtual void getSplitCurves(double param, GeCurve2d* piece1, GeCurve2d* piece2) const;

    // Explode curve into its component sub-curves.
    //
    virtual bool explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const;

    // Return oriented bounding box of curve.
    //
    virtual GeBoundBlock2d  boundBlock() const;
    virtual GeBoundBlock2d  boundBlock(const GeInterval& range) const;

    // Return bounding box whose sides are parallel to coordinate axes.
    //
    virtual GeBoundBlock2d  orthoBoundBlock() const;
    virtual GeBoundBlock2d  orthoBoundBlock(const GeInterval& range) const;

    // Return start and end points.
    //
    virtual bool hasStartPoint(GePoint2d& startPoint) const;
    virtual bool hasEndPoint(GePoint2d& endPoint) const;

    // Evaluate methods.
    //
    virtual GePoint2d evalPoint(double param) const;

    // Assignment operator.
    //
    GeCurve2d& operator =  (const GeCurve2d& curve);

em_protected:
    // Private constructors so that no object of this class can be instantiated.
    GeCurve2d();
    GeCurve2d(const GeCurve2d&);
};


#endif

