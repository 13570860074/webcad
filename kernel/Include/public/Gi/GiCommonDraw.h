#ifndef GICOMMONDRAW_H
#define GICOMMONDRAW_H

#include "acgidefs.h"
#include "RxObject.h"
#include "acgidefs.h"

class GiSubEntityTraits;
class GiContext;
class GePoint3d;
class GiCommonDraw :public RxObject
{
public:
	GiCommonDraw();
    virtual ~GiCommonDraw();

    virtual GiRegenType regenType() const = 0;
    virtual bool regenAbort() const = 0;
    virtual GiSubEntityTraits& subEntityTraits() const = 0;
    virtual bool isDragging() const = 0;

    // This function operates against the current active viewport
    //
    virtual double deviation(const GiDeviationType, const GePoint3d&) const = 0;
    virtual UInt32 numberOfIsolines() const = 0;
    virtual bool secondaryCall() const { return false; }
};



#endif