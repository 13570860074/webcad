#ifndef DBTEXTSTYLETABLE_H
#define DBTEXTSTYLETABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbTextStyleTableRecord.h"


class DbTextStyleTableIterator;
class DbTextStyleTable : public DbSymbolTable
{
public:
    DbTextStyleTable();
    virtual ~DbTextStyleTable();
    DB_DECLARE_MEMBERS(DbTextStyleTable);


    Acad::ErrorStatus getAt(
        const char* entryName,
        DbTextStyleTableRecord*& pRec,
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
    Acad::ErrorStatus newIterator(DbTextStyleTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbTextStyleTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbTextStyleTableRecord* pRecord);
};



#endif