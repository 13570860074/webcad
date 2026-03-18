#include "DbObjectManager.h"
#include "EvEventManager.h"
#include "DbObject.h"
#include "DbObjectId.h"
#include "DbDatabase.h"
#include "DbHandleTable.h"
#include "DbDatabaseImpl.h"
#include "kernel.h"
#include "DbImpl.h"
#include "EdImpl.h"

DbObjectManager::DbObjectManager()
{
	this->m_pImpl = new DbObjectManagerImpl();
}
DbObjectManager::~DbObjectManager()
{
}

void DbObjectManager::onBeginAnimationFrame(DbObjectManager *pointer)
{

}

void DbObjectManager::onInit()
{
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void *)onBeginAnimationFrame, this);
}

DbObjectId DbObjectManager::addDbObject(DbObject *object)
{
	return this->addDbObject(object, object->database());
}
DbObjectId DbObjectManager::addDbObject(DbObject *object, DbDatabase *database)
{

	// 判空
	if (object == NULL || database == NULL)
	{
		return NULL;
	}

	// 创建一个ObjectId
	DB_IMP_OBJECTMANAGER(this->m_pImpl)->overallId++;
	DB_IMP_OBJECT(object->m_pImpl)->objectId.setFromOldId(DB_IMP_OBJECTMANAGER(this->m_pImpl)->overallId);

	// 添加句柄
	DbHandle handle;
	object->getDbHandle(handle);
	if (handle.isNull() == true)
	{
		unsigned int handle = database->handseed();
		this->appendHandleTableRecord(database, DB_IMP_OBJECTMANAGER(this->m_pImpl)->overallId, handle);
		database->setHandseed(handle + 1);
		DB_IMP_OBJECT(object->m_pImpl)->handle = handle;
	}
	else
	{
		unsigned int handle = DB_IMP_OBJECT(object->m_pImpl)->handle;
		this->appendHandleTableRecord(database, DB_IMP_OBJECTMANAGER(this->m_pImpl)->overallId, handle);
		if (handle >= database->handseed())
		{
			database->setHandseed(handle + 1);
		}
	}

	// 添加到链表
	DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.append(object);

	return object->objectId();
}

void DbObjectManager::freeDatabase(DbDatabase *_database)
{
	for (int i = 0; i < DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.length(); i++)
	{
		if (DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.at(i)->database() == _database)
		{
			auto *temp = DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.at(i);
			delete temp;
			DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.setAt(i, NULL);
		}
	}
}
void DbObjectManager::freeObject(DbObject *object)
{
	if (object == NULL)
	{
		return;
	}
	unsigned int asOldId = object->objectId().asOldId();
	if (asOldId == 0)
	{
		return;
	}
	DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.setAt(asOldId - 1, NULL);
	delete object;
}


const AcArray<DbObject*>* DbObjectManager::queryObjects() {
	return &DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects;
}

DbObject *DbObjectManager::openDbObject(unsigned int _id, const bool isInspect)
{

	if (isInspect == false)
	{
		return DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.at(_id - 1);
	}

	DbObject *pObject = NULL;

	do
	{
		// 判空
		if (_id == 0)
		{
			break;
		}
		if (_id > DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.length() || _id <= 0)
		{
			break;
		}

		pObject = DB_IMP_OBJECTMANAGER(this->m_pImpl)->objects.at(_id - 1);

	} while (false);

	return pObject;
}
DbObject *DbObjectManager::openDbObject(DbObjectId _ObjectId, const bool isInspect)
{
	return openDbObject(_ObjectId.asOldId(), isInspect);
}


DbHandleTableRecord* DbObjectManager::appendHandleTableRecord(DbDatabase* _database, unsigned int _objectId, unsigned int handle)
{
	// 获得句柄表
	DbHandleTable* pHandleTable = DB_IMP_OBJECTMANAGER(this->m_pImpl)->handleManage->queryHandleTable(_database);
	if (pHandleTable == NULL)
	{
		pHandleTable = DB_IMP_OBJECTMANAGER(this->m_pImpl)->handleManage->appendDatabase(_database);
	}

	// 添加记录
	return pHandleTable->appendHandle(_objectId, handle);
}
DbHandleTableRecord* DbObjectManager::queryHandleTableRecord(DbDatabase* _database, unsigned int handle)
{
	// 获得句柄表
	DbHandleTable* pHandleTable = DB_IMP_OBJECTMANAGER(this->m_pImpl)->handleManage->queryHandleTable(_database);
	if (pHandleTable == NULL)
	{
		pHandleTable = DB_IMP_OBJECTMANAGER(this->m_pImpl)->handleManage->appendDatabase(_database);
	}
	return pHandleTable->queryRecord(handle);
}