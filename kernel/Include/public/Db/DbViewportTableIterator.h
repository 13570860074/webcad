#ifndef DBVIEWPORTTABLEITERATOR_H
#define DBVIEWPORTTABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbViewportTableRecord.h"

class DbViewportTableIterator : public DbSymbolTableIterator
{
public:
    DbViewportTableIterator();
    RX_DECLARE_MEMBERS(DbViewportTableIterator);

    Acad::ErrorStatus getRecord(
        DbViewportTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif