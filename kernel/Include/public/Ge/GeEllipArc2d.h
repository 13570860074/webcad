#ifndef GEELLIPARC2D_H
#define GEELLIPARC2D_H




#include "GeCurve2d.h"
#include "GePoint2d.h"
#include "GeEntity2dMethods.h"
#include "GePoint2dArray.h"

class GeEllipArc2d : public GeCurve2d
{
public:
    GeEllipArc2d();
    GeEllipArc2d(const GeEllipArc2d& ell);
    GeEllipArc2d(const GeCircArc2d& arc);
    GeEllipArc2d(const GePoint2d& cent, const GeVector2d& majorAxis,
        const GeVector2d& minorAxis, double majorRadius,
        double minorRadius);
    GeEllipArc2d(const GePoint2d& cent, const GeVector2d& majorAxis,
        const GeVector2d& minorAxis, double majorRadius,
        double minorRadius, double startAngle, double endAngle);
    GEENTITY2D_METHODS(GeEllipArc2d);
    GECURVE2D_METHODS(GeEllipArc2d);


    static GePoint2dArray toLineSegment(const GePoint2d& center, const GeVector2d& majorAxis, const GeVector2d& minorAxis, int numSegments);
    static GePoint2dArray toLineSegment(const GePoint2d& center, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double _startAng, double _endAng, int numSegments);

    // Intersection with other geometric objects.
    //
    bool intersectWith(const GeLinearEnt2d& line, int& intn, GePoint2d& p1, GePoint2d& p2) const;
    bool intersectWith(const GeLinearEnt2d& line, int& intn, GePoint2d& p1, GePoint2d& p2, const GeTol& tol) const;
    GePoint2dArray intersectWith(const GeLinearEnt2d& line) const;
    GePoint2dArray intersectWith(const GeLinearEnt2d& line, const GeTol& tol)const;
    GePoint2dArray intersectWith(const GeCircArc2d& arc)const;
    GePoint2dArray intersectWith(const GeCircArc2d& arc, const GeTol& tol)const;

    // Inquiry Methods
    //
    bool isCircular() const;
    bool isCircular(const GeTol& tol) const;

    // Test if point is inside full ellipse
    //
    bool isInside(const GePoint2d& pnt) const;
    bool isInside(const GePoint2d& pnt, const GeTol& tol) const;


    // Definition of ellipse
    //
    GePoint2d    center() const;
    double         minorRadius() const;
    double         majorRadius() const;
    GeVector2d   minorAxis() const;
    GeVector2d   majorAxis() const;
    double         startAng() const;
    double         endAng() const;
    GePoint2d    startPoint() const;
    GePoint2d    endPoint() const;
    bool isClockWise() const;

    GeEllipArc2d& setCenter(const GePoint2d& cent);
    GeEllipArc2d& setMinorRadius(double rad);
    GeEllipArc2d& setMajorRadius(double rad);
    GeEllipArc2d& setAxes(const GeVector2d& majorAxis, const GeVector2d& minorAxis);
    GeEllipArc2d& setAngles(double startAngle, double endAngle);
    GeEllipArc2d& set(const GePoint2d& cent, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double majorRadius, double minorRadius);
    GeEllipArc2d& set(const GePoint2d& cent, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle);
    GeEllipArc2d& set(const GeCircArc2d& arc);

    // Assignment operator.
    //
    GeEllipArc2d& operator =    (const GeEllipArc2d& ell);
};

#endif

