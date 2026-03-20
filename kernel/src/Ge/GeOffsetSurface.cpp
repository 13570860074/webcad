#include "GeOffsetSurface.h"

#include "GeBoundedPlane.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GeSphere.h"
#include <cmath>

namespace {
GeVector3d offset_surface_normal_at(const GeSurface* surface, const GePoint2d& uv)
{
    if (surface == NULL) {
        return GeVector3d::kZAxis;
    }

    const double eps = 1.0e-4;
    GePoint3d p = surface->evalPoint(uv);
    GePoint3d pu = surface->evalPoint(GePoint2d(uv.x + eps, uv.y));
    GePoint3d pv = surface->evalPoint(GePoint2d(uv.x, uv.y + eps));
    GeVector3d du = pu - p;
    GeVector3d dv = pv - p;
    GeVector3d n = du.crossProduct(dv);
    if (n.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kZAxis;
    }
    n.normalize();
    return n;
}
}

GeOffsetSurface::GeOffsetSurface()
    : m_pBaseSurface(NULL)
    , m_isOwner(false)
    , m_offsetDistance(0.0)
{
}

GeOffsetSurface::GeOffsetSurface(GeSurface* baseSurface, double offsetDistance, bool makeCopy)
    : m_pBaseSurface(NULL)
    , m_isOwner(false)
    , m_offsetDistance(0.0)
{
    set(baseSurface, offsetDistance, makeCopy);
}

GeOffsetSurface::GeOffsetSurface(const GeOffsetSurface& source)
    : m_pBaseSurface(NULL)
    , m_isOwner(false)
    , m_offsetDistance(0.0)
{
    *this = source;
}

GeOffsetSurface::~GeOffsetSurface()
{
    clearOwnedSurface();
}

void GeOffsetSurface::clearOwnedSurface()
{
    if (m_isOwner && m_pBaseSurface != NULL) {
        delete m_pBaseSurface;
    }
    m_pBaseSurface = NULL;
    m_isOwner = false;
    m_offsetDistance = 0.0;
}

bool GeOffsetSurface::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == this->type();
}

Ge::EntityId GeOffsetSurface::type() const
{
    return Ge::kOffsetSurface;
}

GeOffsetSurface* GeOffsetSurface::copy() const
{
    return new GeOffsetSurface(*this);
}

bool GeOffsetSurface::operator == (const GeOffsetSurface& entity) const
{
    return isEqualTo(entity);
}

bool GeOffsetSurface::operator != (const GeOffsetSurface& entity) const
{
    return !isEqualTo(entity);
}

bool GeOffsetSurface::isEqualTo(const GeOffsetSurface& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeOffsetSurface::isEqualTo(const GeOffsetSurface& entity, const GeTol& tol) const
{
    if (std::fabs(m_offsetDistance - entity.m_offsetDistance) > tol.equalPoint()) {
        return false;
    }

    if (m_pBaseSurface == NULL || entity.m_pBaseSurface == NULL) {
        return m_pBaseSurface == entity.m_pBaseSurface;
    }

    return m_pBaseSurface->isEqualTo(*entity.m_pBaseSurface, tol);
}

GeOffsetSurface& GeOffsetSurface::transformBy(const GeMatrix3d& xfm)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->transformBy(xfm);
    }
    return *this;
}

GeOffsetSurface& GeOffsetSurface::translateBy(const GeVector3d& translateVec)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->translateBy(translateVec);
    }
    return *this;
}

GeOffsetSurface& GeOffsetSurface::rotateBy(double angle, const GeVector3d& vec)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->rotateBy(angle, vec, GePoint3d::kOrigin);
    }
    return *this;
}

GeOffsetSurface& GeOffsetSurface::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeOffsetSurface& GeOffsetSurface::mirror(const GePlane& plane)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->mirror(plane);
    }
    return *this;
}

GeOffsetSurface& GeOffsetSurface::scaleBy(double scaleFactor)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->scaleBy(scaleFactor, GePoint3d::kOrigin);
    }
    m_offsetDistance *= scaleFactor;
    return *this;
}

GeOffsetSurface& GeOffsetSurface::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    if (m_pBaseSurface != NULL) {
        m_pBaseSurface->scaleBy(scaleFactor, wrtPoint);
    }
    m_offsetDistance *= scaleFactor;
    return *this;
}

bool GeOffsetSurface::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeOffsetSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return distanceTo(pnt, tol) <= tol.equalPoint();
}

bool GeOffsetSurface::isPlane() const
{
    return m_pBaseSurface != NULL && dynamic_cast<GePlane*>(m_pBaseSurface) != NULL;
}

bool GeOffsetSurface::isBoundedPlane() const
{
    return m_pBaseSurface != NULL && dynamic_cast<GeBoundedPlane*>(m_pBaseSurface) != NULL;
}

bool GeOffsetSurface::isSphere() const
{
    return m_pBaseSurface != NULL && dynamic_cast<GeSphere*>(m_pBaseSurface) != NULL;
}

bool GeOffsetSurface::isCylinder() const
{
    return false;
}

bool GeOffsetSurface::isCone() const
{
    return false;
}

bool GeOffsetSurface::isEllipCylinder() const
{
    return false;
}

bool GeOffsetSurface::isEllipCone() const
{
    return false;
}

bool GeOffsetSurface::isTorus() const
{
    return false;
}

bool GeOffsetSurface::getSurface(GeSurface*& simpleSurface) const
{
    simpleSurface = NULL;
    if (m_pBaseSurface == NULL) {
        return false;
    }

    simpleSurface = dynamic_cast<GeSurface*>(copy());
    return simpleSurface != NULL;
}

void GeOffsetSurface::getConstructionSurface(GeSurface*& baseSurface) const
{
    baseSurface = NULL;
    if (m_pBaseSurface == NULL) {
        return;
    }
    baseSurface = dynamic_cast<GeSurface*>(m_pBaseSurface->copy());
}

double GeOffsetSurface::offsetDist() const
{
    return m_offsetDistance;
}

GeOffsetSurface& GeOffsetSurface::set(GeSurface* baseSurface, double offsetDistance, bool makeCopy)
{
    clearOwnedSurface();
    m_offsetDistance = offsetDistance;

    if (baseSurface == NULL) {
        return *this;
    }

    if (makeCopy) {
        GeEntity3d* copied = baseSurface->copy();
        m_pBaseSurface = dynamic_cast<GeSurface*>(copied);
        if (m_pBaseSurface == NULL) {
            delete copied;
            m_pBaseSurface = baseSurface;
            m_isOwner = false;
        }
        else {
            m_isOwner = true;
        }
    }
    else {
        m_pBaseSurface = baseSurface;
        m_isOwner = false;
    }

    return *this;
}

GeOffsetSurface& GeOffsetSurface::operator = (const GeOffsetSurface& surface)
{
    if (this == &surface) {
        return *this;
    }

    clearOwnedSurface();
    m_offsetDistance = surface.m_offsetDistance;
    if (surface.m_pBaseSurface != NULL) {
        set(surface.m_pBaseSurface, surface.m_offsetDistance, true);
    }
    return *this;
}

GePoint2d GeOffsetSurface::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    if (m_pBaseSurface == NULL) {
        return GePoint2d::kOrigin;
    }

    GePoint2d uv = m_pBaseSurface->paramOf(pnt, tol);
    GeVector3d n = offset_surface_normal_at(m_pBaseSurface, uv);
    GePoint3d basePoint = pnt - n * m_offsetDistance;
    return m_pBaseSurface->paramOf(basePoint, tol);
}

GePoint3d GeOffsetSurface::evalPoint(const GePoint2d& param) const
{
    if (m_pBaseSurface == NULL) {
        return GePoint3d::kOrigin;
    }

    GePoint3d basePoint = m_pBaseSurface->evalPoint(param);
    GeVector3d n = offset_surface_normal_at(m_pBaseSurface, param);
    return basePoint + n * m_offsetDistance;
}