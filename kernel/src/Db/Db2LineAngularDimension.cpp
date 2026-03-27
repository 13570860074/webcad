#include "Db2LineAngularDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>


Db2LineAngularDimension::Db2LineAngularDimension() {
	this->m_pImpl = new Db2LineAngularDimensionImpl();
}
Db2LineAngularDimension::Db2LineAngularDimension(
	const GePoint3d& xLine1Start,
	const GePoint3d& xLine1End,
	const GePoint3d& xLine2Start,
	const GePoint3d& xLine2End,
	const GePoint3d& arcPoint) {
	this->m_pImpl = new Db2LineAngularDimensionImpl();
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start = xLine1Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End = xLine1End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start = xLine2Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End = xLine2End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
Db2LineAngularDimension::Db2LineAngularDimension(
	const GePoint3d& xLine1Start,
	const GePoint3d& xLine1End,
	const GePoint3d& xLine2Start,
	const GePoint3d& xLine2End,
	const GePoint3d& arcPoint,
	const ACHAR* dimText,
	DbObjectId       dimStyle) {
	this->m_pImpl = new Db2LineAngularDimensionImpl();
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start = xLine1Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End = xLine1End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start = xLine2Start;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End = xLine2End;
	DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
Db2LineAngularDimension::~Db2LineAngularDimension() {

}





GePoint3d Db2LineAngularDimension::arcPoint() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint;
}
Acad::ErrorStatus Db2LineAngularDimension::setArcPoint(const GePoint3d& arcPt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->arcPoint = arcPt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine1Start() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine1Start(const GePoint3d& xLine1Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1Start = xLine1Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine1End() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine1End(const GePoint3d& xLine1Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine1End = xLine1Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine2Start() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine2Start(const GePoint3d& xLine2Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2Start = xLine2Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d Db2LineAngularDimension::xLine2End() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End;
}
Acad::ErrorStatus Db2LineAngularDimension::setXLine2End(const GePoint3d& xLine2Pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->xLine2End = xLine2Pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Angular dimension extension arc for quadrant
bool Db2LineAngularDimension::extArcOn() const {
	return DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->extArcOn;
}
Acad::ErrorStatus Db2LineAngularDimension::setExtArcOn(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl)->extArcOn = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


















Acad::ErrorStatus Db2LineAngularDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d arcPoint;
	pFiler->readPoint3d(&arcPoint);
	this->setArcPoint(arcPoint);

	GePoint3d xLine1Start;
	pFiler->readPoint3d(&xLine1Start);
	this->setXLine1Start(xLine1Start);

	GePoint3d xLine1End;
	pFiler->readPoint3d(&xLine1End);
	this->setXLine1End(xLine1End);

	GePoint3d xLine2Start;
	pFiler->readPoint3d(&xLine2Start);
	this->setXLine2Start(xLine2Start);

	GePoint3d xLine2End;
	pFiler->readPoint3d(&xLine2End);
	this->setXLine2End(xLine2End);

	bool extArcOn;
	pFiler->readBool(&extArcOn);
	this->setExtArcOn(extArcOn);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db2LineAngularDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->arcPoint());
	pFiler->writePoint3d(this->xLine1Start());
	pFiler->writePoint3d(this->xLine1End());
	pFiler->writePoint3d(this->xLine2Start());
	pFiler->writePoint3d(this->xLine2End());
	pFiler->writeBool(this->extArcOn());

	return Acad::ErrorStatus::eOk;
}
namespace
{
	void drawArrow2Line(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

	// 两线求交点
	bool intersectLines2D(const GePoint3d& p1, const GeVector3d& d1,
		const GePoint3d& p2, const GeVector3d& d2, GePoint3d& result)
	{
		double denom = d1.x * d2.y - d1.y * d2.x;
		if (std::fabs(denom) < 1e-10) return false;
		double t = ((p2.x - p1.x) * d2.y - (p2.y - p1.y) * d2.x) / denom;
		result = GePoint3d(p1.x + d1.x * t, p1.y + d1.y * t, p1.z + d1.z * t);
		return true;
	}
}

bool Db2LineAngularDimension::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d l1s = this->xLine1Start();
	GePoint3d l1e = this->xLine1End();
	GePoint3d l2s = this->xLine2Start();
	GePoint3d l2e = this->xLine2End();
	GePoint3d arcPt = this->arcPoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double asz = this->dimasz() * scale;
	double txt = this->dimtxt() * scale;
	double exo = this->dimexo() * scale;
	double exe = this->dimexe() * scale;

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;

	// 两条线的方向
	GeVector3d dir1 = l1e - l1s;
	GeVector3d dir2 = l2e - l2s;
	if (dir1.length() < 1e-10 || dir2.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	// 求交点作为角度顶点
	GePoint3d cen;
	if (!intersectLines2D(l1s, dir1, l2s, dir2, cen)) {
		// 平行线，用中点
		cen = GePoint3d((l1s.x + l2s.x) * 0.5, (l1s.y + l2s.y) * 0.5, (l1s.z + l2s.z) * 0.5);
	}

	// 向量从顶点到各端点
	GeVector3d v1 = l1e - cen;
	GeVector3d v2 = l2e - cen;
	if (v1.length() < 1e-10 || v2.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}
	v1.normalize();
	v2.normalize();

	double radius = cen.distanceTo(arcPt);
	if (radius < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	GePoint3d a1 = cen + v1 * radius;
	GePoint3d a2 = cen + v2 * radius;

	// 延伸线
	pWd->geometry().line(l1e + v1 * exo, a1 + v1 * exe);
	pWd->geometry().line(l2e + v2 * exo, a2 + v2 * exe);

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
	drawArrow2Line(pWd, a1, tangent1, normal, asz);
	drawArrow2Line(pWd, a2, tangent2, normal, asz);

	// 文字
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

Acad::ErrorStatus Db2LineAngularDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl);
	extents.addPoint(imp->arcPoint);
	extents.addPoint(imp->xLine1Start);
	extents.addPoint(imp->xLine1End);
	extents.addPoint(imp->xLine2Start);
	extents.addPoint(imp->xLine2End);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db2LineAngularDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl);
	imp->arcPoint.transformBy(xform);
	imp->xLine1Start.transformBy(xform);
	imp->xLine1End.transformBy(xform);
	imp->xLine2Start.transformBy(xform);
	imp->xLine2End.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->xLine1Start, imp->xLine1End, imp->xLine2Start, imp->xLine2End, imp->arcPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 6; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl);
		snapPoints.append(imp->xLine1Start);
		snapPoints.append(imp->xLine1End);
		snapPoints.append(imp->xLine2Start);
		snapPoints.append(imp->xLine2End);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_2LINEANGULARDIMENSION(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->xLine1Start += offset; break;
			case 1: imp->xLine1End += offset; break;
			case 2: imp->xLine2Start += offset; break;
			case 3: imp->xLine2End += offset; break;
			case 4: imp->arcPoint += offset; break;
			case 5: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus Db2LineAngularDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus Db2LineAngularDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}