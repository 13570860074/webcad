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


GeCircArc3d::GeCircArc3d() {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);
}
GeCircArc3d::GeCircArc3d(const GeCircArc3d& arc) {
	GE_IMP_MEMORY_ENTITY(GeCircArc3d);

	GE_IMP_CIRCARC3D(this->m_pImpl)->center = GE_IMP_CIRCARC3D(arc.m_pImpl)->center;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = GE_IMP_CIRCARC3D(arc.m_pImpl)->radius;
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = GE_IMP_CIRCARC3D(arc.m_pImpl)->startAngle;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = GE_IMP_CIRCARC3D(arc.m_pImpl)->endAngle;
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec = GE_IMP_CIRCARC3D(arc.m_pImpl)->refVec;
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GE_IMP_CIRCARC3D(arc.m_pImpl)->normal;
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
	if (abs(normal.length()) < 0.000001) {
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
		if (abs(dist - this->radius()) < tol.equalPoint())
		{
			inters.append(vertical);
			break;
		}
		if (dist > this->radius() + tol.equalPoint())
		{
			break;
		}

		double e = sqrt(this->radius() * this->radius() - dist * dist);

		GePoint3d point;
		GeVector3d direction = line.direction();


		point.set(vertical.x, vertical.y, vertical.z);
		point.x += e * direction.x;
		point.y += e * direction.y;
		if (line.isOn(point, tol) == true && this->isOn(point, tol) == true)
		{
			inters.append(point);
		}

		direction.negate();
		point.set(vertical.x, vertical.y, vertical.z);
		point.x += e * direction.x;
		point.y += e * direction.y;
		if (line.isOn(point, tol) == true && this->isOn(point, tol) == true)
		{
			bool isAppend = true;
			if (inters.length() > 0) {
				if (point.isEqualTo(inters[0], tol) == true) {
					isAppend = false;
				}
			}
			if (isAppend == true) {
				inters.append(point);
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

		if (this->normal().isParallelTo(arc.normal()) == true)
		{
			double val = this->center().distanceTo(arc.center());
			// 判断两圆是否在范围外
			if (val - arc.radius() - this->radius() >= tol.equalPoint())
			{
				break;
			}
			// 判断是否圆心重合
			if (this->center().distanceTo(arc.center()) < tol.equalPoint())
			{
				break;
			}
			// 判断两圆是否只存在一个交点
			if (abs(val - arc.radius() - this->radius()) < tol.equalPoint())
			{
				GeVector3d xAxis = arc.center() - this->center();
				xAxis.normalize();
				GePoint3d point1 = this->center() + xAxis * this->radius();
				intersects.append(point1);
				break;
			}
			
			GeVector3d xAxis = arc.center() - this->center();
			xAxis.normalize();
			GeVector3d zAxis = this->normal();
			xAxis.normalize();
			GeVector3d yAxis = zAxis.crossProduct(xAxis);
			yAxis.normalize();

			GeMatrix3d mat;
			mat.setToAlignCoordSys(this->center(), xAxis, yAxis, zAxis, this->center(), GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis);

			GePoint3d center1 = this->center();
			GePoint3d center2 = arc.center();
			center1.transformBy(mat);
			center2.transformBy(mat);

			double dist = center1.distanceTo(center2);
			double b = (((arc.radius() * arc.radius()) - (this->radius() * this->radius())) / dist + dist) / 2.0;
			double a = dist - b;
			val = (this->radius() * this->radius()) - a * a;
			if(val < tol.equalVector()){
				break;
			}
			double h = sqrt(val);

			GePoint3d point1 = center1 + xAxis * a + yAxis * h;
			GePoint3d point2 = center1 + xAxis * a - yAxis * h;
			intersects.append(point1);
			intersects.append(point2);
		}
		else {

		}

	} while (false);


	do
	{
		break;
		if (this->isInside(arc.center(), tol) == false)
		{
			break;
		}

		double dist = this->center().distanceTo(arc.center());

		double val = (dist * dist + this->radius() * this->radius() - arc.radius() * arc.radius()) / (2 * dist);

		// 获得圆的两个交点
		GePoint3dArray points;
		if (abs(val * val - this->radius() * this->radius()) < tol.equalPoint())
		{
			GePoint3d target;
			double t = this->radius() / dist;
			target[0] = this->center()[0] * (1 - t) + arc.center()[0] * t;
			target[1] = this->center()[1] * (1 - t) + arc.center()[1] * t;
			points.append(target);
		}
		else if (val * val < this->radius() * this->radius()) // 相交
		{
			double h = pow((this->radius() * this->radius() - val * val), 0.5);

			// c1-c2的单位方向向量计算
			GeVector3d vector = this->center() - arc.center();
			vector.normalize();

			// 获得垂点
			GePoint3d vertical = this->center() + vector * sqrt((this->radius() * this->radius() - h * h));

			// 方向向量旋转PI/2
			vector.rotateBy(PI / 2, GeVector3d::kZAxis);

			// 获得第一点
			points.append(vertical + vector * h);

			// 获得第二点
			points.append(vertical - vector * h);
		}

		for (int i = 0; i < points.length(); i++)
		{
			if (this->isOn(points[i], tol) == true)
			{
				intersects.append(points[i]);
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
	if (dist > this->radius() + this->radius())
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
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec = refVec;
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = normal;
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.normalize();
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal.normalize();
	return *this;
}
GeCircArc3d& GeCircArc3d::setRadius(double radius) {
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = radius;
	return *this;
}
GeCircArc3d& GeCircArc3d::setAngles(double startAngle, double endAngle) {
	if (startAngle < 0.0) {
		startAngle = PI * 2 + startAngle;
	}
	if (endAngle < 0.0) {
		endAngle = PI * 2 + endAngle;
	}
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = startAngle;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = endAngle;
	return *this;
}

GeCircArc3d& GeCircArc3d::set(const GePoint3d& cent, const GeVector3d& nrm, double radius) {
	GeVector3d refVec = GeVector3d::kXAxis;
	if (nrm.isEqualTo(GeVector3d::kZAxis) == false) {
		refVec = GeVector3d::kZAxis.crossProduct(nrm);
	}
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = cent;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = radius;
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = 0;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = PI * 2;
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.set(refVec.x, refVec.y, refVec.z);
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.normalize();
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = nrm;
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal.normalize();
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GePoint3d& cent, const GeVector3d& nrm, const GeVector3d& refVec, double radius, double startAngle, double endAngle) {
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = cent;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = radius;
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = 0;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = PI * 2;
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.set(refVec.x, refVec.y, refVec.z);
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.normalize();
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = nrm;
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal.normalize();
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GePoint3d& startPoint, const GePoint3d& pnt, const GePoint3d& endPoint) {
	GePoint3d center;
	if (GeCircArc3d::pointToCenter(startPoint, pnt, endPoint, center) == false) {
		this->setRadius(0.0);
		return *this;
	}

	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec = (startPoint - center).normal();
	if (GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.isEqualTo(GeVector3d(1, 0, 0)) == true) {
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GeVector3d::kZAxis;
	}
	else if (GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.isEqualTo(GeVector3d(-1, 0, 0)) == true) {
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GeVector3d(0, 0, -1);
	}
	else {
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.crossProduct(GeVector3d::kZAxis);
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal.normalize();
	}
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = center;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = startPoint.distanceTo(center);
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = 0.0;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.angleToCCW((endPoint - center).normal(), this->normal());
	return *this;
}
GeCircArc3d& GeCircArc3d::set(const GePoint3d& startPoint, const GePoint3d& endPoint, double bulge) {

	GePoint3d midPoint = startPoint + (endPoint - startPoint) / 2;
	double dist = startPoint.distanceTo(midPoint);
	double angle = atan(bulge) * 4;
	double adjacent = dist / tan(angle / 2.0);

	GeVector3d refVec = startPoint - midPoint;
	refVec.normalize();
	refVec.rotateBy(0 - PI / 2.0, this->normal());

	GePoint3d center = midPoint;
	center += (refVec * adjacent);

	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec = (startPoint - center).normal();
	if (GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.isEqualTo(GeVector3d(1, 0, 0)) == true) {
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GeVector3d::kZAxis;
	}
	else if (GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.isEqualTo(GeVector3d(-1, 0, 0)) == true) {
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GeVector3d(0, 0, -1);
	}
	else {
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.crossProduct(GeVector3d::kZAxis);
		GE_IMP_CIRCARC3D(this->m_pImpl)->normal.normalize();
	}
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = center;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = center.distanceTo(startPoint);
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = 0.0;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec.angleToCCW((endPoint - center).normal(), this->normal());
	if (bulge < 0) {
		GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle;
		GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = PI * 2.0;
	}
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
	GE_IMP_CIRCARC3D(this->m_pImpl)->center = GE_IMP_CIRCARC3D(arc.m_pImpl)->center;
	GE_IMP_CIRCARC3D(this->m_pImpl)->radius = GE_IMP_CIRCARC3D(arc.m_pImpl)->radius;
	GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle = GE_IMP_CIRCARC3D(arc.m_pImpl)->startAngle;
	GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle = GE_IMP_CIRCARC3D(arc.m_pImpl)->endAngle;
	GE_IMP_CIRCARC3D(this->m_pImpl)->refVec = GE_IMP_CIRCARC3D(arc.m_pImpl)->refVec;
	GE_IMP_CIRCARC3D(this->m_pImpl)->normal = GE_IMP_CIRCARC3D(arc.m_pImpl)->normal;
	return *this;
}

bool GeCircArc3d::isKindOf(Ge::EntityId entType) const
{
	if (entType == this->type())
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
	GeCircArc3d* arc = new GeCircArc3d();
	GE_IMP_CIRCARC3D(arc->m_pImpl)->center = GE_IMP_CIRCARC3D(this->m_pImpl)->center;
	GE_IMP_CIRCARC3D(arc->m_pImpl)->radius = GE_IMP_CIRCARC3D(this->m_pImpl)->radius;
	GE_IMP_CIRCARC3D(arc->m_pImpl)->startAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle;
	GE_IMP_CIRCARC3D(arc->m_pImpl)->endAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle;
	GE_IMP_CIRCARC3D(arc->m_pImpl)->refVec = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec;
	GE_IMP_CIRCARC3D(arc->m_pImpl)->normal = GE_IMP_CIRCARC3D(this->m_pImpl)->normal;
	return arc;
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
	if (this->center().isEqualTo(entity.center(), tol) == false)
	{
		return false;
	}
	if (abs(this->radius() - entity.radius()) > tol.equalPoint())
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
	GePoint3d center = this->center();
	center.rotateBy(angle, vec, wrtPoint);
	this->setAngles(this->startAng() + angle, this->endAng() + angle);
	return *this;
}
GeCircArc3d& GeCircArc3d::mirror(const GePlane& plane)
{

	this->setCenter(this->center().mirror(plane));

	GePoint3d startPoint = this->startPoint();
	startPoint.mirror(plane);
	GeVector3d vector = startPoint - this->center();
	vector.normalize();
	double startAngle = vector.angleToCCW(this->refVec());

	GePoint3d endPoint = this->endPoint();
	endPoint.mirror(plane);
	vector = endPoint - this->center();
	vector.normalize();
	double endAngle = vector.angleToCCW(this->refVec());

	this->setAngles(startAngle, endAngle);

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
	bool isValue = false;

	do
	{
		if (abs(this->center().distanceTo(pnt) - this->radius()) > tol.equalPoint())
		{
			break;
		}
		if (abs(this->endAng() - this->startAng() - PI * 2) < tol.equalPoint())
		{
			isValue = true;
			break;
		}

		GePoint3d startPoint(this->center().x + this->radius(), this->center().y, this->center().z);
		startPoint.rotateBy(this->startAng(), GeVector3d::kZAxis);

		GePoint3d endPoint(this->center().x + this->radius(), this->center().y, this->center().z);
		endPoint.rotateBy(this->endAng(), GeVector3d::kZAxis);

		GePoint3d midPoint(this->center().x + this->radius(), this->center().y, this->center().z);
		endPoint.rotateBy(this->startAng() + (this->endAng() - this->startAng()) / 2.0, GeVector3d::kZAxis);

		if (isClockWise(startPoint, midPoint, endPoint) == isClockWise(startPoint, pnt, endPoint))
		{
			isValue = true;
		}

	} while (false);

	return isValue;
}
void GeCircArc3d::getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const
{

	GePointOnCurve3d pointOnCurve(*this, param);
	GePoint3d point = pointOnCurve.point();
	if (this->isOn(point) == false)
	{
		return;
	}

	GeVector3d vector = point - this->center();
	double angle = vector.angleToCCW(this->refVec(), this->normal());

	piece1 = new GeCircArc3d(this->center(), this->refVec(), this->normal(), this->radius(), this->startAng(), angle);
	piece2 = new GeCircArc3d(this->center(), this->refVec(), this->normal(), this->radius(), angle, this->endAng());
}
bool GeCircArc3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const
{
	return false;
}
GeBoundBlock3d GeCircArc3d::boundBlock() const
{
	GeInterval range;
	range.set(this->paramOf(this->startPoint()), this->paramOf(this->endPoint()));
	return this->boundBlock(range);
}
GeBoundBlock3d GeCircArc3d::boundBlock(const GeInterval& range) const
{

	GeBoundBlock3d boundBlock;
	if (abs(range.upperBound() - range.lowerBound() - PI * 2.0) < range.tolerance())
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
	range.set(this->paramOf(this->startPoint()), this->paramOf(this->endPoint()));
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
	if (fromParam < 0) {
		fromParam = PI * 2 - fromParam;
	}
	if (toParam < 0) {
		toParam = PI * 2 - toParam;
	}
	if (toParam < fromParam) {
		toParam += 2 * PI;
	}
	return this->radius() * (toParam - fromParam);
}
double GeCircArc3d::area() const
{
	return this->area(GeContext::gTol);
}
double GeCircArc3d::area(const GeTol& tol) const
{
	if (this->isClosed() == false)
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

	param = datumParam + (length / (2 * PI * this->radius()) * (2 * PI));
	if (param > PI * 2) {
		param = param - int(param / PI * 2) * PI * 2;
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

	do
	{

		GePoint3dArray points;

		// 获得点在平面上的投影
		GePoint3d point = pnt.orthoProject(GePlane(this->center(), this->normal()));

		// 获得投影射线和圆弧的交点
		GePoint3dArray intersects = this->intersectWith(GeRay3d(this->center(), point), tol);
		if (intersects.length() > 0)
		{
			points.append(intersects[0]);
		}

		// 取最近点
		double dist = 0.0;
		for (int i = 0; i < points.length(); i++)
		{
			if (i == 0)
			{
				dist = points[i].distanceTo(pnt);
				closest = points[i];
				continue;
			}
			if (points[i].distanceTo(pnt) < dist)
			{
				dist = points[i].distanceTo(pnt);
				closest = points[i];
			}
		}

	} while (false);

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
	return GePoint3d::kOrigin;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const
{
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCircArc3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
	return GePoint3d::kOrigin;
}
void GeCircArc3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const
{
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
}
void GeCircArc3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeCircArc3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeCircArc3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
void GeCircArc3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const
{
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCircArc3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const
{
}
bool GeCircArc3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const
{
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeCircArc3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const
{
	return false;
}
GeCircArc3d* GeCircArc3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const
{
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeCircArc3d* GeCircArc3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const
{
	return NULL;
}
GeCircArc3d* GeCircArc3d::orthoProject(const GePlane& projectionPlane) const
{
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeCircArc3d* GeCircArc3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const
{
	return NULL;
}
bool GeCircArc3d::isOn(const GePoint3d& pnt, double& param) const
{
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeCircArc3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const
{
	GeVector3d v = pnt - this->center();
	v.normalize();
	if (v.isParallelTo(this->normal(), tol.equalVector()) == false) {
		return false;
	}

	param = this->refVec().angleToCCW(v, this->normal());
	return false;
}
bool GeCircArc3d::isOn(double param) const
{
	return this->isOn(param, GeContext::gTol);
}
bool GeCircArc3d::isOn(double param, const GeTol& tol) const
{
	return this->isOn(this->evalPoint(param), tol);
}
GECURVE3D_METHODS_GETCLOSESTPOINTTO(GeCircArc3d);
double GeCircArc3d::paramOf(const GePoint3d& pnt) const
{
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCircArc3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{

	GeCircArc3d c = *this;
	if (c.isOn(pnt, tol) == false)
	{
		return 0;
	}

	GeVector3d vector = pnt - this->center();
	return 0 - vector.angleToCCW(this->refVec());
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

	if (abs(this->radius() - distance) < tol.equalPoint())
	{
		return;
	}

	GeCircArc3d* circArc = new GeCircArc3d();
	GE_IMP_CIRCARC3D(circArc->m_pImpl)->refVec = GE_IMP_CIRCARC3D(this->m_pImpl)->refVec;
	GE_IMP_CIRCARC3D(circArc->m_pImpl)->center = this->center();
	GE_IMP_CIRCARC3D(circArc->m_pImpl)->radius = this->radius() + distance;
	GE_IMP_CIRCARC3D(circArc->m_pImpl)->startAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->startAngle;
	GE_IMP_CIRCARC3D(circArc->m_pImpl)->endAngle = GE_IMP_CIRCARC3D(this->m_pImpl)->endAngle;

	offsetCurveList.append(circArc);
}
bool GeCircArc3d::isClosed() const
{
	return this->isClosed(GeContext::gTol);
}
bool GeCircArc3d::isClosed(const GeTol& tol) const
{
	if (abs(this->endAng() - this->startAng()) - PI * 2 < tol.equalPoint())
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
	return false;
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
	GeLine3d line1;
	line1.set(curve3d.pointOnLine(), curve3d.direction());
	return this->isCoplanarWith(line1, plane, tol);
}
bool GeCircArc3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCircArc3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	GeLine3d line1;
	line1.set(curve3d.pointOnLine(), curve3d.direction());
	return this->isCoplanarWith(line1, plane, tol);
}
bool GeCircArc3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const
{
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCircArc3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const
{
	GeLine3d line1;
	line1.set(curve3d.pointOnLine(), curve3d.direction());
	return this->isCoplanarWith(line1, plane, tol);
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

