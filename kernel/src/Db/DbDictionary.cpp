#include "kernel.h"
#include "DbDictionary.h"
#include "DbDatabase.h"
#include "DbObjectManager.h"
#include "DbDictionaryIterator.h"
#include "DbDictionaryRecord.h"
#include "DbImpl.h"



DbDictionary::DbDictionary() {
	this->m_pImpl = new DbDictionaryImpl();
}
DbDictionary::~DbDictionary() {

}


Acad::ErrorStatus DbDictionary::getAt(const ACHAR* entryName, DbObjectId& entryId) const {
	for (int i = 0; i < this->numEntries(); i++) {
		if (strcmp(DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->name, entryName) == 0) {
			entryId = DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->objectId;
			return Acad::ErrorStatus::eOk;
		}
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
DbObjectId DbDictionary::emsdk_getAt(const emstring& entryName) {
	DbObjectId entryId;
	this->getAt(entryName.c_str(), entryId);
	return entryId;
}
DbObject* DbDictionary::emsdk_getAt(const emstring& entryName, Db::OpenMode mode) {
	DbObject* object = NULL;
	this->getAt(entryName.c_str(), object, mode);
	return object;
}
#endif

Acad::ErrorStatus DbDictionary::nameAt(DbObjectId objId, ACHAR*& name) const {
	AcString sName;
	Acad::ErrorStatus es = this->nameAt(objId, sName);
	acutNewString(sName.constPtr(), name);
	return es;
}
Acad::ErrorStatus DbDictionary::nameAt(DbObjectId objId, AcString& sName) const {

	for (int i = 0; i < this->numEntries(); i++) {
		if (objId == DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->objectId) {
			sName = DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->name;
			break;
		}
	}
	return Acad::ErrorStatus::eOk;
}
#if EMSDK
const emstring DbDictionary::emsdk_nameAt(DbObjectId objId) {
	AcString sName;
	this->nameAt(objId, sName);
	return sName.constPtr();
}
#endif
#if EMSDK
bool DbDictionary::emsdk_has(const std::string& entryName) const {
	return this->has(entryName.c_str());
}
#endif
bool DbDictionary::has(const ACHAR* entryName) const {
	for (int i = 0; i < this->numEntries(); i++) {
		if (DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->name.isEqual(entryName, true) == true) {
			return true;
		}
	}
	return false;
}
bool DbDictionary::has(DbObjectId objId) const {
	for (int i = 0; i < this->numEntries(); i++) {
		if (objId == DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->objectId) {
			return true;
		}
	}
	return false;
}
UInt32 DbDictionary::numEntries() const {
	return DB_IMP_DICTIONARY(this->m_pImpl)->records.length();
}


Acad::ErrorStatus DbDictionary::remove(const ACHAR* key) {
	DbObjectId returnId = NULL;
	return this->remove(key, returnId);
}
Acad::ErrorStatus DbDictionary::remove(const ACHAR* key, DbObjectId& returnId) {
	for (int i = 0; i < this->numEntries(); i++) {
		if (strcmp(DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->name, key) == 0) {
			returnId = DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->objectId;
			realloc_arry_at(DB_IMP_DICTIONARY(this->m_pImpl)->records, i);
			return Acad::ErrorStatus::eOk;
		}
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDictionary::remove(DbObjectId  objId) {
	for (int i = 0; i < this->numEntries(); i++) {
		if (DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->objectId == objId) {
			realloc_arry_at(DB_IMP_DICTIONARY(this->m_pImpl)->records, i);
			return Acad::ErrorStatus::eOk;
		}
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
DbObjectId DbDictionary::emsdk_remove(const emstring& key) {
	DbObjectId objectId = NULL;
	this->remove(key.c_str());
	return objectId;
}
#endif


bool DbDictionary::setName(const ACHAR* oldName, const ACHAR* newName) {
	for (int i = 0; i < this->numEntries(); i++) {
		if (strcmp(DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->name, oldName) == 0) {
			DB_IMP_DICTIONARY(this->m_pImpl)->records[i]->name = newName;
			return true;
		}
	}
	return false;
}
#if EMSDK
bool DbDictionary::emsdk_setName(const emstring& oldName, const emstring& newName) {
	return this->setName(oldName.c_str(), newName.c_str());
}
#endif


Acad::ErrorStatus DbDictionary::setAt(const ACHAR* srchKey, DbObject* newValue, DbObjectId& retObjId) {

	this->database()->addDbObject(retObjId, newValue);

	DbDictionaryRecord* pDictionaryRecord = new DbDictionaryRecord();
	pDictionaryRecord->name = srchKey;
	pDictionaryRecord->objectId = retObjId;

	DB_IMP_DICTIONARY(this->m_pImpl)->records.append(pDictionaryRecord);
	return Acad::ErrorStatus::eOk;
}
#if EMSDK
DbObjectId DbDictionary::emsdk_setAt(const emstring& srchKey, DbObject* newValue) {
	DbObjectId retObjId = NULL;
	this->setAt(srchKey.c_str(), newValue, retObjId);
	return retObjId;
}
#endif

DbDictionaryIterator* DbDictionary::newIterator() const {

	AcArray<RxObject*>* list = new AcArray<RxObject*>();
	for (int i = 0; i < DB_IMP_DICTIONARY(this->m_pImpl)->records.length(); i++) {
		list->append(DB_IMP_DICTIONARY(this->m_pImpl)->records[i]);
	}

	DbDictionaryIterator* pIterator = new DbDictionaryIterator();
	DB_IMP_DICTIONARYITERATOR(pIterator->m_pImpl)->iterator = AcIterator<RxObject*>(list);
	return pIterator;
}




Acad::ErrorStatus DbDictionary::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDictionary::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}