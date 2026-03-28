#ifndef GIGEOMETRYPOOL_H
#define GIGEOMETRYPOOL_H

#include "AcArray.h"

class GiLineSegmentGeometry;
class GiTriangleMeshGeometry;
class GiPointGeometry;
class GiWorldGeometryStake;

/**
 * @brief 几何对象池 — 对四种高频分配类型实现 acquire/release 复用，
 *        避免每帧大量 heap alloc/free。
 *        文档销毁时调用 clear() 执行真正的 delete。
 */
class GiGeometryPool
{
public:
    GiGeometryPool();
    ~GiGeometryPool();

    /** 从池中取出或新建一个 GiLineSegmentGeometry */
    GiLineSegmentGeometry* acquireLineSegment();

    /** 从池中取出或新建一个 GiTriangleMeshGeometry */
    GiTriangleMeshGeometry* acquireTriangleMesh();

    /** 从池中取出或新建一个 GiPointGeometry */
    GiPointGeometry* acquirePoint();

    /** 从池中取出或新建一个 GiWorldGeometryStake */
    GiWorldGeometryStake* acquireStake();

    /** 将 GiLineSegmentGeometry 归还到池 */
    void releaseLineSegment(GiLineSegmentGeometry* p);

    /** 将 GiTriangleMeshGeometry 归还到池 */
    void releaseTriangleMesh(GiTriangleMeshGeometry* p);

    /** 将 GiPointGeometry 归还到池 */
    void releasePoint(GiPointGeometry* p);

    /** 将 GiWorldGeometryStake 归还到池 */
    void releaseStake(GiWorldGeometryStake* p);

    /** 文档销毁时调用 — 真正 delete 所有池中对象 */
    void clear();

private:
    AcArray<GiLineSegmentGeometry*>  m_lineSegments;
    AcArray<GiTriangleMeshGeometry*> m_triangleMeshes;
    AcArray<GiPointGeometry*>        m_points;
    AcArray<GiWorldGeometryStake*>   m_stakes;
};

#endif
