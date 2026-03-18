#include "Db.h"
#include "DbLayout.h"
#include "DbViewport.h"
#include "DbDatabase.h"
#include "DbDictionary.h"
#include "DbObjectManager.h"
#include "DbUCSTableRecord.h"
#include "DbLinetypeTable.h"
#include "DbLayerTableRecord.h"
#include "DbViewTableRecord.h"
#include "DbViewportTableRecord.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbRegAppTable.h"
#include "DbRegAppTableRecord.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbDimStyleTable.h"
#include "DbDimStyleTableRecord.h"
#include "DbDictionaryIterator.h"
#include "DbDatabaseImpl.h"
#include "kernel.h"
#include "DbImpl.h"

DbDatabase::DbDatabase(bool buildDefaultDrawing)
{
	this->pImpl = new DbDatabaseImpl();


	if (buildDefaultDrawing == true)
	{
		// 创建块表
		this->pImpl->pBlockTable = new DbBlockTable();
		this->addDbObject(this->pImpl->pBlockTable);

		// 创建图层表
		this->pImpl->pLayerTable = new DbLayerTable();
		this->addDbObject(this->pImpl->pLayerTable);

		// 创建文字样式表
		this->pImpl->pTextStyleTable = new DbTextStyleTable();
		this->addDbObject(this->pImpl->pTextStyleTable);

		// 创建线型表
		this->pImpl->pLinetypeTable = new DbLinetypeTable();
		this->addDbObject(this->pImpl->pLinetypeTable);

		// 创建视图表
		this->pImpl->pViewTable = new DbViewTable();
		this->addDbObject(this->pImpl->pViewTable);

		// 创建UCS表
		this->pImpl->pUCSTable = new DbUCSTable();
		this->addDbObject(this->pImpl->pUCSTable);

		// 创建视口表
		this->pImpl->pViewportTable = new DbViewportTable();
		this->addDbObject(this->pImpl->pViewportTable);

		// 创建注册应用表
		this->pImpl->pRegAppTable = new DbRegAppTable();
		this->addDbObject(this->pImpl->pRegAppTable);

		// 创建标注样式表
		this->pImpl->pDimStyleTable = new DbDimStyleTable();
		this->addDbObject(this->pImpl->pDimStyleTable);

		// 创建根词典
		this->pImpl->pRootDictionary = new DbDictionary();
		this->addDbObject(this->pImpl->pRootDictionary);

		// 添加组词典
		DbObjectId groupDictionaryId = NULL;
		DbDictionary* pGroupDictionary = new DbDictionary();
		this->pImpl->pRootDictionary->setAt("ACAD_GROUP", pGroupDictionary, groupDictionaryId);

		// 添加打印样式词典
		DbObjectId plotStyleDictionaryId = NULL;
		DbDictionary* pPlotStyleDictionary = new DbDictionary();
		this->pImpl->pRootDictionary->setAt("ACAD_PLOTSTYLENAME", pPlotStyleDictionary, plotStyleDictionaryId);

		// 添加布局词典
		DbObjectId layoutDictionaryId = NULL;
		DbDictionary* pLayoutDictionary = new DbDictionary();
		this->pImpl->pRootDictionary->setAt("ACAD_LAYOUT", pLayoutDictionary, layoutDictionaryId);

		// 添加默认线型
		DbObjectId linetypeId = NULL;
		DbLinetypeTableRecord* pLinetypeTableRecord = new DbLinetypeTableRecord();
		pLinetypeTableRecord->setName("ByLayer");
		this->pImpl->pLinetypeTable->add(linetypeId, pLinetypeTableRecord);
		pLinetypeTableRecord->close();

		pLinetypeTableRecord = new DbLinetypeTableRecord();
		pLinetypeTableRecord->setName("ByBlock");
		this->pImpl->pLinetypeTable->add(linetypeId, pLinetypeTableRecord);
		pLinetypeTableRecord->close();

		DbObjectId defaultLinetypeId = NULL;
		pLinetypeTableRecord = new DbLinetypeTableRecord();
		pLinetypeTableRecord->setName("Continuous");
		pLinetypeTableRecord->setComments("Continuous");
		this->pImpl->pLinetypeTable->add(linetypeId, pLinetypeTableRecord);
		defaultLinetypeId = linetypeId;
		pLinetypeTableRecord->close();

		// 添加默认视口
		DbViewportTableRecord* pViewportTableRecord = new DbViewportTableRecord();
		pViewportTableRecord->setName("*Active");
		pViewportTableRecord->setLowerLeftCorner(GePoint2d(0, 0));
		pViewportTableRecord->setUpperRightCorner(GePoint2d(1, 1));
		DbObjectId activeViewportId = NULL;
		this->pImpl->pViewportTable->add(activeViewportId, pViewportTableRecord);

		// 创建布局模型空间
		DbBlockTableRecord* pBlockTableRecord = new DbBlockTableRecord();
		pBlockTableRecord->setName(ACDB_MODEL_SPACE);
		this->pImpl->pBlockTable->add(pBlockTableRecord);
		DbObjectId modeBlockTableRecordId = pBlockTableRecord->objectId();

		pBlockTableRecord = new DbBlockTableRecord();
		pBlockTableRecord->setName(ACDB_PAPER_SPACE);
		this->pImpl->pBlockTable->add(pBlockTableRecord);
		DbObjectId layout1BlockTableRecordId = pBlockTableRecord->objectId();

		pBlockTableRecord = new DbBlockTableRecord();
		pBlockTableRecord->setName(ACDB_PAPER_SPACE);
		this->pImpl->pBlockTable->add(pBlockTableRecord);
		DbObjectId layout2BlockTableRecordId = pBlockTableRecord->objectId();

		// 创建默认图层
		DbLayerTableRecord* pLayerTableRecord = new DbLayerTableRecord();
		pLayerTableRecord->setName("0");
		pLayerTableRecord->setLinetypeObjectId(defaultLinetypeId);
		this->pImpl->pLayerTable->add(pLayerTableRecord);
		this->pImpl->clayer = pLayerTableRecord->objectId();

		// 创建默认视图
		DbViewTableRecord* pViewTableRecord = new DbViewTableRecord();
		pViewTableRecord->setName("*Active");
		this->pImpl->pViewTable->add(pViewTableRecord);

		// 添加布局
		DbObjectId retObjId = NULL;
		DbLayout* pLayout = new DbLayout();
		pLayout->setLayoutName("Model");
		pLayout->setTabSelected(true);
		DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = activeViewportId;
		pLayout->addToLayoutDict(this, modeBlockTableRecordId);

		pLayout = new DbLayout();
		pLayout->setLayoutName("Layout1");
		pLayout->addToLayoutDict(this, layout1BlockTableRecordId);

		pLayout = new DbLayout();
		pLayout->setLayoutName("Layout2");
		pLayout->addToLayoutDict(this, layout2BlockTableRecordId);
	}
}
DbDatabase::~DbDatabase()
{
	if (this->pImpl != NULL)
	{
		delete this->pImpl;
		this->pImpl = NULL;
	}
}

Acad::ErrorStatus DbDatabase::addDbObject(DbObjectId& id, DbObject* pObj)
{
	DB_IMP_OBJECT(pObj->m_pImpl)->database = this;
	id = ::kernel()->acdbObjectManager()->addDbObject(pObj, this);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDatabase::addDbObject(DbObject* pObj)
{
	DbObjectId id;
	return this->addDbObject(id, pObj);
}


Acad::ErrorStatus DbDatabase::getBlockTable(DbBlockTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pBlockTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getLayerTable(DbLayerTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pLayerTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getTextStyleTable(DbTextStyleTable*& pTable, Db::OpenMode mode) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pTextStyleTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getDimStyleTable(DbDimStyleTable*& pTable, Db::OpenMode mode) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pDimStyleTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}

Acad::ErrorStatus DbDatabase::getLinetypeTable(DbLinetypeTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pLinetypeTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getViewTable(DbViewTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pViewTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getUCSTable(DbUCSTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pUCSTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getViewportTable(DbViewportTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pViewportTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getRegAppTable(DbRegAppTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pRegAppTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}

#if EMSDK
DbBlockTable* DbDatabase::emsdk_getBlockTable()
{
	return this->emsdk_getBlockTable(Db::OpenMode::kForRead);
}
DbLayerTable* DbDatabase::emsdk_getLayerTable()
{
	return this->emsdk_getLayerTable(Db::OpenMode::kForRead);
}
DbLinetypeTable* DbDatabase::emsdk_getLinetypeTable()
{
	return this->emsdk_getLinetypeTable(Db::OpenMode::kForRead);
}
DbViewTable* DbDatabase::emsdk_getViewTable()
{
	return this->emsdk_getViewTable(Db::OpenMode::kForRead);
}
DbUCSTable* DbDatabase::emsdk_getUCSTable()
{
	return this->emsdk_getUCSTable(Db::OpenMode::kForRead);
}
DbViewportTable* DbDatabase::emsdk_getViewportTable()
{
	return this->emsdk_getViewportTable(Db::OpenMode::kForRead);
}
DbRegAppTable* DbDatabase::emsdk_getRegAppTable()
{
	return this->emsdk_getRegAppTable(Db::OpenMode::kForRead);
}
DbBlockTable* DbDatabase::emsdk_getBlockTable(Db::OpenMode mode)
{
	DbBlockTable* pTable = NULL;
	this->getBlockTable(pTable, mode);
	return pTable;
}
DbLayerTable* DbDatabase::emsdk_getLayerTable(Db::OpenMode mode)
{
	DbLayerTable* pTable = NULL;
	this->getLayerTable(pTable, mode);
	return pTable;
}
DbLinetypeTable* DbDatabase::emsdk_getLinetypeTable(Db::OpenMode mode)
{
	DbLinetypeTable* pTable = NULL;
	this->getLinetypeTable(pTable, mode);
	return pTable;
}
DbViewTable* DbDatabase::emsdk_getViewTable(Db::OpenMode mode)
{
	DbViewTable* pTable = NULL;
	this->getViewTable(pTable, mode);
	return pTable;
}
DbUCSTable* DbDatabase::emsdk_getUCSTable(Db::OpenMode mode)
{
	DbUCSTable* pTable = NULL;
	this->getUCSTable(pTable, mode);
	return pTable;
}
DbViewportTable* DbDatabase::emsdk_getViewportTable(Db::OpenMode mode)
{
	DbViewportTable* pTable = NULL;
	this->getViewportTable(pTable, mode);
	return pTable;
}
DbRegAppTable* DbDatabase::emsdk_getRegAppTable(Db::OpenMode mode)
{
	DbRegAppTable* pTable = NULL;
	this->getRegAppTable(pTable, mode);
	return pTable;
}
DbTextStyleTable* DbDatabase::emsdk_getTextStyleTable(Db::OpenMode mode) {
	DbTextStyleTable* pTable = NULL;
	this->getTextStyleTable(pTable, mode);
	return pTable;
}
DbDimStyleTable* DbDatabase::emsdk_getDimStyleTable(Db::OpenMode mode) {
	DbDimStyleTable* pTable = NULL;
	this->getDimStyleTable(pTable, mode);
	return pTable;
}
#endif

Acad::ErrorStatus DbDatabase::getSymbolTable(DbBlockTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pBlockTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbLayerTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pLayerTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbLinetypeTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pLinetypeTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbViewTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pViewTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbUCSTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pUCSTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbViewportTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pViewportTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbRegAppTable*& pTable, Db::OpenMode mode)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pRegAppTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbTextStyleTable*& pTable, Db::OpenMode mode) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pTextStyleTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}
Acad::ErrorStatus DbDatabase::getSymbolTable(DbDimStyleTable*& pTable, Db::OpenMode mode) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		pTable = this->pImpl->pDimStyleTable;
		pTable->subOpen(mode);

	} while (false);

	return es;
}


DbObjectId DbDatabase::blockTableId() const
{
	return this->pImpl->pBlockTable->objectId();
}
DbObjectId DbDatabase::layerTableId() const
{
	return this->pImpl->pLayerTable->objectId();
}
DbObjectId DbDatabase::linetypeTableId() const
{
	return this->pImpl->pLinetypeTable->objectId();
}
DbObjectId DbDatabase::viewTableId() const
{
	return this->pImpl->pViewTable->objectId();
}
DbObjectId DbDatabase::UCSTableId() const
{
	return this->pImpl->pUCSTable->objectId();
}
DbObjectId DbDatabase::viewportTableId() const
{
	return this->pImpl->pViewportTable->objectId();
}
DbObjectId DbDatabase::regAppTableId() const
{
	return this->pImpl->pRegAppTable->objectId();
}
DbObjectId  DbDatabase::textStyleTableId() const {
	return this->pImpl->pTextStyleTable->objectId();
}
DbObjectId  DbDatabase::dimStyleTableId() const {
	return this->pImpl->pDimStyleTable->objectId();
}
DbObjectId DbDatabase::layoutDictionaryId() const {
	DbDictionary* pDict = NULL;
	this->getLayoutDictionary(pDict);
	return pDict->objectId();
}



DbObjectId DbDatabase::layerZero() const
{
	DbObjectId layerId = NULL;
	this->pImpl->pLayerTable->getAt("0", layerId);
	return layerId;
}
DbObjectId DbDatabase::continuousLinetype() const
{
	DbObjectId id = NULL;
	this->pImpl->pLinetypeTable->getAt("Continuous", id);
	return id;
}
DbObjectId DbDatabase::byLayerLinetype() const
{
	DbObjectId id = NULL;
	this->pImpl->pLinetypeTable->getAt("ByLayer", id);
	return id;
}
DbObjectId DbDatabase::byBlockLinetype() const
{
	DbObjectId id = NULL;
	this->pImpl->pLinetypeTable->getAt("ByBlock", id);
	return id;
}
DbObjectId DbDatabase::paperSpaceVportId() const
{

	DbObjectId viewportId = NULL;

	// 获得当前布局
	DbObjectId layoutId = NULL;
	DbDictionary* pLayoutDictionary = NULL;
	this->getLayoutDictionary(pLayoutDictionary, Db::kForRead);
	DbDictionaryIterator* pDictionaryIterator = pLayoutDictionary->newIterator();
	pLayoutDictionary->close();
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
	{

		DbLayout* pLayout = NULL;
		if (pDictionaryIterator->getObject(pLayout) != 0)
		{
			continue;
		}

		if (pLayout->getTabSelected() == true)
		{
			layoutId = pLayout->objectId();
		}

		pLayout->close();
	}
	delete pDictionaryIterator;

	// 获得所有视口
	DbObjectIdArray viewportArray;
	if (layoutId != NULL)
	{
		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId) == Acad::ErrorStatus::eOk)
		{
			viewportArray = pLayout->getViewportArray();
			pLayout->close();
		}
	}

	// 获得PaperSpaceVport
	if (viewportArray.length() > 0)
	{
		for (int i = 0; i < viewportArray.length(); i++)
		{
			DbViewport* pViewport = NULL;
			if (::acdbOpenObject(pViewport, viewportArray[i]) == Acad::ErrorStatus::eOk)
			{
				if (pViewport->number() == 1)
				{
					viewportId = pViewport->objectId();
				}
				pViewport->close();
				if (viewportId != NULL)
				{
					break;
				}
			}
		}
	}

	return viewportId;
}
DbObjectId DbDatabase::currentSpaceId() const
{

	DbObjectId currentSpaceId = NULL;

	// 获得布局词典
	DbDictionary* pDictionary = NULL;
	this->getLayoutDictionary(pDictionary);

	DbDictionaryIterator* pDictionaryIterator = pDictionary->newIterator();
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
	{
		// 获得布局
		DbLayout* pLayout = NULL;
		pDictionaryIterator->getObject(pLayout);
		if (pLayout == NULL)
		{
			continue;
		}

		// 获得布局名称
		AcString name;
		pLayout->getName(name);
		if (name.isEqual("Model", true) == true)
		{
			currentSpaceId = pLayout->getBlockTableRecordId();
		}

		// 获得当前空间
		if (pLayout->getTabSelected() == true)
		{
			currentSpaceId = pLayout->getBlockTableRecordId();
		}

		// 获得所有视口
		DbObjectIdArray viewportIds = pLayout->getViewportArray();

		// 判定是否在视口内
		for (int i = 0; i < viewportIds.length(); i++)
		{
			DbViewport* viewport = NULL;
			if (::acdbOpenObject(viewport, viewportIds[i]) == 0)
			{
				if (viewport->number() != 1 && DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId == viewportIds[i])
				{
					this->pImpl->pBlockTable->getAt(ACDB_MODEL_SPACE, currentSpaceId);
				}
				viewport->close();
			}
		}

		// 关闭布局
		pLayout->close();
	}
	delete pDictionaryIterator;

	return currentSpaceId;
}


DbHandle DbDatabase::handseed() const {
	return this->pImpl->handseed;
}
Acad::ErrorStatus DbDatabase::setHandseed(const DbHandle& handle) {
	if (handle < this->pImpl->handseed) {
		return Acad::ErrorStatus::eFail;
	}
	this->pImpl->handseed = handle;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDatabase::getLayoutDictionary(DbDictionary*& pDict, Db::OpenMode mode) const
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eOk;

	do
	{
		DbObject* object = NULL;
		this->pImpl->pRootDictionary->getAt("ACAD_LAYOUT", object, mode);
		pDict = (DbDictionary*)object;

	} while (false);

	return es;
}
DbDictionary* DbDatabase::getLayoutDictionary() const
{
	return this->getLayoutDictionary(Db::OpenMode::kForRead);
}
DbDictionary* DbDatabase::getLayoutDictionary(Db::OpenMode mode) const
{
	DbDictionary* pDict = NULL;
	this->getLayoutDictionary(pDict, mode);
	return pDict;
}

double DbDatabase::plinewid() const {
	return this->pImpl->plinewid;
}
Acad::ErrorStatus DbDatabase::setPlinewid(double width) {
	this->pImpl->plinewid = width;
	return Acad::ErrorStatus::eOk;
}

CmColor DbDatabase::cecolor() const
{
	return this->pImpl->cecolor;
}
Acad::ErrorStatus DbDatabase::setCecolor(const CmColor& color)
{
	this->pImpl->cecolor = color;
	return Acad::ErrorStatus::eOk;
}

CmTransparency DbDatabase::cetransparency() const
{
	return this->pImpl->cetransparency;
}
Acad::ErrorStatus DbDatabase::setCetransparency(const CmTransparency& transparency)
{
	this->pImpl->cetransparency = transparency;
	return Acad::ErrorStatus::eOk;
}

DbObjectId DbDatabase::clayer()const
{
	return this->pImpl->clayer;
}
Acad::ErrorStatus DbDatabase::setClayer(DbObjectId _layerId)
{
	DbLayerTableRecord* pLayerTableRecord = NULL;
	::acdbOpenObject(pLayerTableRecord, _layerId);
	if (pLayerTableRecord != NULL)
	{
		if (pLayerTableRecord->database() == this)
		{
			this->pImpl->clayer = _layerId;
		}
		pLayerTableRecord->close();
	}
	return Acad::ErrorStatus::eOk;
}

DbObjectId DbDatabase::textstyle() const {
	return this->pImpl->textStyleId;
}
Acad::ErrorStatus DbDatabase::setTextstyle(DbObjectId objId) {

	if (this->pImpl->isInspect == false) {
		this->pImpl->textStyleId.setFromOldId(objId.asOldId());
		return Acad::ErrorStatus::eOk;
	}

	DbTextStyleTableRecord* pTextStyleTableRecord = NULL;
	::acdbOpenObject(pTextStyleTableRecord, objId);
	if (pTextStyleTableRecord != NULL)
	{
		if (pTextStyleTableRecord->database() == this)
		{
			this->pImpl->textStyleId = objId;
		}
		pTextStyleTableRecord->close();
	}
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDatabase::getDbObjectId(DbObjectId& retId, bool createIfNotFound, const DbHandle& objHandle)
{
	return this->getDbObjectId(retId, createIfNotFound, objHandle, 0);
}
Acad::ErrorStatus DbDatabase::getDbObjectId(DbObjectId& retId, bool createIfNotFound, const DbHandle& objHandle, Adesk::UInt32 xRefId)
{

	DbHandleTableRecord* record = ::kernel()->acdbObjectManager()->queryHandleTableRecord(this, objHandle);
	if (record != NULL)
	{
		retId.setFromOldId(record->objectId);
		return Acad::ErrorStatus::eOk;
	}
	if (createIfNotFound == true)
	{
		::kernel()->acdbObjectManager()->appendHandleTableRecord(this, retId.asOldId(), objHandle);
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDatabase::addReactor(DbDatabaseReactor* pReactor) const
{
	this->pImpl->reactors.append(pReactor);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDatabase::removeReactor(DbDatabaseReactor* pReactor) const
{
	for (int i = 0; i < this->pImpl->reactors.length(); i++)
	{
		if (this->pImpl->reactors.at(i) == pReactor)
		{
			realloc_arry_at(this->pImpl->reactors, i);
			break;
		}
	}
	return Acad::ErrorStatus::eOk;
}



void repairDatabase(DbDatabase* database) {

	do {


		// 创建块表
		if (database->pImpl->pBlockTable == NULL) {
			database->pImpl->pBlockTable = new DbBlockTable();
			database->addDbObject(database->pImpl->pBlockTable);
		}

		// 创建图层表
		if (database->pImpl->pLayerTable == NULL) {
			database->pImpl->pLayerTable = new DbLayerTable();
			database->addDbObject(database->pImpl->pLayerTable);
		}

		// 创建文字样式表
		if (database->pImpl->pTextStyleTable == NULL) {
			database->pImpl->pTextStyleTable = new DbTextStyleTable();
			database->addDbObject(database->pImpl->pTextStyleTable);
		}

		// 创建线型表
		if (database->pImpl->pLinetypeTable == NULL) {
			database->pImpl->pLinetypeTable = new DbLinetypeTable();
			database->addDbObject(database->pImpl->pLinetypeTable);
		}

		// 创建视图表
		if (database->pImpl->pViewTable == NULL) {
			database->pImpl->pViewTable = new DbViewTable();
			database->addDbObject(database->pImpl->pViewTable);
		}

		// 创建UCS表
		if (database->pImpl->pUCSTable == NULL) {
			database->pImpl->pUCSTable = new DbUCSTable();
			database->addDbObject(database->pImpl->pUCSTable);
		}

		// 创建视口表
		if (database->pImpl->pViewportTable == NULL) {
			database->pImpl->pViewportTable = new DbViewportTable();
			database->addDbObject(database->pImpl->pViewportTable);
		}

		// 创建注册应用表
		if (database->pImpl->pRegAppTable == NULL) {
			database->pImpl->pRegAppTable = new DbRegAppTable();
			database->addDbObject(database->pImpl->pRegAppTable);
		}

		// 创建标注样式表
		if (database->pImpl->pDimStyleTable == NULL) {
			database->pImpl->pDimStyleTable = new DbDimStyleTable();
			database->addDbObject(database->pImpl->pDimStyleTable);
		}

		// 创建根词典
		if (database->pImpl->pRootDictionary == NULL) {
			database->pImpl->pRootDictionary = new DbDictionary();
			database->addDbObject(database->pImpl->pRootDictionary);
		}

		// 添加组词典
		DbObjectId groupDictionaryId = NULL;
		database->pImpl->pRootDictionary->getAt("ACAD_GROUP", groupDictionaryId);
		if (groupDictionaryId == NULL) {
			DbObjectId groupDictionaryId = NULL;
			DbDictionary* pGroupDictionary = new DbDictionary();
			database->pImpl->pRootDictionary->setAt("ACAD_GROUP", pGroupDictionary, groupDictionaryId);
		}

		// 添加打印样式词典
		DbObjectId plotStyleDictionaryId = NULL;
		database->pImpl->pRootDictionary->getAt("ACAD_PLOTSTYLENAME", plotStyleDictionaryId);
		if (plotStyleDictionaryId == NULL) {
			DbObjectId plotStyleDictionaryId = NULL;
			DbDictionary* pPlotStyleDictionary = new DbDictionary();
			database->pImpl->pRootDictionary->setAt("ACAD_PLOTSTYLENAME", pPlotStyleDictionary, plotStyleDictionaryId);
		}

		// 添加布局词典
		DbObjectId layoutDictionaryId = NULL;
		database->pImpl->pRootDictionary->getAt("ACAD_LAYOUT", layoutDictionaryId);
		if (layoutDictionaryId == NULL) {
			DbObjectId layoutDictionaryId = NULL;
			DbDictionary* pLayoutDictionary = new DbDictionary();
			database->pImpl->pRootDictionary->setAt("ACAD_LAYOUT", pLayoutDictionary, layoutDictionaryId);
		}

		// 判断是否存在模型空间,如果不存在则创建
		bool isModelSpaceId = false;
		DbObjectId modelBlockTableId = NULL;
		DbBlockTable* pBlockTable = NULL;
		database->getBlockTable(pBlockTable);
		if (pBlockTable->has(ACDB_MODEL_SPACE) == true) {
			isModelSpaceId = true;
			pBlockTable->getAt(ACDB_MODEL_SPACE, modelBlockTableId);
		}
		if (isModelSpaceId == false) {
			pBlockTable->subOpen(Db::OpenMode::kForWrite);
			DbBlockTableRecord* pBlockTableRecord = new DbBlockTableRecord();
			pBlockTableRecord->setName(ACDB_MODEL_SPACE);
			pBlockTable->add(modelBlockTableId, pBlockTableRecord);
		}
		pBlockTable->close();

		// 判断是否存在默认线型,如果不存在则创建
		bool isLinetypeByLayer = false;
		bool isLinetypeByBlock = false;
		bool isLinetypeContinuous = false;
		DbLinetypeTable* pLinetypeTable = NULL;
		database->getLinetypeTable(pLinetypeTable);
		if (pLinetypeTable->has("ByLayer") == true) {
			isLinetypeByLayer = true;
		}
		if (pLinetypeTable->has("ByBlock") == true) {
			isLinetypeByBlock = true;
		}
		if (pLinetypeTable->has("Continuous") == true) {
			isLinetypeContinuous = true;
		}
		if (isLinetypeByLayer == false) {
			pLinetypeTable->subOpen(Db::OpenMode::kForWrite);
			DbObjectId linetypeId = NULL;
			DbLinetypeTableRecord* pLinetypeTableRecord = new DbLinetypeTableRecord();
			pLinetypeTableRecord->setName("ByLayer");
			pLinetypeTable->add(linetypeId, pLinetypeTableRecord);
			pLinetypeTableRecord->close();
		}
		if (isLinetypeByBlock == false) {
			pLinetypeTable->subOpen(Db::OpenMode::kForWrite);
			DbObjectId linetypeId = NULL;
			DbLinetypeTableRecord* pLinetypeTableRecord = new DbLinetypeTableRecord();
			pLinetypeTableRecord->setName("ByBlock");
			pLinetypeTable->add(linetypeId, pLinetypeTableRecord);
			pLinetypeTableRecord->close();
		}
		if (isLinetypeContinuous == false) {
			pLinetypeTable->subOpen(Db::OpenMode::kForWrite);
			DbObjectId linetypeId = NULL;
			DbLinetypeTableRecord* pLinetypeTableRecord = new DbLinetypeTableRecord();
			pLinetypeTableRecord->setName("Continuous");
			pLinetypeTable->add(linetypeId, pLinetypeTableRecord);
			pLinetypeTableRecord->close();
		}
		pLinetypeTable->close();

		// 判断是否存在默认视口,如果不存在则创建
		bool isActiveViewport = false;
		DbObjectId activeViewportId = NULL;
		DbViewportTable* pViewportTable = NULL;
		database->getViewportTable(pViewportTable);
		pViewportTable->getAt("*Active", activeViewportId);
		if (activeViewportId == NULL) {
			pViewportTable->subOpen(Db::OpenMode::kForWrite);
			DbViewportTableRecord* pViewportTableRecord = new DbViewportTableRecord();
			pViewportTableRecord->setName("*Active");
			pViewportTableRecord->setLowerLeftCorner(GePoint2d(0, 0));
			pViewportTableRecord->setUpperRightCorner(GePoint2d(1, 1));
			pViewportTable->add(activeViewportId, pViewportTableRecord);
			pViewportTableRecord->close();
		}
		pViewportTable->close();

		// 判断是否存在0号图层,如果不存在则创建
		bool isZeroLayer = false;
		DbLayerTable* pLayerTable = NULL;
		if (database->getLayerTable(pLayerTable) == Acad::ErrorStatus::eOk) {
			if (pLayerTable->has("0") == true) {
				isZeroLayer = true;
			}
			if (isZeroLayer == false) {
				DbObjectId layerId = NULL;
				pLayerTable->subOpen(Db::OpenMode::kForWrite);
				DbLayerTableRecord* pLayerTableRecord = new DbLayerTableRecord();
				pLayerTableRecord->setName("0");
				pLayerTable->add(layerId, pLayerTableRecord);
				pLayerTableRecord->close();

				database->setClayer(layerId);
			}
			pLayerTable->close();
		}

		// 判断当前图层是否正确
		bool isDefaultLayer = false;
		DbLayerTableRecord* pLayerTableRecord = NULL;
		if (::acdbOpenObject(pLayerTableRecord, database->clayer()) == Acad::ErrorStatus::eOk) {
			if (pLayerTableRecord->database() != database) {
				isDefaultLayer = true;
			}
			pLayerTableRecord->close();
		}
		else {
			isDefaultLayer = true;
		}
		if (isDefaultLayer == true) {
			if (database->getLayerTable(pLayerTable) == Acad::ErrorStatus::eOk) {
				DbObjectId layerId = NULL;
				if (pLayerTable->getAt("0", layerId) == Acad::ErrorStatus::eOk)
				{
					database->setClayer(layerId);
				}
				pLayerTable->close();
			}
		}

		// 判断是否存在默认文字样式,如果不存在则创建
		bool isStandard = false;
		DbTextStyleTable* pTextStyleTable = NULL;
		if (database->getTextStyleTable(pTextStyleTable) == Acad::ErrorStatus::eOk) {
			DbObjectId standardId = NULL;
			pTextStyleTable->getAt("Standard", standardId);
			if (standardId == NULL) {
				pTextStyleTable->subOpen(Db::OpenMode::kForWrite);
				DbTextStyleTableRecord* pTextStyleTableRecord = new DbTextStyleTableRecord();
				pTextStyleTableRecord->setName("Standard");
				pTextStyleTable->add(standardId, pTextStyleTableRecord);
				pTextStyleTableRecord->close();
			}
			pTextStyleTable->close();

			if (database->textstyle() == NULL) {
				database->setTextstyle(standardId);
			}
		}

		// 获得所有布局,如果不存在"Model"则创建默认布局
		DbObjectIdArray layoutIds;
		DbDictionary* pDictionary = NULL;
		database->getLayoutDictionary(pDictionary);
		if (pDictionary != NULL) {

			DbDictionaryIterator* dictionaryIterator = pDictionary->newIterator();
			for (; !dictionaryIterator->done(); dictionaryIterator->next()) {
				layoutIds.append(dictionaryIterator->objectId());
			}
			delete dictionaryIterator;

			bool isModelLayout = false;
			if (pDictionary->has("Model") == true) {
				isModelLayout = true;
			}
			if (isModelLayout == false) {
				DbLayout* pLayout = new DbLayout();
				pLayout->setLayoutName("Model");
				pLayout->setTabSelected(true);
				DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = activeViewportId;
				pLayout->addToLayoutDict(database, modelBlockTableId);
				layoutIds.append(pLayout->objectId());
				pLayout->close();
			}
			pDictionary->close();
		}

		// 为默认块表记录创建布局
		DbBlockTableIterator* pBlockTableIterator = NULL;
		database->pImpl->pBlockTable->newIterator(pBlockTableIterator);
		for (pBlockTableIterator->start(); !pBlockTableIterator->done(); pBlockTableIterator->step()) {
			DbBlockTableRecord* pBlockTableRecord = NULL;
			pBlockTableIterator->getRecord(pBlockTableRecord);
			if (pBlockTableRecord == NULL) {
				continue;
			}

			AcString blockName;
			pBlockTableRecord->getName(blockName);
			if (blockName.find("*Paper_Space") == 0) {

				// 判断是否存在对应的布局
				bool isLayout = false;
				for (int i = 0; i < layoutIds.length(); i++) {
					DbLayout* layout = NULL;
					if (::acdbOpenObject(layout, layoutIds[i]) != Acad::ErrorStatus::eOk) {
						continue;
					}
					if (layout->getBlockTableRecordId() == pBlockTableRecord->objectId()) {
						isLayout = true;
						pBlockTableRecord->setLayoutId(layout->objectId());
					}
					layout->close();
					if (isLayout == true) {
						break;
					}
				}

				// 如果不存在则创建对应的布局
				if (isLayout == false) {
					DbLayout* pLayout = new DbLayout();
					AcString layoutName;
					layoutName.format("Layout%d", pLayout->getTabOrder() + 1);
					pLayout->setLayoutName(layoutName);
					pLayout->setTabSelected(false);
					pLayout->addToLayoutDict(database, pBlockTableRecord->objectId());
					layoutIds.append(pLayout->objectId());
					pLayout->close();

					pBlockTableRecord->setLayoutId(layoutIds[layoutIds.length() - 1]);
				}
			}

			pBlockTableRecord->close();
		}
		delete pBlockTableIterator;

		// 为布局创建默认激活视口
		database->getLayoutDictionary(pDictionary);
		if (pDictionary != NULL) {

			// 获得所有布局
			layoutIds.removeAll();
			DbDictionaryIterator* dictionaryIterator = pDictionary->newIterator();
			for (; !dictionaryIterator->done(); dictionaryIterator->next()) {
				layoutIds.append(dictionaryIterator->objectId());
			}
			delete dictionaryIterator;
			pDictionary->close();

			for (int i = 0; i < layoutIds.length(); i++) {

				DbLayout* layout = NULL;
				if (::acdbOpenObject(layout, layoutIds[i]) != Acad::ErrorStatus::eOk) {
					continue;
				}

				AcString layoutName;
				layout->getName(layoutName);
				if (layoutName.isEqual("Model", true) == true) {
					if (DB_IMP_LAYOUT(layout->m_pImpl)->activationViewportId == NULL) {
						DB_IMP_LAYOUT(layout->m_pImpl)->activationViewportId = activeViewportId;
					}
				}
				else {

					// 获得所有视口
					DbObjectIdArray viewportIds = layout->getViewportArray();

					// 设置被激活视口
					if (DB_IMP_LAYOUT(layout->m_pImpl)->activationViewportId == NULL) {
						for (int u = 0; u < viewportIds.length(); u++) {
							DbViewport* viewport = NULL;
							if (::acdbOpenObject(viewport, viewportIds[i]) == Acad::ErrorStatus::eOk) {
								if (viewport->number() == 1 && DB_IMP_LAYOUT(layout->m_pImpl)->activationViewportId == NULL) {
									DB_IMP_LAYOUT(layout->m_pImpl)->activationViewportId = viewportIds[u];
								}
								viewport->close();
							}
						}
					}
				}

				layout->close();
			}
		}

	} while (false);

}

