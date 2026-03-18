#ifndef DBOBJECTIDARRAY_H
#define DBOBJECTIDARRAY_H

#include "AcArray.h"
#include "DbObjectId.h"


#if !EMSDK
typedef AcArray<DbObjectId> DbObjectIdArray;
#endif


// emsdk
#if EMSDK
class DbObjectIdArray
{
public:
    DbObjectIdArray() {}
    DbObjectIdArray(unsigned int count) {}
    ~DbObjectIdArray() {}

public:
    ACARRAY_METHODS(DbObjectId);
};
#endif


#endif