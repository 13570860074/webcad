#include "DbRadialDimensionLarge.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"


DbRadialDimensionLarge::DbRadialDimensionLarge() {
	this->m_pImpl = new DbRadialDimensionLargeImpl();
}
DbRadialDimensionLarge::DbRadialDimensionLarge(
	const GePoint3d& center,
	const GePoint3d& chordPoint,
	const GePoint3d& overrideCenter,
	const GePoint3d& jogPoint,
	double             jogAngle) {
	this->m_pImpl = new DbRadialDimensionLargeImpl();
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->center = center;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->chordPoint = chordPoint;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->overrideCenter = overrideCenter;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogPoint = jogPoint;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogAngle = jogAngle;
}
DbRadialDimensionLarge::DbRadialDimensionLarge(
	const GePoint3d& center,
	const GePoint3d& chordPoint,
	const GePoint3d& overrideCenter,
	const GePoint3d& jogPoint,
	double             jogAngle,
	const ACHAR* dimText,
	DbObjectId       dimStyle) {
	this->m_pImpl = new DbRadialDimensionLargeImpl();
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->center = center;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->chordPoint = chordPoint;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->overrideCenter = overrideCenter;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogPoint = jogPoint;
	DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogAngle = jogAngle;
}
DbRadialDimensionLarge::~DbRadialDimensionLarge() {

}



GePoint3d DbRadialDimensionLarge::center() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->center;
}
Acad::ErrorStatus DbRadialDimensionLarge::setCenter(const GePoint3d& centerPoint) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->center = centerPoint;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;

}
GePoint3d DbRadialDimensionLarge::chordPoint() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->chordPoint;

}
Acad::ErrorStatus DbRadialDimensionLarge::setChordPoint(const GePoint3d& chordPoint) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->chordPoint = chordPoint;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbRadialDimensionLarge::overrideCenter() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->overrideCenter;
}
Acad::ErrorStatus DbRadialDimensionLarge::setOverrideCenter(const GePoint3d& overrideCenterPoint) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->overrideCenter = overrideCenterPoint;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;


}
GePoint3d DbRadialDimensionLarge::jogPoint() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogPoint;
}
Acad::ErrorStatus DbRadialDimensionLarge::setJogPoint(const GePoint3d& jogPoint) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogPoint = jogPoint;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbRadialDimensionLarge::jogAngle() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogAngle;
}
Acad::ErrorStatus DbRadialDimensionLarge::setJogAngle(double jogAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogAngle = jogAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;


}
Acad::ErrorStatus DbRadialDimensionLarge::setOverrideCenterPP(const GePoint3d& overrideCenterPointPP) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->overrideCenter = overrideCenterPointPP;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRadialDimensionLarge::setJogPointPP(const GePoint3d& jogPointPP) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->jogPointPP = jogPointPP;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbRadialDimensionLarge::setTextPositionPP(const GePoint3d& textPointPP) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->textPositionPP = textPointPP;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;

}
double DbRadialDimensionLarge::extArcStartAngle() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->extArcStartAngle;
}
Acad::ErrorStatus DbRadialDimensionLarge::setExtArcStartAngle(double newAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->extArcStartAngle = newAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;

}

double DbRadialDimensionLarge::extArcEndAngle() const {
	return DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->extArcEndAngle;

}
Acad::ErrorStatus DbRadialDimensionLarge::setExtArcEndAngle(double newAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl)->extArcEndAngle = newAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}














Acad::ErrorStatus DbRadialDimensionLarge::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d center;
	pFiler->readPoint3d(&center);
	this->setCenter(center);

	GePoint3d chordPoint;
	pFiler->readPoint3d(&chordPoint);
	this->setChordPoint(chordPoint);

	GePoint3d overrideCenter;
	pFiler->readPoint3d(&overrideCenter);
	this->setOverrideCenter(overrideCenter);

	GePoint3d jogPoint;
	pFiler->readPoint3d(&jogPoint);
	this->setJogPoint(jogPoint);

	double jogAngle;
	pFiler->readDouble(&jogAngle);
	this->setJogAngle(jogAngle);

	double extArcStartAngle;
	pFiler->readDouble(&extArcStartAngle);
	this->setExtArcStartAngle(extArcStartAngle);

	double extArcEndAngle;
	pFiler->readDouble(&extArcEndAngle);
	this->setExtArcEndAngle(extArcEndAngle);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimensionLarge::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->center());
	pFiler->writePoint3d(this->chordPoint());
	pFiler->writePoint3d(this->overrideCenter());
	pFiler->writePoint3d(this->jogPoint());
	pFiler->writeDouble(this->jogAngle());
	pFiler->writeDouble(this->extArcStartAngle());
	pFiler->writeDouble(this->extArcEndAngle());

	return Acad::ErrorStatus::eOk;
}
bool DbRadialDimensionLarge::subWorldDraw(GiWorldDraw* pWd) const {
	return DbDimension::subWorldDraw(pWd);
}

Acad::ErrorStatus DbRadialDimensionLarge::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimensionLarge::subTransformBy(const GeMatrix3d& xform) {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimensionLarge::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {



	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimensionLarge::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimensionLarge::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {


	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimensionLarge::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimensionLarge::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}