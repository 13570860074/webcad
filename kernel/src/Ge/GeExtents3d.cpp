#include "GeExtents3d.h"
#include <cmath>

namespace {
const double GE_INVALID_EXTENTS = 1.0e20;

double ge_ext3d_clamp(double v, double low, double high)
{
    if (v < low) {
        return low;
    }
    if (v > high) {
        return high;
    }
    return v;
}
}

const GeExtents3d GeExtents3d::kInvalid(
    GePoint3d(GE_INVALID_EXTENTS, GE_INVALID_EXTENTS, GE_INVALID_EXTENTS),
    GePoint3d(-GE_INVALID_EXTENTS, -GE_INVALID_EXTENTS, -GE_INVALID_EXTENTS));

GeExtents3d::GeExtents3d()
    : m_min(GE_INVALID_EXTENTS, GE_INVALID_EXTENTS, GE_INVALID_EXTENTS)
    , m_max(-GE_INVALID_EXTENTS, -GE_INVALID_EXTENTS, -GE_INVALID_EXTENTS)
{
}

GeExtents3d::GeExtents3d(const GePoint3d& min, const GePoint3d& max)
{
    this->set(min, max);
}

const GePoint3d& GeExtents3d::minPoint() const
{
    return m_min;
}

const GePoint3d& GeExtents3d::maxPoint() const
{
    return m_max;
}

const GePoint3d& GeExtents3d::operator[] (int index) const
{
    return index == 0 ? m_min : m_max;
}

GePoint3d& GeExtents3d::operator[] (int index)
{
    return index == 0 ? m_min : m_max;
}

void GeExtents3d::set(const GePoint3d& min, const GePoint3d& max)
{
    m_min = min;
    m_max = max;
}

void GeExtents3d::comparingSet(const GePoint3d& pt1, const GePoint3d& pt2)
{
    m_min.set(
        pt1.x < pt2.x ? pt1.x : pt2.x,
        pt1.y < pt2.y ? pt1.y : pt2.y,
        pt1.z < pt2.z ? pt1.z : pt2.z);
    m_max.set(
        pt1.x > pt2.x ? pt1.x : pt2.x,
        pt1.y > pt2.y ? pt1.y : pt2.y,
        pt1.z > pt2.z ? pt1.z : pt2.z);
}

GeExtents3d& GeExtents3d::addPoint(const GePoint3d& point)
{
    if (this->isValidExtents() == false) {
        m_min = point;
        m_max = point;
        return *this;
    }

    if (point.x < m_min.x) m_min.x = point.x;
    if (point.y < m_min.y) m_min.y = point.y;
    if (point.z < m_min.z) m_min.z = point.z;
    if (point.x > m_max.x) m_max.x = point.x;
    if (point.y > m_max.y) m_max.y = point.y;
    if (point.z > m_max.z) m_max.z = point.z;
    return *this;
}

GeExtents3d& GeExtents3d::addPoints(const GePoint3dArray& points)
{
    for (int i = 0; i < points.length(); i++) {
        this->addPoint(points[i]);
    }
    return *this;
}

GeExtents3d& GeExtents3d::addExt(const GeExtents3d& extents)
{
    if (extents.isValidExtents()) {
        this->addPoint(extents.minPoint());
        this->addPoint(extents.maxPoint());
    }
    return *this;
}

bool GeExtents3d::isValidExtents() const
{
    return (m_max.x >= m_min.x) && (m_max.y >= m_min.y) && (m_max.z >= m_min.z);
}

void GeExtents3d::expandBy(const GeVector3d& vect)
{
    m_min -= vect;
    m_max += vect;
}

void GeExtents3d::transformBy(const GeMatrix3d& xfm)
{
    if (this->isValidExtents() == false) {
        return;
    }

    GePoint3d corners[8];
    corners[0] = GePoint3d(m_min.x, m_min.y, m_min.z);
    corners[1] = GePoint3d(m_max.x, m_min.y, m_min.z);
    corners[2] = GePoint3d(m_min.x, m_max.y, m_min.z);
    corners[3] = GePoint3d(m_max.x, m_max.y, m_min.z);
    corners[4] = GePoint3d(m_min.x, m_min.y, m_max.z);
    corners[5] = GePoint3d(m_max.x, m_min.y, m_max.z);
    corners[6] = GePoint3d(m_min.x, m_max.y, m_max.z);
    corners[7] = GePoint3d(m_max.x, m_max.y, m_max.z);

    GeExtents3d out;
    for (int i = 0; i < 8; i++) {
        corners[i].transformBy(xfm);
        out.addPoint(corners[i]);
    }

    *this = out;
}

bool GeExtents3d::contains(const GePoint3d& point, const GeTol& tol) const
{
    double e = tol.equalPoint();
    return point.x >= (m_min.x - e) && point.x <= (m_max.x + e)
        && point.y >= (m_min.y - e) && point.y <= (m_max.y + e)
        && point.z >= (m_min.z - e) && point.z <= (m_max.z + e);
}

bool GeExtents3d::contains(const GeExtents3d& extents, const GeTol& tol) const
{
    return this->contains(extents.minPoint(), tol) && this->contains(extents.maxPoint(), tol);
}

bool GeExtents3d::isDisjoint(const GeExtents3d& extents, const GeTol& tol) const
{
    double e = tol.equalPoint();
    return (m_max.x + e < extents.m_min.x)
        || (extents.m_max.x + e < m_min.x)
        || (m_max.y + e < extents.m_min.y)
        || (extents.m_max.y + e < m_min.y)
        || (m_max.z + e < extents.m_min.z)
        || (extents.m_max.z + e < m_min.z);
}

bool GeExtents3d::isDisjointEuclidean(const GeExtents3d& extents, const GeTol& tol) const
{
    return this->distanceTo(extents) > tol.equalPoint();
}

double GeExtents3d::distanceTo(const GePoint3d& point) const
{
    double cx = ge_ext3d_clamp(point.x, m_min.x, m_max.x);
    double cy = ge_ext3d_clamp(point.y, m_min.y, m_max.y);
    double cz = ge_ext3d_clamp(point.z, m_min.z, m_max.z);
    return point.distanceTo(GePoint3d(cx, cy, cz));
}

double GeExtents3d::distanceTo(const GeExtents3d& extents) const
{
    if (this->isDisjoint(extents) == false) {
        return 0.0;
    }

    double dx = 0.0;
    if (m_max.x < extents.m_min.x) dx = extents.m_min.x - m_max.x;
    else if (extents.m_max.x < m_min.x) dx = m_min.x - extents.m_max.x;

    double dy = 0.0;
    if (m_max.y < extents.m_min.y) dy = extents.m_min.y - m_max.y;
    else if (extents.m_max.y < m_min.y) dy = m_min.y - extents.m_max.y;

    double dz = 0.0;
    if (m_max.z < extents.m_min.z) dz = extents.m_min.z - m_max.z;
    else if (extents.m_max.z < m_min.z) dz = m_min.z - extents.m_max.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

GeExtents3d::IntersectionStatus GeExtents3d::intersectWith(const GeExtents3d& extents, GeExtents3d* pResult) const
{
    if (this->isValidExtents() == false || extents.isValidExtents() == false) {
        return kIntersectUnknown;
    }

    if (this->isDisjoint(extents)) {
        return kIntersectNot;
    }

    if (this->contains(extents)) {
        if (pResult != NULL) {
            *pResult = extents;
        }
        return kIntersectOpIn;
    }

    if (extents.contains(*this)) {
        if (pResult != NULL) {
            *pResult = *this;
        }
        return kIntersectOpOut;
    }

    GeExtents3d result;
    result.m_min.set(
        m_min.x > extents.m_min.x ? m_min.x : extents.m_min.x,
        m_min.y > extents.m_min.y ? m_min.y : extents.m_min.y,
        m_min.z > extents.m_min.z ? m_min.z : extents.m_min.z);
    result.m_max.set(
        m_max.x < extents.m_max.x ? m_max.x : extents.m_max.x,
        m_max.y < extents.m_max.y ? m_max.y : extents.m_max.y,
        m_max.z < extents.m_max.z ? m_max.z : extents.m_max.z);

    if (pResult != NULL) {
        *pResult = result;
    }

    return kIntersectOk;
}

GePoint3d GeExtents3d::center() const
{
    return m_min + (m_max - m_min) * 0.5;
}

GeVector3d GeExtents3d::diagonal() const
{
    return m_max - m_min;
}

bool GeExtents3d::operator == (const GeExtents3d& other) const
{
    return m_min == other.m_min && m_max == other.m_max;
}

bool GeExtents3d::operator != (const GeExtents3d& other) const
{
    return !(*this == other);
}