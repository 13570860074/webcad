
#ifndef GESHAREDCURVE2DPRT_H
#define GESHAREDCURVE2DPRT_H

#include "AcArray.h"
#include "GeCurve2d.h"



#if !EMSDK
typedef AcArray<GeCurve2d*> GeCurve2dPtrArray;
#endif


// emsdk
#if EMSDK
class GeCurve2dPtrArray
{
public:
    GeCurve2dPtrArray() {}
    GeCurve2dPtrArray(unsigned int count) {}
    ~GeCurve2dPtrArray() {}

public:
    ACARRAY_METHODS(GeCurve2d*);
};
#endif


#endif

