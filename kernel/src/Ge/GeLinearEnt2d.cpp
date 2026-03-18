#include "GeLinearEnt2d.h"
#include "GeLine2d.h"
#include "GeMatrix2d.h"
#include "GeImpl.h"




GeLinearEnt2d::GeLinearEnt2d()
{

}
GeLinearEnt2d::GeLinearEnt2d(const GeLinearEnt2d& source)
{

}

GePoint2d GeLinearEnt2d::vertical(const GePoint2d& pt, const GeLinearEnt2d& line) {
	return vertical(pt, line, GeContext::gTol);
}
GePoint2d GeLinearEnt2d::vertical(const GePoint2d& pt, const GeLinearEnt2d& line, const GeTol& tol)
{
	GePoint2d retVal;

	GePoint2d begin = GE_IMP_LINEARENT2D(line.m_pImpl)->origin;
	GePoint2d end = begin + GE_IMP_LINEARENT2D(line.m_pImpl)->vector;

	double dx = begin.x - end.x;
	double dy = begin.y - end.y;
	if (abs(dx) < tol.equalPoint() && abs(dy) < tol.equalPoint())
	{
		retVal = begin;
		return retVal;
	}

	double u = (pt.x - begin.x) * (begin.x - end.x) + (pt.y - begin.y) * (begin.y - end.y);
	u = u / ((dx * dx) + (dy * dy));

	retVal.x = begin.x + u * dx;
	retVal.y = begin.y + u * dy;

	return retVal;



	//GeVector2d vec = pt - GE_IMP_LINEARENT2D(line.m_pImpl)->origin;
	//if (vec.length() < tol.equalPoint())
	//{
	//	return pt;
	//}

	//vec.normalize();

	//double angle = vec.angleTo(GE_IMP_LINEARENT2D(line.m_pImpl)->vector);
	//if (abs(angle - PI) < tol.equalPoint() || abs(angle) < tol.equalPoint()) {
	//	return pt;
	//}
	//if (abs(angle - PI / 2) < tol.equalPoint())
	//{
	//	return GE_IMP_LINEARENT2D(line.m_pImpl)->origin;
	//}

	//double adjacent = pt.distanceTo(GE_IMP_LINEARENT2D(line.m_pImpl)->origin) * cos(angle);

	//GePoint2d p1 = GePoint2d(GE_IMP_LINEARENT2D(line.m_pImpl)->origin);
	//p1.translation(line.direction() * adjacent);

	//GePoint2d p2 = GePoint2d(GE_IMP_LINEARENT2D(line.m_pImpl)->origin);
	//p2.translation(line.direction() * (0 - adjacent));

	//if (p1.distanceTo(pt) < p2.distanceTo(pt))
	//{
	//	return p1;
	//}
	//return p2;
}
void GeLinearEnt2d::generalFormula(const GeLinearEnt2d& line, double& a, double& b, double& c) {
	GePoint2d p1 = line.pointOnLine();
	GePoint2d p2 = line.pointOnLine() + GE_IMP_LINEARENT2D(line.m_pImpl)->vector;
	a = p2.y - p1.y;
	b = p1.x - p2.x;
	c = p2.x * p1.y - p1.x * p2.y;
}
bool GeLinearEnt2d::intersectWith(const GeLinearEnt2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLinearEnt2d::intersectWith(const GeLinearEnt2d& line, GePoint2d& intPnt, const GeTol& tol) const
{
	GePoint2d thisOrigin = GE_IMP_LINEARENT2D(this->m_pImpl)->origin;
	GePoint2d otherOrigin = GE_IMP_LINEARENT2D(line.m_pImpl)->origin;
	GeVector2d thisVector = GE_IMP_LINEARENT2D(this->m_pImpl)->vector;
	GeVector2d otherVector = GE_IMP_LINEARENT2D(line.m_pImpl)->vector;

	double denominator = thisVector.crossProduct(otherVector);
	if (abs(denominator) <= tol.equalVector())
	{
		return false;
	}

	GeVector2d delta = otherOrigin - thisOrigin;
	double thisParam = delta.crossProduct(otherVector) / denominator;
	GePoint2d point = thisOrigin + thisVector * thisParam;
	if (this->isOn(point, tol) == false || line.isOn(point, tol) == false)
	{
		return false;
	}

	intPnt = point;
	return true;
}
bool GeLinearEnt2d::isParallelTo(const GeLinearEnt2d& line) const {
	return this->isParallelTo(line, GeContext::gTol);
}
bool GeLinearEnt2d::isParallelTo(const GeLinearEnt2d& line, const GeTol& tol) const
{
	return GE_IMP_LINEARENT2D(this->m_pImpl)->vector.isParallelTo(GE_IMP_LINEARENT2D(line.m_pImpl)->vector, tol);
}
bool GeLinearEnt2d::isPerpendicularTo(const GeLinearEnt2d& line) const {
	return this->isPerpendicularTo(line, GeContext::gTol);
}
bool GeLinearEnt2d::isPerpendicularTo(const GeLinearEnt2d& line, const GeTol& tol) const
{
	return GE_IMP_LINEARENT2D(this->m_pImpl)->vector.isPerpendicularTo(GE_IMP_LINEARENT2D(line.m_pImpl)->vector, tol);
}
bool GeLinearEnt2d::isColinearTo(const GeLinearEnt2d& line) const {
	return this->isColinearTo(line, GeContext::gTol);
}
bool GeLinearEnt2d::isColinearTo(const GeLinearEnt2d& line, const GeTol& tol) const
{
	if (this->isParallelTo(line, tol) == false)
	{
		return false;
	}
	return this->isOn(line.pointOnLine(), tol);
}
void GeLinearEnt2d::getPerpLine(const GePoint2d& point, GeLine2d& perpLine) const
{
	perpLine.set(point, this->direction().perpVector());
}
GePoint2d GeLinearEnt2d::pointOnLine() const
{
	return GE_IMP_LINEARENT2D(this->m_pImpl)->origin;
}
GeVector2d GeLinearEnt2d::direction() const
{
	return GE_IMP_LINEARENT2D(this->m_pImpl)->vector.normal();
}
void GeLinearEnt2d::getLine(GeLine2d& line) const
{
	line.set(GE_IMP_LINEARENT2D(this->m_pImpl)->origin, GE_IMP_LINEARENT2D(this->m_pImpl)->vector);
}
GeLinearEnt2d& GeLinearEnt2d::operator =(const GeLinearEnt2d& line)
{
	GE_IMP_LINEARENT2D(this->m_pImpl)->origin.set(GE_IMP_LINEARENT2D(line.m_pImpl)->origin.x, GE_IMP_LINEARENT2D(line.m_pImpl)->origin.y);
	GE_IMP_LINEARENT2D(this->m_pImpl)->vector.set(GE_IMP_LINEARENT2D(line.m_pImpl)->vector.x, GE_IMP_LINEARENT2D(line.m_pImpl)->vector.y);
	return *this;
}
