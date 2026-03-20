#include "GeExternalBoundedSurface.h"

#include "GePlane.h"

GeExternalBoundedSurface::GeExternalBoundedSurface()
    : m_pContours(NULL)
    , m_numContours(0)
    , m_contoursOwned(false)
{
}

GeExternalBoundedSurface::GeExternalBoundedSurface(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy)
    : m_pContours(NULL)
    , m_numContours(0)
    , m_contoursOwned(false)
{
    set(pSurfaceDef, surfaceKind, makeCopy);
}

GeExternalBoundedSurface::GeExternalBoundedSurface(const GeExternalBoundedSurface& source)
    : m_pContours(NULL)
    , m_numContours(0)
    , m_contoursOwned(false)
{
    *this = source;
}

GeExternalBoundedSurface::~GeExternalBoundedSurface()
{
    clearContours();
}

void GeExternalBoundedSurface::clearContours()
{
    delete[] m_pContours;
    m_pContours = NULL;
    m_numContours = 0;
    m_contoursOwned = false;
}

bool GeExternalBoundedSurface::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == this->type();
}

Ge::EntityId GeExternalBoundedSurface::type() const
{
    return Ge::kExternalBoundedSurface;
}

GeExternalBoundedSurface* GeExternalBoundedSurface::copy() const
{
    return new GeExternalBoundedSurface(*this);
}

bool GeExternalBoundedSurface::operator == (const GeExternalBoundedSurface& entity) const
{
    return isEqualTo(entity);
}

bool GeExternalBoundedSurface::operator != (const GeExternalBoundedSurface& entity) const
{
    return !isEqualTo(entity);
}

bool GeExternalBoundedSurface::isEqualTo(const GeExternalBoundedSurface& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeExternalBoundedSurface::isEqualTo(const GeExternalBoundedSurface& entity, const GeTol& tol) const
{
    if (!m_baseSurface.isEqualTo(entity.m_baseSurface, tol)) {
        return false;
    }

    if (m_numContours != entity.m_numContours) {
        return false;
    }

    for (int i = 0; i < m_numContours; ++i) {
        if (m_pContours[i].numElements() != entity.m_pContours[i].numElements()) {
            return false;
        }
        if (m_pContours[i].isOwnerOfCurves() != entity.m_pContours[i].isOwnerOfCurves()) {
            return false;
        }
    }

    return true;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::transformBy(const GeMatrix3d& xfm)
{
    m_baseSurface.transformBy(xfm);
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::translateBy(const GeVector3d& translateVec)
{
    m_baseSurface.translateBy(translateVec);
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::rotateBy(double angle, const GeVector3d& vec)
{
    m_baseSurface.rotateBy(angle, vec, GePoint3d::kOrigin);
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_baseSurface.rotateBy(angle, vec, wrtPoint);
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::mirror(const GePlane& plane)
{
    m_baseSurface.mirror(plane);
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::scaleBy(double scaleFactor)
{
    m_baseSurface.scaleBy(scaleFactor, GePoint3d::kOrigin);
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    m_baseSurface.scaleBy(scaleFactor, wrtPoint);
    return *this;
}

bool GeExternalBoundedSurface::isOn(const GePoint3d& pnt) const
{
    return m_baseSurface.isOn(pnt);
}

bool GeExternalBoundedSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return m_baseSurface.isOn(pnt, tol);
}

Ge::ExternalEntityKind GeExternalBoundedSurface::externalSurfaceKind() const
{
    return m_baseSurface.externalSurfaceKind();
}

bool GeExternalBoundedSurface::isDefined() const
{
    return m_baseSurface.isDefined();
}

void GeExternalBoundedSurface::getExternalSurface(void*& pSurfaceDef) const
{
    m_baseSurface.getExternalSurface(pSurfaceDef);
}

void GeExternalBoundedSurface::getBaseSurface(GeSurface*& pSurfaceDef) const
{
    m_baseSurface.isNativeSurface(pSurfaceDef);
}

void GeExternalBoundedSurface::getBaseSurface(GeExternalSurface& unboundedSurfaceDef) const
{
    unboundedSurfaceDef = m_baseSurface;
}

bool GeExternalBoundedSurface::isPlane() const
{
    return m_baseSurface.isPlane();
}

bool GeExternalBoundedSurface::isSphere() const
{
    return m_baseSurface.isSphere();
}

bool GeExternalBoundedSurface::isCylinder() const
{
    return m_baseSurface.isCylinder();
}

bool GeExternalBoundedSurface::isCone() const
{
    return m_baseSurface.isCone();
}

bool GeExternalBoundedSurface::isEllipCylinder() const
{
    return m_baseSurface.isEllipCylinder();
}

bool GeExternalBoundedSurface::isEllipCone() const
{
    return m_baseSurface.isEllipCone();
}

bool GeExternalBoundedSurface::isTorus() const
{
    return m_baseSurface.isTorus();
}

bool GeExternalBoundedSurface::isNurbs() const
{
    return m_baseSurface.isNurbSurface();
}

bool GeExternalBoundedSurface::isExternalSurface() const
{
    GeSurface* nativeSurface = NULL;
    bool isNative = m_baseSurface.isNativeSurface(nativeSurface);
    delete nativeSurface;
    return !isNative;
}

int GeExternalBoundedSurface::numContours() const
{
    return m_numContours;
}

void GeExternalBoundedSurface::getContours(int& numContours, GeCurveBoundary*& contours) const
{
    numContours = m_numContours;
    contours = NULL;

    if (m_numContours <= 0 || m_pContours == NULL) {
        return;
    }

    contours = new GeCurveBoundary[m_numContours];
    for (int i = 0; i < m_numContours; ++i) {
        contours[i] = m_pContours[i];
    }
}

GeExternalBoundedSurface& GeExternalBoundedSurface::set(void* pSurfaceDef, Ge::ExternalEntityKind surfaceKind, bool makeCopy)
{
    m_baseSurface.set(pSurfaceDef, surfaceKind, makeCopy);
    return *this;
}

bool GeExternalBoundedSurface::isOwnerOfSurface() const
{
    return m_baseSurface.isOwnerOfSurface();
}

GeExternalBoundedSurface& GeExternalBoundedSurface::setToOwnSurface()
{
    m_baseSurface.setToOwnSurface();
    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::setContours(int numContours, const GeCurveBoundary* contours, bool makeCopy)
{
    clearContours();

    if (numContours <= 0 || contours == NULL) {
        return *this;
    }

    m_numContours = numContours;
    m_pContours = new GeCurveBoundary[m_numContours];
    m_contoursOwned = true;

    for (int i = 0; i < m_numContours; ++i) {
        m_pContours[i] = contours[i];
        if (!makeCopy && !m_pContours[i].isOwnerOfCurves()) {
            // Preserve external curve ownership semantics when caller requests shallow contour setup.
            continue;
        }
        m_pContours[i].setToOwnCurves();
    }

    return *this;
}

GeExternalBoundedSurface& GeExternalBoundedSurface::operator = (const GeExternalBoundedSurface& extBoundSurf)
{
    if (this == &extBoundSurf) {
        return *this;
    }

    m_baseSurface = extBoundSurf.m_baseSurface;
    clearContours();
    if (extBoundSurf.m_numContours > 0 && extBoundSurf.m_pContours != NULL) {
        setContours(extBoundSurf.m_numContours, extBoundSurf.m_pContours, true);
    }

    return *this;
}