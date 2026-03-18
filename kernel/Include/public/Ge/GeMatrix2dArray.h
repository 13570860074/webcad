
#ifndef GEMATRIX2DARRAY_H
#define GEMATRIX2DARRAY_H

#include "AcArray.h"
#include "GeMatrix2d.h"



#if !EMSDK
typedef AcArray<GeMatrix2d> GeMatrix2dArray;
#endif


// emsdk
#if EMSDK
class GeMatrix2dArray
{
public:
    GeMatrix2dArray() {}
    GeMatrix2dArray(unsigned int count) {}
    ~GeMatrix2dArray() {}

public:
    ACARRAY_METHODS(GeMatrix2d);
};
#endif

#endif
