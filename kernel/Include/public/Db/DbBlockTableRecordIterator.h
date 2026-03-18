#ifndef DBBLOCKTABLERECORDITERATOR_H
#define DBBLOCKTABLERECORDITERATOR_H

#include "Db.h"
#include "RxObject.h"
#include "AcArray.h"


class DbBlockTableRecordIterator :public RxObject
{
public:
    DbBlockTableRecordIterator();
    virtual ~DbBlockTableRecordIterator();
    RX_DECLARE_MEMBERS(DbBlockTableRecordIterator);

    void start(bool atBeginning = true, bool skipDeleted = true);

    bool done() const;

    Acad::ErrorStatus getEntityId(DbObjectId& entityId) const;
    Acad::ErrorStatus getEntity(DbEntity*& pEntity, Db::OpenMode openMode = Db::kForRead, bool openErasedEntity = false) const;

    DbBlockTableRecord* blockTableRecord() const;

    void step(bool forward = true, bool skipDeleted = true);
};


#endif