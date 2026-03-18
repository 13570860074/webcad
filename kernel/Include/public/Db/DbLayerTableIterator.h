#ifndef DBLAYERTABLEITERATOR_H
#define DBLAYERTABLEITERATOR_H

#include "Db.h"
#include "DbSymbolTableIterator.h"
#include "DbLayerTableRecord.h"

class DbLayerTableIterator : public DbSymbolTableIterator
{
public:
    DbLayerTableIterator();
    RX_DECLARE_MEMBERS(DbLayerTableIterator);

#pragma region EMSDK
    DbLayerTableRecord* getRecord() const
    {
        DbLayerTableRecord* pLayerTableRecord = NULL;
        this->getRecord(pLayerTableRecord, Db::OpenMode::kForRead, false);
        return pLayerTableRecord;
    }
    DbLayerTableRecord* getRecord(Db::OpenMode openMode) const
    {
        DbLayerTableRecord* pLayerTableRecord = NULL;
        this->getRecord(pLayerTableRecord, openMode, false);
        return pLayerTableRecord;
    }
    DbLayerTableRecord* getRecord(Db::OpenMode openMode, bool openErasedRec) const
    {
        DbLayerTableRecord* pLayerTableRecord = NULL;
        this->getRecord(pLayerTableRecord, openMode, openErasedRec);
        return pLayerTableRecord;
    }
#pragma endregion


    Acad::ErrorStatus getRecord(DbLayerTableRecord*& pRecord, Db::OpenMode openMode = Db::kForRead, bool openErasedRec = false) const
    {
        return DbSymbolTableIterator::getRecord((DbSymbolTableRecord*&)pRecord, openMode, openErasedRec);
    }
};


#endif