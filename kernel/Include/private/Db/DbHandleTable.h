#ifndef DBHANDLETABLE_H
#define DBHANDLETABLE_H

#include "AcArray.h"
#include "DbHandleTableRecord.h"

class DbObject;
class DbDatabase;
class DbObjectId;
class DbHandleTable {
public:
	DbHandleTable();
	virtual ~DbHandleTable();

private:
	DbDatabase *pDatabase;
	AcArray<DbHandleTableRecord*>* pRecord;


public:

	// 获得数据库对象
	DbDatabase* database();

	// 设置数据库对象
	void setDatabase(DbDatabase* _database);

	// 添加句柄
	DbHandleTableRecord *appendHandle(DbObject *_object);
	DbHandleTableRecord* appendHandle(unsigned int _objectId, unsigned int handle);

	// 通过handle查询记录
	DbHandleTableRecord* queryRecord(unsigned int handle);
};


#endif

