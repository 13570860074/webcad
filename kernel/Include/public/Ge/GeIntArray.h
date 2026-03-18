
#ifndef GeIntArray_H
#define GeIntArray_H

#include "AcArray.h"


#if !EMSDK
typedef AcArray<int> GeIntArray;
#endif


// emsdk
#if EMSDK
class GeIntArray
{
public:
    GeIntArray() {}
    GeIntArray(unsigned int count) {}
    ~GeIntArray() {}

public:
    ACARRAY_METHODS(int);
};
#endif


#endif // OdGeIntArray_INCLUDED

