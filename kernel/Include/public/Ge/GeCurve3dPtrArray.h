
#ifndef GESHAREDCURVE3DPRT_H
#define GESHAREDCURVE3DPRT_H /*!DOM*/

#include "AcArray.h"
#include "GeCurve3d.h"


#if !EMSDK
typedef AcArray<GeCurve3d> GeCurve3dPtrArray;
#endif


// emsdk
#if EMSDK
class GeCurve3dPtrArray
{
public:
    GeCurve3dPtrArray() {}
    GeCurve3dPtrArray(unsigned int count) {}
    ~GeCurve3dPtrArray() {}

public:
    ACARRAY_METHODS(GeCurve3d);
};
#endif

#endif

