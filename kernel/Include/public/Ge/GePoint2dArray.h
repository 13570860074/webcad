

#ifndef GEPOINT2DARRAY_H
#define GEPOINT2DARRAY_H

#include "AcArray.h"
#include "Ge/GePoint2d.h"

/** details
    This template class is a specialization of the AcArray class for GePoint2d objects.
*/
#if !EMSDK
typedef AcArray<GePoint2d> GePoint2dArray;
#endif


// emsdk
#if EMSDK
class GePoint2dArray
{
public:
    GePoint2dArray() {}
    GePoint2dArray(unsigned int count) {}
    ~GePoint2dArray(){}

public:
    ACARRAY_METHODS(GePoint2d);
};
#endif

#endif

