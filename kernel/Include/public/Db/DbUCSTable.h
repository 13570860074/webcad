#ifndef DBUCSTABLE_H
#define DBUCSTABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbUCSTableRecord.h"


class DbUCSTableIterator;
class DbUCSTable : public DbSymbolTable
{
public:
    DbUCSTable();
    virtual ~DbUCSTable();
    DB_DECLARE_MEMBERS(DbUCSTable);

    Acad::ErrorStatus getAt(
        const char* entryName,
        DbUCSTableRecord*& pRec,
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
    Acad::ErrorStatus newIterator(DbUCSTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbUCSTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbUCSTableRecord* pRecord);
};



#endif