#include "DbOrdinateDimension.h"
#include "GeMatrix3d.h"
#include "DbObjectId.h"
#include "DbImpl.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include <cmath>
#include <cstdio>


DbOrdinateDimension::DbOrdinateDimension() {
	this->m_pImpl = new DbOrdinateDimensionImpl();
}
DbOrdinateDimension::DbOrdinateDimension(
	Adesk::Boolean     useXAxis,
	const GePoint3d& definingPoint,
	const GePoint3d& leaderEndPoint) {
	this->m_pImpl = new DbOrdinateDimensionImpl();
}
DbOrdinateDimension::DbOrdinateDimension(
    Adesk::Boolean     useXAxis,
    const GePoint3d& definingPoint,
    const GePoint3d& leaderEndPoint,
    const ACHAR* dimText,
    DbObjectId       dimStyle) {
	this->m_pImpl = new DbOrdinateDimensionImpl();
}
DbOrdinateDimension::~DbOrdinateDimension() {

}



Adesk::Boolean DbOrdinateDimension::isUsingXAxis() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingXAxis;
}
Adesk::Boolean DbOrdinateDimension::isUsingYAxis() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingYAxis;
}
Acad::ErrorStatus DbOrdinateDimension::useXAxis() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingXAxis = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbOrdinateDimension::useYAxis() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingYAxis = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbOrdinateDimension::setUsingXAxis(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingXAxis = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbOrdinateDimension::setUsingYAxis(bool value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->isUsingYAxis = value;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbOrdinateDimension::origin() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->origin;
}
Acad::ErrorStatus DbOrdinateDimension::setOrigin(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->origin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbOrdinateDimension::definingPoint() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->definingPoint;
}
Acad::ErrorStatus DbOrdinateDimension::setDefiningPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->definingPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbOrdinateDimension::leaderEndPoint() const {
	return DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->leaderEndPoint;
}
Acad::ErrorStatus DbOrdinateDimension::setLeaderEndPoint(const GePoint3d&v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ORDINATEDIMENSION(this->m_pImpl)->leaderEndPoint = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

















Acad::ErrorStatus DbOrdinateDimension::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbDimension::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	bool isUsingXAxis;
	pFiler->readBool(&isUsingXAxis);
	this->setUsingXAxis(isUsingXAxis);

	bool isUsingYAxis;
	pFiler->readBool(&isUsingYAxis);
	this->setUsingYAxis(isUsingYAxis);

	GePoint3d origin;
	pFiler->readPoint3d(&origin);
	this->setOrigin(origin);

	GePoint3d definingPoint;
	pFiler->readPoint3d(&definingPoint);
	this->setDefiningPoint(definingPoint);

	GePoint3d leaderEndPoint;
	pFiler->readPoint3d(&leaderEndPoint);
	this->setLeaderEndPoint(leaderEndPoint);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbOrdinateDimension::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbDimension::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeBool(this->isUsingXAxis());
	pFiler->writeBool(this->isUsingYAxis());
	pFiler->writePoint3d(this->origin());
	pFiler->writePoint3d(this->definingPoint());
	pFiler->writePoint3d(this->leaderEndPoint());

	return Acad::ErrorStatus::eOk;
}
bool DbOrdinateDimension::subWorldDraw(GiWorldDraw* pWd) const {
	GePoint3d orig = this->origin();
	GePoint3d defPt = this->definingPoint();
	GePoint3d endPt = this->leaderEndPoint();

	double scale = this->dimscale();
	if (scale < 1e-10) scale = 1.0;
	double txt = this->dimtxt() * scale;

	GeVector3d normal = this->normal();
	if (normal.length() < 1e-10) normal = GeVector3d::kZAxis;

	// 引线: 从定义点到引线端点
	pWd->geometry().line(defPt, endPt);

	// 坐标值文字
	double value = 0.0;
	if (this->isUsingXAxis()) {
		value = defPt.x - orig.x;
	} else {
		value = defPt.y - orig.y;
	}
	char buf[64];
	snprintf(buf, sizeof(buf), "%.2f", value);

	GeVector3d textDir = endPt - defPt;
	if (textDir.length() < 1e-10) textDir = GeVector3d::kXAxis;
	textDir.normalize();
	pWd->geometry().text(endPt, normal, textDir, txt, 1.0, 0.0, buf);

	return true;
}



Acad::ErrorStatus DbOrdinateDimension::subGetGeomExtents(DbExtents &extents) const
{
	auto* imp = DB_IMP_ORDINATEDIMENSION(this->m_pImpl);
	extents.addPoint(imp->origin);
	extents.addPoint(imp->definingPoint);
	extents.addPoint(imp->leaderEndPoint);
	extents.addPoint(DB_IMP_DIMENSION(this->m_pImpl)->textPosition);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbOrdinateDimension::subTransformBy(const GeMatrix3d& xform) {
	auto* imp = DB_IMP_ORDINATEDIMENSION(this->m_pImpl);
	imp->origin.transformBy(xform);
	imp->definingPoint.transformBy(xform);
	imp->leaderEndPoint.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->textPosition.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.transformBy(xform);
	DB_IMP_DIMENSION(this->m_pImpl)->normal.normalize();
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const {
	auto* imp = DB_IMP_ORDINATEDIMENSION(this->m_pImpl);
	GePoint3d pts[] = { imp->origin, imp->definingPoint, imp->leaderEndPoint, DB_IMP_DIMENSION(this->m_pImpl)->textPosition };
	for (int i = 0; i < 4; i++) {
		DbGripData* grip = new DbGripData();
		grip->setGripPoint(pts[i]);
		grips.append(grip);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const {
	if (osnapMode == Db::kOsModeEnd) {
		auto* imp = DB_IMP_ORDINATEDIMENSION(this->m_pImpl);
		snapPoints.append(imp->definingPoint);
		snapPoints.append(imp->leaderEndPoint);
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	auto* imp = DB_IMP_ORDINATEDIMENSION(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		switch (indices[i]) {
			case 0: imp->origin += offset; break;
			case 1: imp->definingPoint += offset; break;
			case 2: imp->leaderEndPoint += offset; break;
			case 3: DB_IMP_DIMENSION(this->m_pImpl)->textPosition += offset; break;
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbOrdinateDimension::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbOrdinateDimension::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}