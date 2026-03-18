#include "DbHandleManage.h"
#include "DbHandleTable.h"
#include "DbObjectId.h"
#include "DbDatabase.h"

DbHandleManage::DbHandleManage() {

}
DbHandleManage::~DbHandleManage() {
	realloc_arry_all(this->tables);
}

// 添加数据库
DbHandleTable* DbHandleManage::appendDatabase(DbDatabase* _database) {
	DbHandleTable* pTable = new DbHandleTable();
	pTable->setDatabase(_database);
	this->tables.append(pTable);
	return pTable;
}

// 查询句柄表
DbHandleTable* DbHandleManage::queryHandleTable(DbDatabase* _database) {
	for (int i = 0; i < this->tables.length(); i++) {
		if (this->tables.at(i)->database() == _database) {
			return this->tables.at(i);
		}
	}
	return NULL;
}

// 移除数据库
void DbHandleManage::removeDatabase(const DbDatabase* _database) {
	for (int i = 0; i < this->tables.length(); i++) {
		if (this->tables.at(i)->database() == _database) {
			realloc_arry_at(this->tables, i);
			return;
		}
	}
}

// 查询句柄记录
DbHandleTableRecord* DbHandleManage::queryHandleTableRecord(DbDatabase* _database, unsigned int handle, bool createIfNotFound) {
	
	DbHandleTable* pTable = NULL;
	for (int i = 0; i < this->tables.length(); i++) {
		if (this->tables.at(i)->database() == _database) {
			pTable = this->tables.at(i);
			break;
		}
	}
	if (pTable == NULL) {
		if (createIfNotFound == true) {
			pTable = new DbHandleTable();
			this->tables.append(pTable);
		}
		else {
			return NULL;
		}
	}

	return pTable->queryRecord(handle);
}