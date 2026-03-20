#include "GeExternalCurve2d.h"

#include "GeLine2d.h"
#include "GeNurbCurve2d.h"

GeExternalCurve2d::GeExternalCurve2d()
    : m_pCurveDef(NULL)
    , m_pCurve(NULL)
    , m_isOwner(false)
    , m_curveKind(Ge::kExternalEntityUndefined)
{
}

GeExternalCurve2d::GeExternalCurve2d(const GeExternalCurve2d& source)
    : m_pCurveDef(NULL)
    , m_pCurve(NULL)
    , m_isOwner(false)
    , m_curveKind(Ge::kExternalEntityUndefined)
{
    *this = source;
}

GeExternalCurve2d::GeExternalCurve2d(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy)
    : m_pCurveDef(NULL)
    , m_pCurve(NULL)
    , m_isOwner(false)
    , m_curveKind(Ge::kExternalEntityUndefined)
{
    set(pCurveDef, curveKind, makeCopy);
}

GeExternalCurve2d::~GeExternalCurve2d()
{
    clearOwnedCurve();
}

void GeExternalCurve2d::clearOwnedCurve()
{
    if (m_isOwner && m_pCurve != NULL) {
        delete m_pCurve;
    }
    m_pCurveDef = NULL;
    m_pCurve = NULL;
    m_isOwner = false;
    m_curveKind = Ge::kExternalEntityUndefined;
}

bool GeExternalCurve2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == this->type();
}

Ge::EntityId GeExternalCurve2d::type() const
{
    return Ge::kExternalCurve2d;
}

GeExternalCurve2d* GeExternalCurve2d::copy() const
{
    return new GeExternalCurve2d(*this);
}

bool GeExternalCurve2d::operator == (const GeExternalCurve2d& entity) const
{
    return isEqualTo(entity);
}

bool GeExternalCurve2d::operator != (const GeExternalCurve2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeExternalCurve2d::isEqualTo(const GeExternalCurve2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeExternalCurve2d::isEqualTo(const GeExternalCurve2d& entity, const GeTol& tol) const
{
    if (m_curveKind != entity.m_curveKind || m_isOwner != entity.m_isOwner) {
        return false;
    }
    if (m_pCurve == NULL || entity.m_pCurve == NULL) {
        return m_pCurve == entity.m_pCurve;
    }
    return m_pCurve->isEqualTo(*entity.m_pCurve, tol);
}

GeExternalCurve2d& GeExternalCurve2d::transformBy(const GeMatrix2d& xfm)
{
    if (m_pCurve != NULL) {
        m_pCurve->transformBy(xfm);
    }
    return *this;
}

GeExternalCurve2d& GeExternalCurve2d::translateBy(const GeVector2d& translateVec)
{
    if (m_pCurve != NULL) {
        m_pCurve->translateBy(translateVec);
    }
    return *this;
}

GeExternalCurve2d& GeExternalCurve2d::rotateBy(double angle)
{
    if (m_pCurve != NULL) {
        m_pCurve->rotateBy(angle, GePoint2d::kOrigin);
    }
    return *this;
}

GeExternalCurve2d& GeExternalCurve2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    if (m_pCurve != NULL) {
        m_pCurve->rotateBy(angle, wrtPoint);
    }
    return *this;
}

GeExternalCurve2d& GeExternalCurve2d::mirror(const GeLine2d& line)
{
    if (m_pCurve != NULL) {
        m_pCurve->mirror(line);
    }
    return *this;
}

GeExternalCurve2d& GeExternalCurve2d::scaleBy(double scaleFactor)
{
    if (m_pCurve != NULL) {
        m_pCurve->scaleBy(scaleFactor, GePoint2d::kOrigin);
    }
    return *this;
}

GeExternalCurve2d& GeExternalCurve2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    if (m_pCurve != NULL) {
        m_pCurve->scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeExternalCurve2d::isOn(const GePoint2d& pnt) const
{
    return m_pCurve != NULL ? m_pCurve->isOn(pnt) : false;
}

bool GeExternalCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    return m_pCurve != NULL ? m_pCurve->isOn(pnt, tol) : false;
}

bool GeExternalCurve2d::isNurbCurve() const
{
    return m_pCurve != NULL && dynamic_cast<GeNurbCurve2d*>(m_pCurve) != NULL;
}

bool GeExternalCurve2d::isNurbCurve(GeNurbCurve2d& nurbCurve) const
{
    GeNurbCurve2d* ptr = m_pCurve != NULL ? dynamic_cast<GeNurbCurve2d*>(m_pCurve) : NULL;
    if (ptr == NULL) {
        return false;
    }
    nurbCurve = *ptr;
    return true;
}

bool GeExternalCurve2d::isDefined() const
{
    return m_pCurveDef != NULL;
}

void GeExternalCurve2d::getExternalCurve(void*& pCurveDef) const
{
    if (m_pCurve != NULL) {
        pCurveDef = m_pCurve->copy();
        return;
    }
    pCurveDef = m_pCurveDef;
}

Ge::ExternalEntityKind GeExternalCurve2d::externalCurveKind() const
{
    return m_curveKind;
}

GeExternalCurve2d& GeExternalCurve2d::set(void* pCurveDef, Ge::ExternalEntityKind curveKind, bool makeCopy)
{
    clearOwnedCurve();

    m_curveKind = curveKind;
    m_pCurveDef = pCurveDef;

    GeCurve2d* inputCurve = reinterpret_cast<GeCurve2d*>(pCurveDef);
    if (inputCurve == NULL) {
        return *this;
    }

    if (makeCopy) {
        GeEntity2d* copied = inputCurve->copy();
        m_pCurve = dynamic_cast<GeCurve2d*>(copied);
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

bool GeExternalCurve2d::isOwnerOfCurve() const
{
    return m_isOwner;
}

GeExternalCurve2d& GeExternalCurve2d::setToOwnCurve()
{
    if (m_pCurve == NULL || m_isOwner) {
        return *this;
    }

    GeEntity2d* copied = m_pCurve->copy();
    GeCurve2d* copiedCurve = dynamic_cast<GeCurve2d*>(copied);
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

GeExternalCurve2d& GeExternalCurve2d::operator = (const GeExternalCurve2d& extCurve)
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