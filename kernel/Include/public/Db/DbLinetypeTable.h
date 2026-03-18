#ifndef DBLINETYPETABLE_H
#define DBLINETYPETABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbLinetypeTableRecord.h"


class DbLinetypeTableIterator;
class DbLinetypeTable : public DbSymbolTable
{
public:
    DbLinetypeTable();
    virtual ~DbLinetypeTable();
    DB_DECLARE_MEMBERS(DbLinetypeTable);

    Acad::ErrorStatus getAt(
        const char* entryName,
        DbLinetypeTableRecord*& pRec,
        Db::OpenMode openMode = Db::kForRead,
        bool openErasedRec = false) const
    {
        return this->openAt(entryName, (DbSymbolTableRecord*&)pRec, openMode, openErasedRec);
    }
    Acad::ErrorStatus getAt(
        const char* entryName,
        DbObjectId& recordId,
        bool getErasedRecord = false) const
    {
        return this->getIdAt(entryName, recordId, getErasedRecord);
    }
    Acad::ErrorStatus newIterator(DbLinetypeTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbLinetypeTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbLinetypeTableRecord* pRecord);
};



#endif