#include "DbRadialDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"


DbRadialDimension::DbRadialDimension() {
    this->m_pImpl = new DbRadialDimensionImpl();
}
DbRadialDimension::DbRadialDimension(
	const GePoint3d& center,
	const GePoint3d& chordPoint,
	double             leaderLength) {
	this->m_pImpl = new DbRadialDimensionImpl();
	DB_IMP_RADIALDIMENSION(this->m_pImpl)->center = center;
	DB_IMP_RADIALDIMENSION(this->m_pImpl)->chordPoint = chordPoint;
	DB_IMP_RADIALDIMENSION(this->m_pImpl)->leaderLength = leaderLength;
}
DbRadialDimension::DbRadialDimension(
    const GePoint3d& center,
    const GePoint3d& chordPoint,
    double             leaderLength,
    const ACHAR* dimText,
    DbObjectId       dimStyle) {
    this->m_pImpl = new DbRadialDimensionImpl();
	DB_IMP_RADIALDIMENSION(this->m_pImpl)->center = center;
	DB_IMP_RADIALDIMENSION(this->m_pImpl)->chordPoint = chordPoint;
	DB_IMP_RADIALDIMENSION(this->m_pImpl)->leaderLength = leaderLength;
}
DbRadialDimension::~DbRadialDimension() {

}



double DbRadialDimension::leaderLength() const {
    return DB_IMP_RADIALDIMENSION(this->m_pImpl)->leaderLength;
}
Acad::ErrorStatus DbRadialDimension::setLeaderLength(double v) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RADIALDIMENSION(this->m_pImpl)->leaderLength = v;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbRadialDimension::center() const {
    return DB_IMP_RADIALDIMENSION(this->m_pImpl)->center;
}
Acad::ErrorStatus DbRadialDimension::setCenter(const GePoint3d&v) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RADIALDIMENSION(this->m_pImpl)->center = v;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbRadialDimension::chordPoint() const {
    return DB_IMP_RADIALDIMENSION(this->m_pImpl)->chordPoint;
}
Acad::ErrorStatus DbRadialDimension::setChordPoint(const GePoint3d&v) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RADIALDIMENSION(this->m_pImpl)->chordPoint = v;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

// Radial dimension extension arc
double DbRadialDimension::extArcStartAngle() const {
    return DB_IMP_RADIALDIMENSION(this->m_pImpl)->extArcStartAngle;
}
Acad::ErrorStatus DbRadialDimension::setExtArcStartAngle(double newAngle) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RADIALDIMENSION(this->m_pImpl)->extArcStartAngle = newAngle;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

double DbRadialDimension::extArcEndAngle() const {
    return DB_IMP_RADIALDIMENSION(this->m_pImpl)->extArcEndAngle;
}
Acad::ErrorStatus DbRadialDimension::setExtArcEndAngle(double newAngle) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_RADIALDIMENSION(this->m_pImpl)->extArcEndAngle = newAngle;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}












Acad::ErrorStatus DbRadialDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	double leaderLength;
	pFiler->readDouble(&leaderLength);
	this->setLeaderLength(leaderLength);

	GePoint3d center;
	pFiler->readPoint3d(&center);
	this->setCenter(center);

	GePoint3d chordPoint;
	pFiler->readPoint3d(&chordPoint);
	this->setChordPoint(chordPoint);

	double extArcStartAngle;
	pFiler->readDouble(&extArcStartAngle);
	this->setExtArcStartAngle(extArcStartAngle);

	double extArcEndAngle;
	pFiler->readDouble(&extArcEndAngle);
	this->setExtArcEndAngle(extArcEndAngle);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeDouble(this->leaderLength());
	pFiler->writePoint3d(this->center());
	pFiler->writePoint3d(this->chordPoint());
	pFiler->writeDouble(this->extArcStartAngle());
	pFiler->writeDouble(this->extArcEndAngle());

	return Acad::ErrorStatus::eOk;
}
bool DbRadialDimension::subWorldDraw(GiWorldDraw* pWd) const {
	return DbDimension::subWorldDraw(pWd);
}


Acad::ErrorStatus DbRadialDimension::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimension::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}