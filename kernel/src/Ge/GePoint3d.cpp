#include "GePoint3d.h"
#include "GeMatrix3d.h"
#include "GePlane.h"
#include <cmath>

namespace {
const double kPerspectiveDividerTol = 1e-100;
}


const GePoint3d GePoint3d::kOrigin = GePoint3d(0, 0, 0);


GePoint3d operator* (const GeMatrix3d& xfm, const GePoint3d& vect) {
	double divider = xfm.entry[3][0] * vect.x + xfm.entry[3][1] * vect.y + xfm.entry[3][2] * vect.z + xfm.entry[3][3];
	if (std::fabs(divider) <= kPerspectiveDividerTol) {
		return vect;
	}

	return GePoint3d(
		(xfm.entry[0][0] * vect.x + xfm.entry[0][1] * vect.y + xfm.entry[0][2] * vect.z + xfm.entry[0][3]) / divider,
		(xfm.entry[1][0] * vect.x + xfm.entry[1][1] * vect.y + xfm.entry[1][2] * vect.z + xfm.entry[1][3]) / divider,
		(xfm.entry[2][0] * vect.x + xfm.entry[2][1] * vect.y + xfm.entry[2][2] * vect.z + xfm.entry[2][3]) / divider);
}

GePoint3d& GePoint3d::setToProduct(const GeMatrix3d& mat, const GePoint3d& pnt)
{
	*this = mat * pnt;
	return *this;
}
GePoint3d& GePoint3d::transformBy(const GeMatrix3d& leftSide)
{
	Ge::ErrorCondition status = Ge::kOk;
	return this->transformBy(leftSide, status);
}
GePoint3d& GePoint3d::transformBy(const GeMatrix3d& leftSide, Ge::ErrorCondition& status)
{
	status = Ge::kOk;
	if (leftSide.isPerspective()) {
		double divider = leftSide.entry[3][0] * this->x + leftSide.entry[3][1] * this->y + leftSide.entry[3][2] * this->z + leftSide.entry[3][3];
		if (std::fabs(divider) <= kPerspectiveDividerTol) {
			status = Ge::kDegenerateGeometry;
			return *this;
		}
	}

	return *this = leftSide * *this;
}
GePoint3d& GePoint3d::rotateBy(double angle, const GeVector3d& vect) {
	return this->rotateBy(angle, vect, GePoint3d::kOrigin);
}
GePoint3d& GePoint3d::rotateBy(double angle, const GeVector3d& vect, const GePoint3d& basePoint)
{
	GeMatrix3d mat;
	mat.setToRotation(angle, vect, basePoint);
	this->transformBy(mat);
	return *this;
}
GePoint3d& GePoint3d::mirror(const GePlane& plane)
{
	GeMatrix3d mat;
	mat.setToMirroring(plane);
	this->transformBy(mat);
	return *this;
}
GePoint3d& GePoint3d::scaleBy(double scaleFactor)
{
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GePoint3d& GePoint3d::scaleBy(double scaleFactor, const GePoint3d& basePoint)
{
	GeMatrix3d mat;
	mat.setToScaling(scaleFactor, basePoint);
	this->transformBy(mat);
	return *this;
}
GePoint3d& GePoint3d::scaleBy(const GeScale3d& scaleFactor)
{
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GePoint2d GePoint3d::convert2d(const GePlanarEnt& planarEnt) const {
	GePlane plane(planarEnt.pointOnPlane(), planarEnt.normal());
	return plane.paramOf(*this);
}
GePoint3d& GePoint3d::scaleBy(const GeScale3d& scaleFactor, const GePoint3d& basePoint)
{
	GeMatrix3d mat;
	mat.setToScaling(scaleFactor, basePoint);
	this->transformBy(mat);
	return *this;
}
bool GePoint3d::isEqualTo(const GePoint3d& point) const {
	return this->isEqualTo(point, GeContext::gTol);
}
bool GePoint3d::isEqualTo(const GePoint3d& pnt, const GeTol& tol) const
{
	return ((*this - pnt).length() <= tol.equalPoint());
}
GePoint3d& GePoint3d::setToSum(const GePoint3d& vector1, const GeVector3d& vector2)
{
	this->x = vector1.x + vector2.x;
	this->y = vector1.y + vector2.y;
	this->z = vector1.z + vector2.z;
	return *this;
}
GePoint3d& GePoint3d::swapWithPoint(GePoint3d& point)
{
	GePoint3d p(point);
	point.set(this->x, this->y, this->z);
	this->set(p.x, p.y, p.z);
	return *this;
}
GePoint3d GePoint3d::project(const GePlane& plane, const GeVector3d& vect) const {
	GeVector3d normal = plane.normal();
	if (normal.isZeroLength(GeContext::gTol) || vect.isZeroLength(GeContext::gTol)) {
		return *this;
	}
	double divider = normal.dotProduct(vect);
	if (std::fabs(divider) <= GeContext::gTol.equalVector()) {
		return *this;
	}

	GeVector3d offset = plane.pointOnPlane() - *this;
	double t = normal.dotProduct(offset) / divider;
	return *this + vect * t;
}
GePoint3d GePoint3d::orthoProject(const GePlane& plane) const {
	GeVector3d normal = plane.normal();
	if (normal.isZeroLength(GeContext::gTol)) {
		return *this;
	}
	normal.normalize();

	GeVector3d offset = *this - plane.pointOnPlane();
	double signedDist = offset.dotProduct(normal);
	return *this - normal * signedDist;
}