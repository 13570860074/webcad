#include "Db3PointAngularDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>

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
namespace
{
	void drawArrow3PtAng(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

bool Db3PointAngularDimension::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d cen = this->centerPoint();
	GePoint3d pt1 = this->xLine1Point();
	GePoint3d pt2 = this->xLine2Point();
	GePoint3d arcPt = this->arcPoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;
	double exo = this->dimexo() * scale;
	double exe = this->dimexe() * scale;

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;

	GeVector3d v1 = pt1 - cen;
	GeVector3d v2 = pt2 - cen;
	if (v1.length() < 1e-10 || v2.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}
	v1.normalize();
	v2.normalize();

	// 弧半径 = arcPoint到center的距离
	double radius = cen.distanceTo(arcPt);
	if (radius < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	// 弧上两端点
	GePoint3d a1 = cen + v1 * radius;
	GePoint3d a2 = cen + v2 * radius;

	// 延伸线
	pWd->geometry().line(pt1 + v1 * exo, a1 + v1 * exe);
	pWd->geometry().line(pt2 + v2 * exo, a2 + v2 * exe);

	// 角度弧
	GeVector3d vArc = arcPt - cen;
	if (vArc.length() > 1e-10) {
		vArc.normalize();
		GePoint3d arcMid = cen + vArc * radius;
		pWd->geometry().circularArc(a1, arcMid, a2);
	}

	// 箭头
	GeVector3d tangent1 = v1.crossProduct(normal);
	GeVector3d tangent2 = normal.crossProduct(v2);
	drawArrow3PtAng(pWd, a1, tangent1, normal, asz);
	drawArrow3PtAng(pWd, a2, tangent2, normal, asz);

	// 文字: 角度值
	double dotVal = v1.dotProduct(v2);
	if (dotVal > 1.0) dotVal = 1.0;
	if (dotVal < -1.0) dotVal = -1.0;
	double angle = std::acos(dotVal);
	double angleDeg = angle * 180.0 / 3.14159265358979323846;
	char buf[64];
	snprintf(buf, sizeof(buf), "%.1f\xc2\xb0", angleDeg);
	GeVector3d vMid = v1 + v2;
	if (vMid.length() > 1e-10) {
		vMid.normalize();
	}
	GePoint3d textPos = cen + vMid * radius;
	GeVector3d textDir = vMid.crossProduct(normal);
	if (textDir.length() < 1e-10) textDir = GeVector3d::kXAxis;
	textDir.normalize();
	pWd->geometry().text(textPos, normal, textDir, txt, 1.0, 0.0, buf);

	return true;
}


Acad::ErrorStatus Db3PointAngularDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl);
	extents.addPoint(imp->arcPoint);
	extents.addPoint(imp->xLine1Point);
	extents.addPoint(imp->xLine2Point);
	extents.addPoint(imp->centerPoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db3PointAngularDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl);
	imp->arcPoint.transformBy(xform);
	imp->xLine1Point.transformBy(xform);
	imp->xLine2Point.transformBy(xform);
	imp->centerPoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db3PointAngularDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->centerPoint, imp->xLine1Point, imp->xLine2Point, imp->arcPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 5; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
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
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl);
		snapPoints.append(imp->xLine1Point);
		snapPoints.append(imp->xLine2Point);
		snapPoints.append(imp->centerPoint);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db3PointAngularDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_3POINTANGULARDIMENSION(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->centerPoint += offset; break;
			case 1: imp->xLine1Point += offset; break;
			case 2: imp->xLine2Point += offset; break;
			case 3: imp->arcPoint += offset; break;
			case 4: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
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