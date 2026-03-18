#ifndef DBSYMBOLTABLEITERATOR_H
#define DBSYMBOLTABLEITERATOR_H


#include "RxObject.h"


class DbSymbolTableRecord;
class DbSymbolTableIterator :public RxObject
{
public:
    virtual ~DbSymbolTableIterator();

#pragma region EMSDK

    DbObjectId getRecordId() const;
    DbSymbolTableRecord* getRecord() const;
    DbSymbolTableRecord* getRecord(bool openErasedRec) const;
    DbSymbolTableRecord* getRecord(Db::OpenMode openMode, bool openErasedRec) const;

#pragma endregion


    void start();
    void start(bool atBeginning);
    void start(bool atBeginning, bool skipDeleted);

    bool done() const;
    Acad::ErrorStatus getRecordId(DbObjectId& id) const;
    Acad::ErrorStatus getRecord(DbSymbolTableRecord*& pRecord, Db::OpenMode openMode = Db::OpenMode::kForRead, bool openErasedRec = false) const;

    void step();
    void step(bool forward);
    void step(bool forward, bool skipDeleted);
};


#endif