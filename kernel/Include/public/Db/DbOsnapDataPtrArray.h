#ifndef GSOSNAPDATAPTRARRAY_H
#define GSOSNAPDATAPTRARRAY_H



#include "AcArray.h"
#include "DbOsnapData.h"



#if !EMSDK
typedef AcArray<DbOsnapData*> DbOsnapDataPtrArray;
#endif



// emsdk
#if EMSDK
class DbOsnapDataPtrArray
{
public:
    DbOsnapDataPtrArray() {}
    DbOsnapDataPtrArray(unsigned int count) {}
    ~DbOsnapDataPtrArray() {}

public:
    ACARRAY_PTR_METHODS(DbOsnapData*);
};
#endif



#endif