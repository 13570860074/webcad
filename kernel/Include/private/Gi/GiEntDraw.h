#ifndef GIENTDRAW_H
#define GIENTDRAW_H

#include "GiWorldDraw.h"
#include "ApDocument.h"
#include "DbObject.h"

class GiEntDrawImpl;
class GiGeometry;
class GiEntDraw : public GiWorldDraw
{
public:
    GiEntDraw();
    virtual ~GiEntDraw();
    RX_DECLARE_MEMBERS(GiEntDraw);

    GiWorldGeometry &geometry() const;
    void draw();

    ApDocument *document() const;
    void setDocument(ApDocument *pDocument);

    virtual GiRegenType regenType() const;
    virtual bool regenAbort() const;
    virtual GiSubEntityTraits &subEntityTraits() const;
    virtual bool isDragging() const;

    // This function operates against the current active viewport
    //
    virtual double deviation(const GiDeviationType, const GePoint3d &) const;
    virtual UInt32 numberOfIsolines() const;

    virtual bool secondaryCall();

    virtual AcArray<unsigned int> &entityIds();
    virtual AcArray<unsigned int> &layerTableRecordIds();
};

#endif