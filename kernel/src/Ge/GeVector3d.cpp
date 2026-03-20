#include "GeVector3d.h"
#include "GeMatrix3d.h"
#include "GePointOnSurface.h"
#include "GePlane.h"
#include <cmath>

const GeVector3d GeVector3d::kIdentity = GeVector3d(0, 0, 0); // Additive identity vector.
const GeVector3d GeVector3d::kXAxis = GeVector3d(1, 0, 0);	  // X-Axis vector.
const GeVector3d GeVector3d::kYAxis = GeVector3d(0, 1, 0);	  // Y-Axis vector.
const GeVector3d GeVector3d::kZAxis = GeVector3d(0, 0, 1);	  // Z-Axis vector.

GeVector3d operator*(const GeMatrix3d &xfm, const GeVector3d &vect)
{
	double aXresult = xfm.entry[0][0] * vect.x + xfm.entry[0][1] * vect.y + xfm.entry[0][2] * vect.z;
	double aYresult = xfm.entry[1][0] * vect.x + xfm.entry[1][1] * vect.y + xfm.entry[1][2] * vect.z;
	double aZresult = xfm.entry[2][0] * vect.x + xfm.entry[2][1] * vect.y + xfm.entry[2][2] * vect.z;

	GeVector3d vec(vect);
	vec.x = aXresult;
	vec.y = aYresult;
	vec.z = aZresult;
	return vec;
}

GeVector3d::GeVector3d(const GePlanarEnt &plane, const GeVector2d &vector2d)
{
	this->set(plane, vector2d);
}
GeVector3d &GeVector3d::setToProduct(const GeMatrix3d &matrix, const GeVector3d &vect)
{
	*this = matrix * vect;
	return *this;
}
GeVector3d &GeVector3d::setToProduct(const GeVector3d &vect, double scale)
{
	x = scale * vect.x;
	y = scale * vect.y;
	z = scale * vect.z;
	return *this;
}
GeVector3d &GeVector3d::transformBy(const GeMatrix3d &xfm)
{
	return *this = xfm * *this;
}
double GeVector3d::angleOnPlane(const GePlanarEnt &plane) const
{

	double angle = this->angleTo(plane.normal());
	return PI / 2.0 - angle;
}
GeVector3d GeVector3d::normal() const
{
	return this->normal(GeContext::gTol);
}
GeVector3d GeVector3d::normal(const GeTol &tol) const
{
	GeVector3d vec(*this);
	return vec.normalize(tol);
}
GeVector3d &GeVector3d::normalize()
{
	return this->normalize(GeContext::gTol);
}
GeVector3d &GeVector3d::normalize(const GeTol &tol)
{
	double len = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	if (len > tol.equalVector())
	{
		this->x = this->x / len;
		this->y = this->y / len;
		this->z = this->z / len;
	}
	return *this;
}
GeVector3d &GeVector3d::rotateBy(double angle, const GeVector3d &axis)
{
	GeMatrix3d mat;
	mat.setToRotation(angle, axis);
	this->transformBy(mat);
	return *this;
}
GeVector3d &GeVector3d::mirror(const GeVector3d &line)
{
	GeMatrix3d mat;
	mat.setToMirroring(GePoint3d(line.x, line.y, line.z));
	this->transformBy(mat);
	return *this;
}
GeVector2d GeVector3d::convert2d(const GePlanarEnt &planarEnt) const
{
	GePlane plane(planarEnt.pointOnPlane(), planarEnt.normal());
	GePoint2d point = plane.paramOf(GePoint3d(this->x, this->y, this->z));
	return GeVector2d(point.x, point.y);
}
double GeVector3d::angle(const GeVector3d &vect) const
{
	GeVector3d v1(*this);
	GeVector3d v2(vect);
	if (v1.isZeroLength() || v2.isZeroLength())
	{
		return 0.0;
	}
	v1.normalize();
	v2.normalize();
	GeVector3d vCross = v1.crossProduct(v2);
	double dot = std::fabs(v1.dotProduct(v2));
	double angle = std::atan2(vCross.length(), dot);
	if (angle < 0.0)
	{
		return 0.0;
	}
	return angle;
}
double GeVector3d::angleTo(const GeVector3d &vec) const
{
	GeVector3d vThis(*this);
	GeVector3d vVec(vec);
	if (vThis.isZeroLength() || vVec.isZeroLength())
	{
		return 0.0;
	}
	vThis.normalize();
	vVec.normalize();
	GeVector3d vCross = vThis.crossProduct(vVec);
	return std::atan2(vCross.length(), vThis.dotProduct(vVec));
}
double GeVector3d::angleToCCW(const GeVector3d &vect) const
{
	double Angle = 0.0;
	if (this->isZeroLength() || vect.isZeroLength())
	{
		return 0.0;
	}

	do
	{

		GeVector3d v1(*this);
		GeVector3d v2(vect);
		v1.normalize();
		v2.normalize();

		// 判断是否为0度
		if (v1.isEqualTo(v2) == true)
		{
			break;
		}

		// 获得两向量夹角
		Angle = v1.angleTo(v2);
		if (std::fabs(Angle) <= GeContext::gTol.equalVector())
		{
			break;
		}

		// 叉乘得到向量的轴
		GeVector3d normal = v1.crossProduct(v2);
		if (normal.isZeroLength())
		{
			if (v1.dotProduct(v2) < 0.0)
			{
				Angle = PI;
			}
			break;
		}

		// 将矢量旋转
		v1.rotateBy(Angle, normal);

		// 通过判断两矢量是否平行得到角度
		if (v1.isEqualTo(v2) == false)
		{
			Angle = PI * 2 - Angle;
		}

	} while (false);

	return Angle;
}
double GeVector3d::angleToCCW(const GeVector3d &vect, const GeVector3d &normal) const
{
	if (this->isZeroLength() || vect.isZeroLength())
	{
		return 0.0;
	}
	double angle = this->angleTo(vect);
	if (normal.isZeroLength())
	{
		return angle;
	}
	GeVector3d tempVec = *this;
	tempVec.rotateBy(angle, normal);
	tempVec.normalize();
	if (tempVec.isEqualTo(vect.normal()) == false)
	{
		angle = PI * 2 - angle;
	}
	return angle;
}
const GePoint3d &GeVector3d::asPoint() const
{
	return *reinterpret_cast<const GePoint3d *>(this);
}
GeVector3d &GeVector3d::setToSum(const GeVector3d &vector1, const GeVector3d &vector2)
{
	this->x = vector1.x + vector2.x;
	this->y = vector1.y + vector2.y;
	this->z = vector1.z + vector2.z;
	return *this;
}
GeVector3d GeVector3d::perpVector(const GeVector3d &OrthoVector) const
{
	return GeVector3d(this->y * OrthoVector.z - OrthoVector.y * this->z, this->z * OrthoVector.x - this->x * OrthoVector.z, this->x * OrthoVector.y - OrthoVector.x * this->y);
}
double GeVector3d::length() const
{
	double Var = this->x * this->x + this->y * this->y + this->z * this->z;
	return sqrt(Var);
}
void GeVector3d::setLength(double length)
{
	GeVector3d vec = this->normal();
	vec.x = vec.x * length;
	vec.y = vec.y * length;
	vec.z = vec.z * length;
	this->set(vec.x, vec.y, vec.z);
}
bool GeVector3d::isZeroLength() const
{
	return this->isZeroLength(GeContext::gTol);
}
bool GeVector3d::isZeroLength(const GeTol &tol) const
{
	double tolValue = tol.equalVector();
	return (this->x * this->x + this->y * this->y + this->z * this->z) <= (tolValue * tolValue);
}
bool GeVector3d::isParallelTo(const GeVector3d &vect) const
{
	return this->isParallelTo(vect, GeContext::gTol);
}
bool GeVector3d::isParallelTo(const GeVector3d &vec, const GeTol &tol) const
{
	double len1 = this->length();
	double len2 = vec.length();
	if (len1 <= tol.equalVector())
	{
		return len2 <= tol.equalVector();
	}
	if (len2 <= tol.equalVector())
	{
		return false;
	}
	GeVector3d v1(*this);
	GeVector3d v2(vec);
	v1 /= len1;
	v2 /= len2;
	return v1.isEqualTo(v2, tol) || v1.isEqualTo(-v2, tol);
}
bool GeVector3d::isCodirectionalTo(const GeVector3d &vect) const
{
	return this->isCodirectionalTo(vect, GeContext::gTol);
}
bool GeVector3d::isCodirectionalTo(const GeVector3d &vec, const GeTol &tol) const
{
	double len1 = this->length();
	double len2 = vec.length();
	if (len1 <= tol.equalVector())
	{
		return len2 <= tol.equalVector();
	}
	if (len2 <= tol.equalVector())
	{
		return false;
	}
	GeVector3d v1(*this);
	GeVector3d v2(vec);
	v1 /= len1;
	v2 /= len2;
	return v1.isEqualTo(v2, tol);
}
bool GeVector3d::isPerpendicularTo(const GeVector3d &vect) const
{
	return this->isPerpendicularTo(vect, GeContext::gTol);
}
bool GeVector3d::isPerpendicularTo(const GeVector3d &vec, const GeTol &tol) const
{
	GeVector3d v1(*this);
	v1.normalize(tol);
	if (v1.isZeroLength(tol))
	{
		return true;
	}
	GeVector3d v2(vec);
	v2.normalize(tol);
	if (v2.isZeroLength(tol))
	{
		return true;
	}
	return std::fabs(v1.dotProduct(v2)) <= tol.equalVector();
}
GeVector3d GeVector3d::crossProduct(const GeVector3d &vect) const
{
	return GeVector3d(this->y * vect.z - vect.y * this->z, this->z * vect.x - this->x * vect.z, this->x * vect.y - vect.x * this->y);
}
GeMatrix3d GeVector3d::rotateTo(const GeVector3d &vector) const
{

	GeVector3d vec1 = *this;
	if (vec1.isZeroLength() || vector.isZeroLength())
	{
		return GeMatrix3d::kIdentity;
	}
	vec1.normalize();
	GeVector3d vec2 = vector;
	vec2.normalize();

	// 判断两个向量是否一样
	if (vec1.isEqualTo(vec2) == true)
	{
		return GeMatrix3d::kIdentity;
	}
	// 判断两个向量是否相反
	if (vec1.isEqualTo(GeVector3d(-vec2.x, -vec2.y, -vec2.z)) == true)
	{
		GeMatrix3d mat;
		mat.setToMirroring(GePlane(GePoint3d::kOrigin, vector));
		return mat;
	}

	GeMatrix3d mat;

	// 两个向量的角度
	double angle = vec1.angleTo(vec2);

	// 叉乘得到法向
	GeVector3d normal = vec1.crossProduct(vec2);
	if (normal.isZeroLength())
	{
		return GeMatrix3d::kIdentity;
	}

	// 获得矩阵
	mat.setToRotation(angle, normal);
	vec1.transformBy(mat);
	if (vec1.isEqualTo(vec2) == false)
	{
		mat.setToRotation(0 - angle, normal);
	}

	return mat;
}
GeMatrix3d GeVector3d::rotateTo(const GeVector3d &vector, const GeVector3d &axis) const
{
	GeVector3d vec1 = *this;
	if (vec1.isZeroLength() || vector.isZeroLength() || axis.isZeroLength())
	{
		return GeMatrix3d::kIdentity;
	}
	vec1.normalize();
	GeVector3d vec2 = vector;
	vec2.normalize();

	// 判断两个向量是否一样
	if (vec1.isEqualTo(vec2) == true)
	{
		return GeMatrix3d::kIdentity;
	}

	// 两个向量的角度
	double angle = vec1.angleTo(vec2);

	// 叉乘得到法向
	GeVector3d normal = axis;
	normal.normalize();

	// 获得矩阵
	GeMatrix3d mat;
	mat.setToRotation(angle, normal);
	vec1.transformBy(mat);
	if (vec1.isEqualTo(vec2) == false)
	{
		mat.setToRotation(0 - angle, normal);
	}

	return mat;
}

GeVector3d GeVector3d::project(const GeVector3d &planeNormal, const GeVector3d &projectDirection) const
{
	return this->project(planeNormal, projectDirection, GeContext::gTol);
}
GeVector3d GeVector3d::project(const GeVector3d &planeNormal, const GeVector3d &projectDirection, const GeTol &tol) const
{

	if (this->length() < tol.equalVector() || planeNormal.length() < tol.equalVector() || projectDirection.length() < tol.equalVector())
	{
		return GeVector3d(0, 0, 0);
	}

	GePoint3d point(this->x, this->y, this->z);
	GePlane plane;
	plane.set(GePoint3d(0, 0, 0), planeNormal);
	point = point.project(plane, projectDirection);
	return GeVector3d(point.x, point.y, point.z);
}
GeVector3d GeVector3d::orthoProject(const GeVector3d &planeNormal) const
{
	return this->orthoProject(planeNormal, GeContext::gTol);
}
GeVector3d GeVector3d::orthoProject(const GeVector3d &planeNormal, const GeTol &tol) const
{
	if (this->length() < tol.equalVector() || planeNormal.length() < tol.equalVector())
	{
		return GeVector3d(0, 0, 0);
	}

	GePoint3d point(this->x, this->y, this->z);
	GePlane plane;
	plane.set(GePoint3d(0, 0, 0), planeNormal);
	point = point.orthoProject(plane);
	return GeVector3d(point.x, point.y, point.z);
}
bool GeVector3d::isEqualTo(const GeVector3d &vect) const
{
	return this->isEqualTo(vect, GeContext::gTol);
}
bool GeVector3d::isEqualTo(const GeVector3d &vec, const GeTol &tol) const
{
	if (std::fabs(vec.x - this->x) > tol.equalVector())
	{
		return false;
	}
	if (std::fabs(vec.y - this->y) > tol.equalVector())
	{
		return false;
	}
	if (std::fabs(vec.z - this->z) > tol.equalVector())
	{
		return false;
	}
	return true;
}
bool GeVector3d::isUnitLength() const
{
	return this->isUnitLength(GeContext::gTol);
}
bool GeVector3d::isUnitLength(const GeTol &tol) const
{
	return std::fabs(this->length() - 1.0) <= tol.equalVector();
}
GeVector3d &GeVector3d::set(const GePlanarEnt &plane, const GeVector2d &vect)
{
	GePointOnSurface surface;
	surface.setSurface((GeSurface &)plane);
	surface.setParameter(GePoint2d(vect.x, vect.y));
	GePoint3d point = surface.point();
	return this->set(point.x, point.y, point.z);
}
GeVector3d::operator GeMatrix3d() const
{
	GeMatrix3d mat;
	mat.setToTranslation(*this);
	return mat;
}