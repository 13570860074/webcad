#ifndef DBVIEWTABLE_H
#define DBVIEWTABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbViewTableRecord.h"


class DbViewTableIterator;
class DbViewTable : public DbSymbolTable
{
public:
    DbViewTable();
    virtual ~DbViewTable();
    DB_DECLARE_MEMBERS(DbViewTable);

    Acad::ErrorStatus getAt(
        const char* entryName,
        DbViewTableRecord*& pRec,
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
    Acad::ErrorStatus newIterator(DbViewTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbViewTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbViewTableRecord* pRecord);
};



#endif