#ifndef GEARC2D_H
#define GEARC2D_H



#include "adesk.h"
#include "GeCurve2d.h"
#include "GePoint2d.h"
#include "GeEntity2dMethods.h"
#include "GePoint2dArray.h"

class GeCircArc2d : public GeCurve2d
{
public:
    GeCircArc2d();
    GeCircArc2d(const GeCircArc2d& arc);
    GeCircArc2d(const GePoint2d& cent, double radius);
    GeCircArc2d(const GePoint2d& cent, double radius, double startAngle, double endAngle);
    GeCircArc2d(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec);
    GeCircArc2d(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec, bool isClockWise);
    GeCircArc2d(const GePoint2d& startPoint, const GePoint2d& point, const GePoint2d& endPoint);
    // If bulgeFlag is kTrue, then bulge is interpreted to be the maximum
    // distance between the arc and the chord between the two input points.
    // If bulgeFlag is kFalse, then bulge is interpreted to be tan(ang/4),
    // where ang is the angle of the arc segment between the two input points.
    GeCircArc2d(const GePoint2d& startPoint, const GePoint2d& endPoint, double bulge, bool bulgeFlag = true);
    GEENTITY2D_METHODS(GeCircArc2d);
    GECURVE2D_METHODS(GeCircArc2d);


    static GePoint2dArray toLineSegment(const GePoint2d& center, double _radius, int numSegments);
    static GePoint2dArray toLineSegment(const GePoint2d& center, double _radius, double _startAng, double _endAng, int numSegments);

    static bool pointToCenter(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint, GePoint2d& _center);
    static bool pointToCenter(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint, GePoint2d& _center, double tol);
    static bool isClockWise(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint);

    // Intersection with other geometric objects.
    //
    Adesk::Boolean intersectWith(const GeLinearEnt2d& line, int& intn, GePoint2d& p1, GePoint2d& p2, const GeTol& tol = GeContext::gTol) const;
    Adesk::Boolean intersectWith(const GeCircArc2d& arc, int& intn, GePoint2d& p1, GePoint2d& p2, const GeTol& tol = GeContext::gTol) const;
    GePoint2dArray intersectWith(const GeLinearEnt2d& line) const;
    GePoint2dArray intersectWith(const GeLinearEnt2d& line, const GeTol& tol)const;
    GePoint2dArray intersectWith(const GeCircArc2d& arc)const;
    GePoint2dArray intersectWith(const GeCircArc2d& arc, const GeTol& tol)const;

    // Tangent line to the circular arc at given point.
    //
    bool tangent(const GePoint2d& pnt, GeLine2d& line) const;
    bool tangent(const GePoint2d& pnt, GeLine2d& line, const GeTol& tol) const;
    // Possible error conditions:  kArg1TooBig, kArg1InsideThis, 
    // kArg1OnThis


    // Test if point is inside circle.
    //
    bool isInside(const GePoint2d& pnt) const;
    bool isInside(const GePoint2d& pnt, const GeTol& tol) const;

    // Definition of circular arc
    //
    GePoint2d center() const;
    double radius() const;
    double startAng() const;
    double endAng() const;
    GeVector2d refVec() const;
    GePoint2d startPoint() const;
    GePoint2d endPoint() const;
    bool isClockWise() const;

    GeCircArc2d& setCenter(const GePoint2d& cent);
    GeCircArc2d& setRadius(double radius);
    GeCircArc2d& setAngles(double startAng, double endAng);
    // Reverses the direction of the arc while keeping the same startand endpoints.For bounded arcs, 
    // this function returns the complement of the original arc.
    //GeCircArc2d& setToComplement();
    GeCircArc2d& set(const GePoint2d& cent, double radius);
    GeCircArc2d& set(const GePoint2d& cent, double radius, double ang1, double ang2);
    GeCircArc2d& set(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec);
    GeCircArc2d& set(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec, bool isClockWise);
    GeCircArc2d& set(const GePoint2d& startPoint, const GePoint2d& pnt, const GePoint2d& endPoint);

    // Possible errors:  kEqualArg1Arg2, kEqualArg1Arg3, kEqualArg2Arg3, 
    // kLinearlyDependentArg1Arg2Arg3.
    // Degenerate results: none.
    // On error, the object is unchanged.

    // If bulgeFlag is kTrue, then bulge is interpreted to be the maximum
    // distance between the arc and the chord between the two input points.
    // If bulgeFlag is kFalse, then bulge is interpreted to be tan(ang/4),
    // where ang is the angle of the arc segment between the two input points.
    GeCircArc2d& set(const GePoint2d& startPoint, const GePoint2d& endPoint, double bulge);
    GeCircArc2d& set(const GePoint2d& startPoint, const GePoint2d& endPoint, double bulge, bool bulgeFlag);


    // Assignment operator.
    //
    GeCircArc2d& operator = (const GeCircArc2d& arc);
};

#endif

