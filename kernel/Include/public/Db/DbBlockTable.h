#ifndef DBBLOCKTABLE_H
#define DBBLOCKTABLE_H


#include "DbSymbolTable.h"
#include "DbBlockTableIterator.h"
#include "DbBlockTableRecord.h"

class DbBlockTable : public DbSymbolTable
{
public:
    DbBlockTable();
    virtual ~DbBlockTable();
    DB_DECLARE_MEMBERS(DbBlockTable);


    Acad::ErrorStatus getAt(
        const ACHAR* entryName,
        DbBlockTableRecord*& pRec,
        Db::OpenMode openMode = Db::kForRead,
        bool openErasedRec = false) const
    {
        return this->openAt(entryName, (DbSymbolTableRecord*&)pRec, openMode, openErasedRec);
    }
    Acad::ErrorStatus getAt(
        const ACHAR* entryName,
        DbObjectId& recordId,
        bool getErasedRecord = false) const
    {
        return this->getIdAt(entryName, recordId, getErasedRecord);
    }
    Acad::ErrorStatus newIterator(DbBlockTableIterator*& pIterator,
        bool atBeginning = true,
        bool skipDeleted = true) const;

    Acad::ErrorStatus add(DbBlockTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbBlockTableRecord* pRecord);
    Acad::ErrorStatus add(DbBlockTableRecord* pRecord, bool _isAllowSamename);
    Acad::ErrorStatus add(DbObjectId& recordId, DbBlockTableRecord* pRecord, bool _isAllowSamename);

#if EMSDK
    DbBlockTableRecord* emsdk_getAt(const emstring& entryName) const;
    DbBlockTableRecord* emsdk_getAt(const emstring& entryName, Db::OpenMode openMode) const;
    DbBlockTableRecord* emsdk_getAt(const emstring& entryName, Db::OpenMode openMode, bool openErasedRec) const;

    DbBlockTableIterator* emsdk_newIterator()const;
    DbBlockTableIterator* emsdk_newIterator(bool atBeginning)const;
    DbBlockTableIterator* emsdk_newIterator(bool atBeginning, bool skipDeleted)const;
#endif
};



#endif