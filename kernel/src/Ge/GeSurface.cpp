#include "GeSurface.h"


GeSurface::GeSurface() {

}
GeSurface::GeSurface(const GeSurface& src) {

}

GePoint2d GeSurface::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
GePoint2d GeSurface::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
	return GePoint2d();
}

bool GeSurface::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	return false;
}
bool GeSurface::isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const {
	return this->isOn(pnt, paramPoint, GeContext::gTol);
}
bool GeSurface::isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const {
	return false;
}

GePoint3d GeSurface::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeSurface::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return GePoint3d();
}
void GeSurface::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const {
	return this->getClosestPointTo(pnt, result, GeContext::gTol);
}
void GeSurface::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const {

}
double GeSurface::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeSurface::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	return false;
}
bool GeSurface::isNormalReversed() const {
	return false;
}
GeSurface& GeSurface::reverseNormal() {
	return *this;
}
GeSurface& GeSurface::operator = (const GeSurface& otherSurface) {
	return *this;
}
GePoint3d GeSurface::evalPoint(const GePoint2d& param) const {
	return GePoint3d();
}
