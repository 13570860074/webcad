#include "DbBlockReference.h"
#include "DbObjectManager.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbBlockReferenceIdIterator.h"
#include "DbBlockTableRecordIterator.h"
#include "GePlane.h"
#include "GiWorldDraw.h"
#include "GiSubEntityTraits.h"
#include "GiWorldGeometry.h"
#include "GiEntityTraits.h"
#include "kernel.h"
#include "DbImpl.h"
#include "GiImpl.h"
#include "DbLine.h"

DbBlockReference::DbBlockReference()
{
	this->m_pImpl = new DbBlockReferenceImpl();
}
DbBlockReference::DbBlockReference(const GePoint3d &position, DbObjectId blockTableRec)
{
	this->m_pImpl = new DbBlockReferenceImpl();
	this->setPosition(position);
	this->setBlockTableRecord(blockTableRec);
}
DbBlockReference::~DbBlockReference()
{
}

DbObjectId DbBlockReference::blockTableRecord() const
{
	return DB_IMP_BLOCKREFERENCE(this->m_pImpl)->blockTableRecordId;
}
Acad::ErrorStatus DbBlockReference::setBlockTableRecord(DbObjectId _id)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isWriteEnabled() != true && this->isNotifyEnabled() != true)
		{
			break;
		}

		DbBlockTableRecord *pBlockTableRecord = (DbBlockTableRecord *)::kernel()->acdbObjectManager()->openDbObject(_id);
		if (pBlockTableRecord == NULL)
		{
			break;
		}
		if (DB_IMP_OBJECT(pBlockTableRecord->m_pImpl)->isBlockTableRecord() == false)
		{
			break;
		}

		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->blockTableRecordId = _id;

		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

GePoint3d DbBlockReference::position() const
{
	return DB_IMP_BLOCKREFERENCE(this->m_pImpl)->position;
}
Acad::ErrorStatus DbBlockReference::setPosition(const GePoint3d &_point)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->position = _point;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeScale3d DbBlockReference::scaleFactors() const
{
	return DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale;
}
Acad::ErrorStatus DbBlockReference::setScaleFactors(const GeScale3d &scale)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale = scale;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbBlockReference::rotation() const
{
	return DB_IMP_BLOCKREFERENCE(this->m_pImpl)->rotation;
}
Acad::ErrorStatus DbBlockReference::setRotation(double newVal)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->rotation = newVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector3d DbBlockReference::normal() const
{
	return DB_IMP_BLOCKREFERENCE(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbBlockReference::setNormal(const GeVector3d &newVal)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->normal = newVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbBlockReference::getPlane(GePlane &_plane, Db::Planarity &_planarity) const
{
	if (this->normal().length() < GeContext::gTol.equalVector())
	{
		_planarity = Db::Planarity::kNonPlanar;
		return Acad::ErrorStatus::eFail;
	}

	_planarity = Db::Planarity::kPlanar;
	_plane.set(this->position(), this->normal());
	return Acad::ErrorStatus::eOk;
}

GeMatrix3d DbBlockReference::blockTransform() const
{

	GeMatrix3d mat;
	mat.setToPlaneToWorld(GePlane(this->position(), this->normal()));

	GeMatrix3d tempMat;
	tempMat.setToRotation(this->rotation(), this->normal(), this->position());
	mat.preMultBy(tempMat);

	tempMat.setToScaling(this->scaleFactors(), this->position());
	mat.preMultBy(tempMat);

	return mat;
}
Acad::ErrorStatus DbBlockReference::setBlockTransform(const GeMatrix3d &mat)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		this->setPosition(mat.translation().asPoint());
		this->setScaleFactors(mat.scale());
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbBlockReference::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}
	GePoint3d position;
	pFiler->readPoint3d(&position);
	this->setPosition(position);

	double rotation;
	pFiler->readDouble(&rotation);
	this->setRotation(rotation);

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	this->setNormal(normal);

	GeScale3d scaleFactors;
	pFiler->readScale3d(&scaleFactors);
	this->setScaleFactors(scaleFactors);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockReference::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	pFiler->writePoint3d(this->position());

	pFiler->writeDouble(this->rotation());

	pFiler->writeVector3d(this->normal());

	pFiler->writeScale3d(this->scaleFactors());

	return Acad::ErrorStatus::eOk;
}
bool DbBlockReference::subWorldDraw(GiWorldDraw *pWd) const
{
	// 获取块表记录
	DbObjectId blockId = this->blockTableRecord();
	if (blockId.isNull()) {
		return true;
	}

	DbBlockTableRecord* pBlock = NULL;
	if (::acdbOpenObject(pBlock, blockId, Db::kForRead) != Acad::eOk || pBlock == NULL) {
		return true;
	}

	// 压入块变换矩阵
	GeMatrix3d xform = this->blockTransform();
	pWd->geometry().pushModelTransform(xform);

	// 遍历块表记录中的所有实体
	DbBlockTableRecordIterator* pIter = NULL;
	pBlock->newIterator(pIter);
	if (pIter != NULL) {
		for (pIter->start(); !pIter->done(); pIter->step()) {
			DbEntity* pEntity = NULL;
			pIter->getEntity(pEntity);
			if (pEntity != NULL) {
				// 嵌套绘制子实体
				pEntity->subWorldDraw(pWd);
				pEntity->close();
			}
		}
		delete pIter;
	}

	// 弹出块变换矩阵
	pWd->geometry().popModelTransform();

	pBlock->close();
	return true;
}


Acad::ErrorStatus DbBlockReference::subGetGeomExtents(DbExtents &extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockReference::subTransformBy(const GeMatrix3d &xform)
{

	// 获得当前参照X轴
	GeMatrix3d mat;
	mat.setToPlaneToWorld(GePlane(GePoint3d::kOrigin, this->normal()));
	GeVector3d referXAxis = GeVector3d::kXAxis;
	referXAxis.transformBy(mat);

	// 得到变换前的原始三个坐标
	GePoint3d origin = DB_IMP_BLOCKREFERENCE(this->m_pImpl)->position;
	GePoint3d xOrigin = origin;
	xOrigin += (referXAxis * 100.0);
	xOrigin = xOrigin.rotateBy(DB_IMP_BLOCKREFERENCE(this->m_pImpl)->rotation, this->normal());
	GePoint3d yOrigin = xOrigin;
	yOrigin = yOrigin.rotateBy(PI / 2.0, this->normal());
	GePoint3d zOrigin = origin;
	zOrigin += (this->normal() * 100.0);

	// 计算缩放值
	if (xform.entry[0][0] < 0.0001)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale.sx = 0.0 - DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale.sx;
	}
	if (xform.entry[1][1] < 0.0001)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale.sy = 0.0 - DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale.sy;
	}
	if (xform.entry[2][2] < 0.0001)
	{
		DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale.sz = 0.0 - DB_IMP_BLOCKREFERENCE(this->m_pImpl)->scale.sz;
	}

	// 获得变换后的坐标
	GePoint3d p = origin;
	GePoint3d px = xOrigin;
	GePoint3d py = yOrigin;
	GePoint3d pz = zOrigin;
	p = p.transformBy(xform);
	px = px.transformBy(xform);
	py = py.transformBy(xform);
	pz = pz.transformBy(xform);

	// 计算变换后的Z轴
	DB_IMP_BLOCKREFERENCE(this->m_pImpl)->normal = pz - p;
	DB_IMP_BLOCKREFERENCE(this->m_pImpl)->normal.normalize();

	// 计算变换后的X轴
	mat.setToIdentity();
	mat.setToPlaneToWorld(GePlane(GePoint3d::kOrigin, DB_IMP_BLOCKREFERENCE(this->m_pImpl)->normal));
	referXAxis = GeVector3d::kXAxis;
	referXAxis.transformBy(mat);

	// 计算变换后的弧度
	GeVector3d v = px - p;
	v.normalize();
	DB_IMP_BLOCKREFERENCE(this->m_pImpl)->rotation = referXAxis.angleToCCW(v, DB_IMP_BLOCKREFERENCE(this->m_pImpl)->normal);
	double angle = DB_IMP_BLOCKREFERENCE(this->m_pImpl)->rotation * 180.0 / PI;

	// 设置当前坐标
	DB_IMP_BLOCKREFERENCE(this->m_pImpl)->position = p;

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockReference::subGetGripPoints(
	DbGripDataPtrArray &grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d &curViewDir,
	const int bitflags) const
{

	DbGripData *gripData = new DbGripData();
	gripData->setGripPoint(this->position());
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockReference::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d &pickPoint,
	const GePoint3d &lastPoint,
	const GeMatrix3d &viewXform,
	GePoint3dArray &snapPoints,
	DbIntArray &geomIds) const
{

	
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockReference::subMoveGripPointsAt(const DbIntArray &indices, const GeVector3d &offset)
{

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{
			DB_IMP_BLOCKREFERENCE(this->m_pImpl)->position += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockReference::subMoveGripPointsAt(const DbVoidPtrArray &gripAppData, const GeVector3d &offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockReference::subIntersectWith(const DbEntity *pEnt, Db::Intersect intType, GePoint3dArray &points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{

	return Acad::ErrorStatus::eOk;
}
