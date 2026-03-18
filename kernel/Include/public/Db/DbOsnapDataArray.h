#ifndef GSOSNAPDATAARRAY_H
#define GSOSNAPDATAARRAY_H



#include "AcArray.h"
#include "DbOsnapData.h"


#if !EMSDK
typedef AcArray<DbOsnapData> DbOsnapDataArray;
#endif



// emsdk
#if EMSDK
class DbOsnapDataArray
{
public:
    DbOsnapDataArray() {}
    DbOsnapDataArray(unsigned int count) {}
    ~DbOsnapDataArray() {}

public:
    ACARRAY_METHODS(DbOsnapData);
};
#endif



#endif