#ifndef GEEXT_H
#define GEEXT_H

#include "AcArray.h"
#include "Ge.h"
#include "GeIntArray.h"
#include "GeTol.h"

class GeSurface;
class GeCurve2d;

namespace GeExt
{
inline Ge::ErrorCondition calcLoopIncludeOrder(
    AcArray<GeIntArray>& loopIndices,
    const GeSurface&,
    bool,
    const AcArray<AcArray<const GeCurve2d*> >&, 
    const AcArray<AcArray<bool> >&, 
    const GeTol&)
{
    loopIndices.removeAll();
    return Ge::kOk;
}
}

#endif