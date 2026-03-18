#ifndef DBREGAPPTABLEITERATOR_H
#define DBREGAPPTABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbRegAppTableRecord.h"

class DbRegAppTableIterator : public DbSymbolTableIterator
{
public:
    DbRegAppTableIterator();
    RX_DECLARE_MEMBERS(DbRegAppTableIterator);

    Acad::ErrorStatus getRecord(
        DbRegAppTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif