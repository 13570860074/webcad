#ifndef GIGEOMETRY_H
#define GIGEOMETRY_H


#include "adesk.h"
#include "RxObject.h"
#include "acgidefs.h"
#include "GeVector3d.h"
#include "GeMatrix3d.h"



typedef enum {
    kGiWorldPosition,
    kGiViewportPosition,
    kGiScreenPosition,
    kGiScreenLocalOriginPosition,
    kGiWorldWithScreenOffsetPosition
} GiPositionTransformBehavior;

typedef enum {
    kGiWorldScale,
    kGiViewportScale,
    kGiScreenScale,
    kGiViewportLocalOriginScale,
    kGiScreenLocalOriginScale
} GiScaleTransformBehavior;

typedef enum {
    kGiWorldOrientation,
    kGiScreenOrientation,
    kGiZAxisOrientation
} GiOrientationTransformBehavior;

class GiEdgeData;
class GiFaceData;
class GiVertexData;
class GiTextStyle;
class GiGeometry : public RxObject
    //
    // The base class for geometry toolkits
    //
{
public:
    enum TransparencyMode
    {
        kTransparencyOff,  // pixel alpha ignored, all pixels are opaque
        kTransparency1Bit, // pixel alpha determines transparency on/off,
            // with 0 to 254 completely transparent and 255 completely opaque
            kTransparency8Bit, // pixel alpha determines transparency level,
            // from 0 (completely transparent) to 255 (completely opaque)
    };


    // Coordinate transformations.
    //
    virtual void getModelToWorldTransform(GeMatrix3d&) const = 0;
    virtual void getWorldToModelTransform(GeMatrix3d&) const = 0;

    virtual bool pushModelTransform(const GeVector3d& vNormal) = 0;// Uses arbitrary Axis algorythm
    virtual bool pushModelTransform(const GeMatrix3d& xMat) = 0;
    virtual bool popModelTransform() = 0;
    virtual GeMatrix3d pushPositionTransform(GiPositionTransformBehavior behavior, const GePoint3d& offset) = 0;
    virtual GeMatrix3d pushPositionTransform(GiPositionTransformBehavior behavior, const GePoint2d& offset) = 0;
    virtual GeMatrix3d pushScaleTransform(GiScaleTransformBehavior behavior, const GePoint3d& extents) = 0;
    virtual GeMatrix3d pushScaleTransform(GiScaleTransformBehavior behavior, const GePoint2d& extents) = 0;
    virtual GeMatrix3d pushOrientationTransform(GiOrientationTransformBehavior behavior) = 0;


    // For drawing various primitives.
    //
    virtual bool  line(const GePoint3d& startPoint, const GePoint3d& endPoint) const = 0;
    virtual bool  circle(const GePoint3d& center,
        const double radius,
        const GeVector3d& normal) const = 0;

    virtual bool  circle(const GePoint3d&, const GePoint3d&,
        const GePoint3d&) const = 0;

    virtual bool  circularArc(const GePoint3d& center,
        const double radius,
        const GeVector3d& normal,
        const GeVector3d& startVector,
        const double sweepAngle,
        const GiArcType arcType = kGiArcSimple) const = 0;

    virtual bool  circularArc(const GePoint3d& start,
        const GePoint3d& point,
        const GePoint3d& end,
        const GiArcType arcType = kGiArcSimple) const = 0;

    virtual Adesk::Boolean  polyline(const Adesk::UInt32 nbPoints,
        const GePoint3d* pVertexList,
        const GeVector3d* pNormal = NULL,
        Adesk::LongPtr lBaseSubEntMarker = -1) const = 0;

    /*
    nbVertex:顶点数量
    pVertexList:顶点数组
    faceListSize:片体数量
    pFaceList:片体顶点索引
    */
    virtual Adesk::Boolean  shell(const Adesk::UInt32 nbVertex,
        const GePoint3d* pVertexList,
        const Adesk::UInt32 faceListSize,
        const Adesk::Int32* pFaceList,
        const GiEdgeData* pEdgeData = NULL,
        const GiFaceData* pFaceData = NULL,
        const GiVertexData* pVertexData = NULL,
        const struct resbuf* pResBuf = NULL,
        const bool bAutoGenerateNormals = true
    ) const = 0;

    virtual Adesk::Boolean  text(const GePoint3d& position,
        const GeVector3d& normal,
        const GeVector3d& direction,
        const double height,
        const double width,
        const double oblique,
        const ACHAR* pMsg) const = 0;

    virtual Adesk::Boolean  text(const GePoint3d& position,
        const GeVector3d& normal,
        const GeVector3d& direction,
        const ACHAR* pMsg,
        const Adesk::Int32 length,
        const Adesk::Boolean raw,
        const GiTextStyle& pTextStyle
    ) const = 0;

    virtual Adesk::Boolean  xline(const GePoint3d&,
        const GePoint3d&) const = 0;

    virtual Adesk::Boolean  ray(const GePoint3d&,
        const GePoint3d&) const = 0;

    // If you push a clip boundary onto the stack you must pop it;
//
    virtual Adesk::Boolean   worldLine(const GePoint3d pnts[2]) {
        GePoint3d polyPnts[2];
        GeMatrix3d mat;
        getWorldToModelTransform(mat);
        polyPnts[0] = mat * pnts[0];
        polyPnts[1] = mat * pnts[1];
        return polyline(2, polyPnts);
    };
    // array of points
    virtual Adesk::Boolean  polypoint(const Adesk::UInt32 nbPoints,
        const GePoint3d* pVertexList,
        const GeVector3d* pNormal = NULL,
        const Adesk::LongPtr* pSubEntMarkers = NULL) const {
        GePoint3d pline[2];
        Adesk::Boolean retval = Adesk::kFalse;
        for (Adesk::UInt32 i = 0; i < nbPoints; i++) {
            pline[1] = pline[0] = pVertexList[i];
            retval = polyline(2, pline, pNormal ? &pNormal[i] : NULL, pSubEntMarkers ? pSubEntMarkers[i] : -1);
            if (retval)
                return retval;
        }
        return retval;
    };
};






#endif