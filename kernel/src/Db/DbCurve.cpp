#include "DbCurve.h"
#include "GeDoubleArray.h"
#include "DbImpl.h"


DbCurve::DbCurve()
{

}
DbCurve::~DbCurve()
{

}

bool DbCurve::subWorldDraw(GiWorldDraw* pWd) const {
	return false;
}


bool DbCurve::isClosed() const {
    return false;
}
bool DbCurve::isPeriodic() const {
    return false;
}

// Get planar and start/end geometric properties.
//
Acad::ErrorStatus DbCurve::getStartParam(double&) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getEndParam(double&) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getStartPoint(GePoint3d&) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getEndPoint(GePoint3d&) const {
    return Acad::ErrorStatus::eOk;
}

// Conversions to/from parametric/world/distance.
//
Acad::ErrorStatus DbCurve::getPointAtParam(double, GePoint3d&) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getParamAtPoint(const GePoint3d&, double&)const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getDistAtParam(double param, double& dist) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getParamAtDist(double dist, double& param) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getDistAtPoint(const GePoint3d&, double&)const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getPointAtDist(double, GePoint3d&) const {
    return Acad::ErrorStatus::eOk;
}

// Derivative information.
//
Acad::ErrorStatus DbCurve::getFirstDeriv(double param, GeVector3d& firstDeriv) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getFirstDeriv(const GePoint3d&, GeVector3d& firstDeriv) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getSecondDeriv(double param, GeVector3d& secDeriv) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getSecondDeriv(const GePoint3d&, GeVector3d& secDeriv) const {
    return Acad::ErrorStatus::eOk;
}

// Closest point on curve.
//
Acad::ErrorStatus DbCurve::getClosestPointTo(const GePoint3d& givenPnt, GePoint3d& pointOnCurve, bool extend) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getClosestPointTo(const GePoint3d& givenPnt, const GeVector3d& direction, GePoint3d& pointOnCurve, bool extend) const {
    return Acad::ErrorStatus::eOk;
}

// Get a projected copy of the curve.
//
Acad::ErrorStatus DbCurve::getOrthoProjectedCurve(const GePlane&, DbCurve*& projCrv) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getProjectedCurve(const GePlane&, const GeVector3d& projDir, DbCurve*& projCrv) const {
    return Acad::ErrorStatus::eOk;
}

// Get offset, spline and split copies of the curve.
//
Acad::ErrorStatus DbCurve::getOffsetCurves(double offsetDist, DbVoidPtrArray& offsetCurves) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getOffsetCurvesGivenPlaneNormal(const GeVector3d& normal, double offsetDist, DbVoidPtrArray& offsetCurves) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getSpline(DbSpline*& spline) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getSplitCurves(const GeDoubleArray& params, DbVoidPtrArray& curveSegments) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::getSplitCurves(const GePoint3dArray& points, DbVoidPtrArray& curveSegments) const {
    return Acad::ErrorStatus::eOk;
}

// Extend the curve.
//
Acad::ErrorStatus DbCurve::extend(double newParam) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::extend(bool extendStart, const GePoint3d& toPoint) {
    return Acad::ErrorStatus::eOk;
}

// Area calculation.
//
Acad::ErrorStatus DbCurve::getArea(double&) const {
    return Acad::ErrorStatus::eOk;
}

// Reverse the curve.
//
Acad::ErrorStatus DbCurve::reverseCurve() {
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbCurve::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const {
    return Acad::ErrorStatus::eOk;
}


Acad::ErrorStatus DbCurve::createFromGeCurve(const GeCurve3d& geCurve, DbCurve*& pDbCurve, GeVector3d* normal, const GeTol& tol) {
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbCurve::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) {
    
    return Acad::ErrorStatus::eOk;
}


Acad::ErrorStatus DbCurve::dwgInFields(DbDwgFiler* pFiler) {
    DbEntity::dwgInFields(pFiler);
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbCurve::dwgOutFields(DbDwgFiler* pFiler) const {
    DbEntity::dwgOutFields(pFiler);
    return Acad::ErrorStatus::eOk;
}


