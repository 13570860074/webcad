#ifndef GSGRIPDATAARRAY_H
#define GSGRIPDATAARRAY_H



#include "AcArray.h"
#include "DbGripData.h"


#if !EMSDK
typedef AcArray<DbGripData> DbGripDataArray;
#endif



// emsdk
#if EMSDK
class DbGripDataArray
{
public:
    DbGripDataArray() {}
    ~DbGripDataArray() {}

public:
    ACARRAY_METHODS(DbGripData);
};
#endif



#endif