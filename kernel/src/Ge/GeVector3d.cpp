#include "GeVector3d.h"
#include "GeMatrix3d.h"
#include "GePointOnSurface.h"
#include "GePlane.h"

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
	double Max = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	if (Max > tol.equalPoint())
	{
		this->x = this->x / Max;
		this->y = this->y / Max;
		this->z = this->z / Max;
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
	double v1 = abs(this->x * vect.x + this->y * vect.y + this->z * vect.z);
	double v2 = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	double v3 = sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
	if (abs(v1 - (v2 * v3)) < 0.000001)
	{
		return 0.0;
	}
	return acos(v1 / (v2 * v3));
}
double GeVector3d::angleTo(const GeVector3d &vec) const
{
	double v1 = sqrt((vec.x - this->x) * (vec.x - this->x) + (vec.y - this->y) * (vec.y - this->y) + (vec.z - this->z) * (vec.z - this->z));
	double v2 = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	double v3 = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (abs(v1 - (v2 * v3)) < 0.000001)
	{
		return 0.0;
	}
	return acos((v2 * v2 + v3 * v3 - v1 * v1) / (2 * v2 * v3));
}
double GeVector3d::angleToCCW(const GeVector3d &vect) const
{
	double Angle = 0;

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

		// 叉乘得到向量的轴
		GeVector3d normal = v1.crossProduct(v2);

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
	double angle = this->angleTo(vect);
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
	if (this->length() < GeContext::gTol.equalVector())
	{
		return true;
	}
	return false;
}
bool GeVector3d::isParallelTo(const GeVector3d &vect) const
{
	return this->isParallelTo(vect, GeContext::gTol);
}
bool GeVector3d::isParallelTo(const GeVector3d &vec, const GeTol &tol) const
{
	double angle = this->angleTo(vec);
	if (abs(angle) < tol.equalVector() || abs(angle - PI) < tol.equalVector())
	{
		return true;
	}
	return false;
}
bool GeVector3d::isCodirectionalTo(const GeVector3d &vect) const
{
	return this->isCodirectionalTo(vect, GeContext::gTol);
}
bool GeVector3d::isCodirectionalTo(const GeVector3d &vec, const GeTol &tol) const
{
	GeVector3d V1(this->x, this->y, this->z);
	V1.normalize();
	GeVector3d V2(vec.x, vec.y, vec.z);
	V2.normalize();
	return V1.isEqualTo(V2, tol);
}
bool GeVector3d::isPerpendicularTo(const GeVector3d &vect) const
{
	return this->isPerpendicularTo(vect, GeContext::gTol);
}
bool GeVector3d::isPerpendicularTo(const GeVector3d &vec, const GeTol &tol) const
{
	double angle = this->angleTo(vec);
	if (abs(abs(angle) - PI / 2) < tol.equalVector())
	{
		return true;
	}
	return false;
}
GeVector3d GeVector3d::crossProduct(const GeVector3d &vect) const
{
	return GeVector3d(this->y * vect.z - vect.y * this->z, this->z * vect.x - this->x * vect.z, this->x * vect.y - vect.x * this->y);
}
GeMatrix3d GeVector3d::rotateTo(const GeVector3d &vector) const
{

	GeVector3d vec1 = *this;
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
	GeVector3d normal = vec1.crossProduct(vector);

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
	if (abs(vec.x - this->x) > tol.equalVector())
	{
		return false;
	}
	if (abs(vec.y - this->y) > tol.equalVector())
	{
		return false;
	}
	if (abs(vec.z - this->z) > tol.equalVector())
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
	if (this->length() < 1.0 + tol.equalVector())
	{
		return true;
	}
	return false;
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