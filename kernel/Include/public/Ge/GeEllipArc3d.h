#ifndef GEELLIPARC3D_H
#define GEELLIPARC3D_H

#include "adesk.h"
#include "GeCurve3d.h"
#include "GePoint3d.h"
#include "GeVector3d.h"
#include "GeEntity3dMethods.h"
#include "GePoint3dArray.h"

class GeLine3d;
class GeLinearEnt3d;
class GeCircArc3d;
class GePlane;

class GeEllipArc3d : public GeCurve3d
{
public:
	GeEllipArc3d();
	GeEllipArc3d(const GeEllipArc3d& arc);
	GeEllipArc3d(const GeCircArc3d& arc);
	GeEllipArc3d(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius);
	GeEllipArc3d(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle);
	GEENTITY3D_METHODS(GeEllipArc3d);
	GECURVE3D_METHODS(GeEllipArc3d);

	static GePoint3dArray toLineSegment(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, int numSegments);
	static GePoint3dArray toLineSegment(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, double startAngle, double endAngle, int numSegments);

	Adesk::Boolean intersectWith(const GeLinearEnt3d& line, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol = GeContext::gTol) const;
	Adesk::Boolean intersectWith(const GeCircArc3d& arc, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol = GeContext::gTol) const;
	GePoint3dArray intersectWith(const GeLinearEnt3d& line) const;
	GePoint3dArray intersectWith(const GeLinearEnt3d& line, const GeTol& tol) const;
	GePoint3dArray intersectWith(const GeCircArc3d& arc) const;
	GePoint3dArray intersectWith(const GeCircArc3d& arc, const GeTol& tol) const;

	void getPlane(GePlane& plane) const;

	Adesk::Boolean isCircular() const;
	Adesk::Boolean isCircular(const GeTol& tol) const;
	Adesk::Boolean isInside(const GePoint3d& pnt) const;
	Adesk::Boolean isInside(const GePoint3d& pnt, const GeTol& tol) const;

	GePoint3d center() const;
	GeVector3d normal() const;
	GeVector3d majorAxis() const;
	GeVector3d minorAxis() const;
	double majorRadius() const;
	double minorRadius() const;
	double startAng() const;
	double endAng() const;
	GePoint3d startPoint() const;
	GePoint3d endPoint() const;

	GeEllipArc3d& setCenter(const GePoint3d& cent);
	GeEllipArc3d& setAxes(const GeVector3d& majorAxis, const GeVector3d& minorAxis);
	GeEllipArc3d& setMajorRadius(double rad);
	GeEllipArc3d& setMinorRadius(double rad);
	GeEllipArc3d& setAngles(double startAngle, double endAngle);
	GeEllipArc3d& set(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius);
	GeEllipArc3d& set(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle);
	GeEllipArc3d& set(const GeCircArc3d& arc);

	GeEllipArc3d& operator = (const GeEllipArc3d& arc);
};

#endif

