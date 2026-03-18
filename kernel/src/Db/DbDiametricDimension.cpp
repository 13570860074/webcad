#include "DbDiametricDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"




DbDiametricDimension::DbDiametricDimension() {
	this->m_pImpl = new DbDiametricDimensionImpl();
}
DbDiametricDimension::DbDiametricDimension(
    const GePoint3d& chordPoint,
    const GePoint3d& farChordPoint,
    double             leaderLength,
    const ACHAR* dimText,
    DbObjectId       dimStyle) {
	this->m_pImpl = new DbDiametricDimensionImpl();
	DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->chordPoint = chordPoint;
	DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->farChordPoint = farChordPoint;
	DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->leaderLength = leaderLength;
}
DbDiametricDimension::~DbDiametricDimension() {

}

double DbDiametricDimension::leaderLength() const {
	return DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->leaderLength;
}
Acad::ErrorStatus DbDiametricDimension::setLeaderLength(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->leaderLength = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbDiametricDimension::chordPoint() const {
	return DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->chordPoint;
}
Acad::ErrorStatus DbDiametricDimension::setChordPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->chordPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbDiametricDimension::farChordPoint() const {
	return DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->farChordPoint;
}
Acad::ErrorStatus DbDiametricDimension::setFarChordPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->farChordPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Diametric dimension extension arc
double DbDiametricDimension::extArcStartAngle() const {
	return DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->extArcStartAngle;
}
Acad::ErrorStatus DbDiametricDimension::setExtArcStartAngle(double newAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->extArcStartAngle = newAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbDiametricDimension::extArcEndAngle() const {
	return DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->extArcEndAngle;
}
Acad::ErrorStatus DbDiametricDimension::setExtArcEndAngle(double newAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIAMETRICDIMENSION(this->m_pImpl)->extArcEndAngle = newAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}










Acad::ErrorStatus DbDiametricDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	double leaderLength;
	pFiler->readDouble(&leaderLength);
	this->setLeaderLength(leaderLength);

	GePoint3d chordPoint;
	pFiler->readPoint3d(&chordPoint);
	this->setChordPoint(chordPoint);

	GePoint3d farChordPoint;
	pFiler->readPoint3d(&farChordPoint);
	this->setFarChordPoint(farChordPoint);

	double extArcStartAngle;
	pFiler->readDouble(&extArcStartAngle);
	this->setExtArcStartAngle(extArcStartAngle);

	double extArcEndAngle;
	pFiler->readDouble(&extArcEndAngle);
	this->setExtArcEndAngle(extArcEndAngle);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDiametricDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeDouble(this->leaderLength());
	pFiler->writePoint3d(this->chordPoint());
	pFiler->writePoint3d(this->farChordPoint());
	pFiler->writeDouble(this->extArcStartAngle());
	pFiler->writeDouble(this->extArcEndAngle());

	return Acad::ErrorStatus::eOk;
}
bool DbDiametricDimension::subWorldDraw(GiWorldDraw* pWd) const {

	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbDiametricDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDiametricDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDiametricDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDiametricDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDiametricDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDiametricDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDiametricDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}