#include "GePlane.h"
#include "GeMatrix3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GePointOnSurface.h"
#include "GeBoundedPlane.h"
#include "GeImpl.h"


const GePlane GePlane::kXYPlane = GePlane(GePoint3d::kOrigin, GeVector3d::kZAxis);
const GePlane GePlane::kYZPlane = GePlane(GePoint3d::kOrigin, GeVector3d::kXAxis);
const GePlane GePlane::kZXPlane = GePlane(GePoint3d::kOrigin, GeVector3d::kYAxis);



GePlane::GePlane() {
	GE_IMP_MEMORY_ENTITY(GePlane);
}
GePlane::GePlane(const GePlane& src) {
	GE_IMP_MEMORY_ENTITY(GePlane);

	this->set(src.pointOnPlane(), src.normal());
}
GePlane::GePlane(const GePoint3d& origin, const GeVector3d& normal) {
	GE_IMP_MEMORY_ENTITY(GePlane);

	this->set(origin, normal);
}
GePlane::GePlane(const GePoint3d& pntU, const GePoint3d& org, const GePoint3d& pntV) {
	GE_IMP_MEMORY_ENTITY(GePlane);

	this->set(pntU, org, pntV);
}
GePlane::GePlane(const GePoint3d& org, const GeVector3d& uAxis, const GeVector3d& vAxis) {
	GE_IMP_MEMORY_ENTITY(GePlane);

	this->set(org, uAxis, vAxis);
}
GePlane::GePlane(double a, double b, double c, double d) {
	GE_IMP_MEMORY_ENTITY(GePlane);

	this->set(a, b, c, d);
}
double GePlane::signedDistanceTo(const GePoint3d& pnt) const {
	GePoint3d point = pnt.orthoProject(*this);
	GeVector3d vec = pnt - point;
	vec.normalize();
	if (vec.isEqualTo(this->normal()) == false) {
		return 0 - point.distanceTo(pnt);
	}
	return point.distanceTo(pnt);
}
bool GePlane::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt) const {
	return this->intersectWith(linEnt, resultPnt, GeContext::gTol);
}
bool GePlane::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt, const GeTol& tol) const {
	return false;
}
bool GePlane::intersectWith(const GePlane& otherPln, GeLine3d& resultLine) const {
	return this->intersectWith(otherPln, resultLine, GeContext::gTol);
}
bool GePlane::intersectWith(const GePlane& otherPln, GeLine3d& resultLine, const GeTol& tol) const {

	//如果两平面平行则无交线
	if (this->isParallelTo(otherPln, tol) == true) {
		return false;
	}

	//获得两法向的夹角
	double angle = this->normal().angle(otherPln.normal());

	//求主平面原点到输入平面的投影点
	GePoint3d point = this->pointOnPlane().orthoProject(otherPln);

	//获得对边长度
	double dist = this->pointOnPlane().distanceTo(point);
	dist = dist / tan(angle);

	//获得投影点到输入平面原点的向量
	GeVector3d vec = point - otherPln.pointOnPlane();
	vec.normalize();

	//获得相交线原点
	point = point + (vec * dist);
	if (this->isOn(point, tol) == false) {
		point.translation(vec * (2.0 * -dist));
	}

	resultLine.set(point, this->normal().crossProduct(otherPln.normal()));

	return true;
}
bool GePlane::intersectWith(const GeBoundedPlane& bndPln, GeLineSeg3d& resultLineSeg) const {
	return this->intersectWith(bndPln, resultLineSeg, GeContext::gTol);
}
bool GePlane::intersectWith(const GeBoundedPlane& bndPln, GeLineSeg3d& resultLineSeg, const GeTol& tol) const {

	GeLine3d line;
	GePlane plane(bndPln.pointOnPlane(), bndPln.normal());
	if (this->intersectWith(plane, line, tol) == false) {
		return false;
	}

	GePoint3d origin;
	GeVector3d xAxis, yAxis;
	bndPln.get(origin, xAxis, yAxis);

	GePoint3dArray points;
	points.append(origin);
	points.append(origin + xAxis);
	points.append(origin + xAxis + yAxis);
	points.append(origin + yAxis);
	points.append(origin);

	GePoint3dArray linePoints;
	for (int i = 1; i < points.length(); i++) {
		GeLineSeg3d lineSeg;
		lineSeg.set(points[i - 1], points[i]);

		GePoint3d intersect;
		if (line.intersectWith(lineSeg, intersect, tol) == true) {
			bool isFind = true;
			for (int u = 0; u < linePoints.length(); u++) {
				if (linePoints[u].isEqualTo(intersect, tol) == true) {
					isFind = false;
					break;
				}
			}
			if (isFind == true) {
				linePoints.append(intersect);
			}
		}
	}

	if (linePoints.length() == 2) {
		resultLineSeg.set(linePoints[0], linePoints[1]);
		return true;
	}
	return false;
}
GePlane& GePlane::set(const GePoint3d& pnt, const GeVector3d& normal) {

	GE_IMP_PLANE(this->m_pImpl)->origin.set(pnt.x, pnt.y, pnt.z);
	GE_IMP_PLANE(this->m_pImpl)->normal.set(normal.x, normal.y, normal.z);
	GE_IMP_PLANE(this->m_pImpl)->normal.normalize();

	//如果方向和z轴平行
	if (normal.isPerpendicularTo(GeVector3d::kZAxis) == true) {
		GE_IMP_PLANE(this->m_pImpl)->xAxis = GeVector3d::kXAxis;
		GE_IMP_PLANE(this->m_pImpl)->yAxis = GeVector3d::kYAxis;
		return *this;
	}

	//获得法向和Z轴的夹角
	double angle = this->normal().angle(GeVector3d::kZAxis);

	//法向和Z轴叉乘
	GeVector3d cross = this->normal().crossProduct(GeVector3d::kZAxis);

	//构建一个旋转矩阵
	GeMatrix3d mat;
	mat.setToRotation(angle, cross);

	//判断旋转矩阵是否正确
	GeVector3d vec = GeVector3d::kZAxis;
	vec.transformBy(mat);
	if (vec.isEqualTo(normal.normal()) == false) {
		mat.setToRotation(0 - angle, cross);
	}

	//设置xy轴
	GE_IMP_PLANE(this->m_pImpl)->xAxis = GeVector3d::kXAxis;
	GE_IMP_PLANE(this->m_pImpl)->xAxis.transformBy(mat);

	GE_IMP_PLANE(this->m_pImpl)->yAxis = GeVector3d::kYAxis;
	GE_IMP_PLANE(this->m_pImpl)->yAxis.transformBy(mat);

	return *this;
}
GePlane& GePlane::set(const GePoint3d& pntU, const GePoint3d& org, const GePoint3d& pntV) {

	GeVector3d v1 = GeVector3d(pntU.x - org.x, pntU.y - org.y, pntU.z - org.z);
	GeVector3d v2 = GeVector3d(pntV.x - org.x, pntV.y - org.y, pntV.z - org.z);

	GeVector3d normal = v1.crossProduct(v2);
	normal.normalize();

	GE_IMP_PLANE(this->m_pImpl)->origin = org;
	GE_IMP_PLANE(this->m_pImpl)->xAxis = v1;
	GE_IMP_PLANE(this->m_pImpl)->yAxis = v2;
	GE_IMP_PLANE(this->m_pImpl)->normal = normal;
	return *this;
}
GePlane& GePlane::set(double a, double b, double c, double d) {


	double aAbs = a;
	if (aAbs < 0) aAbs = -aAbs;
	double bAbs = b;
	if (bAbs < 0) bAbs = -bAbs;
	double cAbs = c;
	if (cAbs < 0) cAbs = -cAbs;
	if (bAbs <= aAbs && bAbs <= cAbs) {
		if (aAbs > cAbs)
		{
			GE_IMP_PLANE(this->m_pImpl)->origin.set(-d / a, 0.0, 0.0);
			GE_IMP_PLANE(this->m_pImpl)->normal.set(a, b, c);
			GE_IMP_PLANE(this->m_pImpl)->xAxis.set(-c, 0.0, a);
			GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);

			//this->set(GePoint3d(-d / a, 0.0, 0.0), GeVector3d(a, b, c));
			//this->set(GePoint3d(-d / a, 0.0, 0.0), GeVector3d(a, b, c), GeVector3d(-c, 0.0, a));
		}
		else {
			GE_IMP_PLANE(this->m_pImpl)->origin.set(0.0, 0.0, -d / c);
			GE_IMP_PLANE(this->m_pImpl)->normal.set(a, b, c);
			GE_IMP_PLANE(this->m_pImpl)->xAxis.set(c, 0.0, -a);
			GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);

			//this->set(GePoint3d(0.0, 0.0, -d / c), GeVector3d(a, b, c));
			//this->set(GePoint3d(0.0, 0.0, -d / c), GeVector3d(a, b, c), GeVector3d(c, 0.0, -a));
		}
	}
	else if (aAbs <= bAbs && aAbs <= cAbs) {
		if (bAbs > cAbs) {
			GE_IMP_PLANE(this->m_pImpl)->origin.set(0.0, -d / b, 0.0);
			GE_IMP_PLANE(this->m_pImpl)->normal.set(a, b, c);
			GE_IMP_PLANE(this->m_pImpl)->xAxis.set(0.0, -c, b);
			GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);

			//this->set(GePoint3d(0.0, -d / b, 0.0), GeVector3d(a, b, c));
			//this->set(GePoint3d(0.0, -d / b, 0.0), GeVector3d(a, b, c), GeVector3d(0.0, -c, b));
		}
		else {
			GE_IMP_PLANE(this->m_pImpl)->origin.set(0.0, 0.0, -d / c);
			GE_IMP_PLANE(this->m_pImpl)->normal.set(a, b, c);
			GE_IMP_PLANE(this->m_pImpl)->xAxis.set(0.0, c, -b);
			GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);

			//this->set(GePoint3d(0.0, 0.0, -d / c), GeVector3d(a, b, c));
			//this->set(GePoint3d(0.0, 0.0, -d / c), GeVector3d(a, b, c), GeVector3d(0.0, c, -b));
		}
	}
	else {
		if (aAbs > bAbs) {
			GE_IMP_PLANE(this->m_pImpl)->origin.set(-d / a, 0.0, 0.0);
			GE_IMP_PLANE(this->m_pImpl)->normal.set(a, b, c);
			GE_IMP_PLANE(this->m_pImpl)->xAxis.set(-b, a, 0.0);
			GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);

			//this->set(GePoint3d(-d / a, 0.0, 0.0), GeVector3d(a, b, c));
			//this->set(GePoint3d(-d / a, 0.0, 0.0), GeVector3d(a, b, c), GeVector3d(-b, a, 0.0));
		}
		else {
			GE_IMP_PLANE(this->m_pImpl)->origin.set(0.0, -d / b, 0.0);
			GE_IMP_PLANE(this->m_pImpl)->normal.set(a, b, c);
			GE_IMP_PLANE(this->m_pImpl)->xAxis.set(b, -a, 0.0);
			GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);

			//this->set(GePoint3d(0.0, -d / b, 0.0), GeVector3d(a, b, c));
			//this->set(GePoint3d(0.0, -d / b, 0.0), GeVector3d(a, b, c), GeVector3d(b, -a, 0.0));
		}
	}

	//法向标准化
	GE_IMP_PLANE(this->m_pImpl)->normal.normalize();
	GE_IMP_PLANE(this->m_pImpl)->xAxis.normalize();
	GE_IMP_PLANE(this->m_pImpl)->yAxis.normalize();

	//计算出d值
	a = this->normal().x;
	b = this->normal().y;
	c = this->normal().z;
	GePoint3d point = this->pointOnPlane();
	d = -(a * point.x + b * point.y + c * point.z);

	//设置原点
	GE_IMP_PLANE(this->m_pImpl)->origin = GePoint3d::kOrigin;
	GE_IMP_PLANE(this->m_pImpl)->origin.translation(this->normal() * -d);

	this->set(GE_IMP_PLANE(this->m_pImpl)->origin, this->normal());

	return *this;
}
GePlane& GePlane::set(const GePoint3d& org, const GeVector3d& uAxis, const GeVector3d& vAxis) {

	GeVector3d normal = uAxis.crossProduct(vAxis);

	GE_IMP_PLANE(this->m_pImpl)->origin = org;
	GE_IMP_PLANE(this->m_pImpl)->xAxis = uAxis;
	GE_IMP_PLANE(this->m_pImpl)->yAxis = vAxis;
	GE_IMP_PLANE(this->m_pImpl)->normal = normal;
	return *this;
}
GePlane& GePlane::operator = (const GePlane& src) {
	GE_IMP_PLANE(this->m_pImpl)->origin = GE_IMP_PLANE(src.m_pImpl)->origin;
	GE_IMP_PLANE(this->m_pImpl)->normal = GE_IMP_PLANE(src.m_pImpl)->normal;
	GE_IMP_PLANE(this->m_pImpl)->xAxis = GE_IMP_PLANE(src.m_pImpl)->xAxis;
	GE_IMP_PLANE(this->m_pImpl)->yAxis = GE_IMP_PLANE(src.m_pImpl)->yAxis;
	return *this;
}



bool GePlane::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GePlane::type() const {
	return Ge::EntityId::kPlane;
}
GePlane* GePlane::copy() const {
	GePlane* pPlane = new GePlane();
	GE_IMP_PLANE(pPlane->m_pImpl)->origin = GE_IMP_PLANE(pPlane->m_pImpl)->origin;
	GE_IMP_PLANE(pPlane->m_pImpl)->normal = GE_IMP_PLANE(pPlane->m_pImpl)->normal;
	GE_IMP_PLANE(pPlane->m_pImpl)->xAxis = GE_IMP_PLANE(pPlane->m_pImpl)->xAxis;
	GE_IMP_PLANE(pPlane->m_pImpl)->yAxis = GE_IMP_PLANE(pPlane->m_pImpl)->yAxis;
	return pPlane;
}
bool GePlane::operator == (const GePlane& entity) const {
	return this->isEqualTo(entity);
}
bool GePlane::operator != (const GePlane& entity) const {
	return !this->isEqualTo(entity);
}
bool GePlane::isEqualTo(const GePlane& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GePlane::isEqualTo(const GePlane& entity, const GeTol& tol) const {
	if (this->pointOnPlane().isEqualTo(entity.pointOnPlane(), tol) == false) {
		return false;
	}
	if (this->normal().isEqualTo(entity.normal(), tol) == false) {
		return false;
	}
	return true;
}
GePlane& GePlane::transformBy(const GeMatrix3d& xfm) {
	GePoint3d origin = this->pointOnPlane();
	GePoint3d xPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal;

	origin.transformBy(xfm);
	xPoint.transformBy(xfm);
	yPoint.transformBy(xfm);
	zPoint.transformBy(xfm);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = zPoint - origin;

	return *this;
}
GePlane& GePlane::translateBy(const GeVector3d& translateVec) {
	GE_IMP_PLANE(this->m_pImpl)->origin.translation(translateVec);
	return *this;
}
GePlane& GePlane::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GePlane& GePlane::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {
	GePoint3d origin = this->pointOnPlane();
	GePoint3d xPoint = origin + GE_IMP_PLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_PLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_PLANE(this->m_pImpl)->normal;

	origin.rotateBy(angle, vec, wrtPoint);
	xPoint.rotateBy(angle, vec, wrtPoint);
	yPoint.rotateBy(angle, vec, wrtPoint);
	zPoint.rotateBy(angle, vec, wrtPoint);

	GE_IMP_PLANE(this->m_pImpl)->origin = origin;
	GE_IMP_PLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_PLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_PLANE(this->m_pImpl)->normal = zPoint - origin;

	return *this;
}
GePlane& GePlane::mirror(const GePlane& plane) {

	GePoint3d p1 = this->pointOnPlane();
	GePoint3d p2 = p1 + this->normal();
	p1.mirror(plane);
	p2.mirror(plane);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = p1;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = p2 - p1;

	return *this;
}
GePlane& GePlane::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GePlane& GePlane::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {
	GePoint3d p1 = this->pointOnPlane();
	p1.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);

	GE_IMP_PLANE(this->m_pImpl)->origin = p1;

	return *this;
}
bool GePlane::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GePlane::isOn(const GePoint3d& pnt, const GeTol& tol) const {

	//判断点是否和平面原点重合
	if (this->pointOnPlane().isEqualTo(pnt, tol) == true) {
		return true;
	}

	//判断向量是否和平面法向垂直如垂直则在平面上
	GeVector3d vec = pnt - this->pointOnPlane();
	if (vec.isPerpendicularTo(this->normal(), tol) == false) {
		return false;
	}
	return true;
}



GePoint2d GePlane::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
GePoint2d GePlane::paramOf(const GePoint3d& pnt, const GeTol& tol) const {

	//获得坐标系
	GePoint3d origin;
	GeVector3d xAxis, yAxis;
	this->getCoordSystem(origin, xAxis, yAxis);
	yAxis = xAxis.crossProduct(this->normal()).normal();

	//获得矩阵
	GeMatrix3d mat;
	mat.setToAlignCoordSys(this->pointOnPlane(), xAxis, yAxis, this->normal(),
		GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis);

	GePoint3d point = pnt;
	point.transformBy(mat);

	return GePoint2d(point.x, point.y);
}
bool GePlane::isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const {
	return this->isOn(pnt, paramPoint, GeContext::gTol);
} 
bool GePlane::isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const {

	if (this->isOn(pnt, tol) == false) {
		return false;
	}

	paramPoint = this->paramOf(pnt, tol);
	return true;
}
GePoint3d GePlane::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GePlane::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return pnt.orthoProject(*this);
}
void GePlane::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const {
	return this->getClosestPointTo(pnt, result, GeContext::gTol);
}
void GePlane::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const {

	GePoint3d closest = this->closestPointTo(pnt, tol);

	result.setSurface(*this);
	result.setParameter(this->paramOf(closest, tol));
}
double GePlane::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GePlane::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d point = pnt.orthoProject(*this);
	return point.distanceTo(pnt);
}
bool GePlane::isNormalReversed() const {
	return GE_IMP_PLANE(this->m_pImpl)->isNormalReversed;
}
GeSurface& GePlane::reverseNormal() {
	if (GE_IMP_PLANE(this->m_pImpl)->isNormalReversed == false) {
		GE_IMP_PLANE(this->m_pImpl)->isNormalReversed = true;
	}
	else {
		GE_IMP_PLANE(this->m_pImpl)->isNormalReversed = false;
	}
	GE_IMP_PLANE(this->m_pImpl)->normal.negate();
	return *this;
}
GePoint3d GePlane::evalPoint(const GePoint2d& param) const {
	GePointOnSurface surface;
	surface.setSurface(*this);
	surface.setParameter(param);
	return surface.point();
}




bool GePlane::intersectWith(const GeLine3d& line, GePoint3d& pnt) const {
	return this->intersectWith(line, pnt, GeContext::gTol);
}
bool GePlane::intersectWith(const GeLine3d& line, GePoint3d& pnt, const GeTol& tol) const {

	//如果直线和平面平行则返回false
	if (this->isParallelTo(line, tol) == true) {
		if (this->isOn(line.pointOnLine(), tol) == true) {
			pnt = line.pointOnLine();
		}
		return false;
	}

	//获得点到平面上的投影
	pnt = line.pointOnLine().project(*this, line.direction());

	return true;
}
bool GePlane::intersectWith(const GeLineSeg3d& line, GePoint3d& pnt) const {
	return this->intersectWith(line, pnt, GeContext::gTol);
}
bool GePlane::intersectWith(const GeLineSeg3d& line, GePoint3d& pnt, const GeTol& tol) const {
	GeLine3d line3d;
	line3d.set(line.pointOnLine(), line.direction());
	if (this->intersectWith(line3d, pnt, tol) == false) {
		return false;
	}

	if (line.isOn(pnt, tol) == false) {
		return false;
	}
	return true;
}
bool GePlane::intersectWith(const GeRay3d& line, GePoint3d& pnt) const {
	return this->intersectWith(line, pnt, GeContext::gTol);
}
bool GePlane::intersectWith(const GeRay3d& line, GePoint3d& pnt, const GeTol& tol) const {

	GeLine3d line3d;
	line3d.set(line.pointOnLine(), line.direction());
	if (this->intersectWith(line3d, pnt, tol) == false) {
		return false;
	}

	if (line.isOn(pnt, tol) == false) {
		return false;
	}
	return true;
}
GePoint3d GePlane::closestPointToLinearEnt(const GeLine3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeLine3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{

	GePoint3d closest;

	//判断是否平行,如果平行则计算点到平面的距离
	if (this->isParallelTo(line, tol) == true) {

		pointOnLine.set(line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		closest = pointOnLine.orthoProject(*this);
		return closest;
	}

	GePoint3dArray pointItselfs, pointOthers;

	//获得直线和平面的交点
	GePoint3d intersect;
	if (this->intersectWith(line, intersect, tol) == true) {
		pointItselfs.append(intersect);
		pointOthers.append(intersect);
	}

	//计算出最近点
	double minDist = 0.0;
	for (int i = 0; i < pointItselfs.length(); i++) {
		for (int u = 0; u < pointOthers.length(); u++) {
			double dist = pointItselfs[i].distanceTo(pointOthers[u]);
			if (i == 0 && u == 0) {
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pointOnLine.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				continue;
			}
			if (dist < minDist)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pointOnLine.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
			}
		}
	}

	return closest;
}
GePoint3d GePlane::closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{
	
	GePoint3d closest;

	//判断是否平行,如果平行则计算点到平面的距离
	if (this->isParallelTo(line, tol) == true) {

		pointOnLine.set(line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		closest = pointOnLine.orthoProject(*this);
		return closest;
	}

	GePoint3dArray pointItselfs, pointOthers;

	//获得直线和平面的交点
	GePoint3d intersect;
	if (this->intersectWith(line, intersect, tol) == true) {
		pointItselfs.append(intersect);
		pointOthers.append(intersect);
	}
	else {
		pointItselfs.append(line.startPoint().orthoProject(*this));
		pointOthers.append(line.startPoint());

		pointItselfs.append(line.endPoint().orthoProject(*this));
		pointOthers.append(line.endPoint());
	}

	//计算出最近点
	double minDist = 0.0;
	for (int i = 0; i < pointItselfs.length(); i++) {
		for (int u = 0; u < pointOthers.length(); u++) {
			double dist = pointItselfs[i].distanceTo(pointOthers[u]);
			if (i == 0 && u == 0) {
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pointOnLine.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				continue;
			}
			if (dist < minDist)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pointOnLine.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
			}
		}
	}

	return closest;
}
GePoint3d GePlane::closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{
	GePoint3d closest;

	//判断是否平行,如果平行则计算点到平面的距离
	if (this->isParallelTo(line, tol) == true) {

		pointOnLine.set(line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		closest = pointOnLine.orthoProject(*this);
		return closest;
	}

	GePoint3dArray pointItselfs, pointOthers;
	pointOthers.append(line.pointOnLine());

	//获得直线和平面的交点
	GePoint3d intersect;
	if (this->intersectWith(line, intersect, tol) == true) {
		pointItselfs.append(intersect);
		pointOthers.append(intersect);
	}
	else {
		pointItselfs.append(line.pointOnLine().orthoProject(*this));
		pointOthers.append(line.pointOnLine());
	}

	//计算出最近点
	double minDist = 0.0;
	for (int i = 0; i < pointItselfs.length(); i++) {
		for (int u = 0; u < pointOthers.length(); u++) {
			double dist = pointItselfs[i].distanceTo(pointOthers[u]);
			if (i == 0 && u == 0) {
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pointOnLine.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
				continue;
			}
			if (dist < minDist)
			{
				minDist = dist;
				closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
				pointOnLine.set(pointOthers[u].x, pointOthers[u].y, pointOthers[u].z);
			}
		}
	}

	return closest;
}
GePoint3d GePlane::closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln) const{
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlane::closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	
	GePoint3d closest;

	//判断是否平行
	if (this->isParallelTo(otherPln, tol) == true) {
		pointOnOtherPln = otherPln.pointOnPlane();
		closest = pointOnOtherPln.orthoProject(*this);
		return closest;
	}

	//获得最近点
	GeLine3d line;
	if (this->intersectWith(otherPln, line, tol) == true) {
		pointOnOtherPln = line.pointOnLine();
		closest = line.pointOnLine();
	}

	return closest;
}
GePoint3d GePlane::closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln) const {
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlane::closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {

	GePoint3d closest;

	if (this->isParallelTo(otherPln, tol) == true) {
		pointOnOtherPln = otherPln.pointOnPlane();
		closest = pointOnOtherPln.orthoProject(*this);
		return closest;
	}

	GePoint3d origin;
	GeVector3d xAxis, yAxis;
	otherPln.get(origin, xAxis, yAxis);

	GePoint3dArray points;
	points.append(origin);
	points.append(origin + xAxis);
	points.append(origin + xAxis + yAxis);
	points.append(origin + yAxis);

	double midDist = 0.0;
	for (int i = 0; i < points.length(); i++) {
		GePoint3d point = points[i].orthoProject(*this);

		double dist = point.distanceTo(points[i]);
		if (i == 0 || dist < midDist) {
			midDist = dist;
			closest = point;
			pointOnOtherPln = points[i];
		}
	}

	return closest;
}
bool GePlane::isParallelTo(const GeLine3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlane::isParallelTo(const GeLine3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GePlane::isParallelTo(const GeLineSeg3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlane::isParallelTo(const GeLineSeg3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GePlane::isParallelTo(const GeRay3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlane::isParallelTo(const GeRay3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GePlane::isParallelTo(const GePlane& otherPlnEnt) const{
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlane::isParallelTo(const GePlane& otherPlnEnt, const GeTol& tol) const{
	if (this->normal().isParallelTo(otherPlnEnt.normal(), tol) == false) {
		return false;
	}
	return true;
}
bool GePlane::isParallelTo(const GeBoundedPlane& otherPlnEnt) const {
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlane::isParallelTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const {
	if (this->normal().isParallelTo(otherPlnEnt.normal(), tol) == false) {
		return false;
	}
	return true;
}
bool GePlane::isPerpendicularTo(const GeLine3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeLine3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GeLineSeg3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeLineSeg3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GeRay3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeRay3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GePlane& otherPlnEnt) const{
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GePlane& otherPlnEnt, const GeTol& tol) const{
	return otherPlnEnt.normal().isPerpendicularTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GeBoundedPlane& otherPlnEnt) const {
	return this->isPerpendicularTo(otherPlnEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const {
	return otherPlnEnt.normal().isPerpendicularTo(this->normal(), tol);
}
bool GePlane::isCoplanarTo(const GePlane& otherPlnEnt) const{
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlane::isCoplanarTo(const GePlane& otherPlnEnt, const GeTol& tol) const {
	if (this->isParallelTo(otherPlnEnt, tol) == false) {
		return false;
	}
	if (this->isOn(otherPlnEnt.pointOnPlane(), tol) == false) {
		return false;
	}
	return true;
}
bool GePlane::isCoplanarTo(const GeBoundedPlane& otherPlnEnt) const {
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlane::isCoplanarTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const {
	if (this->isParallelTo(otherPlnEnt, tol) == false) {
		return false;
	}
	if (this->isOn(otherPlnEnt.pointOnPlane(), tol) == false) {
		return false;
	}
	return true;
}