#ifndef GI_H
#define GI_H


#include "AcString.h"


struct Gi
{
    /* 材质类型 */
    enum MaterialType {
        kUnknownMaterial = 100,                 // 未知材质
        kPointBasic,                            // 点
        kLineBasic,                             // 线
        kMeshBasic,                             // mesh
        kLineDashed,                            // 虚线
        kLinePixelDashed,                       // 不随相机变化的的虚线(主要是用在高亮线上)
    };

    /* 实体类型 */
    enum EntityType
    {
        kUnknownEntity = 200,
        kPoints,
        kLineSegment,
        kMesh,
        kLinear,
        kRay,
    };

    /* 几何对象类型 */
    enum EntityGeometryType {
        kUnknownGeometry = 300,
        kPointGeometry,
        kLineSegmentGeometry,
        kTriangleMeshGeometry,
    };

    /* 图元类型 */
    enum ElementType {
        kUnknownElement = 400,
        kElement,
        kDragging,
        kHighlight,
        kCursor,
        kCoordSystem,
        kGrip,
        kOsnap,
        kGridline,
        kDynDim,
    };
};




class GiObject;
class GiObjectId;
class GiMaterial;
bool acgiOpenObject(GiObject*& pObj, const unsigned id);
bool acgiOpenObject(GiObject*& pObj, const GiObjectId& id);
bool acgiOpenObject(GiMaterial*& pObj, const unsigned id);
bool acgiOpenObject(GiMaterial*& pObj, const GiObjectId& id);


#endif