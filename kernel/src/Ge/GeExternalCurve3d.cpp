#include "GeExternalCurve3d.h"

#include "GeCircArc3d.h"
#include "GeEllipArc3d.h"
#include "GeLine3d.h"
#include "GeLineSeg3d.h"
#include "GeNurbCurve3d.h"
#include "GePlane.h"
#include "GeRay3d.h"

GeExternalCurve3d::GeExternalCurve3d()
    : m_pCurveDef(NULL)
    , m_pCurve(NULL)
    , m_isOwner(false)
    , m_curveKind(Ge::kExternalEntityUndefined)
{
}

GeExternalCurve3d::GeExternalCurve3d(const GeExternalCurve3d& source)
    : m_pCurveDef(NULL)
    , m_pCurve(NULL)
    , m_isOwner(false)
    , m_curveKind(Ge::kExternalEntityUndefined)
{
    *this = source;
}

GeExternalCurve3d::GeExternalCurve3d(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy)
    : m_pCurveDef(NULL)
    , m_pCurve(NULL)
    , m_isOwner(false)
    , m_curveKind(Ge::kExternalEntityUndefined)
{
    set(pCurveDef, curveKind, makeCopy);
}

GeExternalCurve3d::~GeExternalCurve3d()
{
    clearOwnedCurve();
}

void GeExternalCurve3d::clearOwnedCurve()
{
    if (m_isOwner && m_pCurve != NULL) {
        delete m_pCurve;
    }
    m_pCurveDef = NULL;
    m_pCurve = NULL;
    m_isOwner = false;
    m_curveKind = Ge::kExternalEntityUndefined;
}

bool GeExternalCurve3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kCurve3d || entType == this->type();
}

Ge::EntityId GeExternalCurve3d::type() const
{
    return Ge::kExternalCurve3d;
}

GeExternalCurve3d* GeExternalCurve3d::copy() const
{
    return new GeExternalCurve3d(*this);
}

bool GeExternalCurve3d::operator == (const GeExternalCurve3d& entity) const
{
    return isEqualTo(entity);
}

bool GeExternalCurve3d::operator != (const GeExternalCurve3d& entity) const
{
    return !isEqualTo(entity);
}

bool GeExternalCurve3d::isEqualTo(const GeExternalCurve3d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeExternalCurve3d::isEqualTo(const GeExternalCurve3d& entity, const GeTol& tol) const
{
    if (m_curveKind != entity.m_curveKind || m_isOwner != entity.m_isOwner) {
        return false;
    }
    if (m_pCurve == NULL || entity.m_pCurve == NULL) {
        return m_pCurve == entity.m_pCurve;
    }
    return m_pCurve->isEqualTo(*entity.m_pCurve, tol);
}

GeExternalCurve3d& GeExternalCurve3d::transformBy(const GeMatrix3d& xfm)
{
    if (m_pCurve != NULL) {
        m_pCurve->transformBy(xfm);
    }
    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::translateBy(const GeVector3d& translateVec)
{
    if (m_pCurve != NULL) {
        m_pCurve->translateBy(translateVec);
    }
    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::rotateBy(double angle, const GeVector3d& vec)
{
    if (m_pCurve != NULL) {
        m_pCurve->rotateBy(angle, vec, GePoint3d::kOrigin);
    }
    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    if (m_pCurve != NULL) {
        m_pCurve->rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::mirror(const GePlane& plane)
{
    if (m_pCurve != NULL) {
        m_pCurve->mirror(plane);
    }
    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::scaleBy(double scaleFactor)
{
    if (m_pCurve != NULL) {
        m_pCurve->scaleBy(scaleFactor, GePoint3d::kOrigin);
    }
    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    if (m_pCurve != NULL) {
        m_pCurve->scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeExternalCurve3d::isOn(const GePoint3d& pnt) const
{
    return m_pCurve != NULL ? m_pCurve->isOn(pnt) : false;
}

bool GeExternalCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return m_pCurve != NULL ? m_pCurve->isOn(pnt, tol) : false;
}

bool GeExternalCurve3d::isLine() const
{
    return m_pCurve != NULL && dynamic_cast<GeLine3d*>(m_pCurve) != NULL;
}

bool GeExternalCurve3d::isRay() const
{
    return m_pCurve != NULL && dynamic_cast<GeRay3d*>(m_pCurve) != NULL;
}

bool GeExternalCurve3d::isLineSeg() const
{
    return m_pCurve != NULL && dynamic_cast<GeLineSeg3d*>(m_pCurve) != NULL;
}

bool GeExternalCurve3d::isCircArc() const
{
    return m_pCurve != NULL && dynamic_cast<GeCircArc3d*>(m_pCurve) != NULL;
}

bool GeExternalCurve3d::isEllipArc() const
{
    return m_pCurve != NULL && dynamic_cast<GeEllipArc3d*>(m_pCurve) != NULL;
}

bool GeExternalCurve3d::isNurbCurve() const
{
    return m_pCurve != NULL && dynamic_cast<GeNurbCurve3d*>(m_pCurve) != NULL;
}

bool GeExternalCurve3d::isDefined() const
{
    return m_pCurveDef != NULL;
}

bool GeExternalCurve3d::isNativeCurve(GeCurve3d*& nativeCurve) const
{
    nativeCurve = NULL;
    if (m_pCurve == NULL) {
        return false;
    }
    nativeCurve = dynamic_cast<GeCurve3d*>(m_pCurve->copy());
    return nativeCurve != NULL;
}

void GeExternalCurve3d::getExternalCurve(void*& pCurveDef) const
{
    if (m_pCurve != NULL) {
        pCurveDef = m_pCurve->copy();
        return;
    }
    pCurveDef = m_pCurveDef;
}

Ge::ExternalEntityKind GeExternalCurve3d::externalCurveKind() const
{
    return m_curveKind;
}

GeExternalCurve3d& GeExternalCurve3d::set(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy)
{
    clearOwnedCurve();

    m_curveKind = curveKind;
    m_pCurveDef = pCurveDef;

    GeCurve3d* inputCurve = reinterpret_cast<GeCurve3d*>(pCurveDef);
    if (inputCurve == NULL) {
        return *this;
    }

    if (makeCopy) {
        GeEntity3d* copied = inputCurve->copy();
        m_pCurve = dynamic_cast<GeCurve3d*>(copied);
        if (m_pCurve == NULL) {
            delete copied;
            m_pCurve = inputCurve;
            m_isOwner = false;
        }
        else {
            m_isOwner = true;
            m_pCurveDef = m_pCurve;
        }
    }
    else {
        m_pCurve = inputCurve;
        m_isOwner = false;
    }

    return *this;
}

GeExternalCurve3d& GeExternalCurve3d::operator = (const GeExternalCurve3d& extCurve)
{
    if (this == &extCurve) {
        return *this;
    }

    clearOwnedCurve();
    m_curveKind = extCurve.m_curveKind;
    if (extCurve.m_pCurve != NULL) {
        set(extCurve.m_pCurve, extCurve.m_curveKind, true);
    }
    else {
        m_pCurveDef = extCurve.m_pCurveDef;
    }

    return *this;
}

bool GeExternalCurve3d::isOwnerOfCurve() const
{
    return m_isOwner;
}

GeExternalCurve3d& GeExternalCurve3d::setToOwnCurve()
{
    if (m_pCurve == NULL || m_isOwner) {
        return *this;
    }

    GeEntity3d* copied = m_pCurve->copy();
    GeCurve3d* copiedCurve = dynamic_cast<GeCurve3d*>(copied);
    if (copiedCurve != NULL) {
        m_pCurve = copiedCurve;
        m_pCurveDef = m_pCurve;
        m_isOwner = true;
    }
    else {
        delete copied;
    }
    return *this;
}