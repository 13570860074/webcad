#include "GeCurve3d.h"
#include "GePointOnCurve3d.h"
#include "GeBoundBlock3d.h"

GeCurve3d::GeCurve3d()
{

}
GeCurve3d::GeCurve3d(const GeCurve3d& source)
{

}

double GeCurve3d::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeCurve3d::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	return 0;
}
double GeCurve3d::distanceTo(const GeCurve3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCurve3d::distanceTo(const GeCurve3d&, const GeTol& tol) const {
	return 0;
}

GePoint3d GeCurve3d::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeCurve3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
GePoint3d GeCurve3d::closestPointTo(const GeCurve3d& curve2d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCurve3d::closestPointTo(const GeCurve3d& curve2d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}

void GeCurve3d::getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {
	return this->getClosestPointTo(pnt, pntOnCrv, GeContext::gTol);
}
void GeCurve3d::getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {

}
void GeCurve3d::getClosestPointTo(const GeCurve3d& curve2d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCurve3d::getClosestPointTo(const GeCurve3d& curve2d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}

GePoint3d GeCurve3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const {
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeCurve3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const {
	return GePoint3d();
}
GePoint3d GeCurve3d::projClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCurve3d::projClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return GePoint3d();
}

void GeCurve3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const {
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeCurve3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {

}
void GeCurve3d::getProjClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCurve3d::getProjClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {

}

bool GeCurve3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeCurve3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {
	return false;
}

GeBoundBlock3d GeCurve3d::boundBlock() const {
	return GeBoundBlock3d();
}
GeBoundBlock3d GeCurve3d::boundBlock(const GeInterval& range) const {
	return GeBoundBlock3d();
}
GeBoundBlock3d GeCurve3d::orthoBoundBlock() const {
	return GeBoundBlock3d();
}
GeBoundBlock3d GeCurve3d::orthoBoundBlock(const GeInterval& range) const {
	return GeBoundBlock3d();
}

GeEntity3d* GeCurve3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const {
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeEntity3d* GeCurve3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const {
	return NULL;
}
GeEntity3d* GeCurve3d::orthoProject(const GePlane& projectionPlane) const {
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeEntity3d* GeCurve3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const {
	return NULL;
}

bool GeCurve3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	return false;
}
bool GeCurve3d::isOn(const GePoint3d& pnt, double& param) const {
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeCurve3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const {
	return false;
}
bool GeCurve3d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeCurve3d::isOn(double param, const GeTol& tol) const {
	return false;
}

double GeCurve3d::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCurve3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const {
	return 0.0;
}

void GeCurve3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kFillet, GeContext::gTol);
}
void GeCurve3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeCurve3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

}

bool GeCurve3d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeCurve3d::isClosed(const GeTol& tol) const {
	return false;
}
bool GeCurve3d::isPlanar(GePlane& plane) const {
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeCurve3d::isPlanar(GePlane& plane, const GeTol& tol) const {
	return false;
}
bool GeCurve3d::isLinear(GeLine3d& line) const {
	return this->isLinear(line, GeContext::gTol);
}
bool GeCurve3d::isLinear(GeLine3d& line, const GeTol& tol) const {
	return false;
}
bool GeCurve3d::isCoplanarWith(const GeCurve3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCurve3d::isCoplanarWith(const GeCurve3d& curve3d, GePlane& plane, const GeTol& tol) const {
	return false;
}


double GeCurve3d::length() const {
	return 0.0;
}
double GeCurve3d::length(double fromParam, double toParam) const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeCurve3d::length(double fromParam, double toParam, double tol) const {
	return 0.0;
}
double GeCurve3d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeCurve3d::paramAtLength(double datumParam, double length, double tol) const {
	return 0.0;
}
double GeCurve3d::area() const {
	return this->area(GeContext::gTol);
}
double GeCurve3d::area(const GeTol& tol) const {
	return 0.0;
}

void GeCurve3d::getSplitCurves(double param, GeCurve3d* piece1, GeCurve3d* piece2) const {
	return;
}

bool GeCurve3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	return false;
}

bool GeCurve3d::hasStartPoint(GePoint3d& startPoint) const {
	return false;
}
bool GeCurve3d::hasEndPoint(GePoint3d& endPoint) const {
	return false;
}

GePoint3d GeCurve3d::evalPoint(double param) const {
	GePointOnCurve3d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}

GeCurve3d& GeCurve3d::operator =  (const GeCurve3d& curve) {
	return *this;
}
void GeCurve3d::getSamplePoints(double fromParam,
	double toParam,
	double approxEps,
	GePoint3dArray& pointArray,
	GeDoubleArray& paramArray) const {
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray, false);
}
void GeCurve3d::getSamplePoints(double fromParam,
	double toParam,
	double approxEps,
	GePoint3dArray& pointArray,
	GeDoubleArray& paramArray,
	bool forceResampling) const {

}
void GeCurve3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const {

}
void GeCurve3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const {

}