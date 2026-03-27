#ifndef GIWORLDDRAWGEOMETRY_H
#define GIWORLDDRAWGEOMETRY_H

#include "AcArray.h"
#include "GiEntity.h"
#include "GiFaceData.h"
#include "GePoint3dArray.h"
#include "GiWorldGeometry.h"
#include "GiWorldGeometryStake.h"

class GiMesh;
class GiTextStyle;
class GiEntityGeometry;
class GiLineSegment;
class GiEntityTraits;
class GiSubEntityTraits;
class GiWorldDrawGeometry : public GiWorldGeometry
{
public:
    GiWorldDrawGeometry();
    virtual ~GiWorldDrawGeometry();
    RX_DECLARE_MEMBERS(GiWorldDrawGeometry);

private:
    GiSubEntityTraits *getEntityTraits() const;
    void drawPoint(const GiMaterial *pMaterial, const GiEntityTraits *entityTraits, const GePoint3d &point) const;
    void drawFace(const GiMaterial *pMaterial, const GiEntityTraits *entityTraits, const GePoint3dArray &verts) const;
    void drawLine(const GiMaterial *pMaterial, const GiEntityTraits *entityTraits, const GePoint3d &_startPoint, const GePoint3d &_endPoint) const;

public:

    void worldGeometryStakes(AcArray<GiWorldGeometryStake*>&);

    void removeWorldGeometryStakes();


public:
    virtual void getModelToWorldTransform(GeMatrix3d &) const;
    virtual void getWorldToModelTransform(GeMatrix3d &) const;

    virtual bool pushModelTransform(const GeVector3d &vNormal);
    virtual bool pushModelTransform(const GeMatrix3d &xMat);
    virtual bool popModelTransform();
    virtual GeMatrix3d pushPositionTransform(GiPositionTransformBehavior behavior, const GePoint3d &offset);
    virtual GeMatrix3d pushPositionTransform(GiPositionTransformBehavior behavior, const GePoint2d &offset);
    virtual GeMatrix3d pushScaleTransform(GiScaleTransformBehavior behavior, const GePoint3d &extents);
    virtual GeMatrix3d pushScaleTransform(GiScaleTransformBehavior behavior, const GePoint2d &extents);
    virtual GeMatrix3d pushOrientationTransform(GiOrientationTransformBehavior behavior);

    // For drawing various primitives.
    //
    virtual bool line(const GePoint3d &startPoint, const GePoint3d &endPoint) const;
    virtual bool circle(const GePoint3d &center,
                        const double radius,
                        const GeVector3d &normal) const;

    virtual bool circle(const GePoint3d &, const GePoint3d &,
                        const GePoint3d &) const;

    virtual bool circularArc(const GePoint3d &center,
                             const double radius,
                             const GeVector3d &normal,
                             const GeVector3d &startVector,
                             const double sweepAngle,
                             const GiArcType arcType = kGiArcSimple) const;

    virtual bool circularArc(const GePoint3d &start,
                             const GePoint3d &point,
                             const GePoint3d &end,
                             const GiArcType arcType = kGiArcSimple) const;

    virtual Adesk::Boolean polyline(const Adesk::UInt32 nbPoints,
                                    const GePoint3d *pVertexList,
                                    const GeVector3d *pNormal = NULL,
                                    Adesk::LongPtr lBaseSubEntMarker = -1) const;

    virtual Adesk::Boolean shell(const Adesk::UInt32 nbVertex,
                                 const GePoint3d *pVertexList,
                                 const Adesk::UInt32 faceListSize,
                                 const Adesk::Int32 *pFaceList,
                                 const GiEdgeData *pEdgeData = NULL,
                                 const GiFaceData *pFaceData = NULL,
                                 const GiVertexData *pVertexData = NULL,
                                 const struct resbuf *pResBuf = NULL,
                                 const bool bAutoGenerateNormals = true) const;

    virtual Adesk::Boolean text(const GePoint3d &position,
                                const GeVector3d &normal,
                                const GeVector3d &direction,
                                const double height,
                                const double width,
                                const double oblique,
                                const ACHAR *pMsg) const;

    virtual Adesk::Boolean text(const GePoint3d &position,
                                const GeVector3d &normal,
                                const GeVector3d &direction,
                                const ACHAR *pMsg,
                                const Adesk::Int32 length,
                                const Adesk::Boolean raw,
                                const GiTextStyle &pTextStyle) const;

    virtual Adesk::Boolean xline(const GePoint3d &, const GePoint3d &) const;
    virtual Adesk::Boolean ray(const GePoint3d &, const GePoint3d &) const;

    virtual Adesk::Boolean polygon(const Adesk::UInt32 nbPoints, const GePoint3d *pVertexList) const;
    virtual bool ellipArc(const GeEllipArc3d &ellipArc, const GiArcType arcType = kGiArcSimple) const;
    virtual Adesk::Boolean mesh(const Adesk::UInt32 rows, const Adesk::UInt32 columns,
                                const GePoint3d *pVertexList,
                                const GiEdgeData *pEdgeData = NULL,
                                const GiFaceData *pFaceData = NULL,
                                const GiVertexData *pVertexData = NULL) const;
    virtual void draw(const GiDrawable *pDrawable) const;
};

#endif