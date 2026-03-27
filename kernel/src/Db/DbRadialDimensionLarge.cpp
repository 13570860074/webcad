#include "DbRadialDimensionLarge.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>


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
namespace
{
	void drawArrowRadLarge(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
	{
		if (dir.length() < 1e-10 || size < 1e-10) return;
		GeVector3d d = dir;
		d.normalize();
		GeVector3d perp = d.crossProduct(normal);
		if (perp.length() < 1e-10) perp = d.crossProduct(GeVector3d::kZAxis);
		if (perp.length() < 1e-10) return;
		perp.normalize();
		GePoint3d pts[3];
		pts[0] = tip;
		pts[1] = tip + d * size + perp * (size * 0.3);
		pts[2] = tip + d * size - perp * (size * 0.3);
		pWd->geometry().polygon(3, pts);
	}
}

bool DbRadialDimensionLarge::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d cen = this->center();
	GePoint3d chord = this->chordPoint();
	GePoint3d overrideCen = this->overrideCenter();
	GePoint3d jog = this->jogPoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;

	GeVector3d dimDir = chord - overrideCen;
	if (dimDir.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	// 从虚拟中心到折弯点
	pWd->geometry().line(overrideCen, jog);

	// 从折弯点到弦点
	pWd->geometry().line(jog, chord);

	// 箭头在弦点处
	GeVector3d arrowDir = overrideCen - chord;
	drawArrowRadLarge(pWd, chord, arrowDir, normal, asz);

	// 文字
	double radius = cen.distanceTo(chord);
	char buf[64];
	snprintf(buf, sizeof(buf), "R%.2f", radius);
	GePoint3d textPos = jog + (chord - jog) * 0.5;
	GeVector3d textDir = dimDir;
	textDir.normalize();
	pWd->geometry().text(textPos, normal, textDir, txt, 1.0, 0.0, buf);

	return true;
}

Acad::ErrorStatus DbRadialDimensionLarge::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl);
	extents.addPoint(imp->center);
	extents.addPoint(imp->chordPoint);
	extents.addPoint(imp->overrideCenter);
	extents.addPoint(imp->jogPoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimensionLarge::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl);
	imp->center.transformBy(xform);
	imp->chordPoint.transformBy(xform);
	imp->overrideCenter.transformBy(xform);
	imp->jogPoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimensionLarge::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl);
	GePoint3d pts[] = { imp->center, imp->chordPoint, imp->jogPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 4; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
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
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl);
		snapPoints.append(imp->center);
		snapPoints.append(imp->chordPoint);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimensionLarge::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_RADIALDIMENSIONLARGE(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->center += offset; break;
			case 1: imp->chordPoint += offset; break;
			case 2: imp->jogPoint += offset; break;
			case 3: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
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