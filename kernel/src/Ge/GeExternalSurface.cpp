#include "GeExternalSurface.h"

#include "GePlane.h"
#include "GeSphere.h"

GeExternalSurface::GeExternalSurface()
    : m_pSurfaceDef(NULL)
    , m_pSurface(NULL)
    , m_isOwner(false)
    , m_surfaceKind(Ge::kExternalEntityUndefined)
{
}

GeExternalSurface::GeExternalSurface(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy)
    : m_pSurfaceDef(NULL)
    , m_pSurface(NULL)
    , m_isOwner(false)
    , m_surfaceKind(Ge::kExternalEntityUndefined)
{
    set(pSurfaceDef, surfaceKind, makeCopy);
}

GeExternalSurface::GeExternalSurface(const GeExternalSurface& source)
    : m_pSurfaceDef(NULL)
    , m_pSurface(NULL)
    , m_isOwner(false)
    , m_surfaceKind(Ge::kExternalEntityUndefined)
{
    *this = source;
}

GeExternalSurface::~GeExternalSurface()
{
    clearOwnedSurface();
}

void GeExternalSurface::clearOwnedSurface()
{
    if (m_isOwner && m_pSurface != NULL) {
        delete m_pSurface;
    }

    m_pSurfaceDef = NULL;
    m_pSurface = NULL;
    m_isOwner = false;
    m_surfaceKind = Ge::kExternalEntityUndefined;
}

bool GeExternalSurface::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == this->type();
}

Ge::EntityId GeExternalSurface::type() const
{
    return Ge::kExternalSurface;
}

GeExternalSurface* GeExternalSurface::copy() const
{
    return new GeExternalSurface(*this);
}

bool GeExternalSurface::operator == (const GeExternalSurface& entity) const
{
    return isEqualTo(entity);
}

bool GeExternalSurface::operator != (const GeExternalSurface& entity) const
{
    return !isEqualTo(entity);
}

bool GeExternalSurface::isEqualTo(const GeExternalSurface& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeExternalSurface::isEqualTo(const GeExternalSurface& entity, const GeTol& tol) const
{
    if (m_surfaceKind != entity.m_surfaceKind || m_isOwner != entity.m_isOwner) {
        return false;
    }

    if (m_pSurface == NULL || entity.m_pSurface == NULL) {
        return m_pSurface == entity.m_pSurface;
    }

    return m_pSurface->isEqualTo(*entity.m_pSurface, tol);
}

GeExternalSurface& GeExternalSurface::transformBy(const GeMatrix3d& xfm)
{
    if (m_pSurface != NULL) {
        m_pSurface->transformBy(xfm);
    }
    return *this;
}

GeExternalSurface& GeExternalSurface::translateBy(const GeVector3d& translateVec)
{
    if (m_pSurface != NULL) {
        m_pSurface->translateBy(translateVec);
    }
    return *this;
}

GeExternalSurface& GeExternalSurface::rotateBy(double angle, const GeVector3d& vec)
{
    if (m_pSurface != NULL) {
        m_pSurface->rotateBy(angle, vec, GePoint3d::kOrigin);
    }
    return *this;
}

GeExternalSurface& GeExternalSurface::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    if (m_pSurface != NULL) {
        m_pSurface->rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeExternalSurface& GeExternalSurface::mirror(const GePlane& plane)
{
    if (m_pSurface != NULL) {
        m_pSurface->mirror(plane);
    }
    return *this;
}

GeExternalSurface& GeExternalSurface::scaleBy(double scaleFactor)
{
    if (m_pSurface != NULL) {
        m_pSurface->scaleBy(scaleFactor, GePoint3d::kOrigin);
    }
    return *this;
}

GeExternalSurface& GeExternalSurface::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    if (m_pSurface != NULL) {
        m_pSurface->scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeExternalSurface::isOn(const GePoint3d& pnt) const
{
    return m_pSurface != NULL ? m_pSurface->isOn(pnt) : false;
}

bool GeExternalSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return m_pSurface != NULL ? m_pSurface->isOn(pnt, tol) : false;
}

void GeExternalSurface::getExternalSurface(void*& pSurfaceDef) const
{
    if (m_pSurface != NULL) {
        pSurfaceDef = m_pSurface->copy();
        return;
    }
    pSurfaceDef = m_pSurfaceDef;
}

Ge::ExternalEntityKind GeExternalSurface::externalSurfaceKind() const
{
    return m_surfaceKind;
}

bool GeExternalSurface::isPlane() const
{
    return m_pSurface != NULL && dynamic_cast<GePlane*>(m_pSurface) != NULL;
}

bool GeExternalSurface::isSphere() const
{
    return m_pSurface != NULL && dynamic_cast<GeSphere*>(m_pSurface) != NULL;
}

bool GeExternalSurface::isCylinder() const
{
    return false;
}

bool GeExternalSurface::isCone() const
{
    return false;
}

bool GeExternalSurface::isEllipCylinder() const
{
    return false;
}

bool GeExternalSurface::isEllipCone() const
{
    return false;
}

bool GeExternalSurface::isTorus() const
{
    return false;
}

bool GeExternalSurface::isNurbSurface() const
{
    return false;
}

bool GeExternalSurface::isDefined() const
{
    return m_pSurfaceDef != NULL;
}

bool GeExternalSurface::isNativeSurface(GeSurface*& nativeSurface) const
{
    nativeSurface = NULL;
    if (m_pSurface == NULL) {
        return false;
    }

    GeEntity3d* copied = m_pSurface->copy();
    nativeSurface = dynamic_cast<GeSurface*>(copied);
    if (nativeSurface == NULL) {
        delete copied;
    }

    return nativeSurface != NULL;
}

GeExternalSurface& GeExternalSurface::operator = (const GeExternalSurface& extSurf)
{
    if (this == &extSurf) {
        return *this;
    }

    clearOwnedSurface();
    m_surfaceKind = extSurf.m_surfaceKind;

    if (extSurf.m_pSurface != NULL) {
        set(extSurf.m_pSurface, extSurf.m_surfaceKind, true);
    }
    else {
        m_pSurfaceDef = extSurf.m_pSurfaceDef;
    }

    return *this;
}

GeExternalSurface& GeExternalSurface::set(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy)
{
    clearOwnedSurface();

    m_surfaceKind = surfaceKind;
    m_pSurfaceDef = pSurfaceDef;

    GeSurface* inputSurface = reinterpret_cast<GeSurface*>(pSurfaceDef);
    if (inputSurface == NULL) {
        return *this;
    }

    if (makeCopy) {
        GeEntity3d* copied = inputSurface->copy();
        m_pSurface = dynamic_cast<GeSurface*>(copied);
        if (m_pSurface == NULL) {
            delete copied;
            m_pSurface = inputSurface;
            m_isOwner = false;
        }
        else {
            m_isOwner = true;
            m_pSurfaceDef = m_pSurface;
        }
    }
    else {
        m_pSurface = inputSurface;
        m_isOwner = false;
    }

    return *this;
}

bool GeExternalSurface::isOwnerOfSurface() const
{
    return m_isOwner;
}

GeExternalSurface& GeExternalSurface::setToOwnSurface()
{
    if (m_pSurface == NULL || m_isOwner) {
        return *this;
    }

    GeEntity3d* copied = m_pSurface->copy();
    GeSurface* copiedSurface = dynamic_cast<GeSurface*>(copied);
    if (copiedSurface != NULL) {
        m_pSurface = copiedSurface;
        m_pSurfaceDef = m_pSurface;
        m_isOwner = true;
    }
    else {
        delete copied;
    }

    return *this;
}