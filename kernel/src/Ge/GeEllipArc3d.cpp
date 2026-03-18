#include "GeEllipArc3d.h"
#include "GeScale3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeCircArc3d.h"
#include "GePointOnCurve3d.h"
#include "GeInterval.h"
#include "GeBoundBlock3d.h"
#include "GeMatrix3d.h"
#include "GePlane.h"
#include "GeImpl.h"

namespace
{
	double ellip_normalize_param(double param)
	{
		while (param < 0.0)
		{
			param += PI * 2.0;
		}
		while (param >= PI * 2.0)
		{
			param -= PI * 2.0;
		}
		return param;
	}

	double ellip_param_sweep(double fromParam, double toParam)
	{
		fromParam = ellip_normalize_param(fromParam);
		toParam = ellip_normalize_param(toParam);
		if (toParam < fromParam)
		{
			toParam += PI * 2.0;
		}
		return toParam - fromParam;
	}

	double ellip_param_of_axes(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, const GePoint3d& pnt)
	{
		double majorRadius = majorAxis.length();
		double minorRadius = minorAxis.length();
		if (majorRadius < GeContext::gTol.equalPoint() || minorRadius < GeContext::gTol.equalPoint())
		{
			return 0.0;
		}

		GeVector3d vec = pnt - center;
		double x = vec.dotProduct(majorAxis.normal()) / majorRadius;
		double y = vec.dotProduct(minorAxis.normal()) / minorRadius;
		return ellip_normalize_param(atan2(y, x));
	}

	GePoint3d ellip_eval_point(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, double param)
	{
		GePoint3d pnt = center;
		pnt += majorAxis * cos(param);
		pnt += minorAxis * sin(param);
		return pnt;
	}

	GePoint3dArray ellip_sample_points(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, double fromParam, double toParam, int numSegments)
	{
		GePoint3dArray points;

		if (numSegments < 1)
		{
			numSegments = 1;
		}

		double startParam = fromParam;
		double endParam = toParam;
		if (endParam < startParam)
		{
			endParam += PI * 2.0;
		}

		for (int i = 0; i <= numSegments; i++)
		{
			double param = startParam + (endParam - startParam) / numSegments * i;
			points.append(ellip_eval_point(center, majorAxis, minorAxis, ellip_normalize_param(param)));
		}

		return points;
	}

	bool ellip_is_param_on(const GeEllipArc3d& arc, double param, const GeTol& tol)
	{
		if (arc.isClosed(tol) == true)
		{
			return true;
		}

		double startParam = ellip_normalize_param(arc.startAng());
		double endParam = ellip_normalize_param(arc.endAng());
		param = ellip_normalize_param(param);

		if (endParam < startParam)
		{
			endParam += PI * 2.0;
		}
		if (param < startParam)
		{
			param += PI * 2.0;
		}

		if (param < startParam - tol.equalPoint())
		{
			return false;
		}
		if (param > endParam + tol.equalPoint())
		{
			return false;
		}
		return true;
	}
}

GeEllipArc3d::GeEllipArc3d()
{
	GE_IMP_MEMORY_ENTITY(GeEllipArc3d);
}
GeEllipArc3d::GeEllipArc3d(const GeEllipArc3d& arc)
{
	GE_IMP_MEMORY_ENTITY(GeEllipArc3d);

	GE_IMP_ELLIPARC3D(this->m_pImpl)->center = GE_IMP_ELLIPARC3D(arc.m_pImpl)->center;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = GE_IMP_ELLIPARC3D(arc.m_pImpl)->majorAxis;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = GE_IMP_ELLIPARC3D(arc.m_pImpl)->minorAxis;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = GE_IMP_ELLIPARC3D(arc.m_pImpl)->startAngle;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = GE_IMP_ELLIPARC3D(arc.m_pImpl)->endAngle;
}
GeEllipArc3d::GeEllipArc3d(const GeCircArc3d& arc)
{
	GE_IMP_MEMORY_ENTITY(GeEllipArc3d);

	this->set(arc);
}
GeEllipArc3d::GeEllipArc3d(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius)
{
	GE_IMP_MEMORY_ENTITY(GeEllipArc3d);

	this->set(cent, unitNormal, majorAxis, majorRadius, minorRadius);
}
GeEllipArc3d::GeEllipArc3d(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle)
{
	GE_IMP_MEMORY_ENTITY(GeEllipArc3d);

	this->set(cent, unitNormal, majorAxis, majorRadius, minorRadius, startAngle, endAngle);
}

GePoint3dArray GeEllipArc3d::toLineSegment(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, int numSegments)
{
	return GeEllipArc3d::toLineSegment(center, majorAxis, minorAxis, 0.0, PI * 2.0, numSegments);
}
GePoint3dArray GeEllipArc3d::toLineSegment(const GePoint3d& center, const GeVector3d& majorAxis, const GeVector3d& minorAxis, double startAngle, double endAngle, int numSegments)
{
	return ellip_sample_points(center, majorAxis, minorAxis, startAngle, endAngle, numSegments);
}

Adesk::Boolean GeEllipArc3d::intersectWith(const GeLinearEnt3d& line, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol) const
{
	GePoint3dArray points = this->intersectWith(line, tol);
	intn = points.length();
	if (intn > 0)
	{
		p1 = points[0];
	}
	if (intn > 1)
	{
		p2 = points[1];
	}
	return intn > 0;
}
Adesk::Boolean GeEllipArc3d::intersectWith(const GeCircArc3d& arc, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol) const
{
	GePoint3dArray points = this->intersectWith(arc, tol);
	intn = points.length();
	if (intn > 0)
	{
		p1 = points[0];
	}
	if (intn > 1)
	{
		p2 = points[1];
	}
	return intn > 0;
}
GePoint3dArray GeEllipArc3d::intersectWith(const GeLinearEnt3d& line) const
{
	return this->intersectWith(line, GeContext::gTol);
}
GePoint3dArray GeEllipArc3d::intersectWith(const GeLinearEnt3d& line, const GeTol& tol) const
{
	GePoint3dArray points;
	return points;
}
GePoint3dArray GeEllipArc3d::intersectWith(const GeCircArc3d& arc) const
{
	return this->intersectWith(arc, GeContext::gTol);
}
GePoint3dArray GeEllipArc3d::intersectWith(const GeCircArc3d& arc, const GeTol& tol) const
{
	GePoint3dArray points;
	return points;
}

void GeEllipArc3d::getPlane(GePlane& plane) const
{
	plane.set(this->center(), this->majorAxis(), this->minorAxis());
}

Adesk::Boolean GeEllipArc3d::isCircular() const
{
	return this->isCircular(GeContext::gTol);
}
Adesk::Boolean GeEllipArc3d::isCircular(const GeTol& tol) const
{
	if (abs(this->majorRadius() - this->minorRadius()) < tol.equalVector())
	{
		return true;
	}
	return false;
}
Adesk::Boolean GeEllipArc3d::isInside(const GePoint3d& pnt) const
{
	return this->isInside(pnt, GeContext::gTol);
}
Adesk::Boolean GeEllipArc3d::isInside(const GePoint3d& pnt, const GeTol& tol) const
{
	GePlane plane;
	this->getPlane(plane);
	if (plane.isOn(pnt, tol) == false)
	{
		return false;
	}

	double majorRadius = this->majorRadius();
	double minorRadius = this->minorRadius();
	if (majorRadius < tol.equalPoint() || minorRadius < tol.equalPoint())
	{
		return false;
	}

	GeVector3d vec = pnt - this->center();
	double x = vec.dotProduct(this->majorAxis().normal());
	double y = vec.dotProduct(this->minorAxis().normal());
	double res = (x * x) / (majorRadius * majorRadius) + (y * y) / (minorRadius * minorRadius);
	if (res > 1.0 + tol.equalPoint())
	{
		return false;
	}
	return true;
}

GePoint3d GeEllipArc3d::center() const
{
	return GE_IMP_ELLIPARC3D(this->m_pImpl)->center;
}
GeVector3d GeEllipArc3d::normal() const
{
	GeVector3d normal = this->majorAxis().crossProduct(this->minorAxis());
	if (normal.length() < GeContext::gTol.equalVector())
	{
		return GeVector3d::kZAxis;
	}
	normal.normalize();
	return normal;
}
GeVector3d GeEllipArc3d::majorAxis() const
{
	return GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis;
}
GeVector3d GeEllipArc3d::minorAxis() const
{
	return GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis;
}
double GeEllipArc3d::majorRadius() const
{
	return this->majorAxis().length();
}
double GeEllipArc3d::minorRadius() const
{
	return this->minorAxis().length();
}
double GeEllipArc3d::startAng() const
{
	return GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle;
}
double GeEllipArc3d::endAng() const
{
	return GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle;
}
GePoint3d GeEllipArc3d::startPoint() const
{
	return this->evalPoint(this->startAng());
}
GePoint3d GeEllipArc3d::endPoint() const
{
	return this->evalPoint(this->endAng());
}

GeEllipArc3d& GeEllipArc3d::setCenter(const GePoint3d& cent)
{
	GE_IMP_ELLIPARC3D(this->m_pImpl)->center = cent;
	return *this;
}
GeEllipArc3d& GeEllipArc3d::setAxes(const GeVector3d& majorAxis, const GeVector3d& minorAxis)
{
	if (majorAxis.length() < minorAxis.length() - GeContext::gTol.equalVector())
	{
		return *this;
	}

	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = majorAxis;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = minorAxis;
	return *this;
}
GeEllipArc3d& GeEllipArc3d::setMajorRadius(double rad)
{
	if (rad < this->minorRadius() - GeContext::gTol.equalVector())
	{
		return *this;
	}
	if (this->majorAxis().length() < GeContext::gTol.equalVector())
	{
		return *this;
	}

	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = this->majorAxis().normal() * rad;
	return *this;
}
GeEllipArc3d& GeEllipArc3d::setMinorRadius(double rad)
{
	if (rad > this->majorRadius() + GeContext::gTol.equalVector())
	{
		return *this;
	}
	if (this->minorAxis().length() < GeContext::gTol.equalVector())
	{
		return *this;
	}

	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = this->minorAxis().normal() * rad;
	return *this;
}
GeEllipArc3d& GeEllipArc3d::setAngles(double startAngle, double endAngle)
{
	GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = startAngle;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = endAngle;
	return *this;
}
GeEllipArc3d& GeEllipArc3d::set(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius)
{
	return this->set(cent, unitNormal, majorAxis, majorRadius, minorRadius, 0.0, PI * 2.0);
}
GeEllipArc3d& GeEllipArc3d::set(const GePoint3d& cent, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle)
{
	if (majorRadius < minorRadius - GeContext::gTol.equalVector())
	{
		return *this;
	}
	if (majorAxis.length() < GeContext::gTol.equalVector())
	{
		return *this;
	}

	GeVector3d major = majorAxis.normal() * majorRadius;
	GeVector3d normal = unitNormal;
	if (normal.length() < GeContext::gTol.equalVector())
	{
		normal = GeVector3d::kZAxis;
	}
	normal.normalize();

	GeVector3d minor = normal.crossProduct(major);
	if (minor.length() < GeContext::gTol.equalVector())
	{
		return *this;
	}
	minor.normalize();
	minor *= minorRadius;

	GE_IMP_ELLIPARC3D(this->m_pImpl)->center = cent;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = major;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = minor;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = startAngle;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = endAngle;
	return *this;
}
GeEllipArc3d& GeEllipArc3d::set(const GeCircArc3d& arc)
{
	GE_IMP_ELLIPARC3D(this->m_pImpl)->center = arc.center();
	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = arc.refVec().normal() * arc.radius();
	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = arc.normal().crossProduct(arc.refVec()).normal() * arc.radius();
	GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = arc.startAng();
	GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = arc.endAng();
	return *this;
}

GeEllipArc3d& GeEllipArc3d::operator = (const GeEllipArc3d& arc)
{
	GE_IMP_ELLIPARC3D(this->m_pImpl)->center = GE_IMP_ELLIPARC3D(arc.m_pImpl)->center;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = GE_IMP_ELLIPARC3D(arc.m_pImpl)->majorAxis;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = GE_IMP_ELLIPARC3D(arc.m_pImpl)->minorAxis;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = GE_IMP_ELLIPARC3D(arc.m_pImpl)->startAngle;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = GE_IMP_ELLIPARC3D(arc.m_pImpl)->endAngle;
	return *this;
}

bool GeEllipArc3d::isKindOf(Ge::EntityId entType) const
{
	if (entType == this->type())
	{
		return true;
	}
	return false;
}
Ge::EntityId GeEllipArc3d::type() const
{
	return Ge::EntityId::kEllipArc3d;
}
GeEllipArc3d* GeEllipArc3d::copy() const
{
	GeEllipArc3d* arc = new GeEllipArc3d();
	GE_IMP_ELLIPARC3D(arc->m_pImpl)->center = GE_IMP_ELLIPARC3D(this->m_pImpl)->center;
	GE_IMP_ELLIPARC3D(arc->m_pImpl)->majorAxis = GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis;
	GE_IMP_ELLIPARC3D(arc->m_pImpl)->minorAxis = GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis;
	GE_IMP_ELLIPARC3D(arc->m_pImpl)->startAngle = GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle;
	GE_IMP_ELLIPARC3D(arc->m_pImpl)->endAngle = GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle;
	return arc;
}
bool GeEllipArc3d::operator == (const GeEllipArc3d& entity) const
{
	return this->isEqualTo(entity);
}
bool GeEllipArc3d::operator != (const GeEllipArc3d& entity) const
{
	return !this->isEqualTo(entity);
}
bool GeEllipArc3d::isEqualTo(const GeEllipArc3d& entity) const
{
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeEllipArc3d::isEqualTo(const GeEllipArc3d& entity, const GeTol& tol) const
{
	if (this->center().isEqualTo(entity.center(), tol) == false)
	{
		return false;
	}
	if (this->majorAxis().isEqualTo(entity.majorAxis(), tol) == false)
	{
		return false;
	}
	if (this->minorAxis().isEqualTo(entity.minorAxis(), tol) == false)
	{
		return false;
	}
	if (abs(this->startAng() - entity.startAng()) > tol.equalPoint())
	{
		return false;
	}
	if (abs(this->endAng() - entity.endAng()) > tol.equalPoint())
	{
		return false;
	}
	return true;
}
GeEllipArc3d& GeEllipArc3d::transformBy(const GeMatrix3d& xfm)
{
	GePoint3d center = this->center();
	GePoint3d majorPnt = this->center() + this->majorAxis();
	GePoint3d minorPnt = this->center() + this->minorAxis();
	GePoint3d startPnt = this->startPoint();
	GePoint3d endPnt = this->endPoint();

	center.transformBy(xfm);
	majorPnt.transformBy(xfm);
	minorPnt.transformBy(xfm);
	startPnt.transformBy(xfm);
	endPnt.transformBy(xfm);

	GE_IMP_ELLIPARC3D(this->m_pImpl)->center = center;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->majorAxis = majorPnt - center;
	GE_IMP_ELLIPARC3D(this->m_pImpl)->minorAxis = minorPnt - center;

	if (this->isClosed() == true)
	{
		GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = 0.0;
		GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = PI * 2.0;
	}
	else
	{
		GE_IMP_ELLIPARC3D(this->m_pImpl)->startAngle = ellip_param_of_axes(center, this->majorAxis(), this->minorAxis(), startPnt);
		GE_IMP_ELLIPARC3D(this->m_pImpl)->endAngle = ellip_param_of_axes(center, this->majorAxis(), this->minorAxis(), endPnt);
	}

	return *this;
}
GeEllipArc3d& GeEllipArc3d::translateBy(const GeVector3d& translateVec)
{
	this->setCenter(this->center().translation(translateVec));
	return *this;
}
GeEllipArc3d& GeEllipArc3d::rotateBy(double angle, const GeVector3d& vec)
{
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeEllipArc3d& GeEllipArc3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
	GeMatrix3d mat;
	mat.setToRotation(angle, vec, wrtPoint);
	this->transformBy(mat);
	return *this;
}
GeEllipArc3d& GeEllipArc3d::mirror(const GePlane& plane)
{
	GeMatrix3d mat;
	mat.setToMirroring(plane);
	this->transformBy(mat);
	return *this;
}
GeEllipArc3d& GeEllipArc3d::scaleBy(double scaleFactor)
{
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeEllipArc3d& GeEllipArc3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
	GeMatrix3d mat;
	mat.setToScaling(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	this->transformBy(mat);
	return *this;
}
bool GeEllipArc3d::isOn(const GePoint3d& pnt) const
{
	return this->isOn(pnt, GeContext::gTol);
}
bool GeEllipArc3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
	GePlane plane;
	this->getPlane(plane);
	if (plane.isOn(pnt, tol) == false)
	{
		return false;
	}

	double majorRadius = this->majorRadius();
	double minorRadius = this->minorRadius();
	if (majorRadius < tol.equalPoint() || minorRadius < tol.equalPoint())
	{
		return false;
	}

	GeVector3d vec = pnt - this->center();
	double x = vec.dotProduct(this->majorAxis().normal());
	double y = vec.dotProduct(this->minorAxis().normal());
	double res = (x * x) / (majorRadius * majorRadius) + (y * y) / (minorRadius * minorRadius);
	if (abs(res - 1.0) > tol.equalPoint())
	{
		return false;
	}

	double param = ellip_param_of_axes(this->center(), this->majorAxis(), this->minorAxis(), pnt);
	return ellip_is_param_on(*this, param, tol);
}

void GeEllipArc3d::getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const
{
	GePointOnCurve3d pointOnCurve(*this, param);
	GePoint3d point = pointOnCurve.point();
	if (this->isOn(point) == false)
	{
		return;
	}

	piece1 = new GeEllipArc3d(this->center(), this->normal(), this->majorAxis(), this->majorRadius(), this->minorRadius(), this->startAng(), param);
	piece2 = new GeEllipArc3d(this->center(), this->normal(), this->majorAxis(), this->majorRadius(), this->minorRadius(), param, this->endAng());
}
bool GeEllipArc3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const
{
	return false;
}
GeBoundBlock3d GeEllipArc3d::boundBlock() const
{
	GeInterval range;
	range.set(this->startAng(), this->endAng());
	return this->boundBlock(range);
}
GeBoundBlock3d GeEllipArc3d::boundBlock(const GeInterval& range) const
{
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeEllipArc3d::orthoBoundBlock() const
{
	GeInterval range;
	range.set(this->startAng(), this->endAng());
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeEllipArc3d::orthoBoundBlock(const GeInterval& range) const
{
	GeBoundBlock3d boundBlock;
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), range.lowerBound(), range.upperBound(), 256);
	if (points.length() == 0)
	{
		return boundBlock;
	}

	GePoint3d minPoint = points[0];
	GePoint3d maxPoint = points[0];

	for (int i = 1; i < points.length(); i++)
	{
		if (points[i].x < minPoint.x)
		{
			minPoint.x = points[i].x;
		}
		if (points[i].y < minPoint.y)
		{
			minPoint.y = points[i].y;
		}
		if (points[i].z < minPoint.z)
		{
			minPoint.z = points[i].z;
		}
		if (points[i].x > maxPoint.x)
		{
			maxPoint.x = points[i].x;
		}
		if (points[i].y > maxPoint.y)
		{
			maxPoint.y = points[i].y;
		}
		if (points[i].z > maxPoint.z)
		{
			maxPoint.z = points[i].z;
		}
	}

	boundBlock.set(minPoint, maxPoint);
	return boundBlock;
}
bool GeEllipArc3d::hasStartPoint(GePoint3d& startPoint) const
{
	startPoint = this->startPoint();
	return true;
}
bool GeEllipArc3d::hasEndPoint(GePoint3d& endPoint) const
{
	endPoint = this->endPoint();
	return true;
}
GePoint3d GeEllipArc3d::evalPoint(double param) const
{
	return ellip_eval_point(this->center(), this->majorAxis(), this->minorAxis(), param);
}

GePoint3d GeEllipArc3d::closestPointTo(const GePoint3d& pnt) const
{
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeEllipArc3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
	if (points.length() == 0)
	{
		return closest;
	}

	double minDist = points[0].distanceTo(pnt);
	closest = points[0];

	for (int i = 1; i < points.length(); i++)
	{
		double dist = points[i].distanceTo(pnt);
		if (dist < minDist)
		{
			minDist = dist;
			closest = points[i];
		}
	}

	return closest;
}
GePoint3d GeEllipArc3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
	if (points.length() == 0)
	{
		return closest;
	}

	double minDist = 0.0;
	for (int i = 0; i < points.length(); i++)
	{
		GePoint3d other = curve3d.closestPointTo(points[i], tol);
		double dist = points[i].distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = points[i];
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GePoint3d GeEllipArc3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
	if (points.length() == 0)
	{
		return closest;
	}

	double minDist = 0.0;
	for (int i = 0; i < points.length(); i++)
	{
		GePoint3d other = curve3d.closestPointTo(points[i], tol);
		double dist = points[i].distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = points[i];
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GePoint3d GeEllipArc3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
	if (points.length() == 0)
	{
		return closest;
	}

	double minDist = 0.0;
	for (int i = 0; i < points.length(); i++)
	{
		GePoint3d other = curve3d.closestPointTo(points[i], tol);
		double dist = points[i].distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = points[i];
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GePoint3d GeEllipArc3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
	if (points.length() == 0)
	{
		return closest;
	}

	double minDist = 0.0;
	for (int i = 0; i < points.length(); i++)
	{
		GePoint3d other = curve3d.closestPointTo(points[i], tol);
		double dist = points[i].distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = points[i];
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GECURVE3D_METHODS_GETCLOSESTPOINTTO(GeEllipArc3d);
GECURVE3D_METHODS_DISTANCETO(GeEllipArc3d);

GePoint3d GeEllipArc3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const
{
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeEllipArc3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
void GeEllipArc3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const
{
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeEllipArc3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
}
void GeEllipArc3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeEllipArc3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeEllipArc3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeEllipArc3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeEllipArc3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeEllipArc3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeEllipArc3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeEllipArc3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
bool GeEllipArc3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const
{
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeEllipArc3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
	GePoint3d closest = this->closestPointTo(pnt, tol);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(closest, tol));
	return true;
}
GeEllipArc3d* GeEllipArc3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const
{
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeEllipArc3d* GeEllipArc3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const
{
	return NULL;
}
GeEllipArc3d* GeEllipArc3d::orthoProject(const GePlane& projectionPlane) const
{
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeEllipArc3d* GeEllipArc3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const
{
	return NULL;
}
bool GeEllipArc3d::isOn(const GePoint3d& pnt, double& param) const
{
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeEllipArc3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const
{
	param = ellip_param_of_axes(this->center(), this->majorAxis(), this->minorAxis(), pnt);
	return this->isOn(pnt, tol);
}
bool GeEllipArc3d::isOn(double param) const
{
	return this->isOn(param, GeContext::gTol);
}
bool GeEllipArc3d::isOn(double param, const GeTol& tol) const
{
	return ellip_is_param_on(*this, param, tol);
}
double GeEllipArc3d::paramOf(const GePoint3d& pnt) const
{
	return this->paramOf(pnt, GeContext::gTol);
}
double GeEllipArc3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
	if (this->isOn(pnt, tol) == false)
	{
		return 0.0;
	}
	return ellip_param_of_axes(this->center(), this->majorAxis(), this->minorAxis(), pnt);
}
void GeEllipArc3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const
{
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeEllipArc3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const
{
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeEllipArc3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const
{
	if (this->majorRadius() + distance < tol.equalPoint())
	{
		return;
	}
	if (this->minorRadius() + distance < tol.equalPoint())
	{
		return;
	}

	GeEllipArc3d* ellipArc = new GeEllipArc3d();
	ellipArc->set(this->center(), this->normal(), this->majorAxis(), this->majorRadius() + distance, this->minorRadius() + distance, this->startAng(), this->endAng());
	offsetCurveList.append(ellipArc);
}
bool GeEllipArc3d::isClosed() const
{
	return this->isClosed(GeContext::gTol);
}
bool GeEllipArc3d::isClosed(const GeTol& tol) const
{
	if (abs(this->endAng() - this->startAng()) - PI * 2.0 < tol.equalPoint())
	{
		return true;
	}
	return false;
}
bool GeEllipArc3d::isPlanar(GePlane& plane) const
{
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeEllipArc3d::isPlanar(GePlane& plane, const GeTol& tol) const
{
	this->getPlane(plane);
	return true;
}
bool GeEllipArc3d::isLinear(GeLine3d& line) const
{
	return this->isLinear(line, GeContext::gTol);
}
bool GeEllipArc3d::isLinear(GeLine3d& line, const GeTol& tol) const
{
	return false;
}
bool GeEllipArc3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeEllipArc3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	this->getPlane(plane);
	if (plane.isOn(curve3d.pointOnLine(), tol) == false)
	{
		return false;
	}
	if (curve3d.direction().isPerpendicularTo(plane.normal(), tol) == false)
	{
		return false;
	}
	return true;
}
bool GeEllipArc3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeEllipArc3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	this->getPlane(plane);
	if (plane.isOn(curve3d.startPoint(), tol) == false)
	{
		return false;
	}
	if (plane.isOn(curve3d.endPoint(), tol) == false)
	{
		return false;
	}
	return true;
}
bool GeEllipArc3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeEllipArc3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	this->getPlane(plane);
	if (plane.isOn(curve3d.pointOnLine(), tol) == false)
	{
		return false;
	}
	if (curve3d.direction().isPerpendicularTo(plane.normal(), tol) == false)
	{
		return false;
	}
	return true;
}
bool GeEllipArc3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeEllipArc3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	this->getPlane(plane);
	if (this->normal().isParallelTo(curve3d.normal(), tol) == false)
	{
		return false;
	}
	if (plane.isOn(curve3d.center(), tol) == false)
	{
		return false;
	}
	return true;
}
double GeEllipArc3d::length() const
{
	return this->length(this->startAng(), this->endAng());
}
double GeEllipArc3d::length(double fromParam, double toParam) const
{
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeEllipArc3d::length(double fromParam, double toParam, double tol) const
{
	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), fromParam, toParam, 256);
	double len = 0.0;
	for (int i = 1; i < points.length(); i++)
	{
		len += points[i - 1].distanceTo(points[i]);
	}
	return len;
}
double GeEllipArc3d::paramAtLength(double datumParam, double length) const
{
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeEllipArc3d::paramAtLength(double datumParam, double length, double tol) const
{
	double total = 0.0;
	double currentParam = datumParam;
	GePoint3d prevPoint = this->evalPoint(currentParam);

	for (int i = 1; i <= 256; i++)
	{
		double nextParam = datumParam + (PI * 2.0) / 256.0 * i;
		GePoint3d nextPoint = this->evalPoint(nextParam);
		double segLen = prevPoint.distanceTo(nextPoint);

		if (total + segLen >= length)
		{
			return ellip_normalize_param(nextParam);
		}

		total += segLen;
		prevPoint = nextPoint;
		currentParam = nextParam;
	}

	return ellip_normalize_param(currentParam);
}
double GeEllipArc3d::area() const
{
	return this->area(GeContext::gTol);
}
double GeEllipArc3d::area(const GeTol& tol) const
{
	if (this->isClosed(tol) == false)
	{
		return 0.0;
	}
	return PI * this->majorRadius() * this->minorRadius();
}
void GeEllipArc3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const
{
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray, false);
}
void GeEllipArc3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray, bool forceResampling) const
{
	int numSegments = 64;
	if (approxEps > GeContext::gTol.equalPoint())
	{
		double sweep = ellip_param_sweep(fromParam, toParam);
		int tempSegment = (int)(sweep / approxEps);
		if (tempSegment > 1)
		{
			numSegments = tempSegment;
		}
	}

	GePoint3dArray points = ellip_sample_points(this->center(), this->majorAxis(), this->minorAxis(), fromParam, toParam, numSegments);
	double startParam = fromParam;
	double endParam = toParam;
	if (endParam < startParam)
	{
		endParam += PI * 2.0;
	}

	for (int i = 0; i < points.length(); i++)
	{
		double param = startParam + (endParam - startParam) / numSegments * i;
		pointArray.append(points[i]);
		paramArray.append(ellip_normalize_param(param));
	}
}
void GeEllipArc3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const
{
	GeDoubleArray paramArray;
	this->getSamplePoints(numSample, pointArray, paramArray);
}
void GeEllipArc3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const
{
	if (numSample < 2)
	{
		numSample = 2;
	}

	double startParam = this->startAng();
	double endParam = this->endAng();
	if (endParam < startParam)
	{
		endParam += PI * 2.0;
	}

	for (int i = 0; i < numSample; i++)
	{
		double param = startParam + (endParam - startParam) / (numSample - 1) * i;
		pointArray.append(this->evalPoint(param));
		paramArray.append(ellip_normalize_param(param));
	}
}


