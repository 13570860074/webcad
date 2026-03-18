
#ifndef GEMATRIX3DARRAY_H
#define GEMATRIX3DARRAY_H

#include "AcArray.h"
class GeMatrix3d;


#if !EMSDK
typedef AcArray<GeMatrix3d> GeMatrix3dArray;
#endif


// emsdk
#if EMSDK
class GeMatrix3dArray
{
public:
    GeMatrix3dArray() {}
    GeMatrix3dArray(unsigned int count) {}
    ~GeMatrix3dArray() {}

public:
    ACARRAY_METHODS(GeMatrix3d);
};
#endif

#endif
