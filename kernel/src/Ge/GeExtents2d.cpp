#include "GeExtents2d.h"

namespace {
const double GE_INVALID_EXTENTS = 1.0e20;
}

const GeExtents2d GeExtents2d::kInvalid(
    GePoint2d(GE_INVALID_EXTENTS, GE_INVALID_EXTENTS),
    GePoint2d(-GE_INVALID_EXTENTS, -GE_INVALID_EXTENTS));

GeExtents2d::GeExtents2d()
    : m_min(GE_INVALID_EXTENTS, GE_INVALID_EXTENTS)
    , m_max(-GE_INVALID_EXTENTS, -GE_INVALID_EXTENTS)
{
}

GeExtents2d::GeExtents2d(const GePoint2d& min, const GePoint2d& max)
{
    this->set(min, max);
}

const GePoint2d& GeExtents2d::minPoint() const
{
    return m_min;
}

const GePoint2d& GeExtents2d::maxPoint() const
{
    return m_max;
}

GeVector2d GeExtents2d::diagonal() const
{
    return m_max - m_min;
}

void GeExtents2d::set(const GePoint2d& min, const GePoint2d& max)
{
    m_min = min;
    m_max = max;
}

void GeExtents2d::comparingSet(const GePoint2d& pt1, const GePoint2d& pt2)
{
    m_min.set(
        pt1.x < pt2.x ? pt1.x : pt2.x,
        pt1.y < pt2.y ? pt1.y : pt2.y);

    m_max.set(
        pt1.x > pt2.x ? pt1.x : pt2.x,
        pt1.y > pt2.y ? pt1.y : pt2.y);
}

GeExtents2d& GeExtents2d::addPoint(const GePoint2d& point)
{
    if (this->isValidExtents() == false) {
        m_min = point;
        m_max = point;
        return *this;
    }

    if (point.x < m_min.x) m_min.x = point.x;
    if (point.y < m_min.y) m_min.y = point.y;
    if (point.x > m_max.x) m_max.x = point.x;
    if (point.y > m_max.y) m_max.y = point.y;
    return *this;
}

GeExtents2d& GeExtents2d::addPoints(const GePoint2dArray& points)
{
    for (int i = 0; i < points.length(); i++) {
        this->addPoint(points[i]);
    }
    return *this;
}

GeExtents2d& GeExtents2d::addExt(const GeExtents2d& extents)
{
    if (extents.isValidExtents()) {
        this->addPoint(extents.minPoint());
        this->addPoint(extents.maxPoint());
    }
    return *this;
}

bool GeExtents2d::isValidExtents() const
{
    return (m_max.x >= m_min.x) && (m_max.y >= m_min.y);
}

void GeExtents2d::expandBy(const GeVector2d& vect)
{
    m_min -= vect;
    m_max += vect;
}

void GeExtents2d::transformBy(const GeMatrix2d& xfm)
{
    if (this->isValidExtents() == false) {
        return;
    }

    GePoint2d corners[4];
    corners[0] = m_min;
    corners[1].set(m_max.x, m_min.y);
    corners[2] = m_max;
    corners[3].set(m_min.x, m_max.y);

    GeExtents2d out;
    for (int i = 0; i < 4; i++) {
        corners[i].transformBy(xfm);
        out.addPoint(corners[i]);
    }

    *this = out;
}

void GeExtents2d::translate(const GeVector2d& shift)
{
    m_min += shift;
    m_max += shift;
}

bool GeExtents2d::contains(const GePoint2d& point, const GeTol& tol) const
{
    double e = tol.equalPoint();
    return point.x >= (m_min.x - e) && point.x <= (m_max.x + e)
        && point.y >= (m_min.y - e) && point.y <= (m_max.y + e);
}

bool GeExtents2d::contains(const GeExtents2d& extents, const GeTol& tol) const
{
    return this->contains(extents.minPoint(), tol) && this->contains(extents.maxPoint(), tol);
}

bool GeExtents2d::isDisjoint(const GeExtents2d& extents, const GeTol& tol) const
{
    double e = tol.equalPoint();
    return (m_max.x + e < extents.m_min.x)
        || (extents.m_max.x + e < m_min.x)
        || (m_max.y + e < extents.m_min.y)
        || (extents.m_max.y + e < m_min.y);
}

GeExtents2d::IntersectionStatus GeExtents2d::intersectWith(const GeExtents2d& extents, GeExtents2d* pResult) const
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

    GeExtents2d result;
    result.m_min.set(
        m_min.x > extents.m_min.x ? m_min.x : extents.m_min.x,
        m_min.y > extents.m_min.y ? m_min.y : extents.m_min.y);
    result.m_max.set(
        m_max.x < extents.m_max.x ? m_max.x : extents.m_max.x,
        m_max.y < extents.m_max.y ? m_max.y : extents.m_max.y);

    if (pResult != NULL) {
        *pResult = result;
    }

    return kIntersectOk;
}

GePoint2d GeExtents2d::center() const
{
    return GePoint2d(0.5 * (m_min.x + m_max.x), 0.5 * (m_min.y + m_max.y));
}

bool GeExtents2d::operator == (const GeExtents2d& other) const
{
    return m_min == other.m_min && m_max == other.m_max;
}

bool GeExtents2d::operator != (const GeExtents2d& other) const
{
    return !(*this == other);
}