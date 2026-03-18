#include "Db3PointAngularDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"

Db3PointAngularDimension::Db3PointAngularDimension() {
	this->m_pImpl = new Db3PointAngularDimensionImpl();

}
Db3PointAngularDimension::Db3PointAngularDimension(
	const GePoint3d& centerPoint,
	const GePoint3d& xLine1Point,
	const GePoint3d& xLine2Point,
	const GePoint3d& arcPoint) {
	this->m_pImpl = new Db3PointAngularDimensionImpl();
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->centerPoint = centerPoint;
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
Db3PointAngularDimension::Db3PointAngularDimension(
    const GePoint3d& centerPoint,
    const GePoint3d& xLine1Point,
    const GePoint3d& xLine2Point,
    const GePoint3d& arcPoint,
    const ACHAR* dimText,
    DbObjectId       dimStyle) {
	this->m_pImpl = new Db3PointAngularDimensionImpl();
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->centerPoint = centerPoint;
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
	DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
Db3PointAngularDimension::~Db3PointAngularDimension() {

}


GePoint3d Db3PointAngularDimension::arcPoint() const {
	return DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->arcPoint;
}
Acad::ErrorStatus Db3PointAngularDimension::setArcPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->arcPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db3PointAngularDimension::xLine1Point() const {
	return DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine1Point;
}
Acad::ErrorStatus Db3PointAngularDimension::setXLine1Point(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine1Point = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db3PointAngularDimension::xLine2Point() const {
	return DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine2Point;
}
Acad::ErrorStatus Db3PointAngularDimension::setXLine2Point(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->xLine2Point = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db3PointAngularDimension::centerPoint() const {
	return DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->centerPoint;
}
Acad::ErrorStatus Db3PointAngularDimension::setCenterPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->centerPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Angular dimension extension arc for quadrant
bool Db3PointAngularDimension::extArcOn() const {
	return DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->extArcOn;
}
Acad::ErrorStatus Db3PointAngularDimension::setExtArcOn(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl)->extArcOn = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}









Acad::ErrorStatus Db3PointAngularDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d arcPoint;
	pFiler->readPoint3d(&arcPoint);
	this->setArcPoint(arcPoint);

	GePoint3d xLine1Point;
	pFiler->readPoint3d(&xLine1Point);
	this->setXLine1Point(xLine1Point);

	GePoint3d xLine2Point;
	pFiler->readPoint3d(&xLine2Point);
	this->setXLine2Point(xLine2Point);

	GePoint3d centerPoint;
	pFiler->readPoint3d(&centerPoint);
	this->setCenterPoint(centerPoint);

	bool extArcOn;
	pFiler->readBool(&extArcOn);
	this->setExtArcOn(extArcOn);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db3PointAngularDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->arcPoint());
	pFiler->writePoint3d(this->xLine1Point());
	pFiler->writePoint3d(this->xLine2Point());
	pFiler->writePoint3d(this->centerPoint());
	pFiler->writeBool(this->extArcOn());

	return Acad::ErrorStatus::eOk;
}
bool Db3PointAngularDimension::subWorldDraw(GiWorldDraw* pWd) const {
	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus Db3PointAngularDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db3PointAngularDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db3PointAngularDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db3PointAngularDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db3PointAngularDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db3PointAngularDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db3PointAngularDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}