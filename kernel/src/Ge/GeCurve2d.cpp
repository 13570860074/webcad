#include "GeCurve2d.h"
#include "GePointOnCurve2d.h"
#include "GeBoundBlock2d.h"

GeCurve2d::GeCurve2d()
{

}
GeCurve2d::GeCurve2d(const GeCurve2d& source)
{

}

double GeCurve2d::distanceTo(const GePoint2d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeCurve2d::distanceTo(const GePoint2d& pnt, const GeTol& tol) const {
	return 0;
}
double GeCurve2d::distanceTo(const GeCurve2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCurve2d::distanceTo(const GeCurve2d&, const GeTol& tol) const {
	return 0;
}
GePoint2d GeCurve2d::closestPointTo(const GePoint2d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeCurve2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
	return GePoint2d::kOrigin;
}
GePoint2d GeCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint2d::kOrigin;
}
bool GeCurve2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	return false;
}
void GeCurve2d::getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv) const {

}
void GeCurve2d::getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv, const GeTol& tol) const {

}
void GeCurve2d::getClosestPointTo(const GeCurve2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const {

}
void GeCurve2d::getClosestPointTo(const GeCurve2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const {

}
bool GeCurve2d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeCurve2d::isClosed(const GeTol& tol) const {
	return false;
}
double GeCurve2d::paramOf(const GePoint2d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCurve2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {
	return 0.0;
}
void GeCurve2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {

}
void GeCurve2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {

}
void GeCurve2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

}
double GeCurve2d::length() const {
	return 0.0;
}
double GeCurve2d::length(double fromParam, double toParam) const {
	return 0.0;
}
double GeCurve2d::length(double fromParam, double toParam, double tol) const {
	return 0.0;
}
double GeCurve2d::area() const {
	return 0.0;
}
double GeCurve2d::area(const GeTol& tol) const {
	return 0.0;
}
void GeCurve2d::getSplitCurves(double param, GeCurve2d* piece1, GeCurve2d* piece2) const {
	return;
}
bool GeCurve2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	return false;
}
double GeCurve2d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeCurve2d::paramAtLength(double datumParam, double length, double tol) const {
	return 0.0;
}
GeBoundBlock2d GeCurve2d::boundBlock() const {
	return GeBoundBlock2d();
}
GeBoundBlock2d GeCurve2d::boundBlock(const GeInterval& range) const {
	return GeBoundBlock2d();
}
GeBoundBlock2d GeCurve2d::orthoBoundBlock() const {
	return GeBoundBlock2d();
}
GeBoundBlock2d GeCurve2d::orthoBoundBlock(const GeInterval& range) const {
	return GeBoundBlock2d();
}
bool GeCurve2d::hasStartPoint(GePoint2d& startPoint) const {
	return false;
}
bool GeCurve2d::hasEndPoint(GePoint2d& endPoint) const {
	return false;
}
GePoint2d GeCurve2d::evalPoint(double param) const {
	GePointOnCurve2d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}
GeCurve2d& GeCurve2d::operator =  (const GeCurve2d& curve) {
	return *this;
}