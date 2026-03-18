
#ifndef GEVECTOR3DARRAY_H
#define GEVECTOR3DARRAY_H

#include "AcArray.h"
#include "Ge/GeVector3d.h"


#if !EMSDK
typedef AcArray<GeVector3d> GeVector3dArray;
#endif


// emsdk
#if EMSDK
class GeVector3dArray
{
public:
    GeVector3dArray() {}
    GeVector3dArray(unsigned int count) {}
    ~GeVector3dArray() {}

public:
    ACARRAY_METHODS(GeVector3d);
};
#endif


#endif // GEVECTOR3DARRAY_INCLUDED

