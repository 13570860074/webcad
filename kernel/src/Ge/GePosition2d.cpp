#include "GePosition2d.h"

GePosition2d::GePosition2d()
    : m_point(GePoint2d::kOrigin)
{
}

GePosition2d::GePosition2d(const GePoint2d& point)
    : m_point(point)
{
}

GePosition2d::GePosition2d(double x, double y)
    : m_point(x, y)
{
}

GePosition2d::GePosition2d(const GePosition2d& source)
{
    *this = source;
}

bool GePosition2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity2d
        || entType == Ge::EntityId::kPointEnt2d
        || entType == this->type();
}

Ge::EntityId GePosition2d::type() const
{
    return Ge::EntityId::kPosition2d;
}

GePosition2d* GePosition2d::copy() const
{
    return new GePosition2d(*this);
}

bool GePosition2d::operator == (const GePosition2d& entity) const
{
    return this->isEqualTo(entity);
}

bool GePosition2d::operator != (const GePosition2d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GePosition2d::isEqualTo(const GePosition2d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GePosition2d::isEqualTo(const GePosition2d& entity, const GeTol& tol) const
{
    return m_point.isEqualTo(entity.m_point, tol);
}

GePosition2d& GePosition2d::transformBy(const GeMatrix2d& xfm)
{
    m_point.transformBy(xfm);
    return *this;
}

GePosition2d& GePosition2d::translateBy(const GeVector2d& translateVec)
{
    m_point += translateVec;
    return *this;
}

GePosition2d& GePosition2d::rotateBy(double angle)
{
    m_point.rotateBy(angle);
    return *this;
}

GePosition2d& GePosition2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    m_point.rotateBy(angle, wrtPoint);
    return *this;
}

GePosition2d& GePosition2d::mirror(const GeLine2d& line)
{
    m_point.mirror(line);
    return *this;
}

GePosition2d& GePosition2d::scaleBy(double scaleFactor)
{
    m_point.scaleBy(scaleFactor);
    return *this;
}

GePosition2d& GePosition2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    m_point.scaleBy(scaleFactor, wrtPoint);
    return *this;
}

bool GePosition2d::isOn(const GePoint2d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GePosition2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    return m_point.isEqualTo(pnt, tol);
}

GePoint2d GePosition2d::point2d() const
{
    return m_point;
}

GePosition2d::operator GePoint2d () const
{
    return m_point;
}

GePosition2d& GePosition2d::set(const GePoint2d& point)
{
    m_point = point;
    return *this;
}

GePosition2d& GePosition2d::set(double x, double y)
{
    m_point.set(x, y);
    return *this;
}

GePosition2d& GePosition2d::operator = (const GePosition2d& pos)
{
    if (this == &pos) {
        return *this;
    }

    m_point = pos.m_point;
    return *this;
}