#include "GePlanarEnt.h"
#include "GeMatrix3d.h"
#include "GeLine3d.h"
#include "GePlane.h"
#include "GeLinearEnt3d.h"
#include "GeImpl.h"



GePlanarEnt::GePlanarEnt() {

}
GePlanarEnt::GePlanarEnt(const GePlanarEnt& _plane) {
	GE_IMP_PLANARENT(this->m_pImpl)->origin = GE_IMP_PLANARENT(_plane.m_pImpl)->origin;
	GE_IMP_PLANARENT(this->m_pImpl)->normal = GE_IMP_PLANARENT(_plane.m_pImpl)->normal;
	GE_IMP_PLANARENT(this->m_pImpl)->xAxis = GE_IMP_PLANARENT(_plane.m_pImpl)->xAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->yAxis = GE_IMP_PLANARENT(_plane.m_pImpl)->yAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->isNormalReversed = GE_IMP_PLANARENT(_plane.m_pImpl)->isNormalReversed;
}
GePoint2d GePlanarEnt::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
GePoint2d GePlanarEnt::paramOf(const GePoint3d& pnt, const GeTol& tol) const {
	GeVector3d offset = pnt - GE_IMP_PLANARENT(this->m_pImpl)->origin;
	GeVector3d vCrossProdNormal = GE_IMP_PLANARENT(this->m_pImpl)->yAxis.crossProduct(GE_IMP_PLANARENT(this->m_pImpl)->normal);
	GeVector3d normalCrossProdU = GE_IMP_PLANARENT(this->m_pImpl)->normal.crossProduct(GE_IMP_PLANARENT(this->m_pImpl)->xAxis);
	double tripleProduct = vCrossProdNormal.dotProduct(GE_IMP_PLANARENT(this->m_pImpl)->xAxis);
	if (fabs(tripleProduct) <= 1.0e-300)
	{
		return GePoint2d();
	}
	return GePoint2d(
		offset.dotProduct(vCrossProdNormal) / tripleProduct,
		offset.dotProduct(normalCrossProdU) / tripleProduct);
}
GePoint3d GePlanarEnt::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return pnt.orthoProject(GePlane(this->pointOnPlane(), this->normal()));
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
	pointOnLine = line.pointOnLine();
	return pointOnLine.orthoProject(GePlane(this->pointOnPlane(), this->normal()));
}
GePoint3d GePlanarEnt::closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln) const {
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	GePlane thisPlane(this->pointOnPlane(), this->normal());
	GePlane otherPlane(otherPln.pointOnPlane(), otherPln.normal());
	GeLine3d intersection;
	if (thisPlane.intersectWith(otherPlane, intersection, tol))
	{
		pointOnOtherPln = intersection.pointOnLine();
		return intersection.pointOnLine();
	}
	pointOnOtherPln = otherPln.pointOnPlane();
	return pointOnOtherPln.orthoProject(thisPlane);
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
GePlanarEnt& GePlanarEnt::operator = (const GePlanarEnt& src) {
	GE_IMP_PLANARENT(this->m_pImpl)->origin = GE_IMP_PLANARENT(src.m_pImpl)->origin;
	GE_IMP_PLANARENT(this->m_pImpl)->normal = GE_IMP_PLANARENT(src.m_pImpl)->normal;
	GE_IMP_PLANARENT(this->m_pImpl)->xAxis = GE_IMP_PLANARENT(src.m_pImpl)->xAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->yAxis = GE_IMP_PLANARENT(src.m_pImpl)->yAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->isNormalReversed = GE_IMP_PLANARENT(src.m_pImpl)->isNormalReversed;
	return *this;
}



