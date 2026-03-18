#ifndef DBSYMBOLTABLE_H
#define DBSYMBOLTABLE_H


#include "Db.h"
#include "DbObject.h"
#include "DbSymbolTableIterator.h"


class DbSymbolTable : public DbObject
{
public:
	virtual ~DbSymbolTable();
	DB_DECLARE_MEMBERS_VIRTUAL(DbSymbolTable);


	typedef DbSymbolTableRecord RecordType;

	Acad::ErrorStatus getAt(const ACHAR* entryName,
		DbSymbolTableRecord*& pRec,
		Db::OpenMode openMode = Db::kForRead,
		bool openErasedRec = false) const
	{
		return this->openAt(entryName, pRec, openMode, openErasedRec);
	}

	Acad::ErrorStatus getIdAt(const ACHAR* entryName,
		DbObjectId& recordId,
		bool getErasedRecord = false) const;
	Acad::ErrorStatus getAt(const ACHAR* entryName,
		DbObjectId& recordId,
		bool getErasedRecord = false) const
	{
		return this->getIdAt(entryName, recordId, getErasedRecord);
	}

	bool has(const ACHAR* name) const
	{
		DbObjectId id;
		return this->getIdAt(name, id) == Acad::eOk;
	}

	bool      has(DbObjectId id) const;

	virtual Acad::ErrorStatus newIterator(DbSymbolTableIterator*& pIterator,
		bool atBeginning = true,
		bool skipDeleted = true) const;

	Acad::ErrorStatus add(DbSymbolTableRecord* pRecord);
	Acad::ErrorStatus add(DbObjectId& recordId, DbSymbolTableRecord* pRecord);

#if EMSDK
	DbSymbolTableRecord* emsdk_getAt(const emstring& entryName) const;
	DbSymbolTableRecord* emsdk_getAt(const emstring& entryName, Db::OpenMode openMode) const;
	DbSymbolTableRecord* emsdk_getAt(const emstring& entryName, Db::OpenMode openMode, bool openErasedRec) const;

	DbObjectId emsdk_getIdAt(const emstring& entryName) const;
	DbObjectId emsdk_getIdAt(const emstring& entryName, bool getErasedRecord) const;

	bool emsdk_has(const emstring& name) const;
	bool emsdk_has(DbObjectId id) const;

	virtual DbSymbolTableIterator* emsdk_newIterator()const;
	virtual DbSymbolTableIterator* emsdk_newIterator(bool atBeginning)const;
	virtual DbSymbolTableIterator* emsdk_newIterator(bool atBeginning, bool skipDeleted)const;
#endif


protected:
	template<class RecordType> Acad::ErrorStatus openAt(const ACHAR* entryName,
		RecordType*& pRec, Db::OpenMode nMode, bool bOpenErased) const
	{
		pRec = nullptr;
		DbObjectId id;
		Acad::ErrorStatus es = this->getIdAt(entryName, id, bOpenErased);
		if (es == Acad::eOk)
			es = ::acdbOpenObject(pRec, id, nMode, bOpenErased);
		return es;
	}
};



#endif