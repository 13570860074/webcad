#ifndef GILINESEGMENT_H
#define GILINESEGMENT_H

#include "GiEntity.h"
#include "AcArray.h"
#include "GeEntity3d.h"
#include "GiLineSegmentGeometry.h"

class GeRay3d;
class GeLine3d;
class GeVector3d;
class GiLineSegment : public GiEntity
{
public:
    GiLineSegment();
    virtual ~GiLineSegment();
    RX_DECLARE_MEMBERS(GiLineSegment);

public:
    /** 获得类型 */
    virtual Gi::EntityType type() const;

    /** 更新 */
    virtual void update();

    /** 获得顶点数据 */
#if EMSDK
    virtual emscripten::val vertexs();
#else
    virtual double *vertexs();
#endif

    /** 添加线段 */
    void appendLineSegment(GiLineSegmentGeometry *line);
    GiLineSegmentGeometry *appendLineSegment(const GePoint3d &_startPoint, const GePoint3d &_endPoint);

    /** 添加直线 */
    GiLineSegmentGeometry *appendLine(const GePoint3d &basePoint, const GeVector3d &direction);

    /** 添加射线 */
    GiLineSegmentGeometry *appendRay(const GePoint3d &basePoint, const GeVector3d &direction);

    /** 添加圆 */
    void appendCircle(int numSegments, const GePoint3d &_center, const double radius, const GeVector3d &_normal = GeVector3d::kZAxis);

    /** 添加圆弧 */
    void appendArc(int numSegments, const GePoint3d &_center, const double radius, const double startAng, const double endAng, const GeVector3d &_normal = GeVector3d::kZAxis);
};

#endif