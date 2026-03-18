

#ifndef GEDOUBLEARRAY_H
#define GEDOUBLEARRAY_H

#include "AcArray.h"

/** \details
    This template class is a specialization of the OdArray class double objects.
*/


#if !EMSDK
typedef AcArray<double> GeDoubleArray;
#endif


// emsdk
#if EMSDK
class GeDoubleArray
{
public:
    GeDoubleArray() {}
    GeDoubleArray(unsigned int count) {}
    ~GeDoubleArray() {}

public:
    ACARRAY_METHODS(double);
};
#endif


#endif // GEDOUBLEARRAY_INCLUDED

