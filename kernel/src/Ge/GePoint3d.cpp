#include "GePoint3d.h"
#include "GeMatrix3d.h"
#include "GePlane.h"


const GePoint3d GePoint3d::kOrigin = GePoint3d(0, 0, 0);


GePoint3d operator* (const GeMatrix3d& xfm, const GePoint3d& vect) {
	double divider = xfm.entry[3][0] * vect.x + xfm.entry[3][1] * vect.y + xfm.entry[3][2] * vect.z + xfm.entry[3][3];
	if (abs(divider) <= GeContext::gTol.equalPoint()) {
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
		if (abs(divider) <= GeContext::gTol.equalPoint()) {
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
	if (abs(pnt.x - this->x) > tol.equalPoint()) {
		return false;
	}
	if (abs(pnt.y - this->y) > tol.equalPoint()) {
		return false;
	}
	if (abs(pnt.z - this->z) > tol.equalPoint()) {
		return false;
	}
	return true;
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

	//获得点到平面的最近点
	GePoint3d closest = this->orthoProject(plane);

	//获得垂线和法向的夹角
	double angle = plane.normal().angle(vect);
	if (abs(angle) < GeContext::gTol.equalPoint()) {
		return closest;
	}

	//获得邻边尺寸
	double dist = this->distanceTo(closest);
	dist = dist / cos(angle);

	//获得方向标准化
	GeVector3d direction = vect.normal();

	//获得点
	GePoint3d point;
	point.set(this->x, this->y, this->z);
	point += (direction * dist);

	//判断点是否在平面上
	GeVector3d vec = point - closest;
	if (vec.isPerpendicularTo(plane.normal()) == false) {
		point.set(this->x, this->y, this->z);
		point += (direction * -dist);
	}

	return point;
}
GePoint3d GePoint3d::orthoProject(const GePlane& plane) const {

	GePoint3d point;

	do
	{
		//获得点到平面原点的距离(斜边)
		double dist = this->distanceTo(plane.pointOnPlane());
		if (abs(dist) < GeContext::gTol.equalPoint()) {
			point.set(this->x, this->y, this->z);
			break;
		}

		// 获得邻边长度
		GeVector3d vec = *this - plane.pointOnPlane();
		double angle = vec.angle(plane.normal());
		dist = dist * cos(angle);

		//获得平面法向
		GeVector3d normal = plane.normal();
		normal.normalize();
		normal.negate();

		//将点沿着法向移动
		point.set(this->x, this->y, this->z);
		point += (normal * dist);

		//判断点是否在平面上
		vec = point - plane.pointOnPlane();
		if (vec.isPerpendicularTo(plane.normal()) == false) {
			point.set(this->x, this->y, this->z);
			point += (normal * -dist);
		}

	} while (false);

	return point;
}