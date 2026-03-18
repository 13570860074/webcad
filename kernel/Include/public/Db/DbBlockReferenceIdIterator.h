#ifndef DBBLOCKREFERENCEIDITERATOR_H
#define DBBLOCKREFERENCEIDITERATOR_H

#include "Db.h"
#include "DbObjectId.h"
#include "DbObjectIdArray.h"


class DbBlockReference;
class DbBlockReferenceIdIterator
{
public:
    DbBlockReferenceIdIterator(DbObjectIdArray _Ids);
    virtual ~DbBlockReferenceIdIterator();

    void start() { mPos = 0; }

    bool done() const;

    Acad::ErrorStatus getBlockReferenceId(DbObjectId& id) const;
    Acad::ErrorStatus getBlockReference(DbBlockReference*& pBlkRef, Db::OpenMode openMode = Db::kForRead, bool openErasedEntity = false) const;

    void step() { mPos++; }

private:
    int mPos;
    DbObjectIdArray mAry;
};


#endif