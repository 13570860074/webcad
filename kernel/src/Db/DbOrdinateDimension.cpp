#include "DbOrdinateDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"


DbOrdinateDimension::DbOrdinateDimension() {
	this->m_pImpl = new DbOrdinateDimensionImpl();
}
DbOrdinateDimension::DbOrdinateDimension(
	Adesk::Boolean     useXAxis,
	const GePoint3d& definingPoint,
	const GePoint3d& leaderEndPoint) {
	this->m_pImpl = new DbOrdinateDimensionImpl();
}
DbOrdinateDimension::DbOrdinateDimension(
    Adesk::Boolean     useXAxis,
    const GePoint3d& definingPoint,
    const GePoint3d& leaderEndPoint,
    const ACHAR* dimText,
    DbObjectId       dimStyle) {
	this->m_pImpl = new DbOrdinateDimensionImpl();
}
DbOrdinateDimension::~DbOrdinateDimension() {

}



Adesk::Boolean DbOrdinateDimension::isUsingXAxis() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingXAxis;
}
Adesk::Boolean DbOrdinateDimension::isUsingYAxis() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingYAxis;
}
Acad::ErrorStatus DbOrdinateDimension::useXAxis() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingXAxis = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbOrdinateDimension::useYAxis() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingYAxis = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbOrdinateDimension::setUsingXAxis(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingXAxis = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbOrdinateDimension::setUsingYAxis(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingYAxis = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbOrdinateDimension::origin() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->origin;
}
Acad::ErrorStatus DbOrdinateDimension::setOrigin(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->origin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbOrdinateDimension::definingPoint() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->definingPoint;
}
Acad::ErrorStatus DbOrdinateDimension::setDefiningPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->definingPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbOrdinateDimension::leaderEndPoint() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->leaderEndPoint;
}
Acad::ErrorStatus DbOrdinateDimension::setLeaderEndPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->leaderEndPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

















Acad::ErrorStatus DbOrdinateDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	bool isUsingXAxis;
	pFiler->readBool(&isUsingXAxis);
	this->setUsingXAxis(isUsingXAxis);

	bool isUsingYAxis;
	pFiler->readBool(&isUsingYAxis);
	this->setUsingYAxis(isUsingYAxis);

	GePoint3d origin;
	pFiler->readPoint3d(&origin);
	this->setOrigin(origin);

	GePoint3d definingPoint;
	pFiler->readPoint3d(&definingPoint);
	this->setDefiningPoint(definingPoint);

	GePoint3d leaderEndPoint;
	pFiler->readPoint3d(&leaderEndPoint);
	this->setLeaderEndPoint(leaderEndPoint);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbOrdinateDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeBool(this->isUsingXAxis());
	pFiler->writeBool(this->isUsingYAxis());
	pFiler->writePoint3d(this->origin());
	pFiler->writePoint3d(this->definingPoint());
	pFiler->writePoint3d(this->leaderEndPoint());

	return Acad::ErrorStatus::eOk;
}
bool DbOrdinateDimension::subWorldDraw(GiWorldDraw* pWd) const {
	return DbDimension::subWorldDraw(pWd);
}



Acad::ErrorStatus DbOrdinateDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbOrdinateDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbOrdinateDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}