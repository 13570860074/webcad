#include "Db2LineAngularDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"


Db2LineAngularDimension::Db2LineAngularDimension() {
	this->m_pImpl = new Db2LineAngularDimensionImpl();
}
Db2LineAngularDimension::Db2LineAngularDimension(
	const GePoint3d& xLine1Start,
	const GePoint3d& xLine1End,
	const GePoint3d& xLine2Start,
	const GePoint3d& xLine2End,
	const GePoint3d& arcPoint) {
	this->m_pImpl = new Db2LineAngularDimensionImpl();
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start = xLine1Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End = xLine1End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start = xLine2Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End = xLine2End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
Db2LineAngularDimension::Db2LineAngularDimension(
	const GePoint3d& xLine1Start,
	const GePoint3d& xLine1End,
	const GePoint3d& xLine2Start,
	const GePoint3d& xLine2End,
	const GePoint3d& arcPoint,
	const ACHAR* dimText,
	DbObjectId       dimStyle) {
	this->m_pImpl = new Db2LineAngularDimensionImpl();
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start = xLine1Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End = xLine1End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start = xLine2Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End = xLine2End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
Db2LineAngularDimension::~Db2LineAngularDimension() {

}





GePoint3d Db2LineAngularDimension::arcPoint() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint;
}
Acad::ErrorStatus Db2LineAngularDimension::setArcPoint(const GePoint3d& arcPt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine1Start() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine1Start(const GePoint3d& xLine1Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start = xLine1Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine1End() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine1End(const GePoint3d& xLine1Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End = xLine1Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine2Start() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine2Start(const GePoint3d& xLine2Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start = xLine2Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine2End() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine2End(const GePoint3d& xLine2Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End = xLine2Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Angular dimension extension arc for quadrant
bool Db2LineAngularDimension::extArcOn() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->extArcOn;
}
Acad::ErrorStatus Db2LineAngularDimension::setExtArcOn(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->extArcOn = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


















Acad::ErrorStatus Db2LineAngularDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d arcPoint;
	pFiler->readPoint3d(&arcPoint);
	this->setArcPoint(arcPoint);

	GePoint3d xLine1Start;
	pFiler->readPoint3d(&xLine1Start);
	this->setXLine1Start(xLine1Start);

	GePoint3d xLine1End;
	pFiler->readPoint3d(&xLine1End);
	this->setXLine1End(xLine1End);

	GePoint3d xLine2Start;
	pFiler->readPoint3d(&xLine2Start);
	this->setXLine2Start(xLine2Start);

	GePoint3d xLine2End;
	pFiler->readPoint3d(&xLine2End);
	this->setXLine2End(xLine2End);

	bool extArcOn;
	pFiler->readBool(&extArcOn);
	this->setExtArcOn(extArcOn);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db2LineAngularDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->arcPoint());
	pFiler->writePoint3d(this->xLine1Start());
	pFiler->writePoint3d(this->xLine1End());
	pFiler->writePoint3d(this->xLine2Start());
	pFiler->writePoint3d(this->xLine2End());
	pFiler->writeBool(this->extArcOn());

	return Acad::ErrorStatus::eOk;
}
bool Db2LineAngularDimension::subWorldDraw(GiWorldDraw* pWd) const {
	return DbDimension::subWorldDraw(pWd);
}

Acad::ErrorStatus Db2LineAngularDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db2LineAngularDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db2LineAngularDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}