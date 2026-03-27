#include "DbAlignedDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>


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
namespace
{
	void drawArrowAligned(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

bool DbAlignedDimension::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d p1 = this->xLine1Point();
	GePoint3d p2 = this->xLine2Point();
	GePoint3d dimPt = this->dimLinePoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;
	double exo = this->dimexo() * scale;
	double exe = this->dimexe() * scale;

	// 尺寸线方向沿两定义点连线
	GeVector3d dimDir = p2 - p1;
	if (dimDir.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}
	dimDir.normalize();

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;
	GeVector3d extDir = dimDir.crossProduct(normal);
	if (extDir.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}
	extDir.normalize();

	// 计算尺寸线位置(通过dimLinePoint的投影距离)
	double offset = extDir.dotProduct(dimPt - p1);
	GePoint3d d1 = p1 + extDir * offset;
	GePoint3d d2 = p2 + extDir * offset;

	// 尺寸线
	pWd->geometry().line(d1, d2);

	// 延伸线
	GeVector3d ext1Dir = d1 - p1;
	if (ext1Dir.length() > 1e-10) {
		ext1Dir.normalize();
		pWd->geometry().line(p1 + ext1Dir * exo, d1 + ext1Dir * exe);
	}
	GeVector3d ext2Dir = d2 - p2;
	if (ext2Dir.length() > 1e-10) {
		ext2Dir.normalize();
		pWd->geometry().line(p2 + ext2Dir * exo, d2 + ext2Dir * exe);
	}

	// 箭头
	drawArrowAligned(pWd, d1, d2 - d1, normal, asz);
	drawArrowAligned(pWd, d2, d1 - d2, normal, asz);

	// 文字
	double meas = d1.distanceTo(d2);
	char buf[64];
	snprintf(buf, sizeof(buf), "%.2f", meas);
	GePoint3d textPos = d1 + (d2 - d1) * 0.5;
	pWd->geometry().text(textPos, normal, dimDir, txt, 1.0, 0.0, buf);

	return true;
}


Acad::ErrorStatus DbAlignedDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_ALIGNEDDIMENSION(this->m_pImpl);
	extents.addPoint(imp->xLine1Point);
	extents.addPoint(imp->xLine2Point);
	extents.addPoint(imp->dimLinePoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbAlignedDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_ALIGNEDDIMENSION(this->m_pImpl);
	imp->xLine1Point.transformBy(xform);
	imp->xLine2Point.transformBy(xform);
	imp->dimLinePoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbAlignedDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_ALIGNEDDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->xLine1Point, imp->xLine2Point, imp->dimLinePoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 4; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
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
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_ALIGNEDDIMENSION(this->m_pImpl);
		snapPoints.append(imp->xLine1Point);
		snapPoints.append(imp->xLine2Point);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbAlignedDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_ALIGNEDDIMENSION(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->xLine1Point += offset; break;
			case 1: imp->xLine2Point += offset; break;
			case 2: imp->dimLinePoint += offset; break;
			case 3: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
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