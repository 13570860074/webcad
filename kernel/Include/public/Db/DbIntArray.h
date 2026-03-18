
#ifndef DBINTARRAY_H
#define DBINTARRAY_H

#include "AcArray.h"


#if !EMSDK
typedef AcArray<int> DbIntArray;
#endif


// emsdk
#if EMSDK
class DbIntArray
{
public:
    DbIntArray(){}
    ~DbIntArray(){}

public:
    ACARRAY_METHODS(int);
};
#endif

#endif

