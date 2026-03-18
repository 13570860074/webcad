#include "GeRay3d.h"
#include "GeScale3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeCircArc3d.h"
#include "GePlane.h"
#include "GePointOnCurve3d.h"
#include "GeInterval.h"
#include "GeBoundBlock3d.h"
#include "GeImpl.h"


GeRay3d::GeRay3d()
{
	GE_IMP_MEMORY_ENTITY(GeRay3d);
}
GeRay3d::GeRay3d(const GeRay3d& line)
{
	GE_IMP_MEMORY_ENTITY(GeRay3d);

	this->set(GE_IMP_RAY3D(line.m_pImpl)->origin, GE_IMP_RAY3D(line.m_pImpl)->vector);
}
GeRay3d::GeRay3d(const GePoint3d& point, const GeVector3d& vect)
{
	GE_IMP_MEMORY_ENTITY(GeRay3d);

	this->set(point, vect);
}
GeRay3d::GeRay3d(const GePoint3d& point1, const GePoint3d& point2)
{
	GE_IMP_MEMORY_ENTITY(GeRay3d);

	this->set(point1, point2);
}
GeRay3d& GeRay3d::set(const GePoint3d& point, const GeVector3d& vect)
{
	GE_IMP_RAY3D(this->m_pImpl)->origin = GePoint3d(point);
	GE_IMP_RAY3D(this->m_pImpl)->vector = GeVector3d(vect);
	return *this;
}
GeRay3d& GeRay3d::set(const GePoint3d& point1, const GePoint3d& point2)
{
	GE_IMP_RAY3D(this->m_pImpl)->origin = GePoint3d(point1);
	GE_IMP_RAY3D(this->m_pImpl)->vector = GeVector3d(point2 - point1);
	return *this;
}
GeRay3d& GeRay3d::operator =(const GeRay3d& line)
{
	this->set(GE_IMP_RAY3D(line.m_pImpl)->origin, GE_IMP_RAY3D(line.m_pImpl)->vector);
	return *this;
}



bool GeRay3d::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeRay3d::type() const {
	return Ge::EntityId::kRay3d;
}
GeRay3d* GeRay3d::copy() const {
	GeRay3d* pLine = new GeRay3d();
	pLine->set(GE_IMP_RAY3D(this->m_pImpl)->origin, GE_IMP_RAY3D(this->m_pImpl)->vector);
	return pLine;
}
bool GeRay3d::operator == (const GeRay3d& entity) const {
	return this->isEqualTo(entity);
}
bool GeRay3d::operator != (const GeRay3d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeRay3d::isEqualTo(const GeRay3d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeRay3d::isEqualTo(const GeRay3d& entity, const GeTol& tol) const {
	if (this->pointOnLine().isEqualTo(entity.pointOnLine(), tol) == false) {
		return false;
	}
	if (this->direction().isEqualTo(entity.direction(), tol) == false) {
		return false;
	}
	return true;
}
GeRay3d& GeRay3d::transformBy(const GeMatrix3d& xfm) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction();
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(p1, p2);
	return *this;
}
GeRay3d& GeRay3d::translateBy(const GeVector3d& translateVec) {
	this->set(this->pointOnLine() + translateVec, this->direction() * this->length());
	return *this;
}
GeRay3d& GeRay3d::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeRay3d& GeRay3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction();
	p1.rotateBy(angle, vec, wrtPoint);
	p2.rotateBy(angle, vec, wrtPoint);
	this->set(p1, p2);
	return *this;
}
GeRay3d& GeRay3d::mirror(const GePlane& plane) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction();
	p1.mirror(plane);
	p2.mirror(plane);
	this->set(p1, p2);
	return *this;
}
GeRay3d& GeRay3d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeRay3d& GeRay3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction();
	p1.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	p2.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	this->set(p1, p2);
	return *this;
}
bool GeRay3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeRay3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	GeVector3d v1 = pnt - this->pointOnLine();
	GeVector3d v2 = this->direction();
	v1.normalize();
	v2.normalize();
	if (v1.isEqualTo(v2, tol) == false)
	{
		return false;
	}
	return true;
}



double GeRay3d::length() const {
	return GE_IMP_RAY3D(this->m_pImpl)->vector.length();
}
double GeRay3d::length(double fromParam, double toParam)const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeRay3d::length(double fromParam, double toParam, double tol)const {
	return (toParam - fromParam) * this->length();
}
double GeRay3d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeRay3d::paramAtLength(double datumParam, double length, double tol) const {
	double param = 0.0;

	param = datumParam + length / this->length();

	return param;
}
double GeRay3d::area() const {
	return this->area(GeContext::gTol);
}
double GeRay3d::area(const GeTol& tol) const {
	return 0.0;
}
double GeRay3d::distanceTo(const GePoint3d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeRay3d::distanceTo(const GePoint3d& point, const GeTol& tol) const
{
	GePoint3d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeRay3d::distanceTo(const GeLine3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay3d::distanceTo(const GeLine3d& line, const GeTol& tol) const {
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeRay3d::distanceTo(const GeLineSeg3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay3d::distanceTo(const GeLineSeg3d& line, const GeTol& tol) const {
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeRay3d::distanceTo(const GeRay3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay3d::distanceTo(const GeRay3d& line, const GeTol& tol) const {
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeRay3d::distanceTo(const GeCircArc3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay3d::distanceTo(const GeCircArc3d& arc, const GeTol& tol) const {

	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint3d GeRay3d::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeRay3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d point = GeRay3d::vertical(pnt, *this, tol);
	if (this->isOn(point) == true) {
		return point;
	}
	return this->pointOnLine();
}
GePoint3d GeRay3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y,intersect.z);
			closest.set(intersect.x, intersect.y,intersect.z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve3d.closestPointTo(this->pointOnLine(), tol));

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++) {
			for (int u = 0; u < pointOthers.length(); u++) {
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0) {
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeRay3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y,intersect.z);
			closest.set(intersect.x, intersect.y,intersect.z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.startPoint(), tol));
		pointOthers.append(curve3d.startPoint());
		pointItselfs.append(this->closestPointTo(curve3d.endPoint(), tol));
		pointOthers.append(curve3d.endPoint());
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve3d.closestPointTo(this->pointOnLine(), tol));

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++) {
			for (int u = 0; u < pointOthers.length(); u++) {
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0) {
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeRay3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y,intersect.z);
			closest.set(intersect.x, intersect.y,intersect.z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.pointOnLine(), tol));
		pointOthers.append(curve3d.pointOnLine());
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve3d.closestPointTo(this->pointOnLine(), tol));

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++) {
			for (int u = 0; u < pointOthers.length(); u++) {
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0) {
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeRay3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		//获得交点
		GePoint3dArray intersects = curve3d.intersectWith(*this, tol);
		if (intersects.length() > 0) {
			pntOnOtherCrv.set(intersects[0].x, intersects[0].y, intersects[0].z);
			closest.set(intersects[0].x, intersects[0].y, intersects[0].z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.startPoint(), tol));
		pointOthers.append(curve3d.startPoint());
		pointItselfs.append(this->closestPointTo(curve3d.endPoint(), tol));
		pointOthers.append(curve3d.endPoint());
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve3d.closestPointTo(this->pointOnLine(), tol));

		//获得圆弧和垂线是否存在交点
		GePoint3d vertical = GeLine3d::vertical(curve3d.center(), *this);
		intersects = curve3d.intersectWith(GeLineSeg3d(vertical, curve3d.center()), tol);
		if (intersects.length() > 0) {
			if (this->isOn(vertical, tol) == true) {
				pointItselfs.append(vertical);
			}
			if (curve3d.isOn(intersects[0], tol) == true) {
				pointOthers.append(intersects[0]);
			}
		}

		double minDist = 0.0;
		for (int i = 0; i < pointItselfs.length(); i++) {
			for (int u = 0; u < pointOthers.length(); u++) {
				double dist = pointItselfs[i].distanceTo(pointOthers[u]);
				if (i == 0 && u == 0) {
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
					continue;
				}
				if (dist < minDist)
				{
					minDist = dist;
					closest.set(pointItselfs[i].x, pointItselfs[i].y,pointItselfs[i].z);
					pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y,pointOthers[u].z);
				}
			}
		}

	} while (false);

	return closest;
}
GePoint3d GeRay3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const {
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeRay3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
GePoint3d GeRay3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
GePoint3d GeRay3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
GePoint3d GeRay3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
GePoint3d GeRay3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeRay3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
void GeRay3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const {
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeRay3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {

}
void GeRay3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeRay3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}
void GeRay3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeRay3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}
void GeRay3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeRay3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}
void GeRay3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeRay3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}
bool GeRay3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeRay3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {

	GePoint3d vertical = GeRay3d::vertical(pnt, *this);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(vertical, tol));
	return true;
}
GeRay3d* GeRay3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const {
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeRay3d* GeRay3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const {

	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();

	p1.project(projectionPlane, projectDirection);
	p2.project(projectionPlane, projectDirection);

	GeRay3d *line = new GeRay3d();
	line->set(p1, p2 - p1);
	return line;
}
GeRay3d* GeRay3d::orthoProject(const GePlane& projectionPlane) const {
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeRay3d* GeRay3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const {
	return this->project(projectionPlane, projectionPlane.normal(), tol);
}
bool GeRay3d::isOn(const GePoint3d& pnt, double& param) const {
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeRay3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const {
	param = this->paramOf(pnt, tol);
	return this->isOn(pnt, tol);
}
bool GeRay3d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeRay3d::isOn(double param, const GeTol& tol) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);

	return this->isOn(pointOnCurve.point(), tol);
}
GECURVE3D_METHODS_GETCLOSESTPOINTTO(GeRay3d);
double GeRay3d::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeRay3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const {

	double param = 0.0;

	do
	{
		GeLine3d line(this->pointOnLine(), this->direction());
		if (line.isOn(pnt, tol) == false) {
			break;
		}

		GeVector3d direction = pnt - this->pointOnLine();
		direction.normalize();
		if (direction.isEqualTo(this->direction(), tol) == true) {
			param = (pnt.distanceTo(this->pointOnLine())) / this->length();
		}
		else {
			param = 0 - (pnt.distanceTo(this->pointOnLine())) / this->length();
		}
	} while (false);

	return param;
}
void GeRay3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeRay3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeRay3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {
	
	if (abs(distance) < 0.0000001)
	{
		GeRay3d* line = new GeRay3d();
		line->set(GE_IMP_LINEARENT3D(this->m_pImpl)->origin, GE_IMP_LINEARENT3D(this->m_pImpl)->vector);
		offsetCurveList.append(line);
		return;
	}

	GeVector3d axis = this->direction();
	axis.rotateBy(PI / 2.0, planeNormal);
	axis.normalize();

	GePoint3d origin = GE_IMP_LINE3D(this->m_pImpl)->origin + axis * distance;

	GeRay3d* line = new GeRay3d();
	line->set(origin, GE_IMP_LINE3D(this->m_pImpl)->vector);

	offsetCurveList.append(line);
}
bool GeRay3d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeRay3d::isClosed(const GeTol& tol) const {
	return false;
}
bool GeRay3d::isPlanar(GePlane& plane) const {
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeRay3d::isPlanar(GePlane& plane, const GeTol& tol) const {
	return false;
}
bool GeRay3d::isLinear(GeLine3d& line) const {
	return this->isLinear(line, GeContext::gTol);
}
bool GeRay3d::isLinear(GeLine3d& line, const GeTol& tol) const {
	return false;
}
bool GeRay3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeRay3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeRay3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeRay3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeRay3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeRay3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeRay3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeRay3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1;
	line1.set(this->pointOnLine(), this->direction());
	return line1.isCoplanarWith(curve3d, plane, tol);
}
void GeRay3d::getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const {
	GePointOnCurve3d pointOnCurve(*this, param);
	GePoint3d point = pointOnCurve.point();
	if (this->isOn(point) == false) {
		return;
	}
	piece1 = new GeLineSeg3d(this->pointOnLine(), point);
	piece2 = new GeRay3d(point, this->direction());
}
bool GeRay3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	return false;
}
GeBoundBlock3d GeRay3d::boundBlock() const {
	GeInterval range;
	range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	return this->boundBlock(range);
}
GeBoundBlock3d GeRay3d::boundBlock(const GeInterval& range) const {

	GeBoundBlock3d boundBlock;

	GePoint3d basePoint;
	basePoint = this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound();

	GePoint3d endPoint;
	endPoint = this->pointOnLine() + (this->direction() * this->length()) * range.upperBound();

	boundBlock.set(basePoint, endPoint);

	return boundBlock;
}
GeBoundBlock3d GeRay3d::orthoBoundBlock() const {
	GeInterval range;
	range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeRay3d::orthoBoundBlock(const GeInterval& range) const {

	GeBoundBlock3d boundBlock;

	GePoint3dArray points;
	points.append(this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound());
	points.append(this->pointOnLine() + (this->direction() * this->length()) * range.upperBound());

	GePoint3d minPoint, maxPoint;
	for (int i = 0; i < points.length(); i++) {
		if (i == 0) {
			minPoint = points[i];
			maxPoint = points[i];
			continue;
		}

		if (points[i].x < minPoint.x) {
			minPoint.x = points[i].x;
		}
		if (points[i].x > maxPoint.x) {
			maxPoint.x = points[i].x;
		}
		if (points[i].y < minPoint.y) {
			minPoint.y = points[i].y;
		}
		if (points[i].y > maxPoint.y) {
			maxPoint.y = points[i].y;
		}
	}

	boundBlock.set(minPoint, maxPoint);

	return boundBlock;
}
bool GeRay3d::hasStartPoint(GePoint3d& startPoint) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(0.0);
	startPoint = pointOnCurve.point();
	return true;
}
bool GeRay3d::hasEndPoint(GePoint3d& endPoint) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(1.0);
	endPoint = pointOnCurve.point();
	return false;
}
GePoint3d GeRay3d::evalPoint(double param) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}
void GeRay3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const {
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray);
}
void GeRay3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray, bool forceResampling) const {

	double param = fromParam;

	while (true) {

		pointArray.append(this->evalPoint(param));
		paramArray.append(param);

		param = param + approxEps;
		if (param > toParam) {
			break;
		}
	}
}
void GeRay3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const {
	return this->getSamplePoints(numSample, pointArray);
}
void GeRay3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const {
	for (int i = 0; i < numSample; i++) {
		double param = 1.0 / numSample * i;
		pointArray.append(this->evalPoint(param));
		paramArray.append(param);
	}
}



bool GeRay3d::intersectWith(const GeLine3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeRay3d::intersectWith(const GeLine3d& line, GePoint3d& intPnt, const GeTol& tol) const {
	GeLine3d line1(this->pointOnLine(), this->direction());
	GeLine3d line2(line.pointOnLine(), line.direction());
	if (line1.intersectWith(line2, intPnt, tol) == false) {
		return false;
	}
	if (line.isOn(intPnt, tol) == false) {
		return false;
	}
	if (this->isOn(intPnt, tol) == false) {
		return false;
	}
	return true;
}
bool GeRay3d::intersectWith(const GeRay3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeRay3d::intersectWith(const GeRay3d& line, GePoint3d& intPnt, const GeTol& tol) const {

	GeLine3d line1(this->pointOnLine(), this->direction());
	GeLine3d line2(line.pointOnLine(), line.direction());
	if (line1.intersectWith(line2, intPnt, tol) == false) {
		return false;
	}
	if (line.isOn(intPnt, tol) == false) {
		return false;
	}
	if (this->isOn(intPnt, tol) == false) {
		return false;
	}
	return true;
}
bool GeRay3d::intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeRay3d::intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt, const GeTol& tol) const {
	GeLine3d line1(this->pointOnLine(), this->direction());
	GeLine3d line2(line.pointOnLine(), line.direction());
	if (line1.intersectWith(line2, intPnt, tol) == false) {
		return false;
	}
	if (line.isOn(intPnt, tol) == false) {
		return false;
	}
	if (this->isOn(intPnt, tol) == false) {
		return false;
	}
	return true;
}
