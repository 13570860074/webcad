#include "DbDiametricDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>




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
namespace
{
	void drawArrowDiam(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

bool DbDiametricDimension::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d chord = this->chordPoint();
	GePoint3d farChord = this->farChordPoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;

	GeVector3d dimDir = farChord - chord;
	if (dimDir.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;

	// 直径线
	pWd->geometry().line(chord, farChord);

	// 箭头
	drawArrowDiam(pWd, chord, dimDir, normal, asz);
	GeVector3d revDir = chord - farChord;
	drawArrowDiam(pWd, farChord, revDir, normal, asz);

	// 文字
	double diameter = chord.distanceTo(farChord);
	char buf[64];
	snprintf(buf, sizeof(buf), "\xc3\xb8%.2f", diameter);
	GePoint3d textPos = chord + dimDir * 0.5;
	GeVector3d textDir = dimDir;
	textDir.normalize();
	pWd->geometry().text(textPos, normal, textDir, txt, 1.0, 0.0, buf);

	return true;
}


Acad::ErrorStatus DbDiametricDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_DIAMETRICDIMENSION(this->m_pImpl);
	extents.addPoint(imp->chordPoint);
	extents.addPoint(imp->farChordPoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDiametricDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_DIAMETRICDIMENSION(this->m_pImpl);
	imp->chordPoint.transformBy(xform);
	imp->farChordPoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDiametricDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_DIAMETRICDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->chordPoint, imp->farChordPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 3; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
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
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_DIAMETRICDIMENSION(this->m_pImpl);
		snapPoints.append(imp->chordPoint);
		snapPoints.append(imp->farChordPoint);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDiametricDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_DIAMETRICDIMENSION(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->chordPoint += offset; break;
			case 1: imp->farChordPoint += offset; break;
			case 2: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
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