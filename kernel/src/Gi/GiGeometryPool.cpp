#include "GiGeometryPool.h"
#include "GiLineSegmentGeometry.h"
#include "GiTriangleMeshGeometry.h"
#include "GiPointGeometry.h"
#include "GiWorldGeometryStake.h"

GiGeometryPool::GiGeometryPool()
{
}

GiGeometryPool::~GiGeometryPool()
{
    clear();
}

GiLineSegmentGeometry* GiGeometryPool::acquireLineSegment()
{
    if (m_lineSegments.length() > 0)
    {
        GiLineSegmentGeometry* p = m_lineSegments[m_lineSegments.length() - 1];
        m_lineSegments.removeAt(m_lineSegments.length() - 1);
        p->poolReset();
        return p;
    }
    return new GiLineSegmentGeometry();
}

GiTriangleMeshGeometry* GiGeometryPool::acquireTriangleMesh()
{
    if (m_triangleMeshes.length() > 0)
    {
        GiTriangleMeshGeometry* p = m_triangleMeshes[m_triangleMeshes.length() - 1];
        m_triangleMeshes.removeAt(m_triangleMeshes.length() - 1);
        p->poolReset();
        return p;
    }
    return new GiTriangleMeshGeometry();
}

GiPointGeometry* GiGeometryPool::acquirePoint()
{
    if (m_points.length() > 0)
    {
        GiPointGeometry* p = m_points[m_points.length() - 1];
        m_points.removeAt(m_points.length() - 1);
        p->poolReset();
        return p;
    }
    return new GiPointGeometry();
}

GiWorldGeometryStake* GiGeometryPool::acquireStake()
{
    if (m_stakes.length() > 0)
    {
        GiWorldGeometryStake* p = m_stakes[m_stakes.length() - 1];
        m_stakes.removeAt(m_stakes.length() - 1);
        p->poolReset();
        return p;
    }
    return new GiWorldGeometryStake();
}

void GiGeometryPool::releaseLineSegment(GiLineSegmentGeometry* p)
{
    if (p != NULL)
        m_lineSegments.append(p);
}

void GiGeometryPool::releaseTriangleMesh(GiTriangleMeshGeometry* p)
{
    if (p != NULL)
        m_triangleMeshes.append(p);
}

void GiGeometryPool::releasePoint(GiPointGeometry* p)
{
    if (p != NULL)
        m_points.append(p);
}

void GiGeometryPool::releaseStake(GiWorldGeometryStake* p)
{
    if (p != NULL)
        m_stakes.append(p);
}

void GiGeometryPool::clear()
{
    for (int i = 0; i < m_lineSegments.length(); i++)
        delete m_lineSegments[i];
    m_lineSegments.removeAll();

    for (int i = 0; i < m_triangleMeshes.length(); i++)
        delete m_triangleMeshes[i];
    m_triangleMeshes.removeAll();

    for (int i = 0; i < m_points.length(); i++)
        delete m_points[i];
    m_points.removeAll();

    for (int i = 0; i < m_stakes.length(); i++)
        delete m_stakes[i];
    m_stakes.removeAll();
}
