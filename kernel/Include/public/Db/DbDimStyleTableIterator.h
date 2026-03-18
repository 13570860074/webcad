#ifndef DBDIMSTYLETABLEITERATOR_H
#define DBDIMSTYLETABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbDimStyleTableRecord.h"

class DbDimStyleTableIterator : public DbSymbolTableIterator
{
public:
    DbDimStyleTableIterator();
    RX_DECLARE_MEMBERS(DbDimStyleTableIterator);

    Acad::ErrorStatus getRecord(
        DbDimStyleTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif