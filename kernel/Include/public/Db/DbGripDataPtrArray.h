#ifndef GSGRIPDATAPTRARRAY_H
#define GSGRIPDATAPTRARRAY_H



#include "AcArray.h"
#include "DbGripData.h"



#if !EMSDK
typedef AcArray<DbGripData*> DbGripDataPtrArray;
#endif



// emsdk
#if EMSDK
class DbGripDataPtrArray
{
public:
    DbGripDataPtrArray() {}
    ~DbGripDataPtrArray() {}

public:
    ACARRAY_PTR_METHODS(DbGripData*);
};
#endif



#endif