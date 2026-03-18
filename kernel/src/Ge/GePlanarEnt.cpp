#include "GePlanarEnt.h"
#include "GeMatrix3d.h"
#include "GeImpl.h"



GePlanarEnt::GePlanarEnt() {

}
GePlanarEnt::GePlanarEnt(const GePlanarEnt& _plane) {
	GE_IMP_PLANARENT(this->m_pImpl)->origin = GE_IMP_PLANARENT(_plane.m_pImpl)->origin;
	GE_IMP_PLANARENT(this->m_pImpl)->normal = GE_IMP_PLANARENT(_plane.m_pImpl)->normal;
	GE_IMP_PLANARENT(this->m_pImpl)->xAxis = GE_IMP_PLANARENT(_plane.m_pImpl)->xAxis;
	GE_IMP_PLANARENT(this->m_pImpl)->yAxis = GE_IMP_PLANARENT(_plane.m_pImpl)->yAxis;
}
bool GePlanarEnt::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& pnt) const {
	return this->intersectWith(linEnt, pnt, GeContext::gTol);
}
bool GePlanarEnt::intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& pnt, const GeTol& tol) const {
	return false;
}
GePoint3d GePlanarEnt::closestPointToLinearEnt(const GeLinearEnt3d& line, GePoint3d& pointOnLine) const {
	return this->closestPointToLinearEnt(line, pointOnLine, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointToLinearEnt(const GeLinearEnt3d& line, GePoint3d& pointOnLine, const GeTol& tol) const {
	return GePoint3d();
}
GePoint3d GePlanarEnt::closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln) const {
	return this->closestPointToPlanarEnt(otherPln, pointOnOtherPln, GeContext::gTol);
}
GePoint3d GePlanarEnt::closestPointToPlanarEnt(const GePlanarEnt& otherPln, GePoint3d& pointOnOtherPln, const GeTol& tol) const {
	return GePoint3d();
}
bool GePlanarEnt::isParallelTo(const GeLinearEnt3d& linEnt) const {
	return this->isParallelTo(linEnt, GeContext::gTol);
}
bool GePlanarEnt::isParallelTo(const GeLinearEnt3d& linEnt, const GeTol& tol) const {
	return false;
}
bool GePlanarEnt::isParallelTo(const GePlanarEnt& otherPlnEnt) const {
	return this->isParallelTo(otherPlnEnt, GeContext::gTol);
}
bool GePlanarEnt::isParallelTo(const GePlanarEnt& otherPlnEnt, const GeTol& tol) const {
	return false;
}
bool GePlanarEnt::isPerpendicularTo(const GeLinearEnt3d& linEnt) const {
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlanarEnt::isPerpendicularTo(const GeLinearEnt3d& linEnt, const GeTol& tol) const {
	return false;
}
bool GePlanarEnt::isPerpendicularTo(const GePlanarEnt& linEnt) const {
	return this->isPerpendicularTo(linEnt, GeContext::gTol);
}
bool GePlanarEnt::isPerpendicularTo(const GePlanarEnt& linEnt, const GeTol& tol) const {
	return false;
}
bool GePlanarEnt::isCoplanarTo(const GePlanarEnt& otherPlnEnt) const {
	return this->isCoplanarTo(otherPlnEnt, GeContext::gTol);
}
bool GePlanarEnt::isCoplanarTo(const GePlanarEnt& otherPlnEnt, const GeTol& tol) const {
	return false;
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
	return *this;
}



