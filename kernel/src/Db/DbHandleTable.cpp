#include "DbHandleTable.h"
#include "DbDatabase.h"
#include "DbObjectId.h"


DbHandleTable::DbHandleTable() {
	this->pDatabase = NULL;
	this->pRecord = new AcArray<DbHandleTableRecord*>();
}
DbHandleTable::~DbHandleTable() {
	realloc_all(this->pRecord);
}

DbDatabase* DbHandleTable::database() {
	return this->pDatabase;
}
void DbHandleTable::setDatabase(DbDatabase* _database) {
	this->pDatabase = _database;
}
DbHandleTableRecord* DbHandleTable::appendHandle(DbObject* _object) {
	DbHandle handle;
	_object->getDbHandle(handle);
	return this->appendHandle(_object->objectId().asOldId(), handle);
}
DbHandleTableRecord *DbHandleTable::appendHandle(unsigned int _objectId, unsigned int handle) {

	DbHandleTableRecord* pRecord = NULL;

	if (handle < this->pRecord->length() + 1) {
		if (this->pRecord->at(handle - 1) == NULL) {
			pRecord = new DbHandleTableRecord();
			pRecord->objectId = _objectId;
			pRecord->handle = handle;
			this->pRecord->setAt(handle - 1, pRecord);
		}
		else {
			this->pRecord->at(handle - 1)->objectId = _objectId;
			this->pRecord->at(handle - 1)->handle = handle;
			pRecord = this->pRecord->at(handle - 1);
		}
	}
	else if (handle == this->pRecord->length() + 1) {
		pRecord = new DbHandleTableRecord();
		pRecord->objectId = _objectId;
		pRecord->handle = handle;
		this->pRecord->append(pRecord);
	}
	else {
		for (int i = this->pRecord->length(); i < handle; i++) {
			if (i == handle - 1) {
				pRecord = new DbHandleTableRecord();
				pRecord->objectId = _objectId;
				pRecord->handle = handle;
				this->pRecord->append(pRecord);
			}
			else {
				this->pRecord->append(NULL);
			}
		}
	}

	return pRecord;
}
DbHandleTableRecord* DbHandleTable::queryRecord(unsigned int handle) {
	if (handle > this->pRecord->length()) {
		return NULL;
	}
	return this->pRecord->at(handle - 1);
}