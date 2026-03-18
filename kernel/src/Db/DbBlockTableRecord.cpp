#include "kernel.h"
#include "DbObjectManager.h"
#include "DbLayout.h"
#include "DbEntity.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTableRecordIterator.h"
#include "DbBlockTable.h"
#include "DbViewport.h"
#include "kernel.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"

DbBlockTableRecord::DbBlockTableRecord()
{
	this->m_pImpl = new DbBlockTableRecordImpl();
	DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockScaling = DbBlockTableRecord::BlockScaling::kAny;
	DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockInsertUnits = Db::UnitsValue::kUnitsMillimeters;
}
DbBlockTableRecord::~DbBlockTableRecord()
{
}


Acad::ErrorStatus DbBlockTableRecord::appendDbEntity(DbObjectId &pOutputId, DbEntity *pEntity)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{

		// 将实体添加到数据库
		if (this->database() != NULL) {
			this->database()->addDbObject(pOutputId, pEntity);
		}

		// 设置实体默认图层
		if (pEntity->layerId() == NULL && this->database() != NULL) {
			pEntity->setLayer(this->database()->clayer());
		}

		// 设置默认颜色
		if (pEntity->colorIndex() <= 0 || pEntity->colorIndex() >= 255) {
			if (this->database() != NULL) {
				pEntity->setColor(this->database()->cecolor());
			}
		}

		// 添加到实体列表
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->entitys.append(pEntity);

		// 如果是布局中的视口的话则加入视口列表
		if (DB_IMP_OBJECT(pEntity->m_pImpl)->isViewport() == true && this->database() != NULL)
		{
			if (this->isLayout() == true) {
				DbLayout* pLayout = NULL;
				if (::acdbOpenObject(pLayout, this->getLayoutId()) == Acad::ErrorStatus::eOk)
				{
					if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->viewports.length() <= 0)
					{
						DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = pOutputId;
					}
					pLayout->close();
				}
			}

			if (DB_IMP_VIEWPORT(pEntity->m_pImpl)->number == 0) {
				int number = DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->viewports.length() + 1;
				for (int i = 0; i < DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->viewports.length(); i++) {
					DbViewport* viewport = (DbViewport*)DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->viewports[i];
					if (viewport->number() >= number) {
						number = viewport->number() + 1;
					}
				}
				DB_IMP_VIEWPORT(pEntity->m_pImpl)->number = number;
			}

			DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->viewports.append(pEntity);
		}

		// 如果是尺寸标注则为尺寸创建一个默认块表记录
		else if (DB_IMP_OBJECT(pEntity->m_pImpl)->isDimension() == true && this->database() != NULL) {
			if (DB_IMP_DIMENSION(pEntity->m_pImpl)->dimBlockId == NULL) {
				DbBlockTableRecord* pBlockTableRecord = new DbBlockTableRecord();
				pBlockTableRecord->setName("*U");
				DbBlockTable* pBlockTable = (DbBlockTable*)::kernel()->acdbObjectManager()->openDbObject(pEntity->database()->blockTableId());
				if (pBlockTable != NULL) {
					DbObjectId dimBlockId;
					pBlockTable->add(dimBlockId, pBlockTableRecord);
					DB_IMP_DIMENSION(pEntity->m_pImpl)->dimBlockId = dimBlockId;
				}
			}
		}

		// 设置块id
		DB_IMP_ENTITY(pEntity->m_pImpl)->blockId = this->objectId();

	} while (false);

	return es;
}

Acad::ErrorStatus DbBlockTableRecord::newIterator(DbBlockTableRecordIterator *&pIterator, bool atBeginning, bool skipDeleted) const
{
	pIterator = new DbBlockTableRecordIterator();
	DB_IMP_BLOCKTABLERECORDITERATOR(pIterator->m_pImpl)->blockTableRecord = const_cast<DbBlockTableRecord*>(this);
	DB_IMP_BLOCKTABLERECORDITERATOR(pIterator->m_pImpl)->iterator = AcIterator<DbEntity*>((AcArray<DbEntity*>)DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->entitys);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockTableRecord::comments(ACHAR*&pString) const
{
	::acutNewString(DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->comments, pString);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::comments(const ACHAR*&pString) const
{
	pString = AcString(DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->comments);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::comments(AcString &sComments) const
{
	sComments = AcString(DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->comments);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::setComments(const ACHAR*pString)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->comments = pString;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}
#if EMSDK
const emstring DbBlockTableRecord::emsdk_comments()const {
	AcString sComments;
	this->comments(sComments);
	return sComments.constPtr();
}
Acad::ErrorStatus DbBlockTableRecord::emsdk_setComments(const emstring& pString) {
	return this->setComments(pString.c_str());
}
#endif

Acad::ErrorStatus DbBlockTableRecord::pathName(ACHAR*&pPath) const
{
	::acutNewString(DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->pathName, pPath);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::pathName(const ACHAR*&pPath) const
{
	pPath = AcString(DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->pathName);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::pathName(AcString &sPath) const
{
	sPath = AcString(DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->pathName);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::setPathName(const ACHAR*pathStr)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->pathName = pathStr;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}
#if EMSDK
const emstring DbBlockTableRecord::emsdk_pathName()const {
	AcString sPath;
	this->pathName(sPath);
	return sPath.constPtr();
}
Acad::ErrorStatus DbBlockTableRecord::emsdk_setPathName(const emstring& pString) {
	return this->setPathName(pString.c_str());
}
#endif

GePoint3d DbBlockTableRecord::origin() const
{
	return DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->origin;
}
Acad::ErrorStatus DbBlockTableRecord::setOrigin(const GePoint3d &pt)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->origin = pt;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}

Acad::ErrorStatus DbBlockTableRecord::openBlockBegin(DbBlockBegin *&pBlockBegin, Db::OpenMode openMode)
{
	pBlockBegin = NULL;
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbBlockTableRecord::openBlockEnd(DbBlockEnd *&pBlockEnd, Db::OpenMode openMode)
{
	pBlockEnd = NULL;
	return Acad::ErrorStatus::eFail;
}

bool DbBlockTableRecord::hasAttributeDefinitions() const
{
	return false;
}

bool DbBlockTableRecord::isAnonymous() const
{
	AcString sName;
	this->getName(sName);
	if (sName.find("*U") == 0)
	{
		return true;
	}
	return false;
}

bool DbBlockTableRecord::isFromExternalReference() const
{
	return false;
}

bool DbBlockTableRecord::isFromOverlayReference() const
{
	return DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->isFromOverlayReference;
}
Acad::ErrorStatus DbBlockTableRecord::setIsFromOverlayReference(bool bIsOverlay)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->isFromOverlayReference = bIsOverlay;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}

bool DbBlockTableRecord::isLayout() const
{
	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->layoutId == NULL)
	{
		return false;
	}
	return true;
}

DbObjectId DbBlockTableRecord::getLayoutId() const
{
	return DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->layoutId;
}
Acad::ErrorStatus DbBlockTableRecord::setLayoutId(DbObjectId layoutId)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->layoutId = layoutId;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}

Acad::ErrorStatus DbBlockTableRecord::newBlockReferenceIdIterator(DbBlockReferenceIdIterator *&pIter) const
{
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockTableRecord::getBlockReferenceIds(DbObjectIdArray &ids, bool bDirectOnly, bool bForceValidity) const
{
	ids = DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockReferenceIds;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockTableRecord::getErasedBlockReferenceIds(DbObjectIdArray &ids)
{
	ids = DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockReferenceIds;
	return Acad::ErrorStatus::eOk;
}

bool DbBlockTableRecord::isUnloaded() const
{
	return DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->unloaded;
}
Acad::ErrorStatus DbBlockTableRecord::setIsUnloaded(bool isUnloaded)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->unloaded = isUnloaded;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}

Acad::ErrorStatus DbBlockTableRecord::assumeOwnershipOf(DbObjectIdArray &entitiesToMove)
{

	DbBlockTableRecordIterator *pBlockTableRecordIterator = NULL;
	this->newIterator(pBlockTableRecordIterator);

	for (pBlockTableRecordIterator->start(); !pBlockTableRecordIterator->done(); pBlockTableRecordIterator->step())
	{
		DbEntity *pEntity = NULL;
		pBlockTableRecordIterator->getEntity(pEntity);
		if (pEntity == NULL)
		{
			continue;
		}

		entitiesToMove.append(pEntity->objectId());

		pEntity->close();
	}

	return Acad::ErrorStatus::eOk;
}

DbBlockTableRecord::BlockScaling DbBlockTableRecord::blockScaling() const
{
	return (DbBlockTableRecord::BlockScaling)DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockScaling;
}
Acad::ErrorStatus DbBlockTableRecord::setBlockScaling(DbBlockTableRecord::BlockScaling blockScaling)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockScaling = blockScaling;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}

Acad::ErrorStatus DbBlockTableRecord::setExplodable(bool bExplodable)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->explodable = bExplodable;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}
bool DbBlockTableRecord::explodable() const
{
	return DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->explodable;
}

Acad::ErrorStatus DbBlockTableRecord::setBlockInsertUnits(Db::UnitsValue insunits)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	if (DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForWrite || DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockInsertUnits = insunits;

		es = Acad::ErrorStatus::eOk;
	}

	return es;
}
Db::UnitsValue DbBlockTableRecord::blockInsertUnits() const
{
	return (Db::UnitsValue)DB_IMP_BLOCKTABLERECORD(this->m_pImpl)->blockInsertUnits;
}

Acad::ErrorStatus DbBlockTableRecord::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockTableRecord::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
