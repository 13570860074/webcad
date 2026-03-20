#include "GePosition3d.h"

GePosition3d::GePosition3d()
    : m_point(GePoint3d::kOrigin)
{
}

GePosition3d::GePosition3d(const GePoint3d& point)
    : m_point(point)
{
}

GePosition3d::GePosition3d(double x, double y, double z)
    : m_point(x, y, z)
{
}

GePosition3d::GePosition3d(const GePosition3d& source)
{
    *this = source;
}

bool GePosition3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d
        || entType == Ge::EntityId::kPointEnt3d
        || entType == this->type();
}

Ge::EntityId GePosition3d::type() const
{
    return Ge::EntityId::kPosition3d;
}

GePosition3d* GePosition3d::copy() const
{
    return new GePosition3d(*this);
}

bool GePosition3d::operator == (const GePosition3d& entity) const
{
    return this->isEqualTo(entity);
}

bool GePosition3d::operator != (const GePosition3d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GePosition3d::isEqualTo(const GePosition3d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GePosition3d::isEqualTo(const GePosition3d& entity, const GeTol& tol) const
{
    return m_point.isEqualTo(entity.m_point, tol);
}

GePosition3d& GePosition3d::transformBy(const GeMatrix3d& xfm)
{
    m_point.transformBy(xfm);
    return *this;
}

GePosition3d& GePosition3d::translateBy(const GeVector3d& translateVec)
{
    m_point += translateVec;
    return *this;
}

GePosition3d& GePosition3d::rotateBy(double angle, const GeVector3d& vec)
{
    m_point.rotateBy(angle, vec);
    return *this;
}

GePosition3d& GePosition3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_point.rotateBy(angle, vec, wrtPoint);
    return *this;
}

GePosition3d& GePosition3d::mirror(const GePlane& plane)
{
    m_point.mirror(plane);
    return *this;
}

GePosition3d& GePosition3d::scaleBy(double scaleFactor)
{
    m_point.scaleBy(scaleFactor);
    return *this;
}

GePosition3d& GePosition3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    m_point.scaleBy(scaleFactor, wrtPoint);
    return *this;
}

bool GePosition3d::isOn(const GePoint3d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GePosition3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return m_point.isEqualTo(pnt, tol);
}

GePoint3d GePosition3d::point3d() const
{
    return m_point;
}

GePosition3d::operator GePoint3d () const
{
    return m_point;
}

GePosition3d& GePosition3d::set(const GePoint3d& point)
{
    m_point = point;
    return *this;
}

GePosition3d& GePosition3d::set(double x, double y, double z)
{
    m_point.set(x, y, z);
    return *this;
}

GePosition3d& GePosition3d::operator = (const GePosition3d& pos)
{
    if (this == &pos) {
        return *this;
    }

    m_point = pos.m_point;
    return *this;
}