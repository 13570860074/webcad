#ifndef GECLIPBOUNDARY2D_H
#define GECLIPBOUNDARY2D_H

#include "GeEntity2d.h"
#include "GeEntity2dMethods.h"
#include "GeMatrix2d.h"
#include "GePoint2dArray.h"
#include "GeIntArray.h"

class GeClipBoundary2d : public GeEntity2d
{
public:
    GeClipBoundary2d();
    GeClipBoundary2d(const GePoint2d& cornerA, const GePoint2d& cornerB);
    GeClipBoundary2d(const GePoint2dArray& clipBoundary);
    GeClipBoundary2d(const GeClipBoundary2d& src);

    GEENTITY2D_METHODS(GeClipBoundary2d);

    Ge::ClipError set(const GePoint2d& cornerA, const GePoint2d& cornerB);
    Ge::ClipError set(const GePoint2dArray& clipBoundary);

    Ge::ClipError clipPolygon(
        const GePoint2dArray& rawVertices,
        GePoint2dArray& clippedVertices,
        Ge::ClipCondition& clipCondition,
        GeIntArray* pClippedSegmentSourceLabel = NULL) const;

    Ge::ClipError clipPolyline(
        const GePoint2dArray& rawVertices,
        GePoint2dArray& clippedVertices,
        Ge::ClipCondition& clipCondition,
        GeIntArray* pClippedSegmentSourceLabel = NULL) const;

    GeClipBoundary2d& operator = (const GeClipBoundary2d& src);

private:
    GePoint2dArray m_boundary;
    bool m_initialized;
};

#endif