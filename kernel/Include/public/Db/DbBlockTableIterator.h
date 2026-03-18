#ifndef DBBLOCKTABLEITERATOR_H
#define DBBLOCKTABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbBlockTableRecord.h"

class DbBlockTableIterator : public DbSymbolTableIterator
{
public:
    DbBlockTableIterator();
    RX_DECLARE_MEMBERS(DbBlockTableIterator);

    Acad::ErrorStatus getRecord(
        DbBlockTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif