#ifndef DBUCSTABLEITERATOR_H
#define DBUCSTABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbUCSTableRecord.h"

class DbUCSTableIterator : public DbSymbolTableIterator
{
public:
    DbUCSTableIterator();
    RX_DECLARE_MEMBERS(DbUCSTableIterator);

    Acad::ErrorStatus getRecord(
        DbUCSTableRecord*& pRecord,
        Db::OpenMode openMode = Db::kForRead, 
        bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif