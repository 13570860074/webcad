#include "DbRotatedDimension.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "GeMatrix3d.h"
#include "GeLine3d.h"
#include "DbObjectId.h"
#include "DbObjectManager.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTableRecordIterator.h"
#include "DbLine.h"
#include "kernel.h"
#include "DbImpl.h"


DbRotatedDimension::DbRotatedDimension() {
	this->m_pImpl = new DbRotatedDimensionImpl();
}
DbRotatedDimension::DbRotatedDimension(
	double             rotation,
	const GePoint3d& xLine1Point,
	const GePoint3d& xLine2Point,
	const GePoint3d& dimLinePoint) {
	this->m_pImpl = new DbRotatedDimensionImpl();
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->rotation = rotation;
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->dimLinePoint = dimLinePoint;
}
DbRotatedDimension::DbRotatedDimension(
    double rotation,
    const GePoint3d& xLine1Point,
    const GePoint3d& xLine2Point,
    const GePoint3d& dimLinePoint,
    const ACHAR* dimText,
    DbObjectId       dimStyle) {
	this->m_pImpl = new DbRotatedDimensionImpl();
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->rotation = rotation;
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
	DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->dimLinePoint = dimLinePoint;
}
DbRotatedDimension::~DbRotatedDimension() {

}




GePoint3d DbRotatedDimension::xLine1Point() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine1Point;
}
Acad::ErrorStatus DbRotatedDimension::setXLine1Point(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine1Point = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbRotatedDimension::xLine2Point() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine2Point;
}
Acad::ErrorStatus DbRotatedDimension::setXLine2Point(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->xLine2Point = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbRotatedDimension::dimLinePoint() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->dimLinePoint;
}
Acad::ErrorStatus DbRotatedDimension::setDimLinePoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->dimLinePoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbRotatedDimension::oblique() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->oblique;
}
Acad::ErrorStatus DbRotatedDimension::setOblique(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->oblique = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbRotatedDimension::rotation() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->rotation;
}
Acad::ErrorStatus DbRotatedDimension::setRotation(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->rotation = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Rotated dimension Jog symbol
bool DbRotatedDimension::jogSymbolOn() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->jogSymbolOn;
}
Acad::ErrorStatus DbRotatedDimension::setJogSymbolOn(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->jogSymbolOn = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbRotatedDimension::jogSymbolPosition() const {
	return DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->jogSymbolPosition;
}
Acad::ErrorStatus DbRotatedDimension::setJogSymbolPosition(const GePoint3d& pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ROTATEDDIMENSION(this->m_pImpl)->jogSymbolPosition = pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


void DbRotatedDimension::close() {

	DbDimension::close();
}



Acad::ErrorStatus DbRotatedDimension::dwgInFields(DbDwgFiler* pFiler) {

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

	double rotation;
	pFiler->readDouble(&rotation);
	this->setRotation(rotation);

	bool jogSymbolOn;
	pFiler->readBool(&jogSymbolOn);
	this->setJogSymbolOn(jogSymbolOn);

	GePoint3d jogSymbolPosition;
	pFiler->readPoint3d(&jogSymbolPosition);
	this->setJogSymbolPosition(jogSymbolPosition);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRotatedDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->xLine1Point());
	pFiler->writePoint3d(this->xLine2Point());
	pFiler->writePoint3d(this->dimLinePoint());
	pFiler->writeDouble(this->oblique());
	pFiler->writeDouble(this->rotation());
	pFiler->writeBool(this->jogSymbolOn());
	pFiler->writePoint3d(this->jogSymbolPosition());

	return Acad::ErrorStatus::eOk;
}
bool DbRotatedDimension::subWorldDraw(GiWorldDraw* pWd) const {

	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbRotatedDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRotatedDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRotatedDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRotatedDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRotatedDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRotatedDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRotatedDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}