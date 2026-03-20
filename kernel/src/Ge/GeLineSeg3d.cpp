#include "GeLineSeg3d.h"
#include "GePoint3d.h"
#include "GeVector3d.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "GeScale3d.h"
#include "GeCircArc3d.h"
#include "GePlane.h"
#include "GePointOnCurve3d.h"
#include "GeInterval.h"
#include "GeBoundBlock3d.h"
#include "GeImpl.h"
#include <cmath>

namespace {
void selectClosestPair(const GePoint3dArray& pointItselfs, const GePoint3dArray& pointOthers, GePoint3d& closest, GePoint3d& pntOnOtherCrv)
{
	int pairCount = pointItselfs.length();
	if (pointOthers.length() < pairCount) {
		pairCount = pointOthers.length();
	}
	for (int i = 0; i < pairCount; ++i) {
		double dist = pointItselfs[i].distanceTo(pointOthers[i]);
		if (i == 0 || dist < closest.distanceTo(pntOnOtherCrv)) {
			closest.set(pointItselfs[i].x, pointItselfs[i].y, pointItselfs[i].z);
			pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y, pointOthers[i].z);
		}
	}
}
}


GeLineSeg3d::GeLineSeg3d()
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg3d);

	this->set(GePoint3d::kOrigin, GeVector3d::kXAxis);
}
GeLineSeg3d::GeLineSeg3d(const GeLineSeg3d& source)
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg3d);

	GE_IMP_LINESEG3D(this->m_pImpl)->origin = GE_IMP_LINESEG3D(source.m_pImpl)->origin;
	GE_IMP_LINESEG3D(this->m_pImpl)->vector = GE_IMP_LINESEG3D(source.m_pImpl)->vector;
}
GeLineSeg3d::GeLineSeg3d(const GePoint3d& point1, const GePoint3d& point2)
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg3d);

	GE_IMP_LINESEG3D(this->m_pImpl)->origin = point1;
	GE_IMP_LINESEG3D(this->m_pImpl)->vector = point2 - point1;
}
GeLineSeg3d::GeLineSeg3d(const GePoint3d& point, const GeVector3d& vect)
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg3d);

	GE_IMP_LINESEG3D(this->m_pImpl)->origin = point;
	GE_IMP_LINESEG3D(this->m_pImpl)->vector = vect;
}


GeLineSeg3d& GeLineSeg3d::set(const GePoint3d& point, const GeVector3d& vect)
{
	GE_IMP_LINESEG3D(this->m_pImpl)->origin = point;
	GE_IMP_LINESEG3D(this->m_pImpl)->vector = vect;
	return *this;
}
GeLineSeg3d& GeLineSeg3d::set(const GePoint3d& point1, const GePoint3d& point2)
{
	GE_IMP_LINESEG3D(this->m_pImpl)->origin = point1;
	GE_IMP_LINESEG3D(this->m_pImpl)->vector = point2 - point1;
	return *this;
}


void GeLineSeg3d::getBisector(GeLine3d& line) const
{
	GeVector3d dire = this->direction();
	dire.rotateBy(PI / 2, GeVector3d::kZAxis);
	GePoint3d point = this->midPoint();
	line.set(point, dire);
}


GePoint3d GeLineSeg3d::baryComb(double blendCoeff) const
{
	GeVector3d dire = this->direction();
	GePoint3d point = this->startPoint();
	point.x += blendCoeff * dire.x;
	point.y += blendCoeff * dire.y;
	point.z += blendCoeff * dire.z;
	return point;
}


GePoint3d GeLineSeg3d::startPoint() const
{
	return GE_IMP_LINESEG3D(this->m_pImpl)->origin;
}
GePoint3d GeLineSeg3d::midPoint() const
{
	GePoint3d point;
	point.x = GE_IMP_LINESEG3D(this->m_pImpl)->origin.x + GE_IMP_LINESEG3D(this->m_pImpl)->vector.x / 2;
	point.y = GE_IMP_LINESEG3D(this->m_pImpl)->origin.y + GE_IMP_LINESEG3D(this->m_pImpl)->vector.y / 2;
	point.z = GE_IMP_LINESEG3D(this->m_pImpl)->origin.z + GE_IMP_LINESEG3D(this->m_pImpl)->vector.z / 2;
	return point;
}
GePoint3d GeLineSeg3d::endPoint() const
{
	GePoint3d point;
	point.x = GE_IMP_LINESEG3D(this->m_pImpl)->origin.x + GE_IMP_LINESEG3D(this->m_pImpl)->vector.x;
	point.y = GE_IMP_LINESEG3D(this->m_pImpl)->origin.y + GE_IMP_LINESEG3D(this->m_pImpl)->vector.y;
	point.z = GE_IMP_LINESEG3D(this->m_pImpl)->origin.z + GE_IMP_LINESEG3D(this->m_pImpl)->vector.z;
	return point;
}

GeLineSeg3d& GeLineSeg3d::operator =(const GeLineSeg3d& line)
{
	GE_IMP_LINESEG3D(this->m_pImpl)->origin = GE_IMP_LINESEG3D(line.m_pImpl)->origin;
	GE_IMP_LINESEG3D(this->m_pImpl)->vector = GE_IMP_LINESEG3D(line.m_pImpl)->vector;
	return *this;
}

void GeLineSeg3d::getInterval(GeInterval& range) const
{
	range.set(0.0, 1.0);
}

void GeLineSeg3d::getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const
{
	range.set(0.0, 1.0);
	startPoint = this->startPoint();
	endPoint = this->endPoint();
}


bool GeLineSeg3d::isKindOf(Ge::EntityId entType) const {
	return entType == Ge::EntityId::kEntity3d
		|| entType == Ge::EntityId::kCurve3d
		|| entType == Ge::EntityId::kLinearEnt3d
		|| entType == this->type();
}
Ge::EntityId GeLineSeg3d::type() const {
	return Ge::EntityId::kLineSeg3d;
}
GeLineSeg3d* GeLineSeg3d::copy() const {
	GeLineSeg3d* pLine = new GeLineSeg3d();
	pLine->set(GE_IMP_LINESEG3D(this->m_pImpl)->origin, GE_IMP_LINESEG3D(this->m_pImpl)->vector);
	return pLine;
}
bool GeLineSeg3d::operator == (const GeLineSeg3d& entity) const {
	return this->isEqualTo(entity);
}
bool GeLineSeg3d::operator != (const GeLineSeg3d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeLineSeg3d::isEqualTo(const GeLineSeg3d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeLineSeg3d::isEqualTo(const GeLineSeg3d& entity, const GeTol& tol) const {
	if (this->startPoint().isEqualTo(entity.startPoint(), tol) && this->endPoint().isEqualTo(entity.endPoint(), tol)) {
		return true;
	}
	if (this->startPoint().isEqualTo(entity.endPoint(), tol) && this->endPoint().isEqualTo(entity.startPoint(), tol)) {
		return true;
	}
	return false;
}
GeLineSeg3d& GeLineSeg3d::transformBy(const GeMatrix3d& xfm) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(p1, p2);
	return *this;
}
GeLineSeg3d& GeLineSeg3d::translateBy(const GeVector3d& translateVec) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();
	p1 += translateVec;
	p2 += translateVec;
	this->set(p1, p2);
	return *this;
}
GeLineSeg3d& GeLineSeg3d::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeLineSeg3d& GeLineSeg3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();
	p1.rotateBy(angle, vec, wrtPoint);
	p2.rotateBy(angle, vec, wrtPoint);
	this->set(p1, p2);
	return *this;
}
GeLineSeg3d& GeLineSeg3d::mirror(const GePlane& plane) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();
	p1.mirror(plane);
	p2.mirror(plane);
	this->set(p1, p2);
	return *this;
}
GeLineSeg3d& GeLineSeg3d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeLineSeg3d& GeLineSeg3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {
	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();
	p1.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	p2.scaleBy(GeScale3d(scaleFactor, scaleFactor, scaleFactor), wrtPoint);
	this->set(p1, p2);
	return *this;
}
bool GeLineSeg3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeLineSeg3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d start = this->startPoint();
	GeVector3d segment = this->endPoint() - start;
	GePoint3d projected = GeLinearEnt3d::vertical(pnt, *this, tol);
	if (projected.distanceTo(pnt) > tol.equalPoint())
	{
		return false;
	}

	GeVector3d offset = pnt - start;
	double projection = offset.dotProduct(segment);
	if (projection < -tol.equalPoint())
	{
		return false;
	}

	double lengthSqrd = segment.lengthSqrd();
	if (projection > lengthSqrd + tol.equalPoint())
	{
		return false;
	}
	return true;
}



double GeLineSeg3d::length() const {
	return this->startPoint().distanceTo(this->endPoint());
}
double GeLineSeg3d::length(double fromParam, double toParam)const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeLineSeg3d::length(double fromParam, double toParam, double tol)const {
	return (toParam - fromParam) * this->length();
}
double GeLineSeg3d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeLineSeg3d::paramAtLength(double datumParam, double length, double tol) const {
	double param = 0.0;
	double segLen = this->length();
	if (std::fabs(segLen) <= tol)
	{
		return datumParam;
	}

	param = datumParam + length / segLen;

	return param;
}
double GeLineSeg3d::area() const {
	return this->area(GeContext::gTol);
}
double GeLineSeg3d::area(const GeTol& tol) const {
	return 0.0;
}
double GeLineSeg3d::distanceTo(const GePoint3d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeLineSeg3d::distanceTo(const GePoint3d& point, const GeTol& tol) const
{
	GePoint3d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeLineSeg3d::distanceTo(const GeLine3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg3d::distanceTo(const GeLine3d& line, const GeTol& tol) const {
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLineSeg3d::distanceTo(const GeLineSeg3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg3d::distanceTo(const GeLineSeg3d& line, const GeTol& tol) const {
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLineSeg3d::distanceTo(const GeRay3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg3d::distanceTo(const GeRay3d& line, const GeTol& tol) const {
	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLineSeg3d::distanceTo(const GeCircArc3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg3d::distanceTo(const GeCircArc3d& arc, const GeTol& tol) const {

	GePoint3d closest;
	GePoint3d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint3d GeLineSeg3d::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeLineSeg3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d point = GeLineSeg3d::vertical(pnt, *this, tol);
	if (this->isOn(point, tol) == true) {
		return point;
	}
	if (pnt.distanceTo(this->startPoint()) > pnt.distanceTo(this->endPoint())) {
		return this->endPoint();
	}
	return this->startPoint();
}
GePoint3d GeLineSeg3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::closestPointTo(const GeLine3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y, intersect.z);
			closest.set(intersect.x, intersect.y, intersect.z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint3d GeLineSeg3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::closestPointTo(const GeLineSeg3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y, intersect.z);
			closest.set(intersect.x, intersect.y, intersect.z);
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

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint3d GeLineSeg3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::closestPointTo(const GeRay3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d closest;

	do
	{
		GePoint3d intersect;
		if (this->intersectWith(curve3d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y, intersect.z);
			closest.set(intersect.x, intersect.y, intersect.z);
			break;
		}

		GePoint3dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve3d.pointOnLine(), tol));
		pointOthers.append(curve3d.pointOnLine());
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint3d GeLineSeg3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::closestPointTo(const GeCircArc3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve3d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve3d.closestPointTo(this->endPoint(), tol));

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
GePoint3d GeLineSeg3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const {
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const {
	GeLine3d line(pnt, projectDirection);
	GePoint3d pntOnOtherCrv;
	return this->closestPointTo(line, pntOnOtherCrv, tol);
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pointOnThis;
	if (this->projIntersectWith(curve3d, projectDirection, pointOnThis, pntOnOtherCrv, tol) == false) {
		return GePoint3d::kOrigin;
	}
	return pointOnThis;
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pointOnThis;
	if (this->projIntersectWith(curve3d, projectDirection, pointOnThis, pntOnOtherCrv, tol) == false) {
		return GePoint3d::kOrigin;
	}
	return pointOnThis;
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pointOnThis;
	if (this->projIntersectWith(curve3d, projectDirection, pointOnThis, pntOnOtherCrv, tol) == false) {
		return GePoint3d::kOrigin;
	}
	return pointOnThis;
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeLineSeg3d::projClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
void GeLineSeg3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const {
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeLineSeg3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {
	GePoint3d point = this->projClosestPointTo(pnt, projectDirection, tol);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(point, tol));
}
void GeLineSeg3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLineSeg3d::getProjClosestPointTo(const GeLine3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
void GeLineSeg3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLineSeg3d::getProjClosestPointTo(const GeLineSeg3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
void GeLineSeg3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLineSeg3d::getProjClosestPointTo(const GeRay3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}
void GeLineSeg3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeLineSeg3d::getProjClosestPointTo(const GeCircArc3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}
bool GeLineSeg3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeLineSeg3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {

	GePoint3d vertical = GeLineSeg3d::vertical(pnt, *this);
	if (this->isOn(vertical, tol) == false) {
		return false;
	}
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(vertical, tol));
	return true;
}
GeLineSeg3d* GeLineSeg3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const {
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeLineSeg3d* GeLineSeg3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const {

	GePoint3d p1 = this->pointOnLine();
	GePoint3d p2 = p1 + this->direction() * this->length();

	p1.project(projectionPlane, projectDirection);
	p2.project(projectionPlane, projectDirection);

	GeLineSeg3d *line = new GeLineSeg3d();
	line->set(p1, p2 - p1);
	return line;
}
GeLineSeg3d* GeLineSeg3d::orthoProject(const GePlane& projectionPlane) const {
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeLineSeg3d* GeLineSeg3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const {
	return this->project(projectionPlane, projectionPlane.normal(), tol);
}
bool GeLineSeg3d::isOn(const GePoint3d& pnt, double& param) const {
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeLineSeg3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const {
	param = this->paramOf(pnt, tol);
	return this->isOn(pnt, tol);
}
bool GeLineSeg3d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeLineSeg3d::isOn(double param, const GeTol& tol) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);

	return this->isOn(pointOnCurve.point(), tol);
}
GECURVE3D_METHODS_GETCLOSESTPOINTTO(GeLineSeg3d);
double GeLineSeg3d::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeLineSeg3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const {
	if (this->isOn(pnt, tol) == false) {
		return 0.0;
	}

	GeVector3d axis = GE_IMP_LINESEG3D(this->m_pImpl)->vector;
	double lengthSqrd = axis.lengthSqrd();
	if (lengthSqrd <= tol.equalPoint() * tol.equalPoint()) {
		return 0.0;
	}

	GeVector3d offset = pnt - this->startPoint();
	return offset.dotProduct(axis) / lengthSqrd;
}
void GeLineSeg3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeLineSeg3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeLineSeg3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {
	
	if (std::fabs(distance) <= tol.equalPoint())
	{
		GeLineSeg3d* line = new GeLineSeg3d();
		line->set(GE_IMP_LINEARENT3D(this->m_pImpl)->origin, GE_IMP_LINEARENT3D(this->m_pImpl)->vector);
		offsetCurveList.append(line);
		return;
	}

	GeVector3d axis = this->direction();
	axis.rotateBy(PI / 2.0, planeNormal);
	axis.normalize();

	GePoint3d p1 = this->startPoint();
	GePoint3d p2 = this->endPoint();
	p1 = p1 + axis * distance;
	p2 = p2 + axis * distance;

	GeLineSeg3d* line = new GeLineSeg3d(p1, p2);

	offsetCurveList.append(line);
}
bool GeLineSeg3d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeLineSeg3d::isClosed(const GeTol& tol) const {
	return false;
}
bool GeLineSeg3d::isPlanar(GePlane& plane) const {
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeLineSeg3d::isPlanar(GePlane& plane, const GeTol& tol) const {
	GeVector3d direction = this->direction();
	if (direction.length() <= tol.equalVector())
	{
		plane.set(this->startPoint(), GeVector3d::kZAxis);
		return true;
	}

	GeVector3d refAxis = GeVector3d::kZAxis;
	if (direction.isParallelTo(refAxis, tol) == true)
	{
		refAxis = GeVector3d::kXAxis;
	}

	GeVector3d normal = direction.crossProduct(refAxis);
	if (normal.length() <= tol.equalVector())
	{
		refAxis = GeVector3d::kYAxis;
		normal = direction.crossProduct(refAxis);
	}
	if (normal.length() <= tol.equalVector())
	{
		plane.set(this->startPoint(), GeVector3d::kZAxis);
		return true;
	}

	plane.set(this->startPoint(), normal.normal());
	return true;
}
bool GeLineSeg3d::isLinear(GeLine3d& line) const {
	return this->isLinear(line, GeContext::gTol);
}
bool GeLineSeg3d::isLinear(GeLine3d& line, const GeTol& tol) const {
	line.set(this->pointOnLine(), this->direction());
	return true;
}
bool GeLineSeg3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLineSeg3d::isCoplanarWith(const GeLine3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeLineSeg3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLineSeg3d::isCoplanarWith(const GeLineSeg3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeLineSeg3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLineSeg3d::isCoplanarWith(const GeRay3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1, line2;
	line1.set(this->pointOnLine(), this->direction());
	line2.set(curve3d.pointOnLine(), curve3d.direction());
	return line1.isCoplanarWith(line2, plane, tol);
}
bool GeLineSeg3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeLineSeg3d::isCoplanarWith(const GeCircArc3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GeLine3d line1;
	line1.set(this->pointOnLine(), this->direction());
	return line1.isCoplanarWith(curve3d, plane, tol);
}
void GeLineSeg3d::getSplitCurves(double param, GeCurve3d*& piece1, GeCurve3d*& piece2) const {
	piece1 = NULL;
	piece2 = NULL;
	GePointOnCurve3d pointOnCurve(*this, param);
	GePoint3d point = pointOnCurve.point();
	if (this->isOn(point) == false) {
		return;
	}
	piece1 = new GeLineSeg3d(this->startPoint(), point);
	piece2 = new GeLineSeg3d(point, this->endPoint());
}
bool GeLineSeg3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	GeLineSeg3d* line = new GeLineSeg3d(*this);
	explodedCurves.append(line);
	newExplodedCurve.append(1);
	return true;
}
GeBoundBlock3d GeLineSeg3d::boundBlock() const {
	GeInterval range;
	this->getInterval(range);
	return this->boundBlock(range);
}
GeBoundBlock3d GeLineSeg3d::boundBlock(const GeInterval& range) const {

	GeBoundBlock3d boundBlock;

	GePoint3d basePoint;
	basePoint = this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound();

	GePoint3d endPoint;
	endPoint = this->pointOnLine() + (this->direction() * this->length()) * range.upperBound();

	boundBlock.set(basePoint, endPoint);

	return boundBlock;
}
GeBoundBlock3d GeLineSeg3d::orthoBoundBlock() const {
	GeInterval range;
	this->getInterval(range);
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeLineSeg3d::orthoBoundBlock(const GeInterval& range) const {

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
		if (points[i].z < minPoint.z) {
			minPoint.z = points[i].z;
		}
		if (points[i].z > maxPoint.z) {
			maxPoint.z = points[i].z;
		}
	}

	boundBlock.set(minPoint, maxPoint);

	return boundBlock;
}
bool GeLineSeg3d::hasStartPoint(GePoint3d& startPoint) const {
	GeInterval range;
	GePoint3d endPoint;
	this->getInterval(range, startPoint, endPoint);
	return range.isBoundedBelow();
}
bool GeLineSeg3d::hasEndPoint(GePoint3d& endPoint) const {
	GeInterval range;
	GePoint3d startPoint;
	this->getInterval(range, startPoint, endPoint);
	return range.isBoundedAbove();
}
GePoint3d GeLineSeg3d::evalPoint(double param) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}
void GeLineSeg3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const {
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray, false);
}
void GeLineSeg3d::getSamplePoints(double fromParam, double toParam, double approxEps, GePoint3dArray& pointArray, GeDoubleArray& paramArray, bool forceResampling) const {

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
void GeLineSeg3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const {
	GeDoubleArray paramArray;
	return this->getSamplePoints(numSample, pointArray, paramArray);
}
void GeLineSeg3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const {
	for (int i = 0; i < numSample; i++) {
		double param = 1.0 / numSample * i;
		pointArray.append(this->evalPoint(param));
		paramArray.append(param);
	}
}



bool GeLineSeg3d::intersectWith(const GeLine3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLineSeg3d::intersectWith(const GeLine3d& line, GePoint3d& intPnt, const GeTol& tol) const {
	if (this->length() <= tol.equalPoint()) {
		GePoint3d p = this->startPoint();
		if (line.isOn(p, tol) == true) {
			intPnt = p;
			return true;
		}
		return false;
	}

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
bool GeLineSeg3d::intersectWith(const GeRay3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLineSeg3d::intersectWith(const GeRay3d& line, GePoint3d& intPnt, const GeTol& tol) const {
	if (this->length() <= tol.equalPoint()) {
		GePoint3d p = this->startPoint();
		if (line.isOn(p, tol) == true) {
			intPnt = p;
			return true;
		}
		return false;
	}
	if (line.length() <= tol.equalPoint()) {
		GePoint3d p = line.pointOnLine();
		if (this->isOn(p, tol) == true) {
			intPnt = p;
			return true;
		}
		return false;
	}
	
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
bool GeLineSeg3d::intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLineSeg3d::intersectWith(const GeLineSeg3d& line, GePoint3d& intPnt, const GeTol& tol) const {
	if (this->length() <= tol.equalPoint()) {
		GePoint3d p = this->startPoint();
		if (line.isOn(p, tol) == true) {
			intPnt = p;
			return true;
		}
		return false;
	}
	if (line.length() <= tol.equalPoint()) {
		GePoint3d p = line.startPoint();
		if (this->isOn(p, tol) == true) {
			intPnt = p;
			return true;
		}
		return false;
	}

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
