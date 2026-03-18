#ifndef GEARC3D_H
#define GEARC3D_H

#include "adesk.h"
#include "GeCurve3d.h"
#include "GePoint3d.h"
#include "GeVector3d.h"
#include "GeEntity3dMethods.h"
#include "GePoint3dArray.h"


class GeLine3d;
class GeCircArc2d;
class GePlanarEnt;
class GeCircArc3d : public GeCurve3d
{
public:
	GeCircArc3d();
	GeCircArc3d(const GeCircArc3d& arc);
	GeCircArc3d(const GePoint3d& cent, const GeVector3d& nrm, double radius);
	GeCircArc3d(const GePoint3d& cent, const GeVector3d& nrm, const GeVector3d& refVec, double radius, double startAngle, double endAngle);
	GeCircArc3d(const GePoint3d& startPoint, const GePoint3d& pnt, const GePoint3d& endPoint);
	GEENTITY3D_METHODS(GeCircArc3d);
	GECURVE3D_METHODS(GeCircArc3d);

	static GePoint3dArray toLineSegment(const GePoint3d& center, double _radius, const GeVector3d& normal, int numSegments);
	static GePoint3dArray toLineSegment(const GePoint3d& center, double _radius, double _startAng, double _endAng, const GeVector3d& normal, int numSegments);
	static GePoint3dArray toLineSegment(const GePoint3d& center, double _radius, const GeVector3d& normal, const GeVector3d& startVector, const double sweepAngle, int numSegments);
	static bool pointToCenter(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, GePoint3d& _center);
	static bool pointToCenter(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, GePoint3d& _center, double tol);
	static bool pointToBulge(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, double& bulge);
	static bool pointToBulge(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, const GeVector3d& normal, double& bulge);
	static bool pointToBulge(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, const GeVector3d& normal, double& bulge, const double tol);
	static bool isClockWise(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint);
	static bool isClockWise(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, const GeVector3d& normal);

	// Intersection with other geometric objects.
	//
	Adesk::Boolean intersectWith(const GeLinearEnt3d& line, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol = GeContext::gTol) const;
	Adesk::Boolean intersectWith(const GeCircArc3d& arc, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol = GeContext::gTol) const;
	GePoint3dArray intersectWith(const GeLinearEnt3d& line) const;
	GePoint3dArray intersectWith(const GeLinearEnt3d& line, const GeTol& tol)const;
	GePoint3dArray intersectWith(const GeCircArc3d& arc)const;
	GePoint3dArray intersectWith(const GeCircArc3d& arc, const GeTol& tol)const;

	// Tangent to the circular arc.
	//
	Adesk::Boolean tangent(const GePoint3d& pnt, GeLine3d& line) const;
	Adesk::Boolean tangent(const GePoint3d& pnt, GeLine3d& line, const GeTol& tol) const;
	// Possible error conditions:  kArg1TooBig, kArg1InsideThis, 
	// kArg1OnThis, kThisIsInfiniteLine

// Plane of the arc
//
	void           getPlane(GePlane& plane) const;

	// Test if point is inside circle.
	//
	Adesk::Boolean isInside(const GePoint3d& pnt) const;
	Adesk::Boolean isInside(const GePoint3d& pnt, const GeTol& tol) const;

	// Definition of circular arc
	//
	GePoint3d    center() const;
	GeVector3d   normal() const;
	GeVector3d   refVec() const;
	double         radius() const;
	double         startAng() const;
	double         endAng() const;
	GePoint3d    startPoint() const;
	GePoint3d    endPoint() const;

	GeCircArc3d& setCenter(const GePoint3d&);
	GeCircArc3d& setAxes(const GeVector3d& normal, const GeVector3d& refVec);
	GeCircArc3d& setRadius(double);
	GeCircArc3d& setAngles(double startAngle, double endAngle);

	GeCircArc3d& set(const GePoint3d& cent, const GeVector3d& nrm, double radius);
	GeCircArc3d& set(const GePoint3d& cent, const GeVector3d& nrm, const GeVector3d& refVec, double radius, double startAngle, double endAngle);
	GeCircArc3d& set(const GePoint3d& startPoint, const GePoint3d& pnt, const GePoint3d& endPoint);
	GeCircArc3d& set(const GePoint3d& startPoint, const GePoint3d& endPoint, double bulge);
	// Possible errors:  kEqualArg1Arg2, kEqualArg1Arg3, kEqualArg2Arg3, 
	// kLinearlyDependentArg1Arg2Arg3.
	// Degenerate results: none.
	// On error, the object is unchanged.

	GeCircArc3d& set(const GeCurve3d& curve1,
		const GeCurve3d& curve2,
		double radius, double& param1, double& param2,
		Adesk::Boolean& success);
	// On success, this arc becomes the fillet of the given radius between the two curves,
	// whose points of tangency are nearest param1 and param2 respectively.
	GeCircArc3d& set(const GeCurve3d& curve1,
		const GeCurve3d& curve2,
		const GeCurve3d& curve3,
		double& param1, double& param2, double& param3,
		Adesk::Boolean& success);

	// Assignment operator.
	//
	GeCircArc3d& operator =     (const GeCircArc3d& arc);
};





#endif

