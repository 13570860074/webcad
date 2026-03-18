#ifndef DBHANDLEMANAGE_H
#define DBHANDLEMANAGE_H


#include "AcArray.h"
#include "DbHandleTable.h"
#include "DbObjectId.h"

class DbDatabase;
class DbHandleTableRecord;
class DbHandleManage {
public:
	DbHandleManage();
	virtual ~DbHandleManage();

private:
	AcArray<DbHandleTable*> tables;

public:
	// 添加数据库
	DbHandleTable *appendDatabase(DbDatabase* _database);

	// 查询句柄表
	DbHandleTable* queryHandleTable(DbDatabase* _database);

	// 移除数据库
	void removeDatabase(const DbDatabase* _database);

	// 查询句柄记录
	DbHandleTableRecord* queryHandleTableRecord(DbDatabase* database, unsigned int handle, bool createIfNotFound = false);
};


#endif