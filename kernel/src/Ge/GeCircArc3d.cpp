#include "GeCircArc3d.h"
#include "GeScale3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeCircArc3d.h"
#include "GePlane.h"
#include "GePointOnCurve3d.h"
#include "GeInterval.h"
#include "GeBoundBlock3d.h"
#include "GeMatrix3d.h"
#include "GeCircArc2d.h"
#include "GeImpl.h"
#include <cmath>


GeCircArc3d::GeCircArc3d() {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);
}
GeCircArc3d::GeCircArc3d(const GeCircArc3d& arc) {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);
	this->set(arc.center(), arc.normal(), arc.refVec(), arc.radius(), arc.startAng(), arc.endAng());
}
GeCircArc3d::GeCircArc3d(const GePoint3d& cent, const GeVector3d& nrm, double radius) {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);

	this->set(cent, nrm, radius);
}
GeCircArc3d::GeCircArc3d(const GePoint3d& cent, const GeVector3d& nrm, const GeVector3d& refVec, double radius, double startAngle, double endAngle) {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);

	this->set(cent, nrm, refVec, radius, startAngle, endAngle);
}
GeCircArc3d::GeCircArc3d(const GePoint3d& startPoint, const GePoint3d& pnt, const GePoint3d& endPoint) {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);

	this->set(startPoint, pnt, endPoint);
}




GePoint3dArray GeCircArc3d::toLineSegment(const GePoint3d& center, double _radius, const GeVector3d& normal, int numSegments)
{
	// 获得变换矩阵
	GeMatrix3d mat;
	bool isMatrix = false;
	if (normal.isEqualTo(GeVector3d::kZAxis) == false)
	{
		isMatrix = true;
		mat.setToPlaneToWorld(normal);
	}

	// 获得二维点
	GePoint2dArray tempVertexs = GeCircArc2d::toLineSegment(GePoint2d::kOrigin, _radius, numSegments);

	// 计算顶点
	GePoint3dArray vertexs;
	for (int i = 0; i < tempVertexs.length(); i++)
	{
		GePoint3d position(tempVertexs[i].x, tempVertexs[i].y, 0);
		if (isMatrix == true)
		{
			position.transformBy(mat);
		}

		position.x += center.x;
		position.y += center.y;
		position.z += center.z;
		vertexs.append(position);
	}
	return vertexs;
}
GePoint3dArray GeCircArc3d::toLineSegment(const GePoint3d& center, double _radius, double _startAng, double _endAng, const GeVector3d& normal, int numSegments)
{

	// 获得变换矩阵
	GeMatrix3d mat;
	bool isMatrix = false;
	if (normal.isEqualTo(GeVector3d::kZAxis) == false)
	{
		isMatrix = true;
		mat.setToPlaneToWorld(normal);
	}

	// 获得二维点
	GePoint2dArray tempVertexs = GeCircArc2d::toLineSegment(GePoint2d::kOrigin, _radius, _startAng, _endAng, numSegments);

	// 计算顶点
	GePoint3dArray vertexs;
	for (int i = 0; i < tempVertexs.length(); i++)
	{
		GePoint3d position(tempVertexs[i].x, tempVertexs[i].y, 0.0);
		if (isMatrix == true)
		{
			position.transformBy(mat);
		}

		position.x += center.x;
		position.y += center.y;
		position.z += center.z;

		vertexs.append(position);
	}
	return vertexs;
}
GePoint3dArray GeCircArc3d::toLineSegment(const GePoint3d& center, double _radius, const GeVector3d& normal, const GeVector3d& startVector, const double sweepAngle, int numSegments){
      
	// 获得每段圆弧划过的弧度
	double angleSegment = sweepAngle / numSegments;

	// 创建几何对象
	GePoint3dArray vertexs;
	GePoint3d basePoint = GePoint3d::kOrigin;
	basePoint += (startVector.normal() * _radius);
	for (int i = 0; i < numSegments + 1; i++) {
		GePoint3d position(basePoint.x, basePoint.y, basePoint.z);
		position.rotateBy(angleSegment * i, normal);
		position.x += center.x;
		position.y += center.y;
		position.z += center.z;

		vertexs.append(position);
	}
	return vertexs;
}
bool GeCircArc3d::pointToCenter(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, GePoint3d& _center)
{
	return GeCircArc3d::pointToCenter(startPoint, secondPoint, endPoint, _center, 0.00001);
}
bool GeCircArc3d::pointToCenter(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, GePoint3d& _center, double tol)
{
	if (startPoint.isEqualTo(secondPoint, tol) == true) {
		return false;
	}
	if (GeLine3d(startPoint, secondPoint).isOn(endPoint, tol) == true) {
		return false;
	}
	double x1 = startPoint.x, y1 = startPoint.y, z1 = startPoint.z;
	double x2 = secondPoint.x, y2 = secondPoint.y, z2 = secondPoint.z;
	double x3 = endPoint.x, y3 = endPoint.y, z3 = endPoint.z;

	double a1 = (y1 * z2 - y2 * z1 - y1 * z3 + y3 * z1 + y2 * z3 - y3 * z2),
		b1 = -(x1 * z2 - x2 * z1 - x1 * z3 + x3 * z1 + x2 * z3 - x3 * z2),
		c1 = (x1 * y2 - x2 * y1 - x1 * y3 + x3 * y1 + x2 * y3 - x3 * y2),
		d1 = -(x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1);

	double a2 = 2 * (x2 - x1),
		b2 = 2 * (y2 - y1),
		c2 = 2 * (z2 - z1),
		d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

	double a3 = 2 * (x3 - x1),
		b3 = 2 * (y3 - y1),
		c3 = 2 * (z3 - z1),
		d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

	_center.x = -(b1 * c2 * d3 - b1 * c3 * d2 - b2 * c1 * d3 + b2 * c3 * d1 + b3 * c1 * d2 - b3 * c2 * d1)
		/ (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
	_center.y = (a1 * c2 * d3 - a1 * c3 * d2 - a2 * c1 * d3 + a2 * c3 * d1 + a3 * c1 * d2 - a3 * c2 * d1)
		/ (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
	_center.z = -(a1 * b2 * d3 - a1 * b3 * d2 - a2 * b1 * d3 + a2 * b3 * d1 + a3 * b1 * d2 - a3 * b2 * d1)
		/ (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - a3 * b2 * c1);
	return true;
}
bool GeCircArc3d::isClockWise(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint)
{
	GeVector3d v1 = startPoint - secondPoint;
	GeVector3d v2 = endPoint - secondPoint;
	GeVector3d normal = v1.crossProduct(v2);
	if (normal.isZeroLength()) {
		return false;
	}
	normal.normalize();
	return GeCircArc3d::isClockWise(startPoint, secondPoint, endPoint, normal);
}
bool GeCircArc3d::isClockWise(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, const GeVector3d& normal) {

	GePoint3d center;
	if (pointToCenter(startPoint, secondPoint, endPoint, center) == false) {
		return false;
	}

	GeMatrix3d mat;
	mat.setToWorldToPlane(GePlane(center, normal));

	GePoint3d p1 = startPoint;
	GePoint3d p2 = secondPoint;
	GePoint3d p3 = endPoint;
	p1.transformBy(mat);
	p2.transformBy(mat);
	p3.transformBy(mat);
	//double var = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
	double var = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	if (var > 0)
	{
		return false;
	}
	return true;
}
bool GeCircArc3d::pointToBulge(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, double& bulge) {
	GeVector3d v1 = startPoint - secondPoint;
	GeVector3d v2 = endPoint - secondPoint;
	GeVector3d normal = v1.crossProduct(v2);
	normal.normalize();
	return GeCircArc3d::pointToBulge(startPoint, secondPoint, endPoint, normal, bulge);
}
bool GeCircArc3d::pointToBulge(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, const GeVector3d& normal, double& bulge) {
	return GeCircArc3d::pointToBulge(startPoint, secondPoint, endPoint, normal, bulge, 0.0002);
}
bool GeCircArc3d::pointToBulge(const GePoint3d& startPoint, const GePoint3d& secondPoint, const GePoint3d& endPoint, const GeVector3d& normal, double& bulge, const double tol) {

	GePoint3d center;
	if (GeCircArc3d::pointToCenter(startPoint, secondPoint, endPoint, center, tol) == false) {
		return false;
	}

	double radius = center.distanceTo(startPoint);

	GeVector3d refVec = startPoint - center;
	refVec.normalize();
	double angle = refVec.angleToCCW((endPoint - center).normal(), normal);

	bool isClockWise = GeCircArc3d::isClockWise(startPoint, secondPoint, endPoint, normal);
	if (isClockWise == true) {
		bulge = tan(angle / 4.0);
	}
	else {
		bulge = 0 - tan(angle / 4.0);
	}

	return true;
}




Adesk::Boolean GeCircArc3d::intersectWith(const GeLinearEnt3d& line, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol) const {
	GePoint3dArray points = this->intersectWith(line, tol);
	intn = points.length();
	if (intn == 0) {
		return false;
	}
	if (points.length() == 1) {
		p1.set(points[0].x, points[0].y, points[0].z);
	}
	else if (points.length() == 2) {
		p1.set(points[0].x, points[0].y, points[0].z);
		p2.set(points[1].x, points[1].y, points[1].z);
	}
	return true;
}
Adesk::Boolean GeCircArc3d::intersectWith(const GeCircArc3d& arc, int& intn, GePoint3d& p1, GePoint3d& p2, const GeTol& tol) const {
	GePoint3dArray points = this->intersectWith(arc, tol);
	intn = points.length();
	if (intn == 0) {
		return false;
	}
	if (points.length() == 1) {
		p1.set(points[0].x, points[0].y, points[0].z);
	}
	else if (points.length() == 2) {
		p1.set(points[0].x, points[0].y, points[0].z);
		p2.set(points[1].x, points[1].y, points[1].z);
	}
	return true;
}
GePoint3dArray GeCircArc3d::intersectWith(const GeLinearEnt3d& line) const
{
	return this->intersectWith(line, GeContext::gTol);
}
GePoint3dArray GeCircArc3d::intersectWith(const GeLinearEnt3d& line, const GeTol& tol) const
{
	GePoint3dArray inters;

	do
	{
		// 求圆心到直线的垂点
		GePoint3d vertical = GeLinearEnt3d::vertical(this->center(), line, tol);

		// 获得垂点到圆心的距离
		double dist = this->center().distanceTo(vertical);
		if (std::fabs(dist - this->radius()) < tol.equalPoint())
		{
			if (line.isOn(vertical, tol) == true && this->isOn(vertical, tol) == true)
			{
				inters.append(vertical);
			}
			break;
		}
		if (dist > this->radius() + tol.equalPoint())
		{
			break;
		}

		double e = sqrt(this->radius() * this->radius() - dist * dist);

		GeVector3d direction = line.direction();
		if (direction.isZeroLength(tol))
		{
			break;
		}
		direction.normalize();

		GePoint3d point = vertical + direction * e;
		if (line.isOn(point, tol) == true && this->isOn(point, tol) == true)
		{
			inters.append(point);
		}

		GePoint3d point2 = vertical - direction * e;
		if (line.isOn(point2, tol) == true && this->isOn(point2, tol) == true)
		{
			bool isAppend = true;
			if (inters.length() > 0) {
				if (point2.isEqualTo(inters[0], tol) == true) {
					isAppend = false;
				}
			}
			if (isAppend == true) {
				inters.append(point2);
			}
		}

	} while (false);

	return inters;
}
GePoint3dArray GeCircArc3d::intersectWith(const GeCircArc3d& arc) const
{
	return this->intersectWith(arc, GeContext::gTol);
}
GePoint3dArray GeCircArc3d::intersectWith(const GeCircArc3d& arc, const GeTol& tol) const
{
	GePoint3dArray intersects;

	do {

		if (this->normal().isParallelTo(arc.normal(), tol) == true)
		{
			GePlane planeThis(this->center(), this->normal());
			if (planeThis.isOn(arc.center(), tol) == false)
			{
				break;
			}

			double dist = this->center().distanceTo(arc.center());
			double radiusThis = this->radius();
			double radiusOther = arc.radius();

			// 外离
			if (dist > radiusThis + radiusOther + tol.equalPoint())
			{
				break;
			}
			// 内离
			if (dist < std::fabs(radiusThis - radiusOther) - tol.equalPoint())
			{
				break;
			}
			// 圆心重合
			if (dist < tol.equalPoint())
			{
				break;
			}
			
			GeVector3d xAxis = arc.center() - this->center();
			xAxis.normalize();
			GeVector3d zAxis = this->normal();
			GeVector3d yAxis = zAxis.crossProduct(xAxis);
			yAxis.normalize();

			double a = (radiusThis * radiusThis - radiusOther * radiusOther + dist * dist) / (2.0 * dist);
			double h2 = radiusThis * radiusThis - a * a;
			if (h2 < -tol.equalPoint()) {
				break;
			}
			double h = std::sqrt(h2 < 0.0 ? 0.0 : h2);

			GePoint3d basePoint = this->center() + xAxis * a;
			GePoint3d point1 = basePoint + yAxis * h;
			GePoint3d point2 = basePoint - yAxis * h;
			if (this->isOn(point1, tol) && arc.isOn(point1, tol))
			{
				intersects.append(point1);
			}
			if (this->isOn(point2, tol) && arc.isOn(point2, tol))
			{
				bool isDuplicate = false;
				for (int i = 0; i < intersects.length(); ++i)
				{
					if (intersects[i].isEqualTo(point2, tol))
					{
						isDuplicate = true;
						break;
					}
				}
				if (isDuplicate == false)
				{
					intersects.append(point2);
				}
			}
		}
		else {
			GePlane planeThis(this->center(), this->normal());
			GePlane planeOther(arc.center(), arc.normal());
			GeLine3d line;
			if (planeThis.intersectWith(planeOther, line, tol) == false)
			{
				break;
			}

			GePoint3dArray pointsThis = this->intersectWith(line, tol);
			GePoint3dArray pointsOther = arc.intersectWith(line, tol);
			for (int i = 0; i < pointsThis.length(); ++i)
			{
				for (int j = 0; j < pointsOther.length(); ++j)
				{
					if (pointsThis[i].isEqualTo(pointsOther[j], tol) == false)
					{
						continue;
					}

					bool isDuplicate = false;
					for (int k = 0; k < intersects.length(); ++k)
					{
						if (intersects[k].isEqualTo(pointsThis[i], tol))
						{
							isDuplicate = true;
							break;
						}
					}
					if (isDuplicate == false)
					{
						intersects.append(pointsThis[i]);
					}
					break;
				}
			}
		}

	} while (false);

	return intersects;
}

Adesk::Boolean GeCircArc3d::tangent(const GePoint3d& pnt, GeLine3d& line) const {
	return this->tangent(pnt, line, GeContext::gTol);
}
Adesk::Boolean GeCircArc3d::tangent(const GePoint3d& pnt, GeLine3d& line, const GeTol& tol) const {
	bool isValue = false;

	do
	{

		if (pnt.isEqualTo(this->center(), tol) == true)
		{
			break;
		}

		line.set(pnt, this->center());
		line.rotateBy(PI / 2.0, this->normal());

		isValue = true;

	} while (false);

	return isValue;
}
void GeCircArc3d::getPlane(GePlane& plane) const {
	plane.set(this->center(), this->normal());
}

Adesk::Boolean GeCircArc3d::isInside(const GePoint3d& pnt) const
{
	return this->isInside(pnt, GeContext::gTol);
}
Adesk::Boolean GeCircArc3d::isInside(const GePoint3d& pnt, const GeTol& tol) const
{

	// 判断点是否在圆平面上
	GePlane plane(this->center(), this->normal());
	if (plane.isOn(pnt, tol) == false)
	{
		return false;
	}

	// 判断点是否在圆内
	double dist = this->center().distanceTo(pnt);
	if (dist > this->radius() + tol.equalPoint())
	{
		return false;
	}
	return true;
}

GePoint3d GeCircArc3d::center() const {
	return GE_IMP_CIRCARC3D(this->m_pImpl)->center;
}
GeVector3d GeCircArc3d::normal() const {
	return GE_IMP_CIRCARC3D(this->m_pImpl)->normal;
}
GeVector3d GeCircArc3d::refVec() const {
	return GE_IMP_CIRCARC3D(this->m_pImpl)->refVec;
}
double GeCircArc3d::radius() const {
	return GE_IMP_CIRCARC3D(this->m_pImpl)->radius;
}
double GeCircArc3d::startAng() const {
	return GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle;
}
double GeCircArc3d::endAng() const {
	return GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle;
}
GePoint3d GeCircArc3d::startPoint() const
{
	GePoint3d startPoint;
	this->hasStartPoint(startPoint);
	return startPoint;
}
GePoint3d GeCircArc3d::endPoint() const
{
	GePoint3d endPoint;
	this->hasEndPoint(endPoint);
	return endPoint;
}
GeCircArc3d& GeCircArc3d::setCenter(const GePoint3d& cen) {
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = cen;
	return *this;
}
GeCircArc3d& GeCircArc3d::setAxes(const GeVector3d& normal, const GeVector3d& refVec) {
	GeVector3d nrm = normal;
	if (nrm.isZeroLength()) {
		nrm = GeVector3d::kZAxis;
	}
	nrm.normalize();

	GeVector3d ref = refVec;
	if (ref.isZeroLength()) {
		ref = GeVector3d::kXAxis;
	}
	// 参考向量强制投影到圆所在平面
	ref = ref.orthoProject(nrm);
	if (ref.isZeroLength()) {
		GeVector3d axis = (std::fabs(nrm.z) < 0.9) ? GeVector3d::kZAxis : GeVector3d::kXAxis;
		ref = axis.crossProduct(nrm);
	}
	ref.normalize();

	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec = ref;
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = nrm;
	return *this;
}
GeCircArc3d& GeCircArc3d::setRadius(double radius) {
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = radius;
	return *this;
}
GeCircArc3d& GeCircArc3d::setAngles(double startAngle, double endAngle) {
	double sweep = endAngle - startAngle;
	double absSweep = std::fabs(sweep);
	double start = std::fmod(startAngle, 2.0 * PI);
	if (start < 0.0) {
		start += 2.0 * PI;
	}
	double end = std::fmod(endAngle, 2.0 * PI);
	if (end < 0.0) {
		end += 2.0 * PI;
	}

	if (absSweep >= 2.0 * PI - GeContext::gTol.equalPoint())
	{
		end = start + (sweep >= 0.0 ? 2.0 * PI : -2.0 * PI);
	}

	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = start;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = end;
	return *this;
}

GeCircArc3d& GeCircArc3d::set(const GePoint3d& cent, const GeVector3d& nrm, double radius) {
	GeVector3d normal = nrm;
	if (normal.isZeroLength()) {
		normal = GeVector3d::kZAxis;
	}
	normal.normalize();

	GeVector3d refVec = (std::fabs(normal.z) < 0.9) ? GeVector3d::kZAxis.crossProduct(normal) : GeVector3d::kXAxis.crossProduct(normal);
	if (refVec.isZeroLength()) {
		refVec = GeVector3d::kYAxis.crossProduct(normal);
	}
	refVec.normalize();

	GE_IMP_CIRCARC3D(this->m_pImpl)->center = cent;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = radius;
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.set(refVec.x, refVec.y, refVec.z);
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = normal;
	this->setAngles(0.0, 2.0 * PI);
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GePoint3d& cent, const GeVector3d& nrm, const GeVector3d& refVec, double radius, double startAngle, double endAngle) {
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = cent;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = radius;
	this->setAxes(nrm, refVec);
	this->setAngles(startAngle, endAngle);
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GePoint3d& startPoint, const GePoint3d& pnt, const GePoint3d& endPoint) {
	GePoint3d center;
	if (GeCircArc3d::pointToCenter(startPoint, pnt, endPoint, center) == false) {
		this->setRadius(0.0);
		return *this;
	}

	GeVector3d ref = (startPoint - center).normal();
	GeVector3d normal = (startPoint - center).crossProduct(endPoint - center);
	if (normal.isZeroLength()) {
		normal = (startPoint - center).crossProduct(pnt - center);
	}
	if (normal.isZeroLength()) {
		normal = GeVector3d::kZAxis;
	}
	normal.normalize();
	this->setAxes(normal, ref);

	GE_IMP_CIRCARC3D(this->m_pImpl)->center = center;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = startPoint.distanceTo(center);
	double endAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.angleToCCW((endPoint - center).normal(), this->normal());
	this->setAngles(0.0, endAngle);
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GePoint3d& startPoint, const GePoint3d& endPoint, double bulge) {

	GePoint3d midPoint = startPoint + (endPoint - startPoint) / 2;
	double dist = startPoint.distanceTo(midPoint);
	double angle = atan(bulge) * 4;
	double adjacent = dist / tan(angle / 2.0);

	GeVector3d normal = this->normal();
	if (normal.isZeroLength()) {
		normal = GeVector3d::kZAxis;
	}
	normal.normalize();

	GeVector3d refVec = startPoint - midPoint;
	refVec.normalize();
	refVec.rotateBy(0 - PI / 2.0, normal);

	GePoint3d center = midPoint;
	center += (refVec * adjacent);

	this->setAxes(normal, (startPoint - center).normal());
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = center;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = center.distanceTo(startPoint);
	double startAngle = 0.0;
	double endAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.angleToCCW((endPoint - center).normal(), this->normal());
	if (bulge < 0) {
		startAngle = endAngle;
		endAngle = 2.0 * PI;
	}
	this->setAngles(startAngle, endAngle);
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GeCurve3d& curve1, const GeCurve3d& curve2, double radius, double& param1, double& param2, Adesk::Boolean& success) {
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeCurve3d& curve3, double& param1, double& param2, double& param3, Adesk::Boolean& success) {
	return *this;
}





GeCircArc3d& GeCircArc3d::operator=(const GeCircArc3d& arc)
{
	this->set(arc.center(), arc.normal(), arc.refVec(), arc.radius(), arc.startAng(), arc.endAng());
	return *this;
}

bool GeCircArc3d::isKindOf(Ge::EntityId entType) const
{
	if (entType == Ge::kEntity3d || entType == Ge::kCurve3d || entType == this->type())
	{
		return true;
	}
	return false;
}
Ge::EntityId GeCircArc3d::type() const
{
	return Ge::EntityId::kCircArc3d;
}
GeCircArc3d* GeCircArc3d::copy() const
{
	return new GeCircArc3d(*this);
}
bool GeCircArc3d::operator==(const GeCircArc3d& entity) const
{
	return this->isEqualTo(entity);
}
bool GeCircArc3d::operator!=(const GeCircArc3d& entity) const
{
	return !(*this == entity);
}
bool GeCircArc3d::isEqualTo(const GeCircArc3d& entity) const
{
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeCircArc3d::isEqualTo(const GeCircArc3d& entity, const GeTol& tol) const
{
	auto angleDiff = [](double a, double b) {
		double diff = std::fmod(a - b, 2.0 * PI);
		if (diff < 0.0)
		{
			diff += 2.0 * PI;
		}
		if (diff > PI)
		{
			diff = 2.0 * PI - diff;
		}
		return diff;
	};

	if (this->center().isEqualTo(entity.center(), tol) == false)
	{
		return false;
	}
	if (std::fabs(this->radius() - entity.radius()) > tol.equalPoint())
	{
		return false;
	}
	if (this->normal().isEqualTo(entity.normal(), tol) == false)
	{
		return false;
	}
	bool thisClosed = this->isClosed(tol);
	bool entityClosed = entity.isClosed(tol);
	if (thisClosed != entityClosed)
	{
		return false;
	}
	if (thisClosed == true)
	{
		if (this->refVec().isEqualTo(entity.refVec(), tol) == false)
		{
			return false;
		}
		return true;
	}
	if (angleDiff(this->startAng(), entity.startAng()) > tol.equalPoint())
	{
		return false;
	}
	if (angleDiff(this->endAng(), entity.endAng()) > tol.equalPoint())
	{
		return false;
	}
	if (this->refVec().isEqualTo(entity.refVec(), tol) == false)
	{
		return false;
	}
	return true;
}
GeCircArc3d& GeCircArc3d::transformBy(const GeMatrix3d& xfm)
{

	GePoint3d center = this->center();
	GePoint3d startPoint = this->startPoint();
	GePoint3d endPoint = this->endPoint();
	center.transformBy(xfm);
	startPoint.transformBy(xfm);
	endPoint.transformBy(xfm);

	// 求参照轴和法线
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.transformBy(xfm);
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.normalize();
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal.transformBy(xfm);
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal.normalize();

	// 求圆心和半径
	this->setCenter(center);
	this->setRadius(this->center().distanceTo(startPoint));

	// 获得法向
	GeVector3d normal = this->normal();

	// 求起始角度和终止角度
	GeVector3d vector = startPoint - this->center();
	vector.normalize();
	double startAngle = this->refVec().angleToCCW(vector, normal);

	vector = endPoint - this->center();
	vector.normalize();
	double endAngle = this->refVec().angleToCCW(vector, normal);

	this->setAngles(startAngle, endAngle);

	return *this;
}
GeCircArc3d& GeCircArc3d::translateBy(const GeVector3d& translateVec)
{
	this->setCenter(this->center().translation(translateVec));
	return *this;
}
GeCircArc3d& GeCircArc3d::rotateBy(double angle, const GeVector3d& vec)
{
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeCircArc3d& GeCircArc3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
	GeMatrix3d mat;
	mat.setToRotation(angle, vec, wrtPoint);
	this->transformBy(mat);
	return *this;
}
GeCircArc3d& GeCircArc3d::mirror(const GePlane& plane)
{
	GeMatrix3d mat;
	mat.setToMirroring(plane);
	this->transformBy(mat);

	return *this;
}
GeCircArc3d& GeCircArc3d::scaleBy(double scaleFactor)
{
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeCircArc3d& GeCircArc3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
	GePoint3d center = this->center();
	center.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	this->setCenter(center);
	this->setRadius(this->radius() * scaleFactor);
	return *this;
}
bool GeCircArc3d::isOn(const GePoint3d& pnt) const
{
	return this->isOn(pnt, GeContext::gTol);
}
bool GeCircArc3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
	double param = 0.0;
	return this->isOn(pnt, param, tol);
}
void GeCircArc3d::getSplitCurves(double param, GeCurve3d*& piece1, GeCurve3d*& piece2) const
{
	piece1 = NULL;
	piece2 = NULL;

	GePointOnCurve3d pointOnCurve(*this, param);
	GePoint3d point = pointOnCurve.point();
	if (this->isOn(point) == false)
	{
		return;
	}

	GeVector3d vector = point - this->center();
	double angle = vector.angleToCCW(this->refVec(), this->normal());

	piece1 = new GeCircArc3d(this->center(), this->normal(), this->refVec(), this->radius(), this->startAng(), angle);
	piece2 = new GeCircArc3d(this->center(), this->normal(), this->refVec(), this->radius(), angle, this->endAng());
}
bool GeCircArc3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const
{
	GeCircArc3d* arc = new GeCircArc3d(*this);
	explodedCurves.append(arc);
	newExplodedCurve.append(1);
	return true;
}
GeBoundBlock3d GeCircArc3d::boundBlock() const
{
	GeInterval range;
	double start = this->startAng();
	double end = this->endAng();
	if (this->isClosed(GeContext::gTol) == true)
	{
		end = start + 2.0 * PI;
	}
	else if (end < start)
	{
		end += 2.0 * PI;
	}
	range.set(start, end);
	return this->boundBlock(range);
}
GeBoundBlock3d GeCircArc3d::boundBlock(const GeInterval& range) const
{

	GeBoundBlock3d boundBlock;
	if (std::fabs(range.upperBound() - range.lowerBound() - PI * 2.0) < range.tolerance())
	{

		GePoint3d basePoint(this->center());
		basePoint.x -= this->radius();
		basePoint.y -= this->radius();
		boundBlock.set(basePoint,
			GeVector3d(this->radius() * 2, 0.0, 0.0),
			GeVector3d(0.0, this->radius() * 2, 0.0),
			GeVector3d(0.0, 0.0, this->radius() * 2));
	}
	else
	{
		GePointOnCurve3d pointOnCurve;
		pointOnCurve.setCurve(*this);

		// 获得起点
		pointOnCurve.setParameter(range.lowerBound());
		GePoint3d startPoint = pointOnCurve.point();

		// 获得终点
		pointOnCurve.setParameter(range.upperBound());
		GePoint3d endPoint = pointOnCurve.point();

		// 获得起点到终点的长度
		double dist = startPoint.distanceTo(endPoint);

		// 获得圆心到线的垂点
		GePoint3d vertical = GeLine3d::vertical(this->center(), GeLine3d(startPoint, endPoint));

		// 获得基点
		GePoint3d basePoint = vertical;
		basePoint.translation((endPoint - startPoint).negate() * (dist / 2.0));

		// 获得方向1
		GeVector3d v1 = (vertical - basePoint).negate() * dist;

		// 获得方向2
		GeVector3d v2 = (vertical - this->center()).negate() * (this->radius() - this->center().distanceTo(vertical));

		boundBlock.set(basePoint, v1, v2, v2);
	}

	return boundBlock;
}
GeBoundBlock3d GeCircArc3d::orthoBoundBlock() const
{
	GeInterval range;
	double start = this->startAng();
	double end = this->endAng();
	if (this->isClosed(GeContext::gTol) == true)
	{
		end = start + 2.0 * PI;
	}
	else if (end < start)
	{
		end += 2.0 * PI;
	}
	range.set(start, end);
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeCircArc3d::orthoBoundBlock(const GeInterval& range) const
{

	GeBoundBlock3d boundBlock;

	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);

	// 获得起点
	pointOnCurve.setParameter(range.lowerBound());
	GePoint3d startPoint = pointOnCurve.point();

	// 获得终点
	pointOnCurve.setParameter(range.upperBound());
	GePoint3d endPoint = pointOnCurve.point();

	GePoint3dArray points;
	points.append(startPoint);
	points.append(endPoint);

	// 获得象限点
	GePoint3d point = GePoint3d(this->center().x + this->radius(), this->center().y, this->center().z);
	if (this->isOn(point) == true)
	{
		points.append(point);
	}
	point = GePoint3d(this->center().x, this->center().y + this->radius(), this->center().z);
	if (this->isOn(point) == true)
	{
		points.append(point);
	}
	point = GePoint3d(this->center().x - this->radius(), this->center().y, this->center().z);
	if (this->isOn(point) == true)
	{
		points.append(point);
	}
	point = GePoint3d(this->center().x, this->center().y - this->radius(), this->center().z);
	if (this->isOn(point) == true)
	{
		points.append(point);
	}

	GePoint3d minPoint, maxPoint;
	for (int i = 0; i < points.length(); i++)
	{

		if (i == 0)
		{
			minPoint = points[i];
			maxPoint = points[i];
			continue;
		}

		if (points[i].x < minPoint.x)
		{
			minPoint.x = points[i].x;
		}
		if (points[i].x > maxPoint.x)
		{
			maxPoint.x = points[i].x;
		}
		if (points[i].y < minPoint.y)
		{
			minPoint.y = points[i].y;
		}
		if (points[i].y > maxPoint.y)
		{
			maxPoint.y = points[i].y;
		}
	}

	boundBlock.set(minPoint, maxPoint);

	return boundBlock;
}

double GeCircArc3d::length() const
{
	return this->length(this->startAng(), this->endAng());
}
double GeCircArc3d::length(double fromParam, double toParam) const
{
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeCircArc3d::length(double fromParam, double toParam, double tol) const
{
	fromParam = std::fmod(fromParam, 2.0 * PI);
	toParam = std::fmod(toParam, 2.0 * PI);
	if (fromParam < 0.0) {
		fromParam += 2.0 * PI;
	}
	if (toParam < 0.0) {
		toParam += 2.0 * PI;
	}
	if (toParam < fromParam) {
		toParam += 2.0 * PI;
	}
	return this->radius() * (toParam - fromParam);
}
double GeCircArc3d::area() const
{
	return this->area(GeContext::gTol);
}
double GeCircArc3d::area(const GeTol& tol) const
{
	if (this->isClosed(tol) == false)
	{
		return 0.0;
	}
	return PI * this->radius() * this->radius();
}
double GeCircArc3d::paramAtLength(double datumParam, double length) const
{
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeCircArc3d::paramAtLength(double datumParam, double length, double tol) const
{
	double param = 0.0;
	if (std::fabs(this->radius()) <= tol)
	{
		return datumParam;
	}

	param = datumParam + (length / (2 * PI * this->radius()) * (2 * PI));
	param = std::fmod(param, 2.0 * PI);
	if (param < 0.0) {
		param += 2.0 * PI;
	}
	return param;
}
double GeCircArc3d::distanceTo(const GePoint3d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeCircArc3d::distanceTo(const GePoint3d& point, const GeTol& tol) const
{
	GePoint3d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeCircArc3d::distanceTo(const GeLine3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc3d::distanceTo(const GeLine3d& line, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeCircArc3d::distanceTo(const GeLineSeg3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc3d::distanceTo(const GeLineSeg3d& line, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeCircArc3d::distanceTo(const GeRay3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc3d::distanceTo(const GeRay3d& line, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeCircArc3d::distanceTo(const GeCircArc3d& entity) const
{
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc3d::distanceTo(const GeCircArc3d& arc, const GeTol& tol) const
{

	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint3d GeCircArc3d::closestPointTo(const GePoint3d& pnt) const
{
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeCircArc3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
	GePoint3d closest;
	GePoint3dArray points;

	// 获得点在平面上的投影
	GePoint3d point = pnt.orthoProject(GePlane(this->center(), this->normal()));

	// 获得投影射线和圆弧的交点
	GePoint3dArray intersects = this->intersectWith(GeRay3d(this->center(), point), tol);
	if (intersects.length() > 0)
	{
		points.append(intersects[0]);
	}

	// 若投影方向未命中圆弧，则退化为比较两端点距离。
	if (points.length() == 0)
	{
		GePoint3d startPoint = this->startPoint();
		GePoint3d endPoint = this->endPoint();
		closest = startPoint;
		if (pnt.distanceTo(endPoint) < pnt.distanceTo(startPoint))
		{
			closest = endPoint;
		}
		return closest;
	}

	// 取最近点
	double dist = points[0].distanceTo(pnt);
	closest = points[0];
	for (int i = 1; i < points.length(); i++)
	{
		if (points[i].distanceTo(pnt) < dist)
		{
			dist = points[i].distanceTo(pnt);
			closest = points[i];
		}
	}

	return closest;
}
GePoint3d GeCircArc3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		GePoint3dArray intersects = this->intersectWith(curve3d, tol);
		if (intersects.length() > 0)
		{
			pntOnOtherCrv.set(intersects[0].x, intersects[0].y, intersects[0].z);
			closest.set(intersects[0].x, intersects[0].y, intersects[0].z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

		// 获得圆弧和垂线是否存在交点
		GePoint3d vertical = GeLine3d::vertical(this->center(), curve3d);
		intersects = this->intersectWith(GeLineSeg3d(vertical, this->center()), tol);
		if (intersects.length() > 0)
		{
			if (this->isOn(intersects[0], tol) == true)
			{
				pointItselfs.append(intersects[0]);
			}
			if (curve3d.isOn(vertical, tol) == true)
			{
				pointOthers.append(vertical);
			}
		}

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			for (int u = 0; u < pointOthers.length(); u++)
			{
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeCircArc3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		GePoint3dArray intersects = this->intersectWith(curve3d, tol);
		if (intersects.length() > 0)
		{
			pntOnOtherCrv.set(intersects[0].x, intersects[0].y, intersects[0].z);
			closest.set(intersects[0].x, intersects[0].y, intersects[0].z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.startPoint(), tol));
		pointOthers.append(curve3d.startPoint());
		pointItselfs.append(this->closestPointTo(curve3d.endPoint(), tol));
		pointOthers.append(curve3d.endPoint());
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

		// 获得圆弧和垂线是否存在交点
		GePoint3d vertical = GeLine3d::vertical(this->center(), curve3d);
		intersects = this->intersectWith(GeLineSeg3d(vertical, this->center()), tol);
		if (intersects.length() > 0)
		{
			if (this->isOn(intersects[0], tol) == true)
			{
				pointItselfs.append(intersects[0]);
			}
			if (curve3d.isOn(vertical, tol) == true)
			{
				pointOthers.append(vertical);
			}
		}

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			for (int u = 0; u < pointOthers.length(); u++)
			{
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeCircArc3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		GePoint3dArray intersects = this->intersectWith(curve3d, tol);
		if (intersects.length() > 0)
		{
			pntOnOtherCrv.set(intersects[0].x, intersects[0].y, intersects[0].z);
			closest.set(intersects[0].x, intersects[0].y, intersects[0].z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.pointOnLine(), tol));
		pointOthers.append(curve3d.pointOnLine());
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

		// 获得圆弧和垂线是否存在交点
		GePoint3d vertical = GeLine3d::vertical(this->center(), curve3d);
		intersects = this->intersectWith(GeLineSeg3d(vertical, this->center()), tol);
		if (intersects.length() > 0)
		{
			if (this->isOn(intersects[0], tol) == true)
			{
				pointItselfs.append(intersects[0]);
			}
			if (curve3d.isOn(vertical, tol) == true)
			{
				pointOthers.append(vertical);
			}
		}

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			for (int u = 0; u < pointOthers.length(); u++)
			{
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeCircArc3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d closest;

	do
	{
		GePoint3dArray intersects = this->intersectWith(curve3d, tol);
		if (intersects.length() > 0)
		{
			pntOnOtherCrv.set(intersects[0].x, intersects[0].y, intersects[0].z);
			closest.set(intersects[0].x, intersects[0].y, intersects[0].z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.startPoint(), tol));
		pointOthers.append(curve3d.startPoint());
		pointItselfs.append(this->closestPointTo(curve3d.endPoint(), tol));
		pointOthers.append(curve3d.endPoint());
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

		// 获得圆弧和垂线是否存在交点
		intersects = this->intersectWith(GeLineSeg3d(this->center(), curve3d.center()), tol);
		for (int i = 0; i < intersects.length(); i++)
		{
			pointItselfs.append(intersects[i]);
		}
		intersects = curve3d.intersectWith(GeLineSeg3d(this->center(), curve3d.center()), tol);
		for (int i = 0; i < intersects.length(); i++)
		{
			pointOthers.append(intersects[i]);
		}

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++)
		{
			for (int u = 0; u < pointOthers.length(); u++)
			{
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const
{
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const
{
	if (projectDirection.isZeroLength(tol) == true)
	{
		return this->closestPointTo(pnt, tol);
	}

	GeLine3d line(pnt, projectDirection);
	GePoint3dArray points = this->intersectWith(line, tol);
	if (points.length() == 0)
	{
		return this->closestPointTo(pnt, tol);
	}

	GePoint3d closest = points[0];
	double minDist = closest.distanceTo(pnt);
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
GePoint3d GeCircArc3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	if (projectDirection.isZeroLength(tol) == true)
	{
		return this->closestPointTo(curve3d, pntOnOtherCrv, tol);
	}

	GePoint3d closest;
	double startParam = this->startAng();
	double endParam = this->endAng();
	if (this->isClosed(tol) == true)
	{
		endParam = startParam + PI * 2.0;
	}
	else if (endParam < startParam)
	{
		endParam += PI * 2.0;
	}

	double minDist = 0.0;
	for (int i = 0; i <= 256; i++)
	{
		double param = startParam + (endParam - startParam) * i / 256.0;
		GePoint3d point = this->evalPoint(param);
		GePoint3d other = curve3d.projClosestPointTo(point, -projectDirection, tol);
		double dist = point.distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = point;
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	if (projectDirection.isZeroLength(tol) == true)
	{
		return this->closestPointTo(curve3d, pntOnOtherCrv, tol);
	}

	GePoint3d closest;
	double startParam = this->startAng();
	double endParam = this->endAng();
	if (this->isClosed(tol) == true)
	{
		endParam = startParam + PI * 2.0;
	}
	else if (endParam < startParam)
	{
		endParam += PI * 2.0;
	}

	double minDist = 0.0;
	for (int i = 0; i <= 256; i++)
	{
		double param = startParam + (endParam - startParam) * i / 256.0;
		GePoint3d point = this->evalPoint(param);
		GePoint3d other = curve3d.projClosestPointTo(point, -projectDirection, tol);
		double dist = point.distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = point;
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	if (projectDirection.isZeroLength(tol) == true)
	{
		return this->closestPointTo(curve3d, pntOnOtherCrv, tol);
	}

	GePoint3d closest;
	double startParam = this->startAng();
	double endParam = this->endAng();
	if (this->isClosed(tol) == true)
	{
		endParam = startParam + PI * 2.0;
	}
	else if (endParam < startParam)
	{
		endParam += PI * 2.0;
	}

	double minDist = 0.0;
	for (int i = 0; i <= 256; i++)
	{
		double param = startParam + (endParam - startParam) * i / 256.0;
		GePoint3d point = this->evalPoint(param);
		GePoint3d other = curve3d.projClosestPointTo(point, -projectDirection, tol);
		double dist = point.distanceTo(other);
		if (i == 0 || dist < minDist)
		{
			minDist = dist;
			closest = point;
			pntOnOtherCrv = other;
		}
	}

	return closest;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return this->closestPointTo(curve3d, pntOnOtherCrv, tol);
}
void GeCircArc3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const
{
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
	GePoint3d point = this->projClosestPointTo(pnt, projectDirection, tol);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(point, tol));
}
void GeCircArc3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
void GeCircArc3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
void GeCircArc3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
void GeCircArc3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
bool GeCircArc3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const
{
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeCircArc3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
	GePoint3d closest = this->closestPointTo(pnt, tol);
	double param = 0.0;
	if (this->isOn(closest, param, tol) == false)
	{
		return false;
	}
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(param);
	return true;
}
GeCircArc3d* GeCircArc3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const
{
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeCircArc3d* GeCircArc3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const
{
	if (projectDirection.isZeroLength(tol) == true)
	{
		return NULL;
	}
	if (projectionPlane.normal().isParallelTo(this->normal(), tol.equalVector()) == false)
	{
		return NULL;
	}
	if (projectDirection.isPerpendicularTo(this->normal(), tol.equalVector()) == true)
	{
		return NULL;
	}

	GePoint3d center = this->center().project(projectionPlane, projectDirection);
	GeCircArc3d* arc = new GeCircArc3d();
	arc->set(center, this->normal(), this->refVec(), this->radius(), this->startAng(), this->endAng());
	return arc;
}
GeCircArc3d* GeCircArc3d::orthoProject(const GePlane& projectionPlane) const
{
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeCircArc3d* GeCircArc3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const
{
	return this->project(projectionPlane, projectionPlane.normal(), tol);
}
bool GeCircArc3d::isOn(const GePoint3d& pnt, double& param) const
{
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeCircArc3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const
{
	GeVector3d v = pnt - this->center();
	if (v.length() < tol.equalPoint())
	{
		return false;
	}

	GePlane plane(this->center(), this->normal());
	if (plane.isOn(pnt, tol) == false)
	{
		return false;
	}
	if (std::fabs(v.length() - this->radius()) > tol.equalPoint())
	{
		return false;
	}

	v.normalize();
	if (v.isPerpendicularTo(this->normal(), tol.equalVector()) == false)
	{
		return false;
	}

	param = this->refVec().angleToCCW(v, this->normal());
	return this->isOn(param, tol);
}
bool GeCircArc3d::isOn(double param) const
{
	return this->isOn(param, GeContext::gTol);
}
bool GeCircArc3d::isOn(double param, const GeTol& tol) const
{
	if (this->isClosed(tol) == true)
	{
		return true;
	}
	auto normalizeAngle = [](double value) {
		double result = std::fmod(value, 2.0 * PI);
		if (result < 0.0)
		{
			result += 2.0 * PI;
		}
		return result;
	};

	double start = normalizeAngle(this->startAng());
	double end = normalizeAngle(this->endAng());
	double angle = normalizeAngle(param);

	if (end < start)
	{
		end += 2.0 * PI;
	}
	if (angle < start)
	{
		angle += 2.0 * PI;
	}

	return angle >= start - tol.equalPoint() && angle <= end + tol.equalPoint();
}
GECURVE3D_METHODS_GETCLOSESTPOINTTO(GeCircArc3d);
double GeCircArc3d::paramOf(const GePoint3d& pnt) const
{
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCircArc3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
	double param = 0.0;
	if (this->isOn(pnt, param, tol) == false)
	{
		return 0;
	}
	return param;
}
void GeCircArc3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const
{
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeCircArc3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const
{
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeCircArc3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const
{
	double newRadius = this->radius() + distance;
	if (newRadius <= tol.equalPoint())
	{
		return;
	}

	GeCircArc3d* circArc = new GeCircArc3d();
	circArc->set(this->center(), this->normal(), this->refVec(), newRadius, this->startAng(), this->endAng());

	offsetCurveList.append(circArc);
}
bool GeCircArc3d::isClosed() const
{
	return this->isClosed(GeContext::gTol);
}
bool GeCircArc3d::isClosed(const GeTol& tol) const
{
	double absSweep = std::fabs(this->endAng() - this->startAng());
	if (absSweep < 2.0 * PI - tol.equalPoint())
	{
		return false;
	}

	double turns = absSweep / (2.0 * PI);
	double nearestTurns = std::floor(turns + 0.5);
	if (nearestTurns < 1.0)
	{
		nearestTurns = 1.0;
	}

	if (std::fabs(absSweep - nearestTurns * 2.0 * PI) <= tol.equalPoint())
	{
		return true;
	}
	return false;
}
bool GeCircArc3d::isPlanar(GePlane& plane) const
{
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeCircArc3d::isPlanar(GePlane& plane, const GeTol& tol) const
{
	this->getPlane(plane);
	return true;
}
bool GeCircArc3d::isLinear(GeLine3d& line) const
{
	return this->isLinear(line, GeContext::gTol);
}
bool GeCircArc3d::isLinear(GeLine3d& line, const GeTol& tol) const
{
	return false;
}
bool GeCircArc3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCircArc3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane, const GeTol& tol) const
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
bool GeCircArc3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCircArc3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const
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
bool GeCircArc3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCircArc3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const
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
bool GeCircArc3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCircArc3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	if (this->normal().isParallelTo(curve3d.normal(), tol) == false)
	{
		return false;
	}

	plane.set(this->center(), this->normal());
	if (plane.isOn(curve3d.center(), tol) == false)
	{
		return false;
	}
	return true;
}
bool GeCircArc3d::hasStartPoint(GePoint3d& startPnt) const
{
	startPnt = this->evalPoint(this->startAng());
	return true;
}
bool GeCircArc3d::hasEndPoint(GePoint3d& endPnt) const
{
	endPnt = this->evalPoint(this->endAng());
	return true;
}
GePoint3d GeCircArc3d::evalPoint(double param) const
{
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}
void GeCircArc3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const
{
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray);
}
void GeCircArc3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray, bool forceResampling) const
{

	double param = fromParam;

	while (true)
	{

		pointArray.append(this->evalPoint(param));
		paramArray.append(param);

		param = param + approxEps;
		if (param > toParam)
		{
			break;
		}
	}
}
void GeCircArc3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const
{
	return this->getSamplePoints(numSample, pointArray);
}
void GeCircArc3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const
{
	for (int i = 0; i < numSample; i++)
	{
		double param = 1.0 / numSample * i;
		pointArray.append(this->evalPoint(param));
		paramArray.append(param);
	}
}

