#include "DbRotatedDimension.h"
#include "DbExtents.h"
#include "DbGripData.h"
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
#include <cmath>
#include <cstdio>


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
namespace
{
	void drawArrow(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

bool DbRotatedDimension::subWorldDraw(GiWorldDraw* pWd) const {
	// 获取定义点
	GePoint3d p1 = this->xLine1Point();
	GePoint3d p2 = this->xLine2Point();
	GePoint3d dimPt = this->dimLinePoint();
	double rot = this->rotation();

	// dim style 变量
	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;
	double exo = this->dimexo() * scale;
	double exe = this->dimexe() * scale;
	double gap = this->dimgap() * scale;

	// 尺寸线方向
	GeVector3d dimDir(cos(rot), sin(rot), 0.0);
	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;
	GeVector3d extDir = dimDir.crossProduct(normal);
	if (extDir.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}
	extDir.normalize();

	// 将定义点投影到尺寸线方向
	double proj1 = dimDir.dotProduct(p1 - GePoint3d::kOrigin);
	double proj2 = dimDir.dotProduct(p2 - GePoint3d::kOrigin);
	double projDim = extDir.dotProduct(dimPt - GePoint3d::kOrigin);
	double ext1 = extDir.dotProduct(p1 - GePoint3d::kOrigin);

	// 尺寸线上的两个端点
	GePoint3d d1 = p1 + extDir * (projDim - ext1);
	GePoint3d d2 = p2 + extDir * (projDim - extDir.dotProduct(p2 - GePoint3d::kOrigin));

	// 绘制尺寸线
	pWd->geometry().line(d1, d2);

	// 绘制延伸线
	GeVector3d ext1Dir = d1 - p1;
	double ext1Len = ext1Dir.length();
	if (ext1Len > 1e-10) {
		ext1Dir.normalize();
		GePoint3d e1Start = p1 + ext1Dir * exo;
		GePoint3d e1End = d1 + ext1Dir * exe;
		pWd->geometry().line(e1Start, e1End);
	}
	GeVector3d ext2Dir = d2 - p2;
	double ext2Len = ext2Dir.length();
	if (ext2Len > 1e-10) {
		ext2Dir.normalize();
		GePoint3d e2Start = p2 + ext2Dir * exo;
		GePoint3d e2End = d2 + ext2Dir * exe;
		pWd->geometry().line(e2Start, e2End);
	}

	// 绘制箭头
	GeVector3d arrowDir1 = d2 - d1;
	GeVector3d arrowDir2 = d1 - d2;
	drawArrow(pWd, d1, arrowDir1, normal, asz);
	drawArrow(pWd, d2, arrowDir2, normal, asz);

	// 绘制标注文字
	double meas = d1.distanceTo(d2);
	char buf[64];
	snprintf(buf, sizeof(buf), "%.2f", meas);
	GePoint3d textPos = d1 + (d2 - d1) * 0.5;
	GeVector3d textDir = dimDir;
	textDir.normalize();
	pWd->geometry().text(textPos, normal, textDir, txt, 1.0, 0.0, buf);

	return true;
}


Acad::ErrorStatus DbRotatedDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_ROTATEDDIMENSION(this->m_pImpl);
	extents.addPoint(imp->xLine1Point);
	extents.addPoint(imp->xLine2Point);
	extents.addPoint(imp->dimLinePoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRotatedDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_ROTATEDDIMENSION(this->m_pImpl);
	imp->xLine1Point.transformBy(xform);
	imp->xLine2Point.transformBy(xform);
	imp->dimLinePoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRotatedDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_ROTATEDDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->xLine1Point, imp->xLine2Point, imp->dimLinePoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 4; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
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
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_ROTATEDDIMENSION(this->m_pImpl);
		snapPoints.append(imp->xLine1Point);
		snapPoints.append(imp->xLine2Point);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbRotatedDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_ROTATEDDIMENSION(this->m_pImpl);
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