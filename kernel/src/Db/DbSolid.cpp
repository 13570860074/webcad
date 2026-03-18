#include "DbSolid.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbDwgFiler.h"
#include "DbGripData.h"
#include "GeLineSeg3d.h"
#include "GeLine3d.h"
#include "DbImpl.h"


DbSolid::DbSolid() {
	this->m_pImpl = new DbSolidImpl();
}
DbSolid::DbSolid(const GePoint3d& pt0, const GePoint3d& pt1, const GePoint3d& pt2, const GePoint3d& pt3) {
	DB_IMP_SOLID(this->m_pImpl)->p0 = pt0;
	DB_IMP_SOLID(this->m_pImpl)->p1 = pt1;
	DB_IMP_SOLID(this->m_pImpl)->p2 = pt2;
	DB_IMP_SOLID(this->m_pImpl)->p3 = pt3;
	DB_IMP_SOLID(this->m_pImpl)->countPoint = 4;
}
DbSolid::DbSolid(const GePoint3d& pt0, const GePoint3d& pt1, const GePoint3d& pt2) {
	DB_IMP_SOLID(this->m_pImpl)->p0 = pt0;
	DB_IMP_SOLID(this->m_pImpl)->p1 = pt1;
	DB_IMP_SOLID(this->m_pImpl)->p2 = pt2;
	DB_IMP_SOLID(this->m_pImpl)->countPoint = 3;
}
DbSolid::~DbSolid() {

}





Acad::ErrorStatus DbSolid::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	int countPoint = 0;
	pFiler->readInt32(&countPoint);
	DB_IMP_SOLID(this->m_pImpl)->countPoint = countPoint;

	for (int i = 0; i < countPoint; i++) {
		GePoint3d pt;
		pFiler->readPoint3d(&pt);
		this->setPointAt(i, pt);
	}

	double elevation = 0.0;
	pFiler->readDouble(&elevation);
	this->setElevation(elevation);

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	this->setNormal(normal);

	double thickness = 0.0;
	pFiler->readDouble(&thickness);
	this->setThickness(thickness);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSolid::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeInt32(DB_IMP_SOLID(this->m_pImpl)->countPoint);
	for (int i = 0; i < DB_IMP_SOLID(this->m_pImpl)->countPoint; i++) {
		GePoint3d pt;
		this->getPointAt(i, pt);
		pFiler->writePoint3d(pt);
	}

	pFiler->writeDouble(this->elevation());
	pFiler->writeVector3d(this->normal());
	pFiler->writeDouble(this->thickness());

	return Acad::ErrorStatus::eOk;
}
bool DbSolid::subWorldDraw(GiWorldDraw* pWd) const {

	// 计算得到顶点数量和顶点列表
	Adesk::UInt32 nbVertex = DB_IMP_SOLID(this->m_pImpl)->countPoint;
	GePoint3d* pVertexList = new GePoint3d[nbVertex];
	for (int i = 0;; i++) {
		GePoint3d pt;
		if (this->getPointAt(i, pt) != Acad::ErrorStatus::eOk) {
			break;
		}
		pVertexList[i] = pt;
	}

	// 构建片体
	Adesk::UInt32 faceListSize = 0;
	if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 3) {
		faceListSize = 4;
	}
	else if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4) {
		faceListSize = 4 * 2;
	}
	Adesk::Int32* pFaceList = new Adesk::Int32[faceListSize];
	if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 3) {
		pFaceList[0] = 3;
		pFaceList[1] = 0;
		pFaceList[2] = 1;
		pFaceList[3] = 2;
	}
	else if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4) {
		pFaceList[0] = 3;
		pFaceList[1] = 0;
		pFaceList[2] = 1;
		pFaceList[3] = 2;

		pFaceList[4] = 3;
		pFaceList[5] = 1;
		pFaceList[6] = 2;
		pFaceList[7] = 3;
	}

	pWd->geometry().shell(nbVertex, pVertexList, faceListSize, pFaceList);

	return false;
}



Acad::ErrorStatus DbSolid::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSolid::subTransformBy(const GeMatrix3d& xform)
{
	DB_IMP_SOLID(this->m_pImpl)->p0.transformBy(xform);
	DB_IMP_SOLID(this->m_pImpl)->p1.transformBy(xform);
	DB_IMP_SOLID(this->m_pImpl)->p2.transformBy(xform);
	DB_IMP_SOLID(this->m_pImpl)->p3.transformBy(xform);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbSolid::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{

	DbGripData* gripData = new DbGripData();
	gripData->setGripPoint(DB_IMP_SOLID(this->m_pImpl)->p0);
	grips.append(gripData);

	gripData = new DbGripData();
	gripData->setGripPoint(DB_IMP_SOLID(this->m_pImpl)->p1);
	grips.append(gripData);

	gripData = new DbGripData();
	gripData->setGripPoint(DB_IMP_SOLID(this->m_pImpl)->p2);
	grips.append(gripData);

	if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4)
	{
		gripData = new DbGripData();
		gripData->setGripPoint(DB_IMP_SOLID(this->m_pImpl)->p3);
		grips.append(gripData);
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbSolid::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{

	if (osnapMode == Db::OsnapMode::kOsModeNear)
	{

	}
	else if (osnapMode == Db::OsnapMode::kOsModeMid)
	{
		snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p0 + (DB_IMP_SOLID(this->m_pImpl)->p1 - DB_IMP_SOLID(this->m_pImpl)->p0) / 2.0);
		snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p1 + (DB_IMP_SOLID(this->m_pImpl)->p2 - DB_IMP_SOLID(this->m_pImpl)->p1) / 2.0);
		snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p2 + (DB_IMP_SOLID(this->m_pImpl)->p0 - DB_IMP_SOLID(this->m_pImpl)->p2) / 2.0);
		if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4)
		{
			snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p3 + (DB_IMP_SOLID(this->m_pImpl)->p0 - DB_IMP_SOLID(this->m_pImpl)->p3) / 2.0);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{
		snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p0);
		snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p1);
		snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p2);
		if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4) {
			snapPoints.append(DB_IMP_SOLID(this->m_pImpl)->p3);
		}
	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{
		if (lastPoint.isEqualTo(pickPoint) == false) {

			GeLineSeg3d line;
			GePoint3d vertical;

			line.set(DB_IMP_SOLID(this->m_pImpl)->p0, DB_IMP_SOLID(this->m_pImpl)->p1);
			vertical = GeLine3d::vertical(pickPoint, line);
			if (line.isOn(vertical) == true) {
				snapPoints.append(vertical);
			}

			line.set(DB_IMP_SOLID(this->m_pImpl)->p1, DB_IMP_SOLID(this->m_pImpl)->p2);
			vertical = GeLine3d::vertical(pickPoint, line);
			if (line.isOn(vertical) == true) {
				snapPoints.append(vertical);
			}

			line.set(DB_IMP_SOLID(this->m_pImpl)->p2, DB_IMP_SOLID(this->m_pImpl)->p3);
			vertical = GeLine3d::vertical(pickPoint, line);
			if (line.isOn(vertical) == true) {
				snapPoints.append(vertical);
			}

			if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4) {
				line.set(DB_IMP_SOLID(this->m_pImpl)->p3, DB_IMP_SOLID(this->m_pImpl)->p0);
				vertical = GeLine3d::vertical(pickPoint, line);
				if (line.isOn(vertical) == true) {
					snapPoints.append(vertical);
				}
			}

		}
	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbSolid::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_SOLID(this->m_pImpl)->p0 += offset;
		}
		else if (indices.at(i) == 1)
		{
			DB_IMP_SOLID(this->m_pImpl)->p1 += offset;
		}
		else if (indices.at(i) == 2)
		{
			DB_IMP_SOLID(this->m_pImpl)->p2 += offset;
		}
		else if (indices.at(i) == 3)
		{
			DB_IMP_SOLID(this->m_pImpl)->p3 += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSolid::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSolid::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	return Acad::ErrorStatus::eOk;

}




Acad::ErrorStatus DbSolid::getPointAt(Adesk::UInt16 idx, GePoint3d& pntRes) const {
	if (idx == 0) {
		pntRes = DB_IMP_SOLID(this->m_pImpl)->p0;
		return Acad::ErrorStatus::eOk;
	}
	else  if (idx == 1) {
		pntRes = DB_IMP_SOLID(this->m_pImpl)->p1;
		return Acad::ErrorStatus::eOk;
	}
	else  if (idx == 2) {
		pntRes = DB_IMP_SOLID(this->m_pImpl)->p2;
		return Acad::ErrorStatus::eOk;
	}
	else  if (idx == 3) {
		if (DB_IMP_SOLID(this->m_pImpl)->countPoint == 4) {
			pntRes = DB_IMP_SOLID(this->m_pImpl)->p3;
			return Acad::ErrorStatus::eOk;
		}
		else {
			return Acad::ErrorStatus::eFail;
		}
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbSolid::setPointAt(Adesk::UInt16 idx, const GePoint3d& _pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (idx == 0) {
			DB_IMP_SOLID(this->m_pImpl)->p0 = _pt;
		}
		else  if (idx == 1) {
			DB_IMP_SOLID(this->m_pImpl)->p1 = _pt;
		}
		else  if (idx == 2) {
			DB_IMP_SOLID(this->m_pImpl)->p2 = _pt;
		}
		else  if (idx == 3) {
			DB_IMP_SOLID(this->m_pImpl)->p3 = _pt;
			DB_IMP_SOLID(this->m_pImpl)->countPoint = 4;
		}

		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbSolid::thickness() const {
	return DB_IMP_SOLID(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbSolid::setThickness(double _thickness) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SOLID(this->m_pImpl)->thickness = _thickness;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector3d DbSolid::normal() const {
	return DB_IMP_SOLID(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbSolid::setNormal(const GeVector3d& _normal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SOLID(this->m_pImpl)->normal = _normal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbSolid::elevation() const {
	return DB_IMP_SOLID(this->m_pImpl)->elevation;
}
void DbSolid::setElevation(double _elevation) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_SOLID(this->m_pImpl)->elevation = _elevation;
	}
}
