#include "DbEllipse.h"
#include "GePlane.h"
#include "DbImpl.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "GeEllipArc3d.h"


DbEllipse::DbEllipse() {
	this->m_pImpl = new DbEllipseImpl();
}
DbEllipse::DbEllipse(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio) {
	this->m_pImpl = new DbEllipseImpl();
	this->set(center, unitNormal, majorAxis, radiusRatio);
}
DbEllipse::DbEllipse(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio, double startAngle, double endAngle) {
	this->m_pImpl = new DbEllipseImpl();
	this->set(center, unitNormal, majorAxis, radiusRatio, startAngle, endAngle);
}
DbEllipse::~DbEllipse() {

}




Acad::ErrorStatus DbEllipse::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d center;
	pFiler->readPoint3d(&center);
	this->setCenter(center);

	GeVector3d majorAxis;
	pFiler->readVector3d(&majorAxis);
	DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = majorAxis;

	GeVector3d minorAxis;
	pFiler->readVector3d(&minorAxis);
	DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis = minorAxis;

	double radiusRatio;
	pFiler->readDouble(&radiusRatio);
	DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = radiusRatio;

	double startAngle;
	pFiler->readDouble(&startAngle);
	DB_IMP_ELLIPSE(this->m_pImpl)->startAngle = startAngle;

	double endAngle;
	pFiler->readDouble(&endAngle);
	DB_IMP_ELLIPSE(this->m_pImpl)->endAngle = endAngle;

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	DB_IMP_ELLIPSE(this->m_pImpl)->normal = normal;

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->center());
	pFiler->writeVector3d(DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis);
	pFiler->writeVector3d(DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis);
	pFiler->writeDouble(DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio);
	pFiler->writeDouble(DB_IMP_ELLIPSE(this->m_pImpl)->startAngle);
	pFiler->writeDouble(DB_IMP_ELLIPSE(this->m_pImpl)->endAngle);
	pFiler->writeVector3d(DB_IMP_ELLIPSE(this->m_pImpl)->normal);

	return Acad::ErrorStatus::eOk;
}
bool DbEllipse::subWorldDraw(GiWorldDraw* pWd) const {

	// 构造椭圆弧几何体
	GeVector3d major = this->majorAxis();
	double majorR = this->majorRadius();
	double minorR = this->minorRadius();
	GeVector3d unitNormal = this->normal();

	GeEllipArc3d arc;
	arc.set(this->center(), unitNormal, major, majorR, minorR, this->startAngle(), this->endAngle());

	// 绘制椭圆弧
	pWd->geometry().ellipArc(arc);

	return true;
}



GePoint3d DbEllipse::center() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->center;
}
Acad::ErrorStatus DbEllipse::setCenter(const GePoint3d& center) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ELLIPSE(this->m_pImpl)->center = center;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector3d DbEllipse::normal() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->normal;
}
GeVector3d DbEllipse::majorAxis() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;
}
GeVector3d DbEllipse::minorAxis() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis;
}
double DbEllipse::majorRadius() const {
	return this->majorAxis().length();
}
Acad::ErrorStatus DbEllipse::setMajorRadius(double _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		// 获得短轴长度
		double minorRadius = this->minorRadius();

		// 当长轴大于短轴
		if (_v > minorRadius) {
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}
		// 当短轴大于长轴时将两轴调换
		else {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis *= minorRadius;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbEllipse::minorRadius() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length();
}
Acad::ErrorStatus DbEllipse::setMinorRadius(double _v) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		// 获得长轴长度
		double majorRadius = this->majorRadius();

		// 当短轴小于长轴
		if (_v < majorRadius) {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}
		// 将两轴调换
		else {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= majorRadius;
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis *= _v;
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.length() / DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis.length();
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbEllipse::radiusRatio() const {
	return  DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio;
}
Acad::ErrorStatus DbEllipse::setRadiusRatio(double ratio) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (abs(ratio) > 1) {
			return Acad::ErrorStatus::eFail;
		}

		if (ratio > 0) {
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= (this->majorRadius() * ratio);
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = ratio;
		}
		else {
			GeVector3d v = DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;
			DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis;
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis = v;

			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis.normalize();
			DB_IMP_ELLIPSE(this->m_pImpl)->minorAxis *= (this->majorRadius() * abs(ratio));
			DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = abs(ratio);
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbEllipse::startAngle() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->startAngle;
}
Acad::ErrorStatus DbEllipse::setStartAngle(double startAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ELLIPSE(this->m_pImpl)->startAngle = startAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbEllipse::endAngle() const {
	return DB_IMP_ELLIPSE(this->m_pImpl)->endAngle;
}
Acad::ErrorStatus DbEllipse::setEndAngle(double endAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ELLIPSE(this->m_pImpl)->endAngle = endAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbEllipse::setStartParam(double startParam) {
	this->setStartAngle(startParam);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::setEndParam(double endParam) {
	this->setEndAngle(endParam);
	return Acad::ErrorStatus::eOk;
}

double DbEllipse::paramAtAngle(double angle) const {
	return angle;
}
double DbEllipse::angleAtParam(double param) const {
	return param;
}

Acad::ErrorStatus DbEllipse::get(
	GePoint3d& center,
	GeVector3d& unitNormal,
	GeVector3d& majorAxis, double& radiusRatio, double& startAngle,
	double& endAngle) const {
	center = DB_IMP_ELLIPSE(this->m_pImpl)->center;
	unitNormal = DB_IMP_ELLIPSE(this->m_pImpl)->normal;
	majorAxis = DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;
	radiusRatio = DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio;
	startAngle = DB_IMP_ELLIPSE(this->m_pImpl)->startAngle;
	endAngle = DB_IMP_ELLIPSE(this->m_pImpl)->endAngle;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEllipse::set(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio) {
	return this->set(center, unitNormal, majorAxis, radiusRatio, 0.0, PI * 2);
}
Acad::ErrorStatus DbEllipse::set(
	const GePoint3d& center,
	const GeVector3d& unitNormal,
	const GeVector3d& majorAxis,
	double radiusRatio,
	double startAngle,
	double endAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		if (this->isNull() == true) {
			return Acad::ErrorStatus::eFail;
		}

		DB_IMP_ELLIPSE(this->m_pImpl)->center = center;
		DB_IMP_ELLIPSE(this->m_pImpl)->normal = unitNormal;
		DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis = majorAxis;
		DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio = radiusRatio;
		DB_IMP_ELLIPSE(this->m_pImpl)->startAngle = startAngle;
		DB_IMP_ELLIPSE(this->m_pImpl)->endAngle = endAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Adesk::Boolean DbEllipse::isNull() const {
	if (DB_IMP_ELLIPSE(this->m_pImpl)->normal.isPerpendicularTo(DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis) == false) {
		return true;
	}
	if (DB_IMP_ELLIPSE(this->m_pImpl)->radiusRatio > 1 + GeContext::gTol.equalVector()) {
		return true;
	}
	if (abs(DB_IMP_ELLIPSE(this->m_pImpl)->startAngle - DB_IMP_ELLIPSE(this->m_pImpl)->endAngle) < GeContext::gTol.equalVector()) {
		return true;
	}
	return false;
}


Acad::ErrorStatus DbEllipse::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subTransformBy(const GeMatrix3d& xform)
{



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEllipse::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEllipse::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	return Acad::ErrorStatus::eOk;
}






bool DbEllipse::isClosed() const
{
	if (abs(DB_IMP_ELLIPSE(this->m_pImpl)->endAngle - DB_IMP_ELLIPSE(this->m_pImpl)->startAngle) < 0.00002) {
		return true;
	}
	return false;
}
bool DbEllipse::isPeriodic() const
{
	return false;
}
bool DbEllipse::isPlanar() const
{
	return true;
}
Acad::ErrorStatus DbEllipse::getPlane(GePlane& plane, Db::Planarity& planarity) const
{
	plane.set(this->center(), this->normal());
	planarity = Db::Planarity::kPlanar;
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getStartParam(double& _v) const
{
	_v = this->startAngle();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getEndParam(double& _v) const
{
	_v = this->endAngle();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getStartPoint(GePoint3d& point) const
{
	return this->getPointAtParam(this->startAngle(), point);
}
Acad::ErrorStatus DbEllipse::getEndPoint(GePoint3d& point) const
{
	return this->getPointAtParam(this->endAngle(), point);
}
Acad::ErrorStatus DbEllipse::getPointAtParam(double param, GePoint3d& point) const
{
	GeVector3d refVec = DB_IMP_ELLIPSE(this->m_pImpl)->majorAxis;

	point = this->center() + refVec;
	point.rotateBy(param, this->normal(), this->center());
	GeVector3d v = point - this->center();
	v = v * this->radiusRatio();
	point = v.asPoint();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEllipse::getParamAtPoint(const GePoint3d& pt, double& param) const {

	GeVector3d v = pt - this->center();
	v = v / this->radiusRatio();
	if (abs(v.length() - this->majorRadius() > 0.0002)) {
		return Acad::ErrorStatus::eFail;
	}
	param = this->majorAxis().angleToCCW(v,this->normal());
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getDistAtParam(double param, double& dist) const {

	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getParamAtDist(double, double&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getDistAtPoint(const GePoint3d&, double&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getPointAtDist(double, GePoint3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getFirstDeriv(double, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getFirstDeriv(const GePoint3d&, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSecondDeriv(double, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSecondDeriv(const GePoint3d&, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getClosestPointTo(const GePoint3d&, GePoint3d&, bool) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getClosestPointTo(const GePoint3d&, const GeVector3d&, GePoint3d&, bool) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getOrthoProjectedCurve(const GePlane&, DbCurve*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getProjectedCurve(const GePlane&, const GeVector3d&, DbCurve*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getOffsetCurves(double, DbVoidPtrArray&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSpline(DbSpline*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray& curveSegments) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::extend(double) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::extend(bool, const GePoint3d&) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getArea(double&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::reverseCurve() {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEllipse::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) {
	return Acad::ErrorStatus::eFail;
}



