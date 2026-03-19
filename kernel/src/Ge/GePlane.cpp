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
	this->set(GePoint3d::kOrigin, GeVector3d::kZAxis);
}
GePlane::GePlane(const GePlane& src) {
	GE_IMP_MEMORY_ENTITY(GePlane);

	this->set(src.pointOnPlane(), src.normal());
	GE_IMP_PLANE(this->m_pImpl)->isNormalReversed = GE_IMP_PLANE(src.m_pImpl)->isNormalReversed;
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
	return this->normal().dotProduct(pnt - this->pointOnPlane());
}
bool GePlane::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt) const {
	return this->intersectWith(linEnt, resultPnt, GeContext::gTol);
}
bool GePlane::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt, const GeTol& tol) const {
	GeVector3d normal = this->normal();
	GeVector3d direction = linEnt.direction();
	if (direction.length() < tol.equalVector()) {
		return false;
	}

	GeVector3d offset = linEnt.pointOnLine() - this->pointOnPlane();
	double denom = normal.dotProduct(direction);
	double numer = normal.dotProduct(offset);

	if (fabs(denom) <= tol.equalVector()) {
		return false;
	}

	double lineParam = -numer / denom;
	resultPnt = linEnt.pointOnLine() + direction * lineParam;
	if (this->isOn(resultPnt, tol) == false) {
		return false;
	}
	return linEnt.isOn(resultPnt, tol);
}
bool GePlane::intersectWith(const GePlane& otherPln, GeLine3d& resultLine) const {
	return this->intersectWith(otherPln, resultLine, GeContext::gTol);
}
bool GePlane::intersectWith(const GePlane& otherPln, GeLine3d& resultLine, const GeTol& tol) const {
	GeVector3d normal0 = this->normal();
	GeVector3d normal1 = otherPln.normal();
	GeVector3d direction = normal0.crossProduct(normal1);
	if (direction.length() <= tol.equalVector()) {
		return false;
	}

	double coef0 = 0.0;
	double coef1 = 0.0;
	this->getCoefficients(normal0.x, normal0.y, normal0.z, coef0);
	otherPln.getCoefficients(normal1.x, normal1.y, normal1.z, coef1);

	double n00 = normal0.lengthSqrd();
	double n01 = normal0.dotProduct(normal1);
	double n11 = normal1.lengthSqrd();
	double det = n00 * n11 - n01 * n01;
	if (fabs(det) <= tol.equalPoint()) {
		return false;
	}

	double invDet = 1.0 / det;
	double c0 = (n11 * coef0 - n01 * coef1) * invDet;
	double c1 = (n00 * coef1 - n01 * coef0) * invDet;
	GeVector3d tmp = normal0 * c0 + normal1 * c1;
	GePoint3d point(-tmp.x, -tmp.y, -tmp.z);
	resultLine.set(point, direction);
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

	if (linePoints.length() >= 2) {
		int minIndex = 0;
		int maxIndex = 0;
		double minParam = line.paramOf(linePoints[0], tol);
		double maxParam = minParam;
		for (int i = 1; i < linePoints.length(); ++i) {
			double param = line.paramOf(linePoints[i], tol);
			if (param < minParam) {
				minParam = param;
				minIndex = i;
			}
			if (param > maxParam) {
				maxParam = param;
				maxIndex = i;
			}
		}
		if (linePoints[minIndex].isEqualTo(linePoints[maxIndex], tol)) {
			return false;
		}
		resultLineSeg.set(linePoints[minIndex], linePoints[maxIndex]);
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
	GE_IMP_PLANE(this->m_pImpl)->isNormalReversed = GE_IMP_PLANE(src.m_pImpl)->isNormalReversed;
	return *this;
}



bool GePlane::isKindOf(Ge::EntityId entType) const {
	return entType == Ge::EntityId::kEntity3d
		|| entType == Ge::EntityId::kSurface
		|| entType == Ge::EntityId::kPlanarEnt
		|| entType == this->type();
}
Ge::EntityId GePlane::type() const {
	return Ge::EntityId::kPlane;
}
GePlane* GePlane::copy() const {
	GePlane* pPlane = new GePlane();
	GE_IMP_PLANE(pPlane->m_pImpl)->origin = GE_IMP_PLANE(this->m_pImpl)->origin;
	GE_IMP_PLANE(pPlane->m_pImpl)->normal = GE_IMP_PLANE(this->m_pImpl)->normal;
	GE_IMP_PLANE(pPlane->m_pImpl)->xAxis = GE_IMP_PLANE(this->m_pImpl)->xAxis;
	GE_IMP_PLANE(pPlane->m_pImpl)->yAxis = GE_IMP_PLANE(this->m_pImpl)->yAxis;
	GE_IMP_PLANE(pPlane->m_pImpl)->isNormalReversed = GE_IMP_PLANE(this->m_pImpl)->isNormalReversed;
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
	GePoint3d xPoint = origin + GE_IMP_PLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_PLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_PLANE(this->m_pImpl)->normal;

	origin.transformBy(xfm);
	xPoint.transformBy(xfm);
	yPoint.transformBy(xfm);
	zPoint.transformBy(xfm);

	GE_IMP_PLANE(this->m_pImpl)->origin = origin;
	GE_IMP_PLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_PLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_PLANE(this->m_pImpl)->normal = zPoint - origin;

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

	GePoint3d origin = this->pointOnPlane();
	GePoint3d xPoint = origin + GE_IMP_PLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_PLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_PLANE(this->m_pImpl)->normal;
	origin.mirror(plane);
	xPoint.mirror(plane);
	yPoint.mirror(plane);
	zPoint.mirror(plane);

	GE_IMP_PLANE(this->m_pImpl)->origin = origin;
	GE_IMP_PLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_PLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_PLANE(this->m_pImpl)->normal = zPoint - origin;

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
	GeVector3d offset = pnt - GE_IMP_PLANE(this->m_pImpl)->origin;
	GeVector3d vCrossProdNormal = GE_IMP_PLANE(this->m_pImpl)->yAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->normal);
	GeVector3d normalCrossProdU = GE_IMP_PLANE(this->m_pImpl)->normal.crossProduct(GE_IMP_PLANE(this->m_pImpl)->xAxis);
	double tripleProduct = vCrossProdNormal.dotProduct(GE_IMP_PLANE(this->m_pImpl)->xAxis);
	if (fabs(tripleProduct) <= 1.0e-300)
	{
		return GePoint2d();
	}
	return GePoint2d(
		offset.dotProduct(vCrossProdNormal) / tripleProduct,
		offset.dotProduct(normalCrossProdU) / tripleProduct);
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
	return this->isLeftHanded();
}
bool GePlane::isLeftHanded() const {
	return GE_IMP_PLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANE(this->m_pImpl)->yAxis).dotProduct(GE_IMP_PLANE(this->m_pImpl)->normal) < 0.0;
}
GeSurface& GePlane::reverseNormal() {
	GE_IMP_PLANE(this->m_pImpl)->normal.negate();
	return *this;
}
void GePlane::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const {
	rangeU.set();
	rangeV.set();
}
GePoint3d GePlane::evalPoint(const GePoint2d& param) const {
	return GE_IMP_PLANE(this->m_pImpl)->origin
		+ GE_IMP_PLANE(this->m_pImpl)->xAxis * param.x
		+ GE_IMP_PLANE(this->m_pImpl)->yAxis * param.y;
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
	return GePlanarEnt::closestPointToLinearEnt(line, pointOnLine, tol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{
	return GePlanarEnt::closestPointToLinearEnt(line, pointOnLine, tol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlane::closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{
	return GePlanarEnt::closestPointToLinearEnt(line, pointOnLine, tol);
}
GePoint3d GePlane::closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln) const{
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlane::closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	return GePlanarEnt::closestPointToPlanarEnt(otherPln, pointOnOtherPln, tol);
}
GePoint3d GePlane::closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln) const {
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlane::closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	if (this->isParallelTo(otherPln, tol) == true) {
		pointOnOtherPln = otherPln.closestPointTo(this->pointOnPlane(), tol);
		return pointOnOtherPln.orthoProject(*this);
	}

	GeLineSeg3d intersectLine;
	if (this->intersectWith(otherPln, intersectLine, tol) == true) {
		pointOnOtherPln = intersectLine.midPoint();
		return pointOnOtherPln;
	}

	GePoint3d origin;
	GeVector3d xAxis, yAxis;
	otherPln.get(origin, xAxis, yAxis);

	GePoint3d corners[4] = {
		origin,
		origin + xAxis,
		origin + xAxis + yAxis,
		origin + yAxis
	};

	GePoint3d closest;
	double minDist = -1.0;
	for (int i = 0; i < 4; ++i) {
		GeLineSeg3d edge;
		edge.set(corners[i], corners[(i + 1) % 4]);
		GePoint3d candidateOnOther;
		GePoint3d candidateOnThis = this->closestPointToLinearEnt(edge, candidateOnOther, tol);
		double dist = candidateOnThis.distanceTo(candidateOnOther);
		if (minDist < 0.0 || dist < minDist) {
			minDist = dist;
			closest = candidateOnThis;
			pointOnOtherPln = candidateOnOther;
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
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeLine3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GeLineSeg3d& linEnt) const{
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeLineSeg3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GeRay3d& linEnt) const{
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlane::isPerpendicularTo(const GeRay3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlane::isPerpendicularTo(const GePlane& otherPlnEnt) const{
	return this->isPerpendicularTo(otherPlnEnt, GeContext::gTol);
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
	return this->isCoplanarTo(otherPlnEnt, GeContext::gTol);
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
	return this->isCoplanarTo(otherPlnEnt, GeContext::gTol);
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