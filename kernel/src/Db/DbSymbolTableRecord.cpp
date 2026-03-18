#include "DbSymbolTableRecord.h"
#include "DbObjectManager.h"
#include "DbDwgFiler.h"
#include "kernel.h"
#include "DbImpl.h"


DbSymbolTableRecord::DbSymbolTableRecord() {

}
DbSymbolTableRecord::~DbSymbolTableRecord() {

}


Acad::ErrorStatus DbSymbolTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	ACHAR* pName;
	pFiler->readString(&pName);
	this->setName(pName);


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSymbolTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	ACHAR* pName;
	this->getName(pName);
	pFiler->writeString(pName);

	return Acad::ErrorStatus::eOk;
}



Acad::ErrorStatus DbSymbolTableRecord::getName(ACHAR*& pName) const {
	::acutNewString(DB_IMP_SYMBOLTABLERECORD(this->m_pImpl)->name, pName);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSymbolTableRecord::getName(AcString& sName) const
{
	sName = DB_IMP_SYMBOLTABLERECORD(this->m_pImpl)->name;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSymbolTableRecord::getName(const ACHAR*& pName) const {
	pName = DB_IMP_SYMBOLTABLERECORD(this->m_pImpl)->name.constPtr();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSymbolTableRecord::setName(const ACHAR* pName) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DbObjectId ownerId = this->ownerId();
		DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(ownerId);
		if (object != NULL) {
			DB_IMP_SYMBOLTABLE(object->m_pImpl)->pMap->updateRecordKey(DB_IMP_SYMBOLTABLERECORD(this->m_pImpl)->name, AcString(pName), true);
		}
		DB_IMP_SYMBOLTABLERECORD(this->m_pImpl)->name = pName;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
emstring DbSymbolTableRecord::emsdk_getName() const {
	AcString pName;
	this->getName(pName);
	return pName.constPtr();
}
Acad::ErrorStatus DbSymbolTableRecord::emsdk_setName(const emstring& pName) {
	return this->setName(pName.c_str());
}
#endif