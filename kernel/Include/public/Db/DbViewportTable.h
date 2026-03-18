#ifndef DBVIEWPORTTABLE_H
#define DBVIEWPORTTABLE_H


#include "Db.h"
#include "DbSymbolTable.h"
#include "DbViewportTableRecord.h"


class DbViewportTableIterator;
class DbViewportTable : public DbSymbolTable
{
public:
    DbViewportTable();
    virtual ~DbViewportTable();
    DB_DECLARE_MEMBERS(DbViewportTable);

    Acad::ErrorStatus getAt(
        const char* entryName,
        DbViewportTableRecord*& pRec,
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
    Acad::ErrorStatus newIterator(DbViewportTableIterator*& pIterator) const;
    Acad::ErrorStatus add(DbViewportTableRecord* pRecord);
    Acad::ErrorStatus add(DbObjectId& recordId, DbViewportTableRecord* pRecord);
};



#endif