#include "DbAlignedDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"


DbAlignedDimension::DbAlignedDimension() {
	this->m_pImpl = new DbAlignedDimensionImpl();
}
DbAlignedDimension::DbAlignedDimension(const GePoint3d& xLine1Point, const GePoint3d& xLine2Point, const GePoint3d& dimLinePoint) {
	this->m_pImpl = new DbAlignedDimensionImpl();
	DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
	DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
	DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->dimLinePoint = dimLinePoint;
}
DbAlignedDimension::DbAlignedDimension(const GePoint3d& xLine1Point, const GePoint3d& xLine2Point, const GePoint3d& dimLinePoint, const ACHAR* dimText, DbObjectId dimStyle) {
	this->m_pImpl = new DbAlignedDimensionImpl();
	DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
	DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
	DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->dimLinePoint = dimLinePoint;
}

DbAlignedDimension::~DbAlignedDimension() {

}




GePoint3d DbAlignedDimension::xLine1Point() const {
	return DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine1Point;
}
Acad::ErrorStatus DbAlignedDimension::setXLine1Point(const GePoint3d& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine1Point = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbAlignedDimension::xLine2Point() const {
	return DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine2Point;
}
Acad::ErrorStatus DbAlignedDimension::setXLine2Point(const GePoint3d& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->xLine2Point = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbAlignedDimension::dimLinePoint() const {
	return DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->dimLinePoint;
}
Acad::ErrorStatus DbAlignedDimension::setDimLinePoint(const GePoint3d& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->dimLinePoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbAlignedDimension::oblique() const {
	return DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->oblique;
}
Acad::ErrorStatus DbAlignedDimension::setOblique(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->oblique = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Rotated dimension Jog symbol
bool DbAlignedDimension::jogSymbolOn() const {
	return DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->jogSymbolOn;
}
Acad::ErrorStatus DbAlignedDimension::setJogSymbolOn(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->jogSymbolOn = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbAlignedDimension::jogSymbolPosition() const {
	return DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->jogSymbolPosition;
}
Acad::ErrorStatus DbAlignedDimension::setJogSymbolPosition(const GePoint3d& pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ALIGNEDDIMENSION(this->m_pImpl)->jogSymbolPosition = pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}






Acad::ErrorStatus DbAlignedDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	GePoint3d xLine1Point;
	pFiler->readPoint3d(&xLine1Point);
	this->setXLine1Point(xLine1Point);

	GePoint3d xLine2Point;
	pFiler->readPoint3d(&xLine2Point);
	this->setXLine2Point(xLine2Point);

	GePoint3d dimLinePoint;
	pFiler->readPoint3d(&dimLinePoint);
	this->setDimLinePoint(dimLinePoint);

	double oblique;
	pFiler->readDouble(&oblique);
	this->setOblique(oblique);

	bool jogSymbolOn;
	pFiler->readBool(&jogSymbolOn);
	this->setJogSymbolOn(jogSymbolOn);

	GePoint3d jogSymbolPosition;
	pFiler->readPoint3d(&jogSymbolPosition);
	this->setJogSymbolPosition(jogSymbolPosition);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbAlignedDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->xLine1Point());
	pFiler->writePoint3d(this->xLine2Point());
	pFiler->writePoint3d(this->dimLinePoint());
	pFiler->writeDouble(this->oblique());
	pFiler->writeBool(this->jogSymbolOn());
	pFiler->writePoint3d(this->jogSymbolPosition());

	return Acad::ErrorStatus::eOk;
}
bool DbAlignedDimension::subWorldDraw(GiWorldDraw* pWd) const {
	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbAlignedDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbAlignedDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbAlignedDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbAlignedDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbAlignedDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbAlignedDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbAlignedDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}