

#ifndef GEVECTOR2DARRAY_INCLUDED
#define GEVECTOR2DARRAY_INCLUDED /*!DOM*/

#include "AcArray.h"
#include "Ge/GeVector2d.h"


#if !EMSDK
typedef AcArray<GeVector2d> GeVector2dArray;
#endif


// emsdk
#if EMSDK
class GeVector2dArray
{
public:
    GeVector2dArray() {}
    GeVector2dArray(unsigned int count) {}
    ~GeVector2dArray() {}

public:
    ACARRAY_METHODS(GeVector2d);
};
#endif


#endif // GEVECTOR2DARRAY_INCLUDED

