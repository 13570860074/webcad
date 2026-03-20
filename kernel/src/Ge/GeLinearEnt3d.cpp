#include "GeLinearEnt3d.h"
#include "GeLine3d.h"
#include "GeMatrix3d.h"
#include "GePlane.h"
#include "GePlanarEnt.h"
#include "GeImpl.h"
#include <cmath>




GeLinearEnt3d::GeLinearEnt3d()
{

}
GeLinearEnt3d::GeLinearEnt3d(const GeLinearEnt3d& source)
{

}

GePoint3d GeLinearEnt3d::vertical(const GePoint3d& pt, const GeLinearEnt3d& line) {
	return vertical(pt, line, GeContext::gTol);
}
GePoint3d GeLinearEnt3d::vertical(const GePoint3d& pt, const GeLinearEnt3d& line, const GeTol& tol)
{

	GeVector3d vec = pt - GE_IMP_LINEARENT3D(line.m_pImpl)->origin;
	if (vec.length() < tol.equalPoint())
	{
		return pt;
	}
	vec.normalize();
	GeVector3d bb = GE_IMP_LINEARENT3D(line.m_pImpl)->vector.normal();
	double angle = vec.angle(GE_IMP_LINEARENT3D(line.m_pImpl)->vector.normal());
	if (std::fabs(angle - PI) < tol.equalPoint() || std::fabs(angle) < tol.equalPoint()) {
		return pt;
	}
	if (std::fabs(angle - PI / 2) < tol.equalPoint())
	{
		return GE_IMP_LINEARENT3D(line.m_pImpl)->origin;
	}

	double adjacent = pt.distanceTo(GE_IMP_LINEARENT3D(line.m_pImpl)->origin) * cos(angle);

	GePoint3d p1 = GePoint3d(GE_IMP_LINEARENT3D(line.m_pImpl)->origin);
	p1.translation(line.direction() * adjacent);

	GePoint3d p2 = GePoint3d(GE_IMP_LINEARENT3D(line.m_pImpl)->origin);
	p2.translation(line.direction() * (0 - adjacent));

	if (p1.distanceTo(pt) < p2.distanceTo(pt))
	{
		return p1;
	}
	return p2;
}
bool GeLinearEnt3d::intersectWith(const GeLinearEnt3d& line, GePoint3d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLinearEnt3d::intersectWith(const GeLinearEnt3d& line, GePoint3d& intPnt, const GeTol& tol) const
{
	GeLine3d thisLine;
	GeLine3d otherLine;
	this->getLine(thisLine);
	line.getLine(otherLine);
	if (thisLine.intersectWith(otherLine, intPnt, tol) == false)
	{
		return false;
	}
	return this->isOn(intPnt, tol) && line.isOn(intPnt, tol);
}
bool GeLinearEnt3d::intersectWith(const GePlanarEnt& plane, GePoint3d& intPnt) const {
	return this->intersectWith(plane, intPnt, GeContext::gTol);
}
bool GeLinearEnt3d::intersectWith(const GePlanarEnt& plane, GePoint3d& intPnt, const GeTol& tol) const {
	return plane.intersectWith(*this, intPnt, tol);
}

bool GeLinearEnt3d::projIntersectWith(const GeLinearEnt3d& line, const GeVector3d& projDir, GePoint3d& pntOnThisLine, GePoint3d& pntOnOtherLine) const {
	return this->projIntersectWith(line, projDir, pntOnThisLine, pntOnOtherLine, GeContext::gTol);
}
bool GeLinearEnt3d::projIntersectWith(const GeLinearEnt3d& line, const GeVector3d& projDir, GePoint3d& pntOnThisLine, GePoint3d& pntOnOtherLine, const GeTol& tol) const {
	if (this->intersectWith(line, pntOnThisLine, tol) == true)
	{
		pntOnOtherLine = pntOnThisLine;
		return true;
	}

	if (projDir.isZeroLength(tol) == true)
	{
		return false;
	}

	GeVector3d thisDir = this->direction();
	if (thisDir.isParallelTo(projDir, tol) == true)
	{
		return false;
	}

	GeVector3d planeNormal = thisDir.crossProduct(projDir);
	if (planeNormal.isZeroLength(tol) == true)
	{
		return false;
	}

	GePlane projectionPlane(this->pointOnLine(), planeNormal);
	if (projectionPlane.intersectWith(line, pntOnOtherLine, tol) == false)
	{
		return false;
	}

	GeLine3d thisLine;
	this->getLine(thisLine);
	GeLine3d projectionLine(pntOnOtherLine, projDir);
	if (thisLine.intersectWith(projectionLine, pntOnThisLine, tol) == false)
	{
		return false;
	}

	return this->isOn(pntOnThisLine, tol) && line.isOn(pntOnOtherLine, tol);
}

bool GeLinearEnt3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeLinearEnt3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	GePoint3d projected = GeLinearEnt3d::vertical(pnt, *this, tol);
	return projected.distanceTo(pnt) <= tol.equalPoint();
}
bool GeLinearEnt3d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeLinearEnt3d::isOn(double param, const GeTol& tol) const {
	return true;
}
bool GeLinearEnt3d::isOn(const GePlane& plane) const {
	return this->isOn(plane, GeContext::gTol);
}
bool GeLinearEnt3d::isOn(const GePlane& plane, const GeTol& tol) const {
	return plane.isOn(this->pointOnLine(), tol) && this->isParallelTo(plane, tol);
}

bool GeLinearEnt3d::isParallelTo(const GeLinearEnt3d& line) const {
	return this->isParallelTo(line, GeContext::gTol);
}
bool GeLinearEnt3d::isParallelTo(const GeLinearEnt3d& line, const GeTol& tol) const
{
	return GE_IMP_LINEARENT3D(this->m_pImpl)->vector.isParallelTo(GE_IMP_LINEARENT3D(line.m_pImpl)->vector, tol);
}
bool GeLinearEnt3d::isParallelTo(const GePlanarEnt& plane) const {
	return this->isParallelTo(plane, GeContext::gTol);
}
bool GeLinearEnt3d::isParallelTo(const GePlanarEnt& plane, const GeTol& tol) const {
	return this->direction().isPerpendicularTo(plane.normal(), tol);
}

bool GeLinearEnt3d::isPerpendicularTo(const GeLinearEnt3d& line) const {
	return this->isPerpendicularTo(line, GeContext::gTol);
}
bool GeLinearEnt3d::isPerpendicularTo(const GeLinearEnt3d& line, const GeTol& tol) const
{
	double angle = GE_IMP_LINEARENT3D(this->m_pImpl)->vector.angleTo(GE_IMP_LINEARENT3D(line.m_pImpl)->vector);
	if (std::fabs(angle - PI / 2) < tol.equalPoint() || std::fabs(angle - PI / 2 * 2) < tol.equalPoint())
	{
		return true;
	}
	return false;
}
bool GeLinearEnt3d::isPerpendicularTo(const GePlanarEnt& plane) const {
	return this->isPerpendicularTo(plane, GeContext::gTol);
}
bool GeLinearEnt3d::isPerpendicularTo(const GePlanarEnt& plane, const GeTol& tol) const {
	return this->direction().isParallelTo(plane.normal(), tol);
}

bool GeLinearEnt3d::isColinearTo(const GeLinearEnt3d& line) const {
	return this->isColinearTo(line, GeContext::gTol);
}
bool GeLinearEnt3d::isColinearTo(const GeLinearEnt3d& line, const GeTol& tol) const
{
	if (this->isParallelTo(line, tol) == false)
	{
		return false;
	}
	return this->isOn(line.pointOnLine(), tol);
}
void GeLinearEnt3d::getPerpPlane(const GePoint3d& pnt, GePlane& plane) const
{
	plane.set(pnt, this->direction());
}
GePoint3d GeLinearEnt3d::pointOnLine() const
{
	return GE_IMP_LINEARENT3D(this->m_pImpl)->origin;
}
GeVector3d GeLinearEnt3d::direction() const
{
	return GE_IMP_LINEARENT3D(this->m_pImpl)->vector.normal();
}
void GeLinearEnt3d::getLine(GeLine3d& line) const
{
	line.set(GE_IMP_LINEARENT3D(this->m_pImpl)->origin, GE_IMP_LINEARENT3D(this->m_pImpl)->vector);
}
GeLinearEnt3d& GeLinearEnt3d::operator =(const GeLinearEnt3d& line)
{
	GE_IMP_LINEARENT3D(this->m_pImpl)->origin.set(GE_IMP_LINEARENT3D(line.m_pImpl)->origin.x, GE_IMP_LINEARENT3D(line.m_pImpl)->origin.y, GE_IMP_LINEARENT3D(line.m_pImpl)->origin.z);
	GE_IMP_LINEARENT3D(this->m_pImpl)->vector.set(GE_IMP_LINEARENT3D(line.m_pImpl)->vector.x, GE_IMP_LINEARENT3D(line.m_pImpl)->vector.y, GE_IMP_LINEARENT3D(line.m_pImpl)->vector.z);
	return *this;
}
