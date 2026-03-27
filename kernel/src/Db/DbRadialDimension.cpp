#include "DbRadialDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>


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
namespace
{
	void drawArrowRadial(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

bool DbRadialDimension::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d cen = this->center();
	GePoint3d chord = this->chordPoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;

	GeVector3d dimDir = chord - cen;
	if (dimDir.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;

	// 半径线 (center → chordPoint)
	pWd->geometry().line(cen, chord);

	// 箭头在弦点处
	GeVector3d arrowDir = cen - chord;
	drawArrowRadial(pWd, chord, arrowDir, normal, asz);

	// 文字
	double radius = cen.distanceTo(chord);
	char buf[64];
	snprintf(buf, sizeof(buf), "R%.2f", radius);
	GePoint3d textPos = cen + dimDir * 0.5;
	GeVector3d textDir = dimDir;
	textDir.normalize();
	pWd->geometry().text(textPos, normal, textDir, txt, 1.0, 0.0, buf);

	return true;
}


Acad::ErrorStatus DbRadialDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_RADIALDIMENSION(this->m_pImpl);
	extents.addPoint(imp->center);
	extents.addPoint(imp->chordPoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRadialDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_RADIALDIMENSION(this->m_pImpl);
	imp->center.transformBy(xform);
	imp->chordPoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_RADIALDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->center, imp->chordPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 3; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
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
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_RADIALDIMENSION(this->m_pImpl);
		snapPoints.append(imp->center);
		snapPoints.append(imp->chordPoint);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRadialDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_RADIALDIMENSION(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->center += offset; break;
			case 1: imp->chordPoint += offset; break;
			case 2: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
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
