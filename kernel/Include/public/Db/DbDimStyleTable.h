#ifndef DBDIMSTYLETABLE_H
#define DBDIMSTYLETABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbDimStyleTableRecord.h"


class DbDimStyleTableIterator;
class DbDimStyleTable : public DbSymbolTable
{
public:
    DbDimStyleTable();
    virtual ~DbDimStyleTable();
    DB_DECLARE_MEMBERS(DbDimStyleTable);

    Acad::ErrorStatus getAt(
        const char* entryName,
        DbDimStyleTableRecord*& pRec,
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
    Acad::ErrorStatus newIterator(DbDimStyleTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbDimStyleTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbDimStyleTableRecord* pRecord);
};



#endif