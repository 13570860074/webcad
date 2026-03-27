#include "DbArcDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>


// Default constructor
DbArcDimension::DbArcDimension() {
    this->m_pImpl = new DbArcDimensionImpl();
}
DbArcDimension::DbArcDimension(
    const GePoint3d& centerPoint,
    const GePoint3d& xLine1Point,
    const GePoint3d& xLine2Point,
    const GePoint3d& arcPoint) {
    this->m_pImpl = new DbArcDimensionImpl();
    DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint = centerPoint;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
DbArcDimension::DbArcDimension(
    const GePoint3d& centerPoint,
    const GePoint3d& xLine1Point,
    const GePoint3d& xLine2Point,
    const GePoint3d& arcPoint,
    const ACHAR* dimText,
    DbObjectId dimStyle) {
    this->m_pImpl = new DbArcDimensionImpl();
    DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint = centerPoint;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point = xLine1Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point = xLine2Point;
    DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint = arcPoint;
}
// Default destructor
DbArcDimension::~DbArcDimension() {

}



GePoint3d DbArcDimension::arcPoint() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint;
}
Acad::ErrorStatus DbArcDimension::setArcPoint(const GePoint3d& arcPt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcPoint  = arcPt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
GePoint3d DbArcDimension::xLine1Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point;
}    
Acad::ErrorStatus DbArcDimension::setXLine1Point(const GePoint3d& xLine1Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine1Point = xLine1Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
GePoint3d DbArcDimension::xLine2Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point;
}  
Acad::ErrorStatus DbArcDimension::setXLine2Point(const GePoint3d& xLine2Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->xLine2Point = xLine2Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbArcDimension::centerPoint() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint;
} 
Acad::ErrorStatus DbArcDimension::setCenterPoint(const GePoint3d& ctrPt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->centerPoint = ctrPt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbArcDimension::isPartial() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->isPartial;
}
Acad::ErrorStatus DbArcDimension::setIsPartial(const bool& partial) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->isPartial = partial;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
double DbArcDimension::arcStartParam() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcStartParam;
}   
Acad::ErrorStatus DbArcDimension::setArcStartParam(const double& arcParam) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcStartParam = arcParam;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
double DbArcDimension::arcEndParam() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcEndParam;
} 
Acad::ErrorStatus DbArcDimension::setArcEndParam(const double& arcParam) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcEndParam = arcParam;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbArcDimension::hasLeader() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->hasLeader;
}
Acad::ErrorStatus DbArcDimension::setHasLeader(const bool& leaderVal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->hasLeader = leaderVal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbArcDimension::leader1Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->leader1Point;
}    
Acad::ErrorStatus DbArcDimension::setLeader1Point(const GePoint3d& ldr1Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->leader1Point = ldr1Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbArcDimension::leader2Point() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->leader2Point;
}
Acad::ErrorStatus DbArcDimension::setLeader2Point(const GePoint3d& ldr2Pt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->leader2Point = ldr2Pt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

int DbArcDimension::arcSymbolType() const {
    return DB_IMP_ARCDIMENSION(this->m_pImpl)->arcSymbolType;
}

Acad::ErrorStatus DbArcDimension::setArcSymbolType(const int symbol) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_ARCDIMENSION(this->m_pImpl)->arcSymbolType = symbol;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}












Acad::ErrorStatus DbArcDimension::dwgInFields(DbDwgFiler* pFiler) {

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

    bool isPartial;
    pFiler->readBool(&isPartial);
    this->setIsPartial(isPartial);

    double arcStartParam;
    pFiler->readDouble(&arcStartParam);
    this->setArcStartParam(arcStartParam);

    double arcEndParam;
    pFiler->readDouble(&arcEndParam);
    this->setArcEndParam(arcEndParam);

    bool hasLeader;
    pFiler->readBool(&hasLeader);
    this->setHasLeader(hasLeader);

    GePoint3d leader1Point;
    pFiler->readPoint3d(&leader1Point);
    this->setLeader1Point(leader1Point);

    GePoint3d leader2Point;
    pFiler->readPoint3d(&leader2Point);
    this->setLeader2Point(leader2Point);

    int arcSymbolType;
    pFiler->readInt32(&arcSymbolType);
    this->setArcSymbolType(arcSymbolType);

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArcDimension::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    pFiler->writePoint3d(this->arcPoint());
    pFiler->writePoint3d(this->xLine1Point());
    pFiler->writePoint3d(this->xLine2Point());
    pFiler->writePoint3d(this->centerPoint());
    pFiler->writeBool(this->isPartial());
    pFiler->writeDouble(this->arcStartParam());
    pFiler->writeDouble(this->arcEndParam());
    pFiler->writeBool(this->hasLeader());
    pFiler->writePoint3d(this->leader1Point());
    pFiler->writePoint3d(this->leader2Point());
    pFiler->writeInt32(this->arcSymbolType());

    return Acad::ErrorStatus::eOk;
}
namespace
{
	void drawArrowArcDim(GiWorldDraw* pWd, const GePoint3d& tip, const GeVector3d& dir, const GeVector3d& normal, double size)
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

bool DbArcDimension::subWorldDraw(GiWorldDraw* pWd) const {
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

	// 计算弧的半径(用arcPoint到中心的距离)
	double radius = cen.distanceTo(arcPt);
	if (radius < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}

	// 计算两个延伸线方向与角度
	GeVector3d v1 = pt1 - cen;
	GeVector3d v2 = pt2 - cen;
	if (v1.length() < 1e-10 || v2.length() < 1e-10) {
		return DbDimension::subWorldDraw(pWd);
	}
	v1.normalize();
	v2.normalize();

	// 弧上的两个端点
	GePoint3d a1 = cen + v1 * radius;
	GePoint3d a2 = cen + v2 * radius;

	// 延伸线
	GeVector3d ext1Dir = v1;
	pWd->geometry().line(pt1 + ext1Dir * exo, a1 + ext1Dir * exe);
	GeVector3d ext2Dir = v2;
	pWd->geometry().line(pt2 + ext2Dir * exo, a2 + ext2Dir * exe);

	// 尺寸弧线 (三点式)
	GeVector3d vArc = arcPt - cen;
	if (vArc.length() > 1e-10) {
		vArc.normalize();
		GePoint3d arcMid = cen + vArc * radius;
		pWd->geometry().circularArc(a1, arcMid, a2);
	}

	// 箭头
	GeVector3d tangent1 = v1.crossProduct(normal);
	GeVector3d tangent2 = normal.crossProduct(v2);
	drawArrowArcDim(pWd, a1, tangent1, normal, asz);
	drawArrowArcDim(pWd, a2, tangent2, normal, asz);

	// 文字: 弧长
	double angle = std::acos(v1.dotProduct(v2));
	if (angle < 0) angle = -angle;
	double arcLength = radius * angle;
	char buf[64];
	snprintf(buf, sizeof(buf), "%.2f", arcLength);
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


Acad::ErrorStatus DbArcDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_ARCDIMENSION(this->m_pImpl);
	extents.addPoint(imp->arcPoint);
	extents.addPoint(imp->xLine1Point);
	extents.addPoint(imp->xLine2Point);
	extents.addPoint(imp->centerPoint);
	if (imp->hasLeader) {
		extents.addPoint(imp->leader1Point);
		extents.addPoint(imp->leader2Point);
	}
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArcDimension::subTransformBy(const GeMatrix3d& xform) {
    auto* imp = DB_IMP_ARCDIMENSION(this->m_pImpl);
    imp->arcPoint.transformBy(xform);
    imp->xLine1Point.transformBy(xform);
    imp->xLine2Point.transformBy(xform);
    imp->centerPoint.transformBy(xform);
    if (imp->hasLeader) {
        imp->leader1Point.transformBy(xform);
        imp->leader2Point.transformBy(xform);
    }
    DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
    DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
    DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subGetGripPoints(
    DbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const GeVector3d& curViewDir,
    const int bitflags) const {
    auto* imp = DB_IMP_ARCDIMENSION(this->m_pImpl);
    GePoint3d pts[] = { imp->centerPoint, imp->xLine1Point, imp->xLine2Point, imp->arcPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
    for (int i = 0; i < 5; i++) {
        DbGripData* grip = new DbGripData();
        grip->setGripPoint(pts[i]);
        grips.append(grip);
    }
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subGetOsnapPoints(
    Db::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const GePoint3d& pickPoint,
    const GePoint3d& lastPoint,
    const GeMatrix3d& viewXform,
    GePoint3dArray& snapPoints,
    DbIntArray& geomIds) const {
    if (osnapMode == Db::kOsModeEnd) {
        auto* imp = DB_IMP_ARCDIMENSION(this->m_pImpl);
        snapPoints.append(imp->xLine1Point);
        snapPoints.append(imp->xLine2Point);
    }
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbArcDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
    auto* imp = DB_IMP_ARCDIMENSION(this->m_pImpl);
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

Acad::ErrorStatus DbArcDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbArcDimension::subIntersectWith(
    const DbEntity* pEnt,
    Db::Intersect intType,
    GePoint3dArray& points,
    Adesk::GsMarker thisGsMarker,
    Adesk::GsMarker otherGsMarker) const {
    return Acad::ErrorStatus::eOk;
}