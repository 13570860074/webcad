#include "kernel.h"
#include "DbDatabase.h"
#include "DbLayout.h"
#include "DbViewport.h"
#include "DbLayoutManager.h"
#include "DbLayoutManagerReactor.h"
#include "DbDictionaryIterator.h"
#include "DbHostApplicationServices.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "ReRendererManager.h"
#include "DbImpl.h"
#include "EdImpl.h"

DbLayoutManager::DbLayoutManager()
{
	this->m_pImpl = new DbLayoutManagerImpl();
}
DbLayoutManager::~DbLayoutManager()
{
}
#if EMSDK
Acad::ErrorStatus DbLayoutManager::emsdk_setCurrentLayout(const emstring &newname)
{
	return this->setCurrentLayout(newname.c_str(), acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::emsdk_setCurrentLayout(const emstring &newname, DbDatabase *pDb)
{
	return this->setCurrentLayout(newname.c_str(), pDb);
}
#endif
Acad::ErrorStatus DbLayoutManager::setCurrentLayout(const ACHAR *newname)
{
	return this->setCurrentLayout(newname, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::setCurrentLayout(const ACHAR*newname, DbDatabase *pDb)
{
	DbObjectId layoutId = this->findLayoutNamed(newname, pDb);
	return this->setCurrentLayoutId(layoutId);
}
Acad::ErrorStatus DbLayoutManager::setCurrentLayoutId(DbObjectId layoutId)
{

	// 打开对象
	DbLayout *pLayout = NULL;
	if (::acdbOpenObject(pLayout, layoutId) != 0)
	{
		return Acad::ErrorStatus::eOk;
	}
	if (pLayout->database() == NULL)
	{
		pLayout->close();
		return Acad::ErrorStatus::eOk;
	}

	// 获得布局名称
	AcString newName;
	pLayout->getName(newName);

	// 获得布局词典
	DbDictionary* pLayoutDictionary = NULL;
	pLayout->database()->getLayoutDictionary(pLayoutDictionary, Db::OpenMode::kForWrite);

	// 创建布局遍历器
	DbDictionaryIterator* pDictionaryIterator = pLayoutDictionary->newIterator();

	// 设置布局
	AcString oldLayoutName;
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
	{
		// 获得布局
		DbLayout* pLayout = NULL;
		if (pDictionaryIterator->getObject(pLayout) != 0)
		{
			continue;
		}

		// 获得布局名称
		AcString layoutName;
		pLayout->getName(layoutName);
		if (pLayout->getTabSelected() == true)
		{
			oldLayoutName = layoutName;
		}

		// 设置布局激活选项
		if (pLayout->objectId() == layoutId)
		{
			// 设置布局选中
			pLayout->setTabSelected(true);

			// 如果是非模型空间则创建默认视口
			if (layoutName.isEqual("Model", true) == false) {

				// 创建默认视口
				DbObjectIdArray viewportIds = pLayout->getViewportArray();
				if (viewportIds.length() <= 0)
				{
					DbObjectId blockTableRecordId = pLayout->getBlockTableRecordId();

					// 打开块表记录
					DbBlockTableRecord* pBlockTableRecord = NULL;
					if (::acdbOpenObject(pBlockTableRecord, blockTableRecordId, Db::OpenMode::kForWrite) == 0)
					{
						DbObjectId viewportId = NULL;

						// 创建布局视口
						DbViewport* pViewport = new DbViewport();
						pViewport->setOn();
						pViewport->setCenterPoint(GePoint3d());
						pBlockTableRecord->appendDbEntity(viewportId, pViewport);
						pViewport->close();

						// 创建默认视口
						pViewport = new DbViewport();
						pViewport->setCenterPoint(GePoint3d(5.25, 4.0, 0.0));
						pViewport->setViewHeight(9.226);
						pViewport->setViewCenter(GePoint2d(6.0, 4.5));
						pBlockTableRecord->appendDbEntity(viewportId, pViewport);
						pViewport->close();

						pBlockTableRecord->close();
					}
				}
				viewportIds = pLayout->getViewportArray();

				// 如果布局被激活视口为空则激活第一个视口
				if (DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId == NULL && viewportIds.length() > 0) {
					DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = viewportIds[0];
				}
			}
		}
		else
		{
			pLayout->setTabSelected(false);
		}

		// 关闭布局对象
		pLayout->close();
	}
	delete pDictionaryIterator;

	// 关闭布局词典
	pLayoutDictionary->close();

	// 触发布局反应器切换布局事件和刷新布局选项卡事件
	for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
	{
		DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutSwitched(newName.constPtr(), layoutId);
		DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->refreshLayoutTabs();
	}

	pLayout->close();

	return Acad::ErrorStatus::eOk;
}
#if EMSDK
emstring DbLayoutManager::emsdk_getActiveLayoutName(bool allowModel)
{
	AcString sName;
	this->getActiveLayoutName(sName, allowModel);
	return sName.constPtr();
}
emstring DbLayoutManager::emsdk_getActiveLayoutName(bool allowModel, const DbDatabase *pDb)
{
	AcString sName;
	this->getActiveLayoutName(sName, allowModel, pDb);
	return sName.constPtr();
}
#endif
Acad::ErrorStatus DbLayoutManager::getActiveLayoutName(AcString &sName, bool allowModel)
{
	return this->getActiveLayoutName(sName, allowModel, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::getActiveLayoutName(AcString &sName, bool allowModel, const DbDatabase *pDb)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	// 获得被激活的布局id
	DbObjectId layoutId = this->getActiveLayoutBTRId(pDb);

	// 获得布局名称
	DbLayout *pLayout = NULL;
	if (::acdbOpenObject(pLayout, layoutId, Db::OpenMode::kForRead) == 0)
	{
		es = pLayout->getName(sName);
		pLayout->close();
	}

	return es;
}
#if EMSDK
emstring DbLayoutManager::emsdk_findActiveLayout(bool allowModel)
{
	return this->emsdk_findActiveLayout(allowModel, acdbHostApplicationServices()->workingDatabase());
}
emstring DbLayoutManager::emsdk_findActiveLayout(bool allowModel, const DbDatabase *pDb)
{

	AcString sName;
	this->getActiveLayoutName(sName, allowModel, pDb);
	return sName.constPtr();
}
#endif
const ACHAR* DbLayoutManager::findActiveLayout(bool allowModel, const DbDatabase* pDb ) {
	AcString sName;
	this->getActiveLayoutName(sName, allowModel, pDb);
	return sName.constPtr();
}
DbObjectId DbLayoutManager::getActiveLayoutBTRId()
{
	return this->getActiveLayoutBTRId(acdbHostApplicationServices()->workingDatabase());
}
DbObjectId DbLayoutManager::getActiveLayoutBTRId(const DbDatabase *pDb)
{

	if (pDb == NULL)
	{
		return NULL;
	}

	DbObjectId layoutId = NULL;

	// 获得布局词典
	DbDictionary *pLayoutDictionary = NULL;
	pDb->getLayoutDictionary(pLayoutDictionary, Db::OpenMode::kForRead);

	// 创建布局遍历器
	DbDictionaryIterator *pDictionaryIterator = pLayoutDictionary->newIterator();
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
	{

		DbLayout *pLayout = NULL;
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

	// 关闭布局词典
	pLayoutDictionary->close();

	return layoutId;
}
#if EMSDK
DbObjectId DbLayoutManager::emsdk_findLayoutNamed(const emstring &name)
{
	return this->findLayoutNamed(name.c_str(), acdbHostApplicationServices()->workingDatabase());
}
DbObjectId DbLayoutManager::emsdk_findLayoutNamed(const emstring &name, const DbDatabase *pDb)
{
	return this->findLayoutNamed(name.c_str(), pDb);
}
#endif
DbObjectId DbLayoutManager::findLayoutNamed(const ACHAR*name)
{
	return this->findLayoutNamed(name, acdbHostApplicationServices()->workingDatabase());
}
DbObjectId DbLayoutManager::findLayoutNamed(const ACHAR*name, const DbDatabase *pDb)
{

	DbObjectId layoutId = NULL;

	// 获得布局词典
	DbDictionary *pLayoutDictionary = NULL;
	pDb->getLayoutDictionary(pLayoutDictionary, Db::OpenMode::kForWrite);

	// 创建布局遍历器
	DbDictionaryIterator *pDictionaryIterator = pLayoutDictionary->newIterator();
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
	{
		DbLayout *pLayout = NULL;
		if (pDictionaryIterator->getObject(pLayout) != 0)
		{
			continue;
		}

		ACHAR*layoutName;
		pLayout->getLayoutName(layoutName);
		if (strcmp(layoutName, name) == 0)
		{
			layoutId = pLayout->objectId();
		}

		pLayout->close();
	}
	delete pDictionaryIterator;

	// 关闭布局词典
	pLayoutDictionary->close();

	return layoutId;
}
#if EMSDK
DbLayout *DbLayoutManager::emsdk_getLayoutNamed(const emstring &name)
{
	DbLayout *pLayout = NULL;
	this->getLayoutNamed(name.c_str(), pLayout, Db::OpenMode::kForRead, acdbHostApplicationServices()->workingDatabase());
	return pLayout;
}
DbLayout *DbLayoutManager::emsdk_getLayoutNamed(const emstring &name, Db::OpenMode mode, const DbDatabase *pDb)
{
	DbLayout *pLayout = NULL;
	this->getLayoutNamed(name.c_str(), pLayout, mode, pDb);
	return pLayout;
}
#endif
Acad::ErrorStatus DbLayoutManager::getLayoutNamed(const ACHAR*name, DbLayout *&pLayout)
{
	return this->getLayoutNamed(name, pLayout, Db::OpenMode::kForRead, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::getLayoutNamed(const ACHAR*name, DbLayout *&pLayout, Db::OpenMode mode, const DbDatabase *pDb)
{

	DbObjectId layoutId = this->findLayoutNamed(name, pDb);
	if (::acdbOpenObject(pLayout, layoutId, mode) == 0)
	{
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
bool DbLayoutManager::emsdk_layoutExists(const emstring &name)
{
	return this->layoutExists(name.c_str(), acdbHostApplicationServices()->workingDatabase());
}
bool DbLayoutManager::emsdk_layoutExists(const emstring &name, const DbDatabase *pDb)
{
	return this->layoutExists(name.c_str(), pDb);
}
#endif
bool DbLayoutManager::layoutExists(const ACHAR *name)
{
	return this->layoutExists(name, acdbHostApplicationServices()->workingDatabase());
}
bool DbLayoutManager::layoutExists(const ACHAR*name, const DbDatabase *pDb)
{
	if (this->findLayoutNamed(name, pDb) == NULL)
	{
		return false;
	}
	return true;
}
#if EMSDK
Acad::ErrorStatus DbLayoutManager::emsdk_copyLayout(const emstring &copyname, const emstring &newname)
{
	return this->copyLayout(copyname.c_str(), newname.c_str(), acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::emsdk_copyLayout(const emstring &copyname, const emstring &newname, DbDatabase *pDb)
{
	return this->copyLayout(copyname.c_str(), newname.c_str(), pDb);
}
#endif
Acad::ErrorStatus DbLayoutManager::copyLayout(const ACHAR*copyname, const ACHAR*newname)
{
	return this->copyLayout(copyname, newname, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::copyLayout(const ACHAR*copyname, const ACHAR*newname, DbDatabase *pDb)
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

#if 0

	do {


		//判断新布局名称是否存在
		if (this->layoutExists(newname, pDb) == true) {
			break;
		}

		//找到要被复制的布局
		const char* oldLayoutName = copyname;
		DbObjectId oldLayoutId = this->findLayoutNamed(copyname);
		const char* newLayoutname = newname;
		DbObjectId newLayoutId;

		//获得数据库对象
		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, oldLayoutId) != 0) {
			break;
		}
		DbDatabase* pDatabse = pLayout->database();
		pLayout->close();

		//克隆布局
		DbLayout* pNewLayout = NULL;
		this->cloneLayout(pNewLayout, newname, -1, pDb);


		//创建块表记录
		DbObjectId newBlockTableRecordId = NULL;
		do{

			//获得布局的块表记录
			DbLayout* pLayout = NULL;
			if (::acdbOpenObject(pLayout, oldLayoutId) != 0) {
				break;
			}
			DbObjectId blockTableRecordId = pLayout->getBlockTableRecordId();
			pLayout->close();

			//复制块表记录
			DbBlockTableRecord* pBlockTableRecord = NULL;
			if (::acdbOpenObject(pBlockTableRecord, blockTableRecordId) != 0) {
				break;
			}
			DbBlockTableRecord* pNewBlockTableRecord = pBlockTableRecord->clone();
			pNewBlockTableRecord->setName(ACDB_PAPER_SPACE);
			pBlockTableRecord->close();

			//获得数据库的块表
			DbBlockTable* pBlockTable = NULL;
			pLayout->database()->getBlockTable(pBlockTable, Db::OpenMode::kForWrite);
			if (pBlockTable == NULL) {
				break;
			}

			//添加块表记录到块表记录
			pBlockTable->add(newBlockTableRecordId, pNewBlockTableRecord);
			pBlockTable->close();

			//关闭块表
			pBlockTable->close();

		} while (false);


		//创建布局并加入到词典中
		while (newBlockTableRecordId != NULL) {

			//获得老布局名称
			AcString layoutName;
			pLayout->getName(layoutName);

			//获得新布局名称
			pLayout->getName(layoutName);
			AcString newLayoutName;
			for (int i = layoutName.length() - 1; i >= 0; i--) {
				if (i == layoutName.length() - 1) {
					if (layoutName[i] != ')') {
						break;
					}
				}
				if (layoutName[i] == ')') {
					break;
				}
				if (layoutName[i] == '(') {
					newLayoutName = layoutName.slice(0, i);
					break;
				}
			}
			for (int i = 1; i < 1000; i++) {
				AcString name;
				name.format(_T("%s(%d)"), newLayoutName.c_str(), i + 1);
				if (this->layoutExists(name.c_str(), pDb) == false) {
					newLayoutName = name;
					break;
				}
			}

			oldLayoutName = layoutName.c_str();
			newLayoutname = newLayoutName.c_str();

			//复制布局
			DbLayout* pNewLayout = pLayout->clone();
			pNewLayout->setLayoutName(newLayoutName.c_str());
			pNewLayout->setBlockTableRecordId(newBlockTableRecordId);

			//添加布局到布局词典中
			{
				//打开布局词典表
				DbDictionary* pDictionary = NULL;
				pLayout->database()->getLayoutDictionary(pDictionary, Db::OpenMode::kForWrite);

				//添加到布局词典中
				pDictionary->setAt(newLayoutName.c_str(), pNewLayout, newLayoutId);

				//关闭布局词典
				pDictionary->close();
			}


			break;
		};


		//触发复制布局事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++) {
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutCopied(oldLayoutName, oldLayoutId, newLayoutname, newLayoutId);
		}


		es = Acad::ErrorStatus::eOk;

	} while (false);

#endif

	return es;
}
#if EMSDK
Acad::ErrorStatus DbLayoutManager::emsdk_deleteLayout(const emstring &delname)
{
	return this->deleteLayout(delname.c_str());
}
Acad::ErrorStatus DbLayoutManager::emsdk_deleteLayout(const emstring &delname, DbDatabase *pDb)
{
	return this->deleteLayout(delname.c_str(), pDb);
}
#endif
Acad::ErrorStatus DbLayoutManager::deleteLayout(const ACHAR*delname)
{
	return this->deleteLayout(delname, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::deleteLayout(const ACHAR*delname, DbDatabase *pDb)
{

	do
	{
		AcString layoutName = delname;

		// 如果是模型空间则退出
		if (layoutName.isEqual("Model", true) == true) {
			return Acad::ErrorStatus::eFail;
		}

		// 打开布局词典表
		DbDictionary *pDictionary = NULL;
		pDb->getLayoutDictionary(pDictionary, Db::OpenMode::kForWrite);

		// 遍历
		DbObjectId layoutId = NULL;
		DbObjectId lastLayoutId = NULL;
		DbDictionaryIterator *pDictionaryIterator = pDictionary->newIterator();
		for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
		{
			if (layoutName.isEqual(pDictionaryIterator->name().c_str(), true) == true)
			{
				layoutId = pDictionaryIterator->objectId();
				break;
			}
			else {
				lastLayoutId = pDictionaryIterator->objectId();
			}
		}
		delete pDictionaryIterator;
		if (layoutId == NULL) {
			break;
		}

		// 触发开始删除布局事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
		{
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutToBeRemoved(delname, layoutId);
		}

		// 设置删除
		DbLayout *pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId, Db::OpenMode::kForWrite) == 0)
		{
			pLayout->erase();
			pLayout->close();
		}

		// 关闭布局词典
		pDictionary->close();

		// 触发删除布局事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
		{
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutRemoved(delname, layoutId);
		}

		// 触发刷新布局选项卡事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
		{
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->refreshLayoutTabs();
		}

		// 设置新布局
		if (lastLayoutId != NULL)
		{
			this->setCurrentLayoutId(lastLayoutId);
		}

	} while (false);

	return Acad::ErrorStatus::eOk;
}
#if EMSDK
DbObjectId DbLayoutManager::emsdk_createLayout(const emstring &newname)
{
	DbObjectId layoutId = NULL;
	DbObjectId blockTableRecId = NULL;
	this->createLayout(newname.c_str(), layoutId, blockTableRecId);
	return layoutId;
}
DbObjectId DbLayoutManager::emsdk_createLayout(const emstring &newname, DbDatabase *pDb)
{
	DbObjectId layoutId = NULL;
	DbObjectId blockTableRecId = NULL;
	this->createLayout(newname.c_str(), layoutId, blockTableRecId, pDb);
	return layoutId;
}
#endif
Acad::ErrorStatus DbLayoutManager::createLayout(const ACHAR*newname, DbObjectId &layoutId, DbObjectId &blockTableRecId)
{
	return this->createLayout(newname, layoutId, blockTableRecId, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::createLayout(const ACHAR*newname, DbObjectId &layoutId, DbObjectId &blockTableRecId, DbDatabase *pDb)
{

	// 判断布局是否存在
	if (this->layoutExists(newname, pDb) == true)
	{
		return Acad::ErrorStatus::eFail;
	}

	// 获得数据库的块表
	DbBlockTable *pBlockTable = NULL;
	pDb->getBlockTable(pBlockTable, Db::OpenMode::kForWrite);
	if (pBlockTable == NULL)
	{
		return Acad::ErrorStatus::eFail;
	}

	// 创建块表记录
	DbObjectId blockTableRecordId = NULL;
	DbBlockTableRecord *pBlockTableRecord = new DbBlockTableRecord();
	pBlockTableRecord->setName(ACDB_PAPER_SPACE);
	pBlockTable->add(blockTableRecordId, pBlockTableRecord);
	pBlockTableRecord->close();
	pBlockTable->close();

	// 创建布局
	DbLayout *pLayout = new DbLayout();
	pLayout->setLayoutName(newname);
	pLayout->setTabSelected(false);
	pLayout->addToLayoutDict(pDb, blockTableRecordId);

	// 触发事件
	for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
	{
		DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutCreated(newname, layoutId);
		DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->refreshLayoutTabs();
	}
	return Acad::ErrorStatus::eOk;
}
#if EMSDK
Acad::ErrorStatus DbLayoutManager::emsdk_renameLayout(const emstring &oldname, const emstring &newname)
{
	return this->renameLayout(oldname.c_str(), newname.c_str(), acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::emsdk_renameLayout(const emstring &oldname, const emstring &newname, DbDatabase *pDb)
{
	return this->renameLayout(oldname.c_str(), newname.c_str(), pDb);
}
#endif
Acad::ErrorStatus DbLayoutManager::renameLayout(const ACHAR*oldname, const ACHAR*newname)
{
	return this->renameLayout(oldname, newname, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::renameLayout(const ACHAR*oldname, const ACHAR*newname, DbDatabase *pDb)
{

	do
	{

		// 判断布局是否存在
		if (this->layoutExists(newname, pDb) == true)
		{
			return Acad::ErrorStatus::eFail;
		}

		// 获得布局对象
		DbObjectId layoutId = this->findLayoutNamed(oldname, pDb);
		if (layoutId == NULL)
		{
			break;
		}

		// 触发修改布局名称开始事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
		{
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutToBeRenamed(oldname, newname, layoutId);
		}

		// 设置名称
		DbLayout *pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId, Db::OpenMode::kForWrite) == 0)
		{
			pLayout->setLayoutName(newname);
			pLayout->close();
		}
		pLayout->close();

		// 触发修改布局名称事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
		{
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->layoutRenamed(oldname, newname, layoutId);
		}

		// 触发刷新布局选项卡事件
		for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
		{
			DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.at(i)->refreshLayoutTabs();
		}

	} while (false);

	return Acad::ErrorStatus::eOk;
}
#if EMSDK
Acad::ErrorStatus DbLayoutManager::emsdk_cloneLayout(DbLayout *pLBTR, const emstring &newname)
{
	return this->cloneLayout(pLBTR, newname.c_str());
}
Acad::ErrorStatus DbLayoutManager::emsdk_cloneLayout(DbLayout *pLBTR, const emstring &newname, int newTabOrder)
{
	return this->cloneLayout(pLBTR, newname.c_str(), newTabOrder);
}
Acad::ErrorStatus DbLayoutManager::emsdk_cloneLayout(DbLayout *pLBTR, const emstring &newname, int newTabOrder, DbDatabase *pDb)
{
	return this->cloneLayout(pLBTR, newname.c_str(), newTabOrder, pDb);
}
#endif
Acad::ErrorStatus DbLayoutManager::cloneLayout(DbLayout *pLBTR, const ACHAR*newname)
{
	return this->cloneLayout(pLBTR, newname, 0, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::cloneLayout(DbLayout *pLBTR, const ACHAR*newname, int newTabOrder)
{
	return this->cloneLayout(pLBTR, newname, newTabOrder, acdbHostApplicationServices()->workingDatabase());
}
Acad::ErrorStatus DbLayoutManager::cloneLayout(DbLayout *pLBTR, const ACHAR*newname, int newTabOrder, DbDatabase *pDb)
{

	do
	{
		// 获得当前布局
		DbObjectId layoutId = this->getActiveLayoutBTRId();

		// 打开布局对象
		DbLayout *pLayoyt = NULL;
		if (::acdbOpenObject(pLayoyt, layoutId) != 0)
		{
			break;
		}

		// 克隆布局
		pLBTR = DbLayout::cast(pLayoyt->clone());
		pLBTR->setLayoutName(newname);
		pLBTR->setTabOrder(newTabOrder);

		// 关闭对象
		pLayoyt->close();

	} while (false);

	return Acad::ErrorStatus::eOk;
}
DbObjectId DbLayoutManager::getNonRectVPIdFromClipId(DbObjectId clipId)
{
	return NULL;
}
bool DbLayoutManager::isVpnumClipped(int index)
{
	return this->isVpnumClipped(index, acdbHostApplicationServices()->workingDatabase());
}
bool DbLayoutManager::isVpnumClipped(int index, const DbDatabase *pDb)
{
	return false;
}
int DbLayoutManager::countLayouts()
{
	return this->countLayouts(acdbHostApplicationServices()->workingDatabase());
}
int DbLayoutManager::countLayouts(DbDatabase *pDb)
{

	int count = 0;

	// 获得布局词典
	DbDictionary *pLayoutDictionary = NULL;
	pDb->getLayoutDictionary(pLayoutDictionary, Db::OpenMode::kForRead);

	// 创建布局遍历器
	DbDictionaryIterator *pDictionaryIterator = pLayoutDictionary->newIterator();

	// 获得数量
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next())
	{
		count++;
	}

	// 删除遍历器
	delete pDictionaryIterator;

	return count;
}
void DbLayoutManager::addReactor(DbLayoutManagerReactor *newObj)
{
	DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.append(newObj);
}
void DbLayoutManager::removeReactor(DbLayoutManagerReactor *delObj)
{
	for (int i = 0; i < DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor.length(); i++)
	{
		if (DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor[i] == delObj)
		{
			realloc_arry_at(DB_IMP_LAYOUTMANAGER(this->m_pImpl)->layoutManagerReactor, i);
			break;
		}
	}
}
