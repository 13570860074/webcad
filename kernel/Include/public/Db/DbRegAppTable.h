#ifndef DBREGAPPTABLE_H
#define DBREGAPPTABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbRegAppTableRecord.h"


class DbRegAppTableIterator;
class DbRegAppTable : public DbSymbolTable
{
public:
    DbRegAppTable();
    virtual ~DbRegAppTable();
    DB_DECLARE_MEMBERS(DbRegAppTable);


    Acad::ErrorStatus getAt(
        const char* entryName,
        DbRegAppTableRecord*& pRec,
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
    Acad::ErrorStatus newIterator(DbRegAppTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbRegAppTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbRegAppTableRecord* pRecord);
};



#endif