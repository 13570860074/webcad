#ifndef GEPOINT3DARRAY_H
#define GEPOINT3DARRAY_H

#include "AcArray.h"
#include "GePoint3d.h"


#if !EMSDK
typedef AcArray<GePoint3d> GePoint3dArray;
#endif


// emsdk
#if EMSDK
class GePoint3dArray
{
public:
    GePoint3dArray() {}
    GePoint3dArray(unsigned int count) {}
    ~GePoint3dArray() {}

public:
    ACARRAY_METHODS(GePoint3d);
};
#endif

#endif

