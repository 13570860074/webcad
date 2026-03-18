#include "GeLinearEnt3d.h"
#include "GeLine3d.h"
#include "GeMatrix3d.h"
#include "GeImpl.h"




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
	if (abs(angle - PI) < tol.equalPoint() || abs(angle) < tol.equalPoint()) {
		return pt;
	}
	if (abs(angle - PI / 2) < tol.equalPoint())
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
	return false;
}
bool GeLinearEnt3d::intersectWith(const GePlanarEnt& plane, GePoint3d& intPnt) const {
	return this->intersectWith(plane, intPnt, GeContext::gTol);
}
bool GeLinearEnt3d::intersectWith(const GePlanarEnt& plane, GePoint3d& intPnt, const GeTol& tol) const {
	return false;
}

bool GeLinearEnt3d::projIntersectWith(const GeLinearEnt3d& line, const GeVector3d& projDir, GePoint3d& pntOnThisLine, GePoint3d& pntOnOtherLine) const {
	return this->projIntersectWith(line, projDir, pntOnThisLine, pntOnOtherLine, GeContext::gTol);
}
bool GeLinearEnt3d::projIntersectWith(const GeLinearEnt3d& line, const GeVector3d& projDir, GePoint3d& pntOnThisLine, GePoint3d& pntOnOtherLine, const GeTol& tol) const {
	return false;
}

bool GeLinearEnt3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeLinearEnt3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	return false;
}
bool GeLinearEnt3d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeLinearEnt3d::isOn(double param, const GeTol& tol) const {
	return false;
}
bool GeLinearEnt3d::isOn(const GePlane& plane) const {
	return this->isOn(plane, GeContext::gTol);
}
bool GeLinearEnt3d::isOn(const GePlane& plane, const GeTol& tol) const {
	return false;
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
	return false;
}

bool GeLinearEnt3d::isPerpendicularTo(const GeLinearEnt3d& line) const {
	return this->isPerpendicularTo(line, GeContext::gTol);
}
bool GeLinearEnt3d::isPerpendicularTo(const GeLinearEnt3d& line, const GeTol& tol) const
{
	double angle = GE_IMP_LINEARENT3D(this->m_pImpl)->vector.angleTo(GE_IMP_LINEARENT3D(line.m_pImpl)->vector);
	if (abs(angle - PI / 2) < tol.equalPoint() || abs(angle - PI / 2 * 2) < tol.equalPoint())
	{
		return true;
	}
	return false;
}
bool GeLinearEnt3d::isPerpendicularTo(const GePlanarEnt& plane) const {
	return this->isPerpendicularTo(plane, GeContext::gTol);
}
bool GeLinearEnt3d::isPerpendicularTo(const GePlanarEnt& plane, const GeTol& tol) const {
	return false;
}

bool GeLinearEnt3d::isColinearTo(const GeLinearEnt3d& line) const {
	return this->isColinearTo(line, GeContext::gTol);
}
bool GeLinearEnt3d::isColinearTo(const GeLinearEnt3d& line, const GeTol& tol) const
{
	GeVector3d v1 = GE_IMP_LINEARENT3D(this->m_pImpl)->vector;
	GeVector3d v2 = GE_IMP_LINEARENT3D(line.m_pImpl)->vector;
	v1.normalize();
	v2.normalize();
	if (abs(v1.angleTo(v2)) < tol.equalVector() || abs(v1.angleTo(v2) - PI) < tol.equalVector())
	{
		return false;
	}
	return true;
}
void GeLinearEnt3d::getPerpPlane(const GePoint3d& pnt, GePlane& plane) const
{

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
