#include "GeBoundedPlane.h"
#include "GeMatrix3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GePlane.h"
#include "GePointOnSurface.h"
#include "GeImpl.h"
#include <cmath>


namespace {
double bounded_plane_triple_product(const GeVector3d& vec1, const GeVector3d& vec2, const GeVector3d& vec3)
{
	return vec1.dotProduct(vec2.crossProduct(vec3));
}

bool bounded_plane_param_of(const GePoint3d& origin, const GeVector3d& xAxis, const GeVector3d& yAxis, const GeVector3d& normal, const GePoint3d& point, const GeTol& tol, GePoint2d& param)
{
	GeVector3d offset = point - origin;
	double normalLength = normal.length();
	if (normalLength <= tol.equalVector())
	{
		param.set(0.0, 0.0);
		return false;
	}

	if (std::fabs(offset.dotProduct(normal)) > tol.equalPoint() * normalLength)
	{
		param.set(0.0, 0.0);
		return false;
	}

	double denominator = normal.lengthSqrd();
	if (denominator <= tol.equalVector() * tol.equalVector())
	{
		param.set(0.0, 0.0);
		return false;
	}

	double u = bounded_plane_triple_product(offset, yAxis, normal) / denominator;
	double v = bounded_plane_triple_product(xAxis, offset, normal) / denominator;
	param.set(u, v);
	return true;
}

void bounded_plane_select_closest_pair(const GePoint3dArray& pointsOnThis, const GePoint3dArray& pointsOnOther, GePoint3d& closest, GePoint3d& pointOnOther)
{
	int pairCount = pointsOnThis.length();
	if (pointsOnOther.length() < pairCount) {
		pairCount = pointsOnOther.length();
	}
	for (int i = 0; i < pairCount; ++i) {
		double dist = pointsOnThis[i].distanceTo(pointsOnOther[i]);
		if (i == 0 || dist < closest.distanceTo(pointOnOther)) {
			closest = pointsOnThis[i];
			pointOnOther = pointsOnOther[i];
		}
	}
}

void bounded_plane_get_loop_points(const GePoint3d& origin, const GeVector3d& xAxis, const GeVector3d& yAxis, GePoint3dArray& points)
{
	points.append(origin);
	points.append(origin + xAxis);
	points.append(origin + xAxis + yAxis);
	points.append(origin + yAxis);
	points.append(origin);
}

}




GeBoundedPlane::GeBoundedPlane() {
	GE_IMP_MEMORY_ENTITY(GeBoundedPlane);
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = GePoint3d::kOrigin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = GeVector3d::kXAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = GeVector3d::kYAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = GeVector3d::kZAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->isNormalReversed = false;
}
GeBoundedPlane::GeBoundedPlane(const GeBoundedPlane& src) {
	GE_IMP_MEMORY_ENTITY(GeBoundedPlane);
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->normal;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->xAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->yAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->isNormalReversed = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->isNormalReversed;
}
GeBoundedPlane::GeBoundedPlane(const GePoint3d& pntU, const GePoint3d& org, const GePoint3d& pntV) {
	GE_IMP_MEMORY_ENTITY(GeBoundedPlane);

	this->set(pntU, org, pntV);
}
GeBoundedPlane::GeBoundedPlane(const GePoint3d& org, const GeVector3d& uAxis, const GeVector3d& vAxis) {
	GE_IMP_MEMORY_ENTITY(GeBoundedPlane);

	this->set(org, uAxis, vAxis);
}
bool GeBoundedPlane::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt) const {
	return this->intersectWith(linEnt, resultPnt, GeContext::gTol);
}
bool GeBoundedPlane::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& resultPnt, const GeTol& tol) const {
	GePlane plane(this->pointOnPlane(), this->normal());
	if (plane.intersectWith(linEnt, resultPnt, tol) == false) {
		return false;
	}
	return this->isOn(resultPnt, tol);
}
bool GeBoundedPlane::intersectWith(const GePlane& otherPln, GeLineSeg3d& resultLine) const {
	return this->intersectWith(otherPln, resultLine, GeContext::gTol);
}
bool GeBoundedPlane::intersectWith(const GePlane& otherPln, GeLineSeg3d& resultLine, const GeTol& tol) const {
	return otherPln.intersectWith(*this, resultLine, tol);
}
bool GeBoundedPlane::intersectWith(const GeBoundedPlane& bndPln, GeLineSeg3d& resultLineSeg) const {
	return this->intersectWith(bndPln, resultLineSeg, GeContext::gTol);
}
bool GeBoundedPlane::intersectWith(const GeBoundedPlane& bndPln, GeLineSeg3d& resultLineSeg, const GeTol& tol) const {
	
	GePlane plane;

	GeLineSeg3d lineSeg1;
	plane.set(bndPln.pointOnPlane(), bndPln.normal());
	if (this->intersectWith(plane, lineSeg1, tol) == false) {
		return false;
	}

	GeLineSeg3d lineSeg2;
	plane.set(this->pointOnPlane(), this->normal());
	if (bndPln.intersectWith(plane, lineSeg2, tol) == false) {
		return false;
	}

	GePoint3dArray linePoints;
	GePoint3d candidates[4] = {
		lineSeg1.startPoint(),
		lineSeg1.endPoint(),
		lineSeg2.startPoint(),
		lineSeg2.endPoint()
	};
	for (int i = 0; i < 4; ++i) {
		if (this->isOn(candidates[i], tol) == false || bndPln.isOn(candidates[i], tol) == false) {
			continue;
		}
		bool isUnique = true;
		for (int u = 0; u < linePoints.length(); ++u) {
			if (linePoints[u].isEqualTo(candidates[i], tol)) {
				isUnique = false;
				break;
			}
		}
		if (isUnique) {
			linePoints.append(candidates[i]);
		}
	}

	if (linePoints.length() < 2) {
		return false;
	}

	int firstIndex = 0;
	int secondIndex = 1;
	double maxDistance = linePoints[0].distanceTo(linePoints[1]);
	for (int i = 0; i < linePoints.length(); ++i) {
		for (int u = i + 1; u < linePoints.length(); ++u) {
			double dist = linePoints[i].distanceTo(linePoints[u]);
			if (dist > maxDistance) {
				maxDistance = dist;
				firstIndex = i;
				secondIndex = u;
			}
		}
	}
	resultLineSeg.set(linePoints[firstIndex], linePoints[secondIndex]);
	return true;
}
GeBoundedPlane& GeBoundedPlane::set(const GePoint3d& pntU, const GePoint3d& org, const GePoint3d& pntV) {

	GeVector3d v1 = GeVector3d(pntU.x - org.x, pntU.y - org.y, pntU.z - org.z);
	GeVector3d v2 = GeVector3d(pntV.x - org.x, pntV.y - org.y, pntV.z - org.z);

	//叉乘得到法向
	GeVector3d normal = v1.crossProduct(v2);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = org;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = v1;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = v2;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = normal;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->isNormalReversed = false;
	return *this;
}
GeBoundedPlane& GeBoundedPlane::set(const GePoint3d& org, const GeVector3d& uAxis, const GeVector3d& vAxis) {

	//叉乘得到法向
	GeVector3d normal = uAxis.crossProduct(vAxis);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = org;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = uAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = vAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = normal;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->isNormalReversed = false;
	return *this;
}
GeBoundedPlane& GeBoundedPlane::operator = (const GeBoundedPlane& src) {
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->normal;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->xAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->yAxis;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->isNormalReversed = GE_IMP_BOUNDEDPLANE(src.m_pImpl)->isNormalReversed;
	return *this;
}



bool GeBoundedPlane::isKindOf(Ge::EntityId entType) const {
	if (entType == Ge::EntityId::kEntity3d
		|| entType == Ge::EntityId::kSurface
		|| entType == Ge::EntityId::kPlanarEnt
		|| entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeBoundedPlane::type() const {
	return Ge::EntityId::kBoundedPlane;
}
GeBoundedPlane* GeBoundedPlane::copy() const {
	GeBoundedPlane* pPlane = new GeBoundedPlane();
	GE_IMP_BOUNDEDPLANE(pPlane->m_pImpl)->origin = GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin;
	GE_IMP_BOUNDEDPLANE(pPlane->m_pImpl)->normal = GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal;
	GE_IMP_BOUNDEDPLANE(pPlane->m_pImpl)->xAxis = GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis;
	GE_IMP_BOUNDEDPLANE(pPlane->m_pImpl)->yAxis = GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis;
	GE_IMP_BOUNDEDPLANE(pPlane->m_pImpl)->isNormalReversed = GE_IMP_BOUNDEDPLANE(this->m_pImpl)->isNormalReversed;
	return pPlane;
}
bool GeBoundedPlane::operator == (const GeBoundedPlane& entity) const {
	return this->isEqualTo(entity);
}
bool GeBoundedPlane::operator != (const GeBoundedPlane& entity) const {
	return !this->isEqualTo(entity);
}
bool GeBoundedPlane::isEqualTo(const GeBoundedPlane& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeBoundedPlane::isEqualTo(const GeBoundedPlane& entity, const GeTol& tol) const {
	if (this->pointOnPlane().isEqualTo(entity.pointOnPlane(), tol) == false) {
		return false;
	}
	if (this->normal().isEqualTo(entity.normal(), tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis.isEqualTo(GE_IMP_BOUNDEDPLANE(entity.m_pImpl)->xAxis, tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis.isEqualTo(GE_IMP_BOUNDEDPLANE(entity.m_pImpl)->yAxis, tol) == false) {
		return false;
	}
	return true;
}
GeBoundedPlane& GeBoundedPlane::transformBy(const GeMatrix3d& xfm) {

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
GeBoundedPlane& GeBoundedPlane::translateBy(const GeVector3d& translateVec) {
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin.translation(translateVec);
	return *this;
}
GeBoundedPlane& GeBoundedPlane::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeBoundedPlane& GeBoundedPlane::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {
	GePoint3d origin = this->pointOnPlane();
	GePoint3d xPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal;

	origin.rotateBy(angle, vec, wrtPoint);
	xPoint.rotateBy(angle, vec, wrtPoint);
	yPoint.rotateBy(angle, vec, wrtPoint);
	zPoint.rotateBy(angle, vec, wrtPoint);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = zPoint - origin;

	return *this;
}
GeBoundedPlane& GeBoundedPlane::mirror(const GePlane& plane) {
	GePoint3d origin = this->pointOnPlane();
	GePoint3d xPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal;
	origin.mirror(plane);
	xPoint.mirror(plane);
	yPoint.mirror(plane);
	zPoint.mirror(plane);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = zPoint - origin;

	return *this;
}
GeBoundedPlane& GeBoundedPlane::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeBoundedPlane& GeBoundedPlane::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {

	GePoint3d origin = this->pointOnPlane();
	GePoint3d xPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis;
	GePoint3d yPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis;
	GePoint3d zPoint = origin + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal;

	origin.scaleBy(scaleFactor, wrtPoint);
	xPoint.scaleBy(scaleFactor, wrtPoint);
	yPoint.scaleBy(scaleFactor, wrtPoint);
	zPoint.scaleBy(scaleFactor, wrtPoint);

	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->origin = origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis = xPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis = yPoint - origin;
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal = zPoint - origin;

	return *this;
}
bool GeBoundedPlane::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeBoundedPlane::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint2d paramPoint;
	if (bounded_plane_param_of(this->pointOnPlane(), GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis, GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis, this->normal(), pnt, tol, paramPoint) == false) {
		return false;
	}
	return paramPoint.x >= -tol.equalPoint() &&
		paramPoint.x <= 1.0 + tol.equalPoint() &&
		paramPoint.y >= -tol.equalPoint() &&
		paramPoint.y <= 1.0 + tol.equalPoint();
}



GePoint2d GeBoundedPlane::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
GePoint2d GeBoundedPlane::paramOf(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint2d param;
	bounded_plane_param_of(this->pointOnPlane(), GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis, GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis, this->normal(), pnt, tol, param);
	return param;
}
bool GeBoundedPlane::isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const {
	return this->isOn(pnt, paramPoint, GeContext::gTol);
} 
bool GeBoundedPlane::isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const {
	if (bounded_plane_param_of(this->pointOnPlane(), GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis, GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis, this->normal(), pnt, tol, paramPoint) == false) {
		return false;
	}
	if (paramPoint.x < -tol.equalPoint() || paramPoint.x > 1.0 + tol.equalPoint() ||
		paramPoint.y < -tol.equalPoint() || paramPoint.y > 1.0 + tol.equalPoint()) {
		return false;
	}
	return true;
}
GePoint3d GeBoundedPlane::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeBoundedPlane::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	
	GePlane plane(this->pointOnPlane(), this->normal());
	GePoint3d point = pnt.orthoProject(plane);
	if (this->isOn(point, tol) == true) {
		return point;
	}
	
	GePoint3dArray points;
	points.append(this->pointOnPlane());
	points.append(this->pointOnPlane() + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis);
	points.append(this->pointOnPlane() + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis);
	points.append(this->pointOnPlane() + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis);
	points.append(this->pointOnPlane());

	GePoint3d closest = points[0];
	double minDist = -1.0;
	for (int i = 1; i < points.length(); i++) {
		GeLineSeg3d lineSeg;
		lineSeg.set(points[i - 1], points[i]);

		//获得点距离线段的最小点
		point = lineSeg.closestPointTo(pnt, tol);

		//获得距离
		double dist = point.distanceTo(pnt);

		if (minDist < 0.0 || dist < tol.equalPoint() || dist < minDist) {
			closest = point;
			minDist = dist;
		}
	}

	return closest;
}
void GeBoundedPlane::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const {
	return this->getClosestPointTo(pnt, result, GeContext::gTol);
}
void GeBoundedPlane::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const {
	GePoint3d closest = this->closestPointTo(pnt, tol);

	result.setSurface(*this);
	result.setParameter(this->paramOf(closest, tol));
}
double GeBoundedPlane::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeBoundedPlane::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d closest = this->closestPointTo(pnt, tol);
	return closest.distanceTo(pnt);
}
bool GeBoundedPlane::isNormalReversed() const {
	return this->isLeftHanded();
}
bool GeBoundedPlane::isLeftHanded() const {
	return GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis.crossProduct(GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis).dotProduct(GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal) < 0.0;
}
GeSurface& GeBoundedPlane::reverseNormal() {
	GE_IMP_BOUNDEDPLANE(this->m_pImpl)->normal.negate();
	return *this;
}
void GeBoundedPlane::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const {
	rangeU.set(0.0, 1.0);
	rangeV.set(0.0, 1.0);
}
GePoint3d GeBoundedPlane::evalPoint(const GePoint2d& param) const {
	return this->pointOnPlane()
		+ GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis * param.x
		+ GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis * param.y;
}



bool GeBoundedPlane::intersectWith(const GeLine3d& line, GePoint3d& pnt) const {
	return this->intersectWith(line, pnt, GeContext::gTol);
}
bool GeBoundedPlane::intersectWith(const GeLine3d& line, GePoint3d& pnt, const GeTol& tol) const {

	//如果直线和平面平行则返回false
	if (this->isParallelTo(line, tol) == true) {
		return false;
	}

	//获得点到平面上的投影
	GePlane plane(this->pointOnPlane(), this->normal());
	pnt = line.pointOnLine().project(plane, line.direction());

	//判断点是否在平面内
	if (this->isOn(pnt, tol) == false) {
		return false;
	}

	return true;
}
bool GeBoundedPlane::intersectWith(const GeLineSeg3d& line, GePoint3d& pnt) const {
	return this->intersectWith(line, pnt, GeContext::gTol);
}
bool GeBoundedPlane::intersectWith(const GeLineSeg3d& line, GePoint3d& pnt, const GeTol& tol) const {

	//如果直线和平面平行则返回false
	if (this->isParallelTo(line, tol) == true) {
		return false;
	}

	//获得点到平面上的投影
	GePlane plane(this->pointOnPlane(), this->normal());
	pnt = line.pointOnLine().project(plane, line.direction());

	//判断点是否在平面内
	if (this->isOn(pnt, tol) == false) {
		return false;
	}

	//判断点是否在线段内
	if (line.isOn(pnt, tol) == false) {
		return false;
	}

	return true;
}
bool GeBoundedPlane::intersectWith(const GeRay3d& line, GePoint3d& pnt) const {
	return this->intersectWith(line, pnt, GeContext::gTol);
}
bool GeBoundedPlane::intersectWith(const GeRay3d& line, GePoint3d& pnt, const GeTol& tol) const {

	//如果直线和平面平行则返回false
	if (this->isParallelTo(line, tol) == true) {
		return false;
	}

	//获得点到平面上的投影
	GePlane plane(this->pointOnPlane(), this->normal());
	pnt = line.pointOnLine().project(plane, line.direction());

	//判断点是否在平面内
	if (this->isOn(pnt, tol) == false) {
		return false;
	}

	//判断点是否在线段内
	if (line.isOn(pnt, tol) == false) {
		return false;
	}

	return true;
}
GePoint3d GeBoundedPlane::closestPointToLinearEnt(const GeLine3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GeBoundedPlane::closestPointToLinearEnt(const GeLine3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{

	GePoint3d closest;

	//判断平面和直线是否相交
	GePoint3d intersect;
	if (this->intersectWith(line, intersect, tol) == true) {
		pointOnLine.set(intersect.x, intersect.y, intersect.z);
		closest.set(intersect.x, intersect.y, intersect.z);
		return closest;
	}

	GePoint3dArray pointItselfs, pointOthers;

	//获得平面的四个边界和直线的最近点
	GePoint3dArray points;
	bounded_plane_get_loop_points(this->pointOnPlane(), GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis, GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis, points);
	for (int i = 1; i < points.length(); i++) {

		GeLineSeg3d lineSeg(points[i - 1], points[i]);

		GePoint3d pntOnOtherCrv;
		GePoint3d point = lineSeg.closestPointTo(line, pntOnOtherCrv, tol);

		pointOthers.append(pntOnOtherCrv);
		pointItselfs.append(point);
	}
	bounded_plane_select_closest_pair(pointItselfs, pointOthers, closest, pointOnLine);

	return closest;
}
GePoint3d GeBoundedPlane::closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GeBoundedPlane::closestPointToLinearEnt(const GeLineSeg3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{

	GePoint3d closest;

	//判断平面和直线是否相交
	GePoint3d intersect;
	if (this->intersectWith(line, intersect, tol) == true) {
		pointOnLine.set(intersect.x, intersect.y, intersect.z);
		closest.set(intersect.x, intersect.y, intersect.z);
		return closest;
	}

	GePoint3dArray pointItselfs, pointOthers;

	//获得平面的四个边界和直线的最近点
	GePoint3dArray points;
	bounded_plane_get_loop_points(this->pointOnPlane(), GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis, GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis, points);
	for (int i = 1; i < points.length(); i++) {

		GeLineSeg3d lineSeg(points[i - 1], points[i]);

		GePoint3d pntOnOtherCrv;
		GePoint3d point = lineSeg.closestPointTo(line, pntOnOtherCrv, tol);

		pointOthers.append(pntOnOtherCrv);
		pointItselfs.append(point);
	}
	bounded_plane_select_closest_pair(pointItselfs, pointOthers, closest, pointOnLine);

	return closest;
}
GePoint3d GeBoundedPlane::closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine) const{
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GeBoundedPlane::closestPointToLinearEnt(const GeRay3d& line, GePoint3d& pointOnLine, const GeTol& tol) const{

	GePoint3d closest;

	//判断平面和直线是否相交
	GePoint3d intersect;
	if (this->intersectWith(line, intersect, tol) == true) {
		pointOnLine.set(intersect.x, intersect.y, intersect.z);
		closest.set(intersect.x, intersect.y, intersect.z);
		return closest;
	}

	GePoint3dArray pointItselfs, pointOthers;

	//获得平面的四个边界和直线的最近点
	GePoint3dArray points;
	bounded_plane_get_loop_points(this->pointOnPlane(), GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis, GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis, points);
	for (int i = 1; i < points.length(); i++) {

		GeLineSeg3d lineSeg(points[i - 1], points[i]);

		GePoint3d pntOnOtherCrv;
		GePoint3d point = lineSeg.closestPointTo(line, pntOnOtherCrv, tol);

		pointOthers.append(pntOnOtherCrv);
		pointItselfs.append(point);
	}
	bounded_plane_select_closest_pair(pointItselfs, pointOthers, closest, pointOnLine);

	return closest;
}
GePoint3d GeBoundedPlane::closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln) const{
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GeBoundedPlane::closestPointToPlanarEnt(const GePlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	GePoint3d pointOnThis;
	pointOnOtherPln = otherPln.closestPointToPlanarEnt(*this, pointOnThis, tol);
	return pointOnThis;
}
GePoint3d GeBoundedPlane::closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln) const {
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GeBoundedPlane::closestPointToPlanarEnt(const GeBoundedPlane& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {

	GePoint3d closest;
	GeLineSeg3d intersectLine;
	if (this->intersectWith(otherPln, intersectLine, tol) == true) {
		pointOnOtherPln = intersectLine.midPoint();
		return pointOnOtherPln;
	}

	GePoint3dArray points1;
	points1.append(this->pointOnPlane());
	points1.append(this->pointOnPlane() + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis);
	points1.append(this->pointOnPlane() + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->xAxis + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis);
	points1.append(this->pointOnPlane() + GE_IMP_BOUNDEDPLANE(this->m_pImpl)->yAxis);
	points1.append(this->pointOnPlane());

	GePoint3dArray points2;
	points2.append(otherPln.pointOnPlane());
	points2.append(otherPln.pointOnPlane() + GE_IMP_BOUNDEDPLANE(otherPln.m_pImpl)->xAxis);
	points2.append(otherPln.pointOnPlane() + GE_IMP_BOUNDEDPLANE(otherPln.m_pImpl)->xAxis + GE_IMP_BOUNDEDPLANE(otherPln.m_pImpl)->yAxis);
	points2.append(otherPln.pointOnPlane() + GE_IMP_BOUNDEDPLANE(otherPln.m_pImpl)->yAxis);
	points2.append(otherPln.pointOnPlane());

	GePoint3dArray pointItselfs, pointOthers;

	for (int i = 1; i < points1.length(); i++) {
		
		GeLineSeg3d lineSegItself(points1[i - 1], points1[i]);

		pointItselfs.append(lineSegItself.startPoint());
		pointOthers.append(otherPln.closestPointTo(lineSegItself.startPoint(), tol));

		pointItselfs.append(lineSegItself.endPoint());
		pointOthers.append(otherPln.closestPointTo(lineSegItself.endPoint(), tol));

		for (int u = 1; u < points2.length(); u++) {

			GeLineSeg3d lineSegOtherf(points2[u - 1], points2[u]);

			//获得两线段最近点
			GePoint3d pntOnOtherCrv;
			GePoint3d point = lineSegItself.closestPointTo(lineSegOtherf, pntOnOtherCrv, tol);

			pointItselfs.append(point);
			pointOthers.append(pntOnOtherCrv);
		}
	}

	for (int i = 1; i < points2.length(); i++) {
		GeLineSeg3d lineSegOther(points2[i - 1], points2[i]);
		pointItselfs.append(this->closestPointTo(lineSegOther.startPoint(), tol));
		pointOthers.append(lineSegOther.startPoint());

		pointItselfs.append(this->closestPointTo(lineSegOther.endPoint(), tol));
		pointOthers.append(lineSegOther.endPoint());
	}

	bounded_plane_select_closest_pair(pointItselfs, pointOthers, closest, pointOnOtherPln);

	return closest;
}
bool GeBoundedPlane::isParallelTo(const GeLine3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GeBoundedPlane::isParallelTo(const GeLine3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GeBoundedPlane::isParallelTo(const GeLineSeg3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GeBoundedPlane::isParallelTo(const GeLineSeg3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GeBoundedPlane::isParallelTo(const GeRay3d& linEnt) const{
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GeBoundedPlane::isParallelTo(const GeRay3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GeBoundedPlane::isParallelTo(const GePlane& otherPlnEnt) const{
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GeBoundedPlane::isParallelTo(const GePlane& otherPlnEnt, const GeTol& tol) const{
	if (this->normal().isParallelTo(otherPlnEnt.normal(), tol) == false) {
		return false;
	}
	return true;
}
bool GeBoundedPlane::isParallelTo(const GeBoundedPlane& otherPlnEnt) const {
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GeBoundedPlane::isParallelTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const {
	if (this->normal().isParallelTo(otherPlnEnt.normal(), tol) == false) {
		return false;
	}
	return true;
}
bool GeBoundedPlane::isPerpendicularTo(const GeLine3d& linEnt) const{
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeLine3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeLineSeg3d& linEnt) const{
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeLineSeg3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeRay3d& linEnt) const{
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeRay3d& linEnt, const GeTol& tol) const{
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GeBoundedPlane::isPerpendicularTo(const GePlane& otherPlnEnt) const{
	return this->isPerpendicularTo(otherPlnEnt, GeContext::gTol);
}
bool GeBoundedPlane::isPerpendicularTo(const GePlane& otherPlnEnt, const GeTol& tol) const{
	return otherPlnEnt.normal().isPerpendicularTo(this->normal(), tol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeBoundedPlane& otherPlnEnt) const {
	return this->isPerpendicularTo(otherPlnEnt, GeContext::gTol);
}
bool GeBoundedPlane::isPerpendicularTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const {
	return otherPlnEnt.normal().isPerpendicularTo(this->normal(), tol);
}
bool GeBoundedPlane::isCoplanarTo(const GePlane& otherPlnEnt) const{
	return this->isCoplanarTo(otherPlnEnt, GeContext::gTol);
}
bool GeBoundedPlane::isCoplanarTo(const GePlane& otherPlnEnt, const GeTol& tol) const {
	if (this->isParallelTo(otherPlnEnt, tol) == false) {
		return false;
	}
	if (this->isOn(otherPlnEnt.pointOnPlane(), tol) == false) {
		return false;
	}
	return true;
}
bool GeBoundedPlane::isCoplanarTo(const GeBoundedPlane& otherPlnEnt) const {
	return this->isCoplanarTo(otherPlnEnt, GeContext::gTol);
}
bool GeBoundedPlane::isCoplanarTo(const GeBoundedPlane& otherPlnEnt, const GeTol& tol) const {
	if (this->isParallelTo(otherPlnEnt, tol) == false) {
		return false;
	}
	if (this->isOn(otherPlnEnt.pointOnPlane(), tol) == false) {
		return false;
	}
	return true;
}