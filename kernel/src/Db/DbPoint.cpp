#include "DbPoint.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbExtents.h"
#include "GePlane.h"
#include "DbImpl.h"



DbPoint::DbPoint() {
	this->m_pImpl = new DbPointImpl();
}
DbPoint::DbPoint(const GePoint3d& position) {
	this->m_pImpl = new DbPointImpl();
	this->setPosition(position);
}
DbPoint::~DbPoint() {

}



GePoint3d DbPoint::position() const {
	return DB_IMP_POINT(this->m_pImpl)->position;
}
Acad::ErrorStatus DbPoint::setPosition(const GePoint3d& _point) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_POINT(this->m_pImpl)->position = _point;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbPoint::thickness() const {
	return DB_IMP_POINT(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbPoint::setThickness(double _thickness) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_POINT(this->m_pImpl)->thickness = _thickness;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector3d DbPoint::normal() const {
	return DB_IMP_POINT(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbPoint::setNormal(const GeVector3d& _normal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_POINT(this->m_pImpl)->normal = _normal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbPoint::ecsRotation() const {
	return DB_IMP_POINT(this->m_pImpl)->ecsRotation;
}
Acad::ErrorStatus DbPoint::setEcsRotation(double _ecsRotation) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_POINT(this->m_pImpl)->ecsRotation = _ecsRotation;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbPoint::getPlane(GePlane& plane, Db::Planarity& planarity) const  {
	plane.set(this->position(), this->normal());
	planarity = Db::Planarity::kPlanar;
	return Acad::ErrorStatus::eOk;
}



Acad::ErrorStatus DbPoint::dwgInFields(DbDwgFiler* pFiler)
{

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	GePoint3d position;
	pFiler->readPoint3d(&position);
	this->setPosition(position);

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	this->setNormal(normal);

	double ecsRotation;
	pFiler->readDouble(&ecsRotation);
	this->setEcsRotation(ecsRotation);

	double thickness;
	pFiler->readDouble(&thickness);
	this->setThickness(thickness);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPoint::dwgOutFields(DbDwgFiler* pFiler) const
{

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	pFiler->writePoint3d(this->position());

	pFiler->writeVector3d(this->normal());

	pFiler->writeDouble(this->ecsRotation());

	pFiler->writeDouble(this->thickness());

	return Acad::ErrorStatus::eOk;
}
bool DbPoint::subWorldDraw(GiWorldDraw* pWd) const
{
	GePoint3d pVertexList[1];
	pVertexList[0] = this->position();
	pWd->geometry().polypoint(1, pVertexList);
	return false;
}

Acad::ErrorStatus DbPoint::subGetGeomExtents(DbExtents &extents) const
{
	extents.addPoint(this->position());
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPoint::subTransformBy(const GeMatrix3d& xform)
{
	DB_IMP_POINT(this->m_pImpl)->position.transformBy(xform);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPoint::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{

	DbGripData* gripData = new DbGripData();
	gripData->setGripPoint(this->position());
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbPoint::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{

	if (osnapMode == Db::OsnapMode::kOsModeNode)
	{
		snapPoints.append(this->position());
	}
	else if (osnapMode == Db::OsnapMode::kOsModeNear)
	{
		snapPoints.append(this->position());
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		snapPoints.append(this->position());
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbPoint::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_POINT(this->m_pImpl)->position += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPoint::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPoint::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	return Acad::ErrorStatus::eOk;

}
