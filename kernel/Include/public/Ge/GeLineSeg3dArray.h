
#ifndef GELINESEG3DARRAY_H
#define GELINESEG3DARRAY_H

#include "AcArray.h"
#include "GeLineSeg3d.h"



#if !EMSDK
typedef AcArray<GeLineSeg3d> GeLineSeg3dArray;
#endif


// emsdk
#if EMSDK
class GeLineSeg3dArray
{
public:
    GeLineSeg3dArray() {}
    GeLineSeg3dArray(unsigned int count) {}
    ~GeLineSeg3dArray() {}

public:
    ACARRAY_METHODS(GeLineSeg3d);
};
#endif


#endif

