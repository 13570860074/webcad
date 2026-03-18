#ifndef DBTEXTSTYLETABLEITERATOR_H
#define DBTEXTSTYLETABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbTextStyleTableRecord.h"

class DbTextStyleTableIterator : public DbSymbolTableIterator
{
public:
    DbTextStyleTableIterator();
    RX_DECLARE_MEMBERS(DbTextStyleTableIterator);

    Acad::ErrorStatus getRecord(
        DbTextStyleTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif