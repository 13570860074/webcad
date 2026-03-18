#ifndef DBLINETYPETABLEITERATOR_H
#define DBLINETYPETABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbLinetypeTableRecord.h"

class DbLinetypeTableIterator : public DbSymbolTableIterator
{
public:
    DbLinetypeTableIterator();
    RX_DECLARE_MEMBERS(DbLinetypeTableIterator);

    Acad::ErrorStatus getRecord(
        DbLinetypeTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif