
#ifndef GELINESEG2DARRAY_H
#define GELINESEG2DARRAY_H

#include "AcArray.h"
#include "GeLineSeg2d.h"


#if !EMSDK
typedef AcArray<GeLineSeg2d> GeLineSeg2dArray;
#endif


// emsdk
#if EMSDK
class GeLineSeg2dArray
{
public:
    GeLineSeg2dArray() {}
    GeLineSeg2dArray(unsigned int count) {}
    ~GeLineSeg2dArray() {}

public:
    ACARRAY_METHODS(GeLineSeg2d);
};
#endif

#endif

