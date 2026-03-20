#include "GePlanarEnt.h"
#include "GeMatrix3d.h"
#include "GeLine3d.h"
#include "GePlane.h"
#include "GeLinearEnt3d.h"
#include "GeImpl.h"
#include <cmath>


static double planar_signed_distance(const GePoint3d& pointOnPlane, const GeVector3d& unitNormal, const GePoint3d& point)
{
	return unitNormal.dotProduct(point - pointOnPlane);
}

static GePoint3d planar_ortho_project(const GePoint3d& pointOnPlane, const GeVector3d& unitNormal, const GePoint3d& point)
{
	return point + unitNormal * unitNormal.dotProduct(pointOnPlane - point);
}



GePlanarEnt::GePlanarEnt() {

}
GePlanarEnt::GePlanarEnt(const GePlanarEnt& _plane) {
	GE_IMP_PLANARENT(this->m_pImpl)->origin = GE_IMP_PLANARENT(_plane.m_pImpl)->origin;
	GE_IMP_PLANARENT(this->m_pImpl)->normal = GE_IMP_PLANARENT(_plane.m_pImpl)->normal;
	GE_IMP_PLANARENT(this->m_pImpl)->xAxis = GE_IMP_PLANARENT(_plane.m_pImpl)->xAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->yAxis = GE_IMP_PLANARENT(_plane.m_pImpl)->yAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->isNormalReversed = GE_IMP_PLANARENT(_plane.m_pImpl)->isNormalReversed;
}
bool GePlanarEnt::isOnPlane(const GePoint3d& point) const {
	return this->isOnPlane(point, GeContext::gTol);
}
bool GePlanarEnt::isOnPlane(const GePoint3d& point, const GeTol& tol) const {
	return std::fabs(planar_signed_distance(this->pointOnPlane(), this->normal(), point)) <= tol.equalPoint();
}
GePoint2d GePlanarEnt::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
GePoint2d GePlanarEnt::paramOf(const GePoint3d& pnt, const GeTol& tol) const {
	GeVector3d offset = pnt - GE_IMP_PLANARENT(this->m_pImpl)->origin;
	GeVector3d vCrossProdNormal = GE_IMP_PLANARENT(this->m_pImpl)->yAxis.crossProduct(GE_IMP_PLANARENT(this->m_pImpl)->normal);
	GeVector3d normalCrossProdU = GE_IMP_PLANARENT(this->m_pImpl)->normal.crossProduct(GE_IMP_PLANARENT(this->m_pImpl)->xAxis);
	double tripleProduct = vCrossProdNormal.dotProduct(GE_IMP_PLANARENT(this->m_pImpl)->xAxis);
	double tripleTol = tol.equalVector() * tol.equalVector() * tol.equalVector();
	if (tripleTol < 1.0e-300)
	{
		tripleTol = 1.0e-300;
	}
	if (std::fabs(tripleProduct) <= tripleTol)
	{
		return GePoint2d();
	}
	return GePoint2d(
		offset.dotProduct(vCrossProdNormal) / tripleProduct,
		offset.dotProduct(normalCrossProdU) / tripleProduct);
}
bool GePlanarEnt::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GePlanarEnt::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint2d paramPoint;
	return this->isOn(pnt, paramPoint, tol);
}
bool GePlanarEnt::isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const {
	return this->isOn(pnt, paramPoint, GeContext::gTol);
}
bool GePlanarEnt::isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const {
	if (this->isOnPlane(pnt, tol) == false) {
		return false;
	}

	GeInterval rangeU;
	GeInterval rangeV;
	this->getEnvelope(rangeU, rangeV);
	paramPoint = this->paramOf(pnt, tol);
	rangeU.setTolerance(tol.equalPoint());
	rangeV.setTolerance(tol.equalPoint());
	return rangeU.contains(paramPoint.x) && rangeV.contains(paramPoint.y);
}
GePoint3d GePlanarEnt::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return planar_ortho_project(this->pointOnPlane(), this->normal(), pnt);
}
double GePlanarEnt::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GePlanarEnt::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d closest = this->closestPointTo(pnt, tol);
	return closest.distanceTo(pnt);
}
bool GePlanarEnt::isNormalReversed() const {
	return this->isLeftHanded();
}
bool GePlanarEnt::isLeftHanded() const {
	return GE_IMP_PLANARENT(this->m_pImpl)->xAxis.crossProduct(GE_IMP_PLANARENT(this->m_pImpl)->yAxis).dotProduct(GE_IMP_PLANARENT(this->m_pImpl)->normal) < 0.0;
}
GeSurface& GePlanarEnt::reverseNormal() {
	GE_IMP_PLANARENT(this->m_pImpl)->normal *= -1.0;
	return *this;
}
void GePlanarEnt::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const {
	rangeU.set();
	rangeV.set();
}
GePoint3d GePlanarEnt::evalPoint(const GePoint2d& param) const {
	return GE_IMP_PLANARENT(this->m_pImpl)->origin
		+ GE_IMP_PLANARENT(this->m_pImpl)->xAxis * param.x
		+ GE_IMP_PLANARENT(this->m_pImpl)->yAxis * param.y;
}
bool GePlanarEnt::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& pnt) const {
	return this->intersectWith(linEnt, pnt, GeContext::gTol);
}
bool GePlanarEnt::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& pnt, const GeTol& tol) const {
	GePlane plane(this->pointOnPlane(), this->normal());
	return plane.intersectWith(linEnt, pnt, tol);
}
GePoint3d GePlanarEnt::closestPointToLinearEnt(const GeLinearEnt3d& line, GePoint3d& pointOnLine) const {
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointToLinearEnt(const GeLinearEnt3d& line, GePoint3d& pointOnLine, const GeTol& tol) const {
	GePoint3d closest;
	if (this->intersectWith(line, closest, tol))
	{
		pointOnLine = closest;
		return closest;
	}

	GeInterval interval;
	line.getInterval(interval);
	double lowerParam = interval.lowerBound();
	double upperParam = interval.upperBound();
	GePoint3d lowerPoint = line.evalPoint(lowerParam);
	GePoint3d upperPoint = line.evalPoint(upperParam);
	double lowerDist = std::fabs(planar_signed_distance(this->pointOnPlane(), this->normal(), lowerPoint));
	double upperDist = std::fabs(planar_signed_distance(this->pointOnPlane(), this->normal(), upperPoint));
	pointOnLine = (lowerDist < upperDist) ? lowerPoint : upperPoint;
	return planar_ortho_project(this->pointOnPlane(), this->normal(), pointOnLine);
}
GePoint3d GePlanarEnt::closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln) const {
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	GeVector3d normal0 = this->normal();
	GeVector3d normal1 = otherPln.normal();
	double coef0 = 0.0;
	double coef1 = 0.0;
	this->getCoefficients(normal0.x, normal0.y, normal0.z, coef0);
	otherPln.getCoefficients(normal1.x, normal1.y, normal1.z, coef1);
	double n00 = normal0.lengthSqrd();
	double n01 = normal0.dotProduct(normal1);
	double n11 = normal1.lengthSqrd();
	double det = n00 * n11 - n01 * n01;
	if (std::fabs(det) <= tol.equalPoint())
	{
		double envelope1 = std::fabs(planar_signed_distance(this->pointOnPlane(), normal0, otherPln.pointOnPlane()));
		double envelope2 = std::fabs(planar_signed_distance(otherPln.pointOnPlane(), normal1, this->pointOnPlane()));
		if (envelope1 >= envelope2)
		{
			pointOnOtherPln = otherPln.pointOnPlane();
			return planar_ortho_project(this->pointOnPlane(), normal0, pointOnOtherPln);
		}

		pointOnOtherPln = planar_ortho_project(otherPln.pointOnPlane(), normal1, this->pointOnPlane());
		return this->pointOnPlane();
	}

	double invDet = 1.0 / det;
	double c0 = (n11 * coef0 - n01 * coef1) * invDet;
	double c1 = (n00 * coef1 - n01 * coef0) * invDet;
	GeVector3d tmp = normal0 * c0 + normal1 * c1;
	pointOnOtherPln.set(-tmp.x, -tmp.y, -tmp.z);
	return pointOnOtherPln;
}
bool GePlanarEnt::isParallelTo(const GeLinearEnt3d& linEnt) const {
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlanarEnt::isParallelTo(const GeLinearEnt3d& linEnt, const GeTol& tol) const {
	return linEnt.direction().isPerpendicularTo(this->normal(), tol);
}
bool GePlanarEnt::isParallelTo(const GePlanarEnt& otherPlnEnt) const {
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlanarEnt::isParallelTo(const GePlanarEnt& otherPlnEnt, const GeTol& tol) const {
	return this->normal().isParallelTo(otherPlnEnt.normal(), tol);
}
bool GePlanarEnt::isPerpendicularTo(const GeLinearEnt3d& linEnt) const {
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlanarEnt::isPerpendicularTo(const GeLinearEnt3d& linEnt, const GeTol& tol) const {
	return linEnt.direction().isParallelTo(this->normal(), tol);
}
bool GePlanarEnt::isPerpendicularTo(const GePlanarEnt& linEnt) const {
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlanarEnt::isPerpendicularTo(const GePlanarEnt& linEnt, const GeTol& tol) const {
	return this->normal().isPerpendicularTo(linEnt.normal(), tol);
}
bool GePlanarEnt::isCoplanarTo(const GePlanarEnt& otherPlnEnt) const {
	return this->isCoplanarTo(otherPlnEnt, GeContext::gTol);
}
bool GePlanarEnt::isCoplanarTo(const GePlanarEnt& otherPlnEnt, const GeTol& tol) const {
	return this->isParallelTo(otherPlnEnt, tol) && this->pointOnPlane().isEqualTo(otherPlnEnt.pointOnPlane().orthoProject(GePlane(this->pointOnPlane(), this->normal())), tol);
}
void GePlanarEnt::get(GePoint3d& origin, GeVector3d& uVec, GeVector3d& vVec) const {
	origin.set(this->pointOnPlane().x, this->pointOnPlane().y, this->pointOnPlane().z);
	uVec = GE_IMP_PLANARENT(this->m_pImpl)->xAxis;
	vVec = GE_IMP_PLANARENT(this->m_pImpl)->yAxis;
}
void GePlanarEnt::get(GePoint3d& uPnt, GePoint3d& origin, GePoint3d& vPnt) const {
	origin = this->pointOnPlane();
	uPnt = origin + GE_IMP_PLANARENT(this->m_pImpl)->xAxis;
	vPnt = origin + GE_IMP_PLANARENT(this->m_pImpl)->yAxis;
}
GePoint3d GePlanarEnt::pointOnPlane() const {
	return GE_IMP_PLANARENT(this->m_pImpl)->origin;
}
GeVector3d GePlanarEnt::normal() const {
	return GE_IMP_PLANARENT(this->m_pImpl)->normal.normal();
}
void GePlanarEnt::getCoefficients(double& a, double& b, double& c, double& d) const {
	GeVector3d aDir = this->normal();
	a = aDir.x;
	b = aDir.y;
	c = aDir.z;
	GePoint3d point = this->pointOnPlane();
	d = -(a * point.x + b * point.y + c * point.z);
}
void GePlanarEnt::getCoordSystem(GePoint3d& origin, GeVector3d& axis1, GeVector3d& axis2) const {
	origin.set(this->pointOnPlane().x, this->pointOnPlane().y, this->pointOnPlane().z);
	axis1.set(GE_IMP_PLANARENT(this->m_pImpl)->xAxis.x, GE_IMP_PLANARENT(this->m_pImpl)->xAxis.y, GE_IMP_PLANARENT(this->m_pImpl)->xAxis.z);
	axis2.set(GE_IMP_PLANARENT(this->m_pImpl)->yAxis.x, GE_IMP_PLANARENT(this->m_pImpl)->yAxis.y, GE_IMP_PLANARENT(this->m_pImpl)->yAxis.z);
	axis1.normalize();
	axis2.normalize();
}
bool GePlanarEnt::project(const GePoint3d& p, const GeVector3d& unitDir, GePoint3d& projP) const {
	return this->project(p, unitDir, projP, GeContext::gTol);
}
bool GePlanarEnt::project(const GePoint3d& p, const GeVector3d& unitDir, GePoint3d& projP, const GeTol& tol) const {
	double denom = unitDir.dotProduct(this->normal());
	if (std::fabs(denom) < tol.equalVector()) {
		return false;
	}
	double factor = this->normal().dotProduct(this->pointOnPlane() - p) / denom;
	projP = p + unitDir * factor;
	return true;
}
GePlanarEnt& GePlanarEnt::operator = (const GePlanarEnt& src) {
	GE_IMP_PLANARENT(this->m_pImpl)->origin = GE_IMP_PLANARENT(src.m_pImpl)->origin;
	GE_IMP_PLANARENT(this->m_pImpl)->normal = GE_IMP_PLANARENT(src.m_pImpl)->normal;
	GE_IMP_PLANARENT(this->m_pImpl)->xAxis = GE_IMP_PLANARENT(src.m_pImpl)->xAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->yAxis = GE_IMP_PLANARENT(src.m_pImpl)->yAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->isNormalReversed = GE_IMP_PLANARENT(src.m_pImpl)->isNormalReversed;
	return *this;
}



