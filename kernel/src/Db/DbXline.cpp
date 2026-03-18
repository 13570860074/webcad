#include "DbXline.h"
#include "DbDwgFiler.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbLine.h"
#include "DbRay.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "GeCircArc3d.h"
#include "DbOsnapPointCompute.h"
#include "DbImpl.h"


DbXline::DbXline() {
    this->m_pImpl = new DbXlineImpl();
}
DbXline::~DbXline() {

}






Acad::ErrorStatus DbXline::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    GePoint3d basePoint;
    pFiler->readPoint3d(&basePoint);
    this->setBasePoint(basePoint);

    GeVector3d unitDir;
    pFiler->readVector3d(&unitDir);
    this->setUnitDir(unitDir);

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    pFiler->writePoint3d(this->basePoint());

    pFiler->writeVector3d(this->unitDir());

    return Acad::ErrorStatus::eOk;
}
bool DbXline::subWorldDraw(GiWorldDraw* pWd) const {
	pWd->geometry().xline(this->basePoint(), this->basePoint() + this->unitDir());
    return true;
}



Acad::ErrorStatus DbXline::getOffsetCurvesGivenPlaneNormal(const GeVector3d& normal, double offsetDist, DbVoidPtrArray& offsetCurves) const {
	GeRay3d line(this->basePoint(), this->unitDir());
	GeVoidPointerArray entitys;
	line.getTrimmedOffset(offsetDist, normal, entitys);
	for (int i = entitys.length() - 1; i >= 0; i--)
	{
		GeEntity3d* entity = (GeEntity3d*)entitys[i];
		if (entity->isKindOf(Ge::EntityId::kLine3d) == true)
		{
			DbXline* line = DbXline::cast(this->clone());
			line->setBasePoint(((GeLine3d*)entity)->pointOnLine());
			line->setUnitDir(((GeLine3d*)entity)->direction());
			offsetCurves.append(line);
		}
		else if (entity->isKindOf(Ge::EntityId::kRay3d) == true)
		{
			DbXline* line = DbXline::cast(this->clone());
			line->setBasePoint(((GeRay3d*)entity)->pointOnLine());
			line->setUnitDir(((GeRay3d*)entity)->direction());
			offsetCurves.append(line);
		}
		delete entity;
	}
	return Acad::ErrorStatus::eOk;
}

GePoint3d DbXline::basePoint() const {
    return DB_IMP_XLINE(this->m_pImpl)->basePoint;
}
void DbXline::setBasePoint(const GePoint3d& pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_XLINE(this->m_pImpl)->basePoint = pt;
    }
}

GeVector3d DbXline::unitDir() const {
    return DB_IMP_XLINE(this->m_pImpl)->direction;
}
void DbXline::setUnitDir(const GeVector3d& vec) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_XLINE(this->m_pImpl)->direction = vec;
    }
}

GePoint3d DbXline::secondPoint() const {
    return DB_IMP_XLINE(this->m_pImpl)->basePoint + DB_IMP_XLINE(this->m_pImpl)->direction;
}
Acad::ErrorStatus DbXline::setSecondPoint(GePoint3d const& pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_XLINE(this->m_pImpl)->direction = pt - DB_IMP_XLINE(this->m_pImpl)->basePoint;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}




Acad::ErrorStatus DbXline::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::subTransformBy(const GeMatrix3d& xform)
{
	GePoint3d firstPoint = DB_IMP_XLINE(this->m_pImpl)->basePoint;
	GePoint3d secondPoint = DB_IMP_XLINE(this->m_pImpl)->basePoint + DB_IMP_XLINE(this->m_pImpl)->direction;

	firstPoint.transformBy(xform);
	secondPoint.transformBy(xform);
	DB_IMP_XLINE(this->m_pImpl)->basePoint = firstPoint;
	DB_IMP_XLINE(this->m_pImpl)->direction = secondPoint - firstPoint;
	DB_IMP_XLINE(this->m_pImpl)->direction.normalize();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{

	DbGripData* gripData = new DbGripData();
	gripData->setGripPoint(this->basePoint());
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbXline::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{

	if (osnapMode == Db::OsnapMode::kOsModeNear)
	{
		GePoint3dArray points = ::osnap_near_to_line(pickPoint, this, viewXform);
		for(int i = 0;i<points.length();i++){
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeMid)
	{
		GePoint3dArray points = ::osnap_mid_to_line(pickPoint, this, viewXform);
		for(int i = 0;i<points.length();i++){
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
	
	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{
		GePoint3dArray points = ::osnap_perp_to_line(pickPoint, lastPoint, this, viewXform);
		for(int i = 0;i<points.length();i++){
			snapPoints.append(points[i]);
		}
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbXline::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{
	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_XLINE(this->m_pImpl)->basePoint += offset;
		}
		else if (indices.at(i) == 1)
		{

		}
		else if (indices.at(i) == 2)
		{
		
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine())
	{
		GePoint3dArray ps = ::osnap_inters_line_to_lintseg(this, (DbLine *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isXline())
	{
		GePoint3dArray ps = ::osnap_inters_line_to_line(this, (DbXline *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isRay())
	{
		GePoint3dArray ps = ::osnap_inters_line_to_ray(this, (DbRay *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle())
	{
		GePoint3dArray ps = ::osnap_inters_line_to_circle(this, (DbCircle *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isArc())
	{
		GePoint3dArray ps = ::osnap_inters_line_to_arc(this, (DbArc *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isPolyline())
	{
		GePoint3dArray ps = ::osnap_inters_line_to_polyline(this, (DbPolyline *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isBlockReference())
    {

    }

	return Acad::ErrorStatus::eOk;

}


bool DbXline::isClosed() const
{
	return false;
}
bool DbXline::isPeriodic() const
{
	return false;
}
bool DbXline::isPlanar() const
{
	return false;
}
Acad::ErrorStatus DbXline::getPlane(GePlane& plane, Db::Planarity& planarity) const
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getStartParam(double& _v) const
{
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getEndParam(double& _v) const
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getStartPoint(GePoint3d& _v) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getEndPoint(GePoint3d& _v) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getPointAtParam(double param, GePoint3d& point) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getParamAtPoint(const GePoint3d& point, double& param) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getDistAtParam(double param, double& dist) const {
	dist = abs(param);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getParamAtDist(double dist, double& param) const {
	param = dist;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getDistAtPoint(const GePoint3d& point, double& dist) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getPointAtDist(double dist, GePoint3d& point) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getFirstDeriv(double, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getFirstDeriv(const GePoint3d&, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getSecondDeriv(double, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getSecondDeriv(const GePoint3d&, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getClosestPointTo(const GePoint3d& point, GePoint3d& closest, bool) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getClosestPointTo(const GePoint3d& point, const GeVector3d& projectDirection, GePoint3d& closest, bool) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getOrthoProjectedCurve(const GePlane& plane, DbCurve*& curve) const {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getProjectedCurve(const GePlane& plane, const GeVector3d& normal, DbCurve*& curve) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::getOffsetCurves(double v, DbVoidPtrArray& curves) const {

	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getSpline(DbSpline*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray& curveSegments) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::extend(double) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::extend(bool, const GePoint3d&) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getArea(double& area) const {
	area = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::reverseCurve() {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbXline::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXline::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) {
	return Acad::ErrorStatus::eFail;
}
