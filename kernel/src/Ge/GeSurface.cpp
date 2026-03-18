#include "GeSurface.h"
#include "GeInterval.h"
#include "GePointOnSurface.h"


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
	GePoint2d paramPoint;
	return this->isOn(pnt, paramPoint, tol);
}
bool GeSurface::isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const {
	return this->isOn(pnt, paramPoint, GeContext::gTol);
}
bool GeSurface::isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const {
	paramPoint = this->paramOf(pnt, tol);
	return this->evalPoint(paramPoint).isEqualTo(pnt, tol);
}

GePoint3d GeSurface::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeSurface::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return this->evalPoint(this->paramOf(pnt, tol));
}
void GeSurface::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const {
	return this->getClosestPointTo(pnt, result, GeContext::gTol);
}
void GeSurface::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const {
	GePoint3d closest = this->closestPointTo(pnt, tol);
	GePoint2d param = this->paramOf(closest, tol);
	result.setSurface(*this);
	result.setParameter(param);
}
double GeSurface::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeSurface::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	return this->closestPointTo(pnt, tol).distanceTo(pnt);
}
bool GeSurface::isNormalReversed() const {
	return false;
}
bool GeSurface::isLeftHanded() const {
	return false;
}
GeSurface& GeSurface::reverseNormal() {
	return *this;
}
void GeSurface::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const {
	rangeU.set();
	rangeV.set();
}
GeSurface& GeSurface::operator = (const GeSurface& otherSurface) {
	return *this;
}
GePoint3d GeSurface::evalPoint(const GePoint2d& param) const {
	return GePoint3d();
}
