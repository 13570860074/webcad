#ifndef DBOBJECTMANAGER_H
#define DBOBJECTMANAGER_H

#include "AcArray.h"
#include "AcString.h"
#include "RxManager.h"

class DbObject;
class DbObjectId;
class DbHandle;
class DbDatabase;
class DbHandleTableRecord;
class DbObjectManager : public RxObject, public RxManager
{
public:
	DbObjectManager();
	virtual ~DbObjectManager();
	RX_DECLARE_MEMBERS(DbObjectManager);
	RX_MANAGER_DECLARE_MEMBERS(DbObjectManager);

public:
	static void onBeginAnimationFrame(DbObjectManager *pointer);

public:
	/* 初始化 */
	virtual void onInit();

	/** 添加对象 */
	DbObjectId addDbObject(DbObject *_pObject);
	DbObjectId addDbObject(DbObject *_pObject, DbDatabase *database);

	/** 打开对象 */
	DbObject *openDbObject(unsigned int _id, const bool isInspect = true);
	DbObject *openDbObject(DbObjectId _ObjectId, const bool isInspect = true);

	/** 释放数据库中的所有对象 */
	void freeDatabase(DbDatabase *_database);

	/** 释放对象 */
	void freeObject(DbObject *_object);

	/** 查询数据库对象 */
	const AcArray<DbObject*>* queryObjects();

	/** 添加句柄记录 */
	DbHandleTableRecord* appendHandleTableRecord(DbDatabase* _database, unsigned int _objectId, unsigned int handle);

	/** 查询句柄记录 */
	DbHandleTableRecord* queryHandleTableRecord(DbDatabase* _database, unsigned int handle);
};

#endif
