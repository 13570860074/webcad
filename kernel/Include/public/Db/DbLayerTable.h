#ifndef DBLAYERTABLE_H
#define DBLAYERTABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbLayerTableRecord.h"
#include "DbLayerTableIterator.h"

class DbLayerTable : public DbSymbolTable
{
public:
    DbLayerTable();
    virtual ~DbLayerTable();
    DB_DECLARE_MEMBERS(DbLayerTable);

#if EMSDK
    DbLayerTableRecord* emsdk_getAt(const emstring& entryName) const;
    DbLayerTableRecord* emsdk_getAt(const emstring& entryName, Db::OpenMode openMode) const;
    DbLayerTableRecord* emsdk_getAt(const emstring& entryName, Db::OpenMode openMode, bool openErasedRec) const;

    DbLayerTableIterator* emsdk_newIterator()const;
#endif

    Acad::ErrorStatus getAt(const ACHAR* entryName, DbLayerTableRecord*& pRec, Db::OpenMode openMode = Db::kForRead, bool openErasedRec = false) const
    {
        return this->openAt(entryName, (DbSymbolTableRecord*&)pRec, openMode, openErasedRec);
    }
    Acad::ErrorStatus getAt(const ACHAR* entryName, DbObjectId& recordId, bool getErasedRecord = false) const
    {
        return this->getIdAt(entryName, recordId, getErasedRecord);
    }

    Acad::ErrorStatus newIterator(DbLayerTableIterator*& pIterator) const;

    Acad::ErrorStatus add(DbLayerTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbLayerTableRecord* pRecord);
};



#endif