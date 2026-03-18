#include "DbRay.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbLine.h"
#include "DbXline.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeLine3d.h"
#include "GeCircArc3d.h"
#include "DbImpl.h"
#include "DbOsnapPointCompute.h"

DbRay::DbRay() {
    this->m_pImpl = new DbRayImpl();
}
DbRay::~DbRay() {

}




Acad::ErrorStatus DbRay::dwgInFields(DbDwgFiler* pFiler) {

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
Acad::ErrorStatus DbRay::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

	pFiler->writePoint3d(this->basePoint());
	pFiler->writeVector3d(this->unitDir());

    return Acad::ErrorStatus::eOk;
}
bool DbRay::subWorldDraw(GiWorldDraw* pWd) const {
	pWd->geometry().ray(this->basePoint(), this->basePoint() + this->unitDir());
    return true;
}


Acad::ErrorStatus DbRay::getOffsetCurvesGivenPlaneNormal(const GeVector3d& normal, double offsetDist, DbVoidPtrArray& offsetCurves) const {
	
	GeRay3d line(this->basePoint(), this->unitDir());
	GeVoidPointerArray entitys;
	line.getTrimmedOffset(offsetDist, normal, entitys);
	for (int i = entitys.length() - 1; i >= 0; i--)
	{
		GeEntity3d* entity = (GeEntity3d*)entitys[i];
		if (entity->isKindOf(Ge::EntityId::kLine3d) == true)
		{
			DbRay* line = DbRay::cast(this->clone());
			line->setBasePoint(((GeLine3d*)entity)->pointOnLine());
			line->setUnitDir(((GeLine3d*)entity)->direction());
			offsetCurves.append(line);
		}
		else if (entity->isKindOf(Ge::EntityId::kRay3d) == true)
		{
			DbRay* line = DbRay::cast(this->clone());
			line->setBasePoint(((GeRay3d*)entity)->pointOnLine());
			line->setUnitDir(((GeRay3d*)entity)->direction());
			offsetCurves.append(line);
		}
		delete entity;
	}
	return Acad::ErrorStatus::eOk;
}

GePoint3d DbRay::basePoint() const {
    return DB_IMP_RAY(this->m_pImpl)->basePoint;
}
void DbRay::setBasePoint(const GePoint3d& pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RAY(this->m_pImpl)->basePoint = pt;
    }
}

GeVector3d DbRay::unitDir() const {
    return DB_IMP_RAY(this->m_pImpl)->direction;
}
void DbRay::setUnitDir(const GeVector3d& vec) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RAY(this->m_pImpl)->direction = vec;
    }
}

GePoint3d DbRay::secondPoint() const {
    return DB_IMP_RAY(this->m_pImpl)->basePoint + DB_IMP_RAY(this->m_pImpl)->direction;
}
Acad::ErrorStatus DbRay::setSecondPoint(GePoint3d const& pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RAY(this->m_pImpl)->direction = pt - DB_IMP_RAY(this->m_pImpl)->basePoint;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}




Acad::ErrorStatus DbRay::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::subTransformBy(const GeMatrix3d& xform)
{
	GePoint3d firstPoint = DB_IMP_RAY(this->m_pImpl)->basePoint;
	GePoint3d secondPoint = DB_IMP_RAY(this->m_pImpl)->basePoint + DB_IMP_RAY(this->m_pImpl)->direction;

	firstPoint.transformBy(xform);
	secondPoint.transformBy(xform);
	DB_IMP_RAY(this->m_pImpl)->basePoint = firstPoint;
	DB_IMP_RAY(this->m_pImpl)->direction = secondPoint - firstPoint;
	DB_IMP_RAY(this->m_pImpl)->direction.normalize();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::subGetGripPoints(
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

Acad::ErrorStatus DbRay::subGetOsnapPoints(
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
		GePoint3dArray points = ::osnap_near_to_ray(pickPoint, this, viewXform);
		for(int i = 0;i<points.length();i++){
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		GePoint3dArray points = ::osnap_end_to_ray(pickPoint, this, viewXform);
		for(int i = 0;i<points.length();i++){
			snapPoints.append(points[i]);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{
		GePoint3dArray points = ::osnap_perp_to_ray(pickPoint, lastPoint, this, viewXform);
		for(int i = 0;i<points.length();i++){
			snapPoints.append(points[i]);
		}
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRay::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_RAY(this->m_pImpl)->basePoint += offset;
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
Acad::ErrorStatus DbRay::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine())
	{
		GePoint3dArray ps = ::osnap_inters_ray_to_lintseg(this, (DbLine *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isXline())
	{
		GePoint3dArray ps = ::osnap_inters_ray_to_line(this, (DbXline *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isRay())
	{
		GePoint3dArray ps = ::osnap_inters_ray_to_ray(this, (DbRay *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle())
	{
		GePoint3dArray ps = ::osnap_inters_ray_to_circle(this, (DbCircle *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isArc())
	{
		GePoint3dArray ps = ::osnap_inters_ray_to_arc(this, (DbArc *)pEnt, intType);
		for (int i = 0; i < ps.length(); i++)
		{
			points.append(ps[i]);
		}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isPolyline())
	{
		GePoint3dArray ps = ::osnap_inters_ray_to_polyline(this, (DbPolyline *)pEnt, intType);
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


bool DbRay::isClosed() const
{
	return false;
}
bool DbRay::isPeriodic() const
{
	return false;
}
bool DbRay::isPlanar() const
{
	return false;
}
Acad::ErrorStatus DbRay::getPlane(GePlane& plane, Db::Planarity& planarity) const
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getStartParam(double& _v) const
{
	_v = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getEndParam(double& _v) const
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getStartPoint(GePoint3d& _v) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getEndPoint(GePoint3d& _v) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getPointAtParam(double param, GePoint3d& point) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getParamAtPoint(const GePoint3d& point, double& param) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getDistAtParam(double param, double& dist) const {
	dist = abs(param);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getParamAtDist(double dist, double& param) const {
	param = dist;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getDistAtPoint(const GePoint3d& point, double& dist) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getPointAtDist(double dist, GePoint3d& point) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getFirstDeriv(double, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getFirstDeriv(const GePoint3d&, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getSecondDeriv(double, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getSecondDeriv(const GePoint3d&, GeVector3d&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getClosestPointTo(const GePoint3d& point, GePoint3d& closest, bool) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getClosestPointTo(const GePoint3d& point, const GeVector3d& projectDirection, GePoint3d& closest, bool) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getOrthoProjectedCurve(const GePlane& plane, DbCurve*& curve) const {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getProjectedCurve(const GePlane& plane, const GeVector3d& normal, DbCurve*& curve) const {

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::getOffsetCurves(double v, DbVoidPtrArray& curves) const {

	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getSpline(DbSpline*&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getSplitCurves(const GeDoubleArray&, DbVoidPtrArray& curveSegments) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getSplitCurves(const GePoint3dArray&, DbVoidPtrArray&) const {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::extend(double) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::extend(bool, const GePoint3d&) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getArea(double& area) const {
	area = 0.0;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::reverseCurve() {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRay::getGeCurve(GeCurve3d*& pGeCurve, const GeTol& tol) const {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRay::setFromGeCurve(const GeCurve3d& geCurve, GeVector3d* normal, const GeTol& tol) {
	return Acad::ErrorStatus::eFail;
}