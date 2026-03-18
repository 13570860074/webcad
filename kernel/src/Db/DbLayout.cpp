#include "DbLayout.h"
#include "AcString.h"
#include "DbBlockTableRecord.h"
#include "DbDictionary.h"
#include "DbDictionaryIterator.h"
#include "DbViewport.h"
#include "DbImpl.h"

DbLayout::DbLayout()
{
	this->m_pImpl = new DbLayoutImpl();
}
DbLayout::~DbLayout()
{
}
DbObjectId DbLayout::getBlockTableRecordId() const
{
	return DB_IMP_LAYOUT(this->m_pImpl)->blockTableRecordId;
}

Acad::ErrorStatus DbLayout::setBlockTableRecordId(DbObjectId BlockTableRecordId)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{

		// 打开块表记录
		DbBlockTableRecord *pBlockTableRecord = NULL;
		if (::acdbOpenObject(pBlockTableRecord, BlockTableRecordId, Db::OpenMode::kForRead) != 0)
		{
			break;
		}

		// 判断数据库对象是否一样
		if (pBlockTableRecord->database() != this->database())
		{
			pBlockTableRecord->close();
			break;
		}

		// 判断是否是布局
		AcString name;
		pBlockTableRecord->getName(name);
		if (
			name.makeLower().find(AcString(ACDB_MODEL_SPACE).makeLower()) != 0 &&
			name.makeLower().find(AcString(ACDB_PAPER_SPACE).makeLower()) != 0)
		{
			pBlockTableRecord->close();
			break;
		}

		// 设置块表记录
		DB_IMP_LAYOUT(this->m_pImpl)->blockTableRecordId = BlockTableRecordId;

		pBlockTableRecord->close();

		es = Acad::ErrorStatus::eOk;

	} while (false);

	return es;
}

Acad::ErrorStatus DbLayout::addToLayoutDict(DbDatabase *towhichDb, DbObjectId BlockTableRecordId)
{


	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	// 获得布局词典
	DbDictionary* layoutDictionary = NULL;
	if (towhichDb->getLayoutDictionary(layoutDictionary) != 0)
	{
		return es;
	}

	// 添加布局到词典中
	ACHAR* name = NULL;
	DB_IMP_OBJECT(this->m_pImpl)->database = towhichDb;
	this->setBlockTableRecordId(BlockTableRecordId);
	this->getLayoutName(name);

	DbObjectId retObjId;
	es = layoutDictionary->setAt(name, this, retObjId);

	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (::acdbOpenObject(pBlockTableRecord, BlockTableRecordId, Db::OpenMode::kForWrite) == 0)
	{
		pBlockTableRecord->setLayoutId(retObjId);
		pBlockTableRecord->close();
	}

	return es;
}

Acad::ErrorStatus DbLayout::getName(AcString &sName) const
{
	sName = DB_IMP_LAYOUT(this->m_pImpl)->layoutName;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbLayout::getLayoutName(ACHAR *&layoutName) const
{
	::acutNewString(DB_IMP_LAYOUT(this->m_pImpl)->layoutName.constPtr(), layoutName);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbLayout::setLayoutName(const ACHAR*layoutName)
{
	DB_IMP_LAYOUT(this->m_pImpl)->layoutName = layoutName;
	return Acad::ErrorStatus::eOk;
}
#if EMSDK
emstring DbLayout::emsdk_getName() const {
	AcString sName;
	this->getName(sName);
	return sName.constPtr();
}
emstring DbLayout::emsdk_getLayoutName() const {
	ACHAR* layoutName;
	this->getLayoutName(layoutName);
	AcString sName = layoutName;
	delete[]layoutName;
	return sName.constPtr();
}
Acad::ErrorStatus DbLayout::emsdk_setLayoutName(const emstring& layoutName) {
	return this->setLayoutName(layoutName.c_str());
}
#endif

int DbLayout::getTabOrder() const
{
	return DB_IMP_LAYOUT(this->m_pImpl)->order;
}

void DbLayout::setTabOrder(int newOrder)
{
	DB_IMP_LAYOUT(this->m_pImpl)->order = newOrder;
}

bool DbLayout::getTabSelected() const
{
	return DB_IMP_LAYOUT(this->m_pImpl)->tabSelected;
}
void DbLayout::setTabSelected(bool tabSelected)
{
	DB_IMP_LAYOUT(this->m_pImpl)->tabSelected = tabSelected;
}

DbObjectIdArray DbLayout::getViewportArray() const
{
	DbObjectIdArray ids;

	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (::acdbOpenObject(pBlockTableRecord, DB_IMP_LAYOUT(this->m_pImpl)->blockTableRecordId) == Acad::ErrorStatus::eOk) {
		for (int i = 0; i < DB_IMP_BLOCKTABLERECORD(pBlockTableRecord->m_pImpl)->viewports.length(); i++) {
			ids.append(DB_IMP_BLOCKTABLERECORD(pBlockTableRecord->m_pImpl)->viewports.at(i)->objectId());
		}
		pBlockTableRecord->close();
	}
	return ids;
}



Acad::ErrorStatus DbLayout::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbPlotSettings::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLayout::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbPlotSettings::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}




void DbLayout::close() {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DbObjectIdArray viewportIds = this->getViewportArray();
		for (int i = 0; i < viewportIds.length(); i++) {
			DbViewport* viewport = NULL;
			if (::acdbOpenObject(viewport, viewportIds[i], Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {
				viewport->close();
			}
		}
	}

	DbPlotSettings::close();
}