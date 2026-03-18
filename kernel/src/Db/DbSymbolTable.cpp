#include "kernel.h"
#include "DbObjectManager.h"
#include "DbDatabase.h"
#include "DbSymbolTable.h"
#include "DbViewportTable.h"
#include "DbViewTable.h"
#include "DbImpl.h"

DbSymbolTable::~DbSymbolTable()
{
}

Acad::ErrorStatus DbSymbolTable::getIdAt(const ACHAR *entryName, DbObjectId &recordId, bool getErasedRecord) const
{

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	DbSymbolTableRecord *record = DB_IMP_SYMBOLTABLE(this->m_pImpl)->pMap->find(AcString(entryName), true);
	if (record != NULL)
	{
		if (record->isErased() == false)
		{
			es = Acad::ErrorStatus::eOk;
			recordId = record->objectId();
		}
		else if (record->isErased() == true && getErasedRecord == true)
		{
			es = Acad::ErrorStatus::eOk;
			recordId = record->objectId();
		}
	}

	return es;
}

bool DbSymbolTable::has(DbObjectId id) const
{
	bool isHas = false;

	do
	{
		DbSymbolTableIterator *pSymbolTableIterator = NULL;
		this->newIterator(pSymbolTableIterator);

		for (pSymbolTableIterator->start(); !pSymbolTableIterator->done(); pSymbolTableIterator->step())
		{
			DbSymbolTableRecord *pRecord = NULL;
			pSymbolTableIterator->getRecord(pRecord);
			if (pRecord == NULL)
			{
				continue;
			}

			if (pRecord->objectId() == id)
			{
				isHas = true;
			}

			pRecord->close();
		}

		delete pSymbolTableIterator;

	} while (false);

	return isHas;
}

Acad::ErrorStatus DbSymbolTable::add(DbSymbolTableRecord *pRecord)
{
	DbObjectId recordId = NULL;
	return add(recordId, pRecord);
}
Acad::ErrorStatus DbSymbolTable::add(DbObjectId &recordId, DbSymbolTableRecord *pRecord)
{
	this->database()->addDbObject(recordId, pRecord);
	if (recordId == NULL)
	{
		return Acad::ErrorStatus::eFail;
	}
	AcString name;
	pRecord->getName(name);
	pRecord->setOwnerId(this->objectId());
	DB_IMP_SYMBOLTABLE(this->m_pImpl)->pMap->insert(name, pRecord);
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbSymbolTable::newIterator(DbSymbolTableIterator *&pIterator,
											 bool atBeginning,
											 bool skipDeleted) const
{
	AcArray<DbSymbolTableRecord *> allObjs;
	DB_IMP_SYMBOLTABLE(this->m_pImpl)->pMap->findAll(allObjs);

	AcArray<DbSymbolTableRecord *> objs;
	for (int i = 0; i < allObjs.length(); i++)
	{
		if(allObjs[i]->isErased() == true && skipDeleted == true){
			continue;
		}
		objs.append(allObjs[i]);
	}

	DB_IMP_SYMBOLTABLEITERATOR(pIterator->m_pImpl)->iterator = AcIterator<RxObject *>((AcArray<RxObject *> &)objs);

	return Acad::ErrorStatus::eFail;
}

#if EMSDK

DbSymbolTableRecord *DbSymbolTable::emsdk_getAt(const emstring &entryName) const
{
	return this->emsdk_getAt(entryName, Db::OpenMode::kForRead);
}
DbSymbolTableRecord *DbSymbolTable::emsdk_getAt(const emstring &entryName, Db::OpenMode openMode) const
{
	return this->emsdk_getAt(entryName, openMode, false);
}
DbSymbolTableRecord *DbSymbolTable::emsdk_getAt(const emstring &entryName, Db::OpenMode openMode, bool openErasedRec) const
{
	DbSymbolTableRecord *pSymbolTableRecord = NULL;
	this->getAt(entryName.c_str(), pSymbolTableRecord, openMode, openErasedRec);
	return pSymbolTableRecord;
}

DbObjectId DbSymbolTable::emsdk_getIdAt(const emstring &entryName) const
{
	return this->emsdk_getIdAt(entryName.c_str(), false);
}
DbObjectId DbSymbolTable::emsdk_getIdAt(const emstring &entryName, bool getErasedRecord) const
{
	DbObjectId id;
	this->getIdAt(entryName.c_str(), id, getErasedRecord);
	return id;
}

bool DbSymbolTable::emsdk_has(const emstring &name) const
{
	return this->has(name.c_str());
}
bool DbSymbolTable::emsdk_has(DbObjectId id) const
{
	return this->has(id);
}

DbSymbolTableIterator *DbSymbolTable::emsdk_newIterator() const
{
	return this->emsdk_newIterator(true, true);
}
DbSymbolTableIterator *DbSymbolTable::emsdk_newIterator(bool atBeginning) const
{
	return this->emsdk_newIterator(atBeginning, true);
}
DbSymbolTableIterator *DbSymbolTable::emsdk_newIterator(bool atBeginning, bool skipDeleted) const
{
	DbSymbolTableIterator *pSymbolTableIterator = NULL;
	this->newIterator(pSymbolTableIterator, atBeginning, skipDeleted);
	return pSymbolTableIterator;
}

#endif

Acad::ErrorStatus DbSymbolTable::dwgInFields(DbDwgFiler *pFiler)
{

	Acad::ErrorStatus es = DbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSymbolTable::dwgOutFields(DbDwgFiler *pFiler) const
{

	Acad::ErrorStatus es = DbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
	{
		return (es);
	}

	return Acad::ErrorStatus::eOk;
}