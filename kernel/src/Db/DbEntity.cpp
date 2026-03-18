#include "AcDebug.h"
#include "ApDocument.h"
#include "ApDocManager.h"
#include "DbEntity.h"
#include "DbObjectId.h"
#include "DbDatabase.h"
#include "DbDwgCopyFiler.h"
#include "DbLinetypeTable.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbLayerTableIterator.h"
#include "DbLinetypeTableRecord.h"
#include "DbLinetypeTableIterator.h"
#include "DbHostApplicationServices.h"
#include "GiEntityTraits.h"
#include "GiSubEntityTraits.h"
#include "GiWorldDrawManager.h"
#include "GiEntityHighlander.h"
#include "GiWorldDraw.h"
#include "ReRendererManager.h"
#include "RxClassManager.h"
#include "kernel.h"
#include "DbImpl.h"

DbEntity::DbEntity()
{
}
DbEntity::~DbEntity()
{
}

Acad::ErrorStatus DbEntity::dwgInFields(DbDwgFiler* pFiler)
{

	Acad::ErrorStatus es = DbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	if (pFiler->filerType() == Db::FilerType::kFileFiler)
	{
		DbHandle handle;

		pFiler->readDbHandle(&handle);
		DbObjectId blockId = NULL;
		this->database()->getDbObjectId(blockId, false, handle);
		DB_IMP_ENTITY(this->m_pImpl)->blockId.setFromOldId(blockId.asOldId());

		pFiler->readDbHandle(&handle);
		DbObjectId layerId = NULL;
		if (this->database()->getDbObjectId(layerId, false, handle) == Acad::ErrorStatus::eOk)
		{
			DB_IMP_ENTITY(this->m_pImpl)->layerId.setFromOldId(layerId.asOldId());
		}
	}
	else if (pFiler->filerType() == Db::FilerType::kUndoFiler)
	{
		unsigned int blockId = 0;
		pFiler->readUInt32(&blockId);
		DB_IMP_ENTITY(this->m_pImpl)->blockId.setFromOldId(blockId);

		unsigned int layerId = 0;
		pFiler->readUInt32(&layerId);
		DB_IMP_ENTITY(this->m_pImpl)->layerId.setFromOldId(layerId);
	}
	else if (pFiler->filerType() == Db::FilerType::kCopyFiler)
	{
		unsigned int blockId = 0;
		pFiler->readUInt32(&blockId);
		DB_IMP_ENTITY(this->m_pImpl)->blockId.setFromOldId(blockId);

		unsigned int layerId = 0;
		pFiler->readUInt32(&layerId);
		DB_IMP_ENTITY(this->m_pImpl)->layerId.setFromOldId(layerId);
	}

	int colorIndex = 0;
	pFiler->readInt32(&colorIndex);
	DB_IMP_ENTITY(this->m_pImpl)->color.setColorIndex(colorIndex);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::dwgOutFields(DbDwgFiler* pFiler) const
{

	Acad::ErrorStatus es = DbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	if (pFiler->filerType() == Db::FilerType::kFileFiler)
	{
		pFiler->writeDbHandle(this->blockId().handle());
		pFiler->writeDbHandle(this->layerId().handle());
	}
	else if (pFiler->filerType() == Db::FilerType::kUndoFiler)
	{
		pFiler->writeUInt32(this->blockId().asOldId());
		pFiler->writeUInt32(this->layerId().asOldId());
	}
	else if (pFiler->filerType() == Db::FilerType::kCopyFiler)
	{
		pFiler->writeUInt32(0);
		pFiler->writeUInt32(this->layerId().asOldId());
	}

	// pFiler->writeUInt32(this->color().color());
	pFiler->writeInt32(this->colorIndex());

	return Acad::ErrorStatus::eOk;
}
bool DbEntity::subWorldDraw(GiWorldDraw* pWd) const { return false; }

DbObjectId DbEntity::blockId() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->blockId;
}

CmColor DbEntity::color() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->color;
}
Acad::ErrorStatus DbEntity::setColor(const CmColor& color)
{
	return this->setColor(color, true);
}
Acad::ErrorStatus DbEntity::setColor(const CmColor& color, bool doSubents)
{
	if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
	{
		return Acad::ErrorStatus::eFail;
	}

	DB_IMP_ENTITY(this->m_pImpl)->color = color;
	return Acad::ErrorStatus::eOk;
}
UInt16 DbEntity::colorIndex() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->color.colorIndex();
}
Acad::ErrorStatus DbEntity::setColorIndex(UInt16 color)
{
	return this->setColorIndex(color, true);
}
Acad::ErrorStatus DbEntity::setColorIndex(UInt16 color, bool doSubents)
{
	if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
	{
		return Acad::ErrorStatus::eFail;
	}

	DB_IMP_ENTITY(this->m_pImpl)->color.setColorIndex(color);
	return Acad::ErrorStatus::eOk;
}
CmEntityColor DbEntity::entityColor() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->color.entityColor();
}

CmTransparency DbEntity::transparency() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->Transparency;
}
Acad::ErrorStatus DbEntity::setTransparency(const CmTransparency& trans, bool doSubents)
{
	if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
	{
		return Acad::ErrorStatus::eFail;
	}

	DB_IMP_ENTITY(this->m_pImpl)->Transparency = trans;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::layer(AcString& sName) const
{
	DbLayerTableRecord* pLayerTableRecord = NULL;
	if (::acdbOpenObject(pLayerTableRecord, DB_IMP_ENTITY(this->m_pImpl)->layerId, Db::OpenMode::kForRead) != Acad::ErrorStatus::eOk)
	{
		sName = "";
		return Acad::ErrorStatus::eFail;
	}
	char* pName = NULL;
	pLayerTableRecord->getName(pName);
	sName = pName;
	return Acad::ErrorStatus::eOk;
}
ACHAR* DbEntity::layer() const
{
	AcString sName;
	this->layer(sName);
	ACHAR* buf = NULL;
	::acutNewString(sName.constPtr(), buf);
	return buf;
}
#if EMSDK
emstring DbEntity::emsdk_layer() const
{
	AcString str;
	this->layer(str);
	return str.constPtr();
}
#endif

DbObjectId DbEntity::layerId() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->layerId;
}

Acad::ErrorStatus DbEntity::setLayer(const ACHAR* newVal, Adesk::Boolean doSubents, bool allowHiddenLayer)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		// 获得数据库
		DbDatabase* pDatabase = this->database();
		if (pDatabase == NULL)
		{
			pDatabase = acdbHostApplicationServices()->workingDatabase();
		}
		if (pDatabase == NULL)
		{
			break;
		}

		// 获得层表
		DbLayerTable* pLayerTable = NULL;
		this->database()->getLayerTable(pLayerTable);
		if (pLayerTable == NULL)
		{
			break;
		}

		// 获得图层
		DbLayerTableRecord* pLayerTableRecord = NULL;
		pLayerTable->getAt(newVal, pLayerTableRecord);
		pLayerTable->close();
		if (pLayerTableRecord == NULL)
		{
			break;
		}

		// 获得图层id
		DbObjectId layerId = pLayerTableRecord->objectId();

		// 关闭
		pLayerTableRecord->close();

		es = this->setLayer(layerId, doSubents, allowHiddenLayer);

	} while (false);

	return es;
}
Acad::ErrorStatus DbEntity::setLayer(DbObjectId newVal, Adesk::Boolean doSubents, bool allowHiddenLayer)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		if (this->database() != NULL)
		{
			// 获得层表记录
			DbLayerTableRecord* pLayerTableRecord = NULL;
			::acdbOpenObject(pLayerTableRecord, newVal);
			if (pLayerTableRecord != NULL)
			{
				// 判断数据库是否相同
				if (pLayerTableRecord->database() == this->database())
				{
					// 设置图层
					if (pLayerTableRecord->isHidden() == false || allowHiddenLayer == true)
					{
						DB_IMP_ENTITY(this->m_pImpl)->layerId = newVal;
						es = Acad::ErrorStatus::eOk;
					}
				}

				pLayerTableRecord->close();
			}
		}
		else
		{
			DB_IMP_ENTITY(this->m_pImpl)->layerId = newVal;
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus DbEntity::setLayer(DbObjectId newVal)
{
	return this->setLayer(newVal, true, false);
}
Acad::ErrorStatus DbEntity::setLayer(DbObjectId newVal, Adesk::Boolean doSubents)
{
	return this->setLayer(newVal, doSubents, false);
}

#if EMSDK
Acad::ErrorStatus DbEntity::emsdk_setLayer(const emstring& newVal)
{
	return this->emsdk_setLayer(newVal, true, false);
}
Acad::ErrorStatus DbEntity::emsdk_setLayer(const emstring& newVal, Adesk::Boolean doSubents)
{
	return this->emsdk_setLayer(newVal, doSubents, false);
}
Acad::ErrorStatus DbEntity::emsdk_setLayer(const emstring& newVal, Adesk::Boolean doSubents, bool allowHiddenLayer)
{
	return this->setLayer(newVal.c_str(), doSubents, allowHiddenLayer);
}
#endif

Acad::ErrorStatus DbEntity::linetype(AcString& sName) const
{
	sName = this->linetype();
	if (sName != "")
	{
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
ACHAR* DbEntity::linetype() const
{
	DbLinetypeTableRecord* pLinetypeTableRecord = NULL;
	if (::acdbOpenObject(pLinetypeTableRecord, DB_IMP_ENTITY(this->m_pImpl)->linetypeId, Db::OpenMode::kForRead) != Acad::ErrorStatus::eOk)
	{
		return "";
	}
	ACHAR* pName = NULL;
	pLinetypeTableRecord->getName(pName);
	return pName;
}
#if EMSDK
emstring DbEntity::emsdk_linetype() const
{
	AcString sName;
	this->linetype(sName);
	return sName.constPtr();
}
#endif
DbObjectId DbEntity::linetypeId() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->linetypeId;
}

Acad::ErrorStatus DbEntity::setLinetype(const ACHAR* newVal, Adesk::Boolean doSubents)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		// 获得数据库
		DbDatabase* pDatabase = this->database();
		if (pDatabase == NULL)
		{
			pDatabase = acdbHostApplicationServices()->workingDatabase();
		}
		if (pDatabase == NULL)
		{
			break;
		}

		// 获得层表
		DbLinetypeTable* pLinetypeTable = NULL;
		this->database()->getLinetypeTable(pLinetypeTable);
		if (pLinetypeTable == NULL)
		{
			break;
		}

		// 获得线型
		DbLinetypeTableRecord* pLinetypeTableRecord = NULL;
		pLinetypeTable->getAt(newVal, pLinetypeTableRecord);
		pLinetypeTable->close();
		if (pLinetypeTableRecord == NULL)
		{
			break;
		}

		// 获得id
		DbObjectId LinetypeId = pLinetypeTableRecord->objectId();

		// 关闭
		pLinetypeTableRecord->close();

		es = this->setLinetype(LinetypeId, doSubents);

	} while (false);

	return es;
}
Acad::ErrorStatus DbEntity::setLinetype(DbObjectId newVal)
{
	return this->setLinetype(newVal, true);
}
Acad::ErrorStatus DbEntity::setLinetype(DbObjectId newVal, Adesk::Boolean doSubents)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		if (this->database() != NULL)
		{
			// 获得线型表记录
			DbLinetypeTableRecord* pLinetypeTableRecord = NULL;
			::acdbOpenObject(pLinetypeTableRecord, newVal);
			if (pLinetypeTableRecord != NULL)
			{
				// 判断数据库是否相同
				if (pLinetypeTableRecord->database() == this->database())
				{
					DB_IMP_ENTITY(this->m_pImpl)->linetypeId = newVal;
					es = Acad::ErrorStatus::eOk;
				}

				pLinetypeTableRecord->close();
			}
		}
		else
		{
			DB_IMP_ENTITY(this->m_pImpl)->linetypeId = newVal;
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}

#if EMSDK
Acad::ErrorStatus DbEntity::emsdk_setLinetype(const emstring& newVal)
{
	return this->emsdk_setLinetype(newVal, true);
}
Acad::ErrorStatus DbEntity::emsdk_setLinetype(const emstring& newVal, bool doSubents)
{
	return this->setLinetype(newVal.c_str(), doSubents);
}
#endif

double DbEntity::linetypeScale() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->linetypeScale;
}
Acad::ErrorStatus DbEntity::setLinetypeScale(double newval, bool doSubents)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		DB_IMP_ENTITY(this->m_pImpl)->linetypeScale = newval;
		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

Db::Visibility DbEntity::visibility() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->visibility;
}
Acad::ErrorStatus DbEntity::setVisibility(Db::Visibility newVal, bool doSubents)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		DB_IMP_ENTITY(this->m_pImpl)->visibility = newVal;
		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

Db::LineWeight DbEntity::lineWeight() const
{
	return DB_IMP_ENTITY(this->m_pImpl)->lineWeight;
}

Acad::ErrorStatus DbEntity::setLineWeight(Db::LineWeight newVal, bool doSubents)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
		{
			break;
		}

		DB_IMP_ENTITY(this->m_pImpl)->lineWeight = newVal;
		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

Db::CollisionType DbEntity::collisionType() const
{
	return Db::CollisionType::kCollisionTypeNone;
}

bool DbEntity::castShadows() const
{
	return false;
}
RxObject* DbEntity::clone() const
{
	return DbObject::clone();
}

void DbEntity::setCastShadows(bool newVal)
{
}

bool DbEntity::receiveShadows() const
{
	return false;
}

void DbEntity::setReceiveShadows(bool newVal) {}

Acad::ErrorStatus DbEntity::setPropertiesFrom(const DbEntity* pEntity, bool doSubents)
{
	return Acad::ErrorStatus::eOk;
}

void DbEntity::getEcs(GeMatrix3d& retVal) const
{
}

Acad::ErrorStatus DbEntity::getGeomExtents(DbExtents& extents) const
{
	return this->subGetGeomExtents(extents);
}

// Subentity acquisition and manipulation
//
Acad::ErrorStatus DbEntity::addSubentPaths(const DbFullSubentPathArray& newPaths)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::deleteSubentPaths(const DbFullSubentPathArray& paths)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::transformSubentPathsBy(
	const DbFullSubentPathArray& paths,
	const GeMatrix3d& xform)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getGripPointsAtSubentPath(
	const DbFullSubentPath& path,
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::moveGripPointsAtSubentPaths(
	const DbFullSubentPathArray& paths,
	const DbVoidPtrArray& gripAppData,
	const GeVector3d& offset,
	const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getSubentPathGeomExtents(
	const DbFullSubentPath& path,
	DbExtents& extents) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getSubentPathsAtGsMarker(
	Db::SubentType type,
	Adesk::GsMarker gsMark,
	const GePoint3d& pickPoint,
	const GeMatrix3d& viewXform,
	int& numPaths,
	DbFullSubentPath*& subentPaths,
	int numInserts,
	DbObjectId* entAndInsertStack) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getGsMarkersAtSubentPath(
	const DbFullSubentPath& subPath,
	AcArray<Adesk::GsMarker>& gsMarkers) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::highlight(
	const DbFullSubentPath& subId,
	const bool highlightAll) const
{

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ENTITY(this->m_pImpl)->highlight = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEntity::unhighlight(
	const DbFullSubentPath& subId,
	const bool highlightAll) const
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		DB_IMP_ENTITY(this->m_pImpl)->highlight = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
DbEntity* DbEntity::subentPtr(const DbFullSubentPath& id) const
{
	return NULL;
}

Acad::ErrorStatus DbEntity::getOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds,
	const GeMatrix3d& insertionMat) const
{
	return Acad::ErrorStatus::eOk;
}

bool DbEntity::isContentSnappable() const
{
	return false;
}

Acad::ErrorStatus DbEntity::getGripPoints(
	GePoint3dArray& gripPoints,
	DbIntArray& osnapModes,
	DbIntArray& geomIds) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::moveGripPointsAt(
	const DbIntArray& indices,
	const GeVector3d& offset)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::moveGripPointsAt(
	const DbVoidPtrArray& gripAppData,
	const GeVector3d& offset,
	const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::getStretchPoints(GePoint3dArray& stretchPoints) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::moveStretchPointsAt(
	const DbIntArray& indices,
	const GeVector3d& offset)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::intersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::intersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	const GePlane& projPlane,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::boundingBoxIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::boundingBoxIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	const GePlane& projPlane,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}

void DbEntity::list() const {}

Acad::ErrorStatus DbEntity::transformBy(const GeMatrix3d& xform)
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true)
	{
		this->subTransformBy(xform);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbEntity::getTransformedCopy(const GeMatrix3d& xform, DbEntity*& pEnt) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::explode(DbVoidPtrArray& entitySet) const
{
	return Acad::ErrorStatus::eOk;
}
void DbEntity::recordGraphicsModified(bool setModified)
{
}

Acad::ErrorStatus DbEntity::draw()
{
	// 获得文档对象
	ApDocument* pDocument = acDocManager->document(this->database());
	if (pDocument != NULL)
	{
		// 获得工作绘图器
		GiWorldDraw* pWorldDraw = ::kernel()->acgiWorldDrawManager()->worldDraw(pDocument);

		// 设置文档
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).setDocument(pDocument);

		// 设置实体
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).setRootEntity(this);
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).setEntity(this);

		// 初始化矩阵
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).openMatrix3d(false);
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).setMatrix3d(GeMatrix3d::kIdentity);

		// 初始化
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).init();

		// 删除实体桩对象
		if (DB_IMP_ENTITY(this->m_pImpl)->stake != NULL) {
			DbEntityStake::free(DB_IMP_ENTITY(this->m_pImpl)->stake);
			DB_IMP_ENTITY(this->m_pImpl)->stake = NULL;
		}

		// 是否可见
		if (((GiEntityTraits&)pWorldDraw->subEntityTraits()).isVisibility() == false)
		{
			return Acad::ErrorStatus::eOk;
		}

		// 创建实体桩对象
		DB_IMP_ENTITY(this->m_pImpl)->stake = new DbEntityStake();
		DB_IMP_ENTITY(this->m_pImpl)->stake->setEntity(this);
		((GiEntityTraits&)pWorldDraw->subEntityTraits()).setStake(DB_IMP_ENTITY(this->m_pImpl)->stake);

		// 绘制
		this->subWorldDraw(pWorldDraw);
	}
	return Acad::ErrorStatus::eOk;
}
void DbEntity::close()
{
	DbObject::close();
}

void DbEntity::saveAs(GiWorldDraw* mode, Db::SaveType st)
{
}

Acad::ErrorStatus DbEntity::getCompoundObjectTransform(GeMatrix3d& xMat) const
{
	return Acad::ErrorStatus::eOk;
}

void DbEntity::setDatabaseDefaults()
{
}
void DbEntity::setDatabaseDefaults(DbDatabase* pDb)
{
}

void DbEntity::subSetDatabaseDefaults(DbDatabase* pDb)
{
}

GiDrawable* DbEntity::drawable()
{
	return this;
}

// GiDrawable interface
//
bool DbEntity::bounds(DbExtents& _bounds) const
{
	return false;
}

bool DbEntity::cloneMeForDragging()
{
	return false;
}
bool DbEntity::hideMeForDragging() const
{
	return false;
}

void DbEntity::dragStatus(const Db::DragStat status)
{
}
void DbEntity::gripStatus(const Db::GripStat status)
{
}
void DbEntity::subentGripStatus(const Db::GripStat status, const DbFullSubentPath& subentity)
{
}

Acad::ErrorStatus DbEntity::getGripEntityUCS(
	const void* pGripAppData,
	GeVector3d& normalVec,
	GePoint3d& origin,
	GeVector3d& xAxis) const
{
	return Acad::ErrorStatus::eOk;
}

// DbOsnapOverrule
Acad::ErrorStatus DbEntity::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds,
	const GeMatrix3d& insertionMat) const
{
	return Acad::ErrorStatus::eOk;
}

bool DbEntity::subIsContentSnappable() const
{
	return false;
}

// DbTransformOverrule
Acad::ErrorStatus DbEntity::subTransformBy(const GeMatrix3d& xform)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::subGetTransformedCopy(const GeMatrix3d& xform, DbEntity*& pEnt) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::subExplode(DbVoidPtrArray& entitySet) const
{
	return Acad::ErrorStatus::eOk;
}
bool DbEntity::subCloneMeForDragging()
{
	return false;
}
bool DbEntity::subHideMeForDragging() const
{
	return false;
}

// DbGripOverrule
Acad::ErrorStatus DbEntity::subGetGripPoints(
	GePoint3dArray& gripPoints,
	DbIntArray& osnapModes,
	DbIntArray& geomIds) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subMoveGripPointsAt(const DbIntArray& indices,
	const GeVector3d& offset)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData,
	const GeVector3d& offset,
	const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetStretchPoints(
	GePoint3dArray& stretchPoints) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subMoveStretchPointsAt(
	const DbIntArray& indices,
	const GeVector3d& offset)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetGeomExtents(DbExtents& extents) const
{
	return Acad::ErrorStatus::eOk;
}

// DbSubentityOverrule related
Acad::ErrorStatus DbEntity::subAddSubentPaths(const DbFullSubentPathArray& newPaths)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subDeleteSubentPaths(const DbFullSubentPathArray& paths)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subTransformSubentPathsBy(
	const DbFullSubentPathArray& paths,
	const GeMatrix3d& xform)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetGripPointsAtSubentPath(
	const DbFullSubentPath& path,
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subMoveGripPointsAtSubentPaths(
	const DbFullSubentPathArray& paths,
	const DbVoidPtrArray& gripAppData,
	const GeVector3d& offset,
	const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetSubentPathGeomExtents(const DbFullSubentPath& path, DbExtents& extents) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetSubentPathsAtGsMarker(
	Db::SubentType type,
	Adesk::GsMarker gsMark,
	const GePoint3d& pickPoint,
	const GeMatrix3d& viewXform,
	int& numPaths,
	DbFullSubentPath*& subentPaths,
	int numInserts,
	DbObjectId* entAndInsertStack) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetGsMarkersAtSubentPath(
	const DbFullSubentPath& subPath,
	AcArray<Adesk::GsMarker>& gsMarkers) const
{
	return Acad::ErrorStatus::eOk;
}

DbEntity* DbEntity::subSubentPtr(const DbFullSubentPath& id) const
{
	return NULL;
}

void DbEntity::subGripStatus(const Db::GripStat status)
{
}

void DbEntity::subSubentGripStatus(const Db::GripStat status, const DbFullSubentPath& subentity)
{
}

// DbHighlightOverrule related

Acad::ErrorStatus DbEntity::subHighlight(
	const DbFullSubentPath& subId,
	const bool highlightAll) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subUnhighlight(
	const DbFullSubentPath& subId,
	const bool highlightAll) const
{
	return Acad::ErrorStatus::eOk;
}

// DbVisibilityOverrule related
Db::Visibility DbEntity::subVisibility() const
{
	return Db::Visibility::kVisible;
}
Acad::ErrorStatus DbEntity::subSetVisibility(Db::Visibility newVal, bool doSubents)
{
	return Acad::ErrorStatus::eOk;
}

// DbGeometryOverrule related

Acad::ErrorStatus DbEntity::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subIntersectWith(
	const DbEntity* pEnt,
	Db::Intersect intType,
	const GePlane& projPlane,
	GePoint3dArray& points,
	Adesk::GsMarker thisGsMarker,
	Adesk::GsMarker otherGsMarker) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetGripEntityUCS(
	const void* pGripAppData,
	GeVector3d& normalVec,
	GePoint3d& origin,
	GeVector3d& xAxis) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::subGetCompoundObjectTransform(GeMatrix3d& xMat) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbEntity::pushHighlight(const DbFullSubentPath& subId, GiHighlightStyle highlightStyle)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::popHighlight(const DbFullSubentPath& subId)
{
	return Acad::ErrorStatus::eOk;
}
GiHighlightStyle DbEntity::highlightState(const DbFullSubentPath& subId)
{
	return GiHighlightStyle::kGiHighlightNone;
}

Acad::ErrorStatus DbEntity::subPushHighlight(const DbFullSubentPath& subId, GiHighlightStyle highlightStyle)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbEntity::subPopHighlight(const DbFullSubentPath& subId)
{
	return Acad::ErrorStatus::eOk;
}
GiHighlightStyle DbEntity::subHighlightState(const DbFullSubentPath& subId)
{
	return GiHighlightStyle::kGiHighlightNone;
}
