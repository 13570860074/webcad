#ifndef DBVIEWTABLEITERATOR_H
#define DBVIEWTABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbViewTableRecord.h"

class DbViewTableIterator : public DbSymbolTableIterator
{
public:
    DbViewTableIterator();
    RX_DECLARE_MEMBERS(DbViewTableIterator);

    Acad::ErrorStatus getRecord(
        DbViewTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif