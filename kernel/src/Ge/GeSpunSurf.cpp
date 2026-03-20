#include "GeSpunSurf.h"

#include "GeCurve3d.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
double clamp_to_interval_spun(double value, const GeInterval& range)
{
    double out = value;
    if (range.isBoundedBelow() && out < range.lowerBound()) {
        out = range.lowerBound();
    }
    if (range.isBoundedAbove() && out > range.upperBound()) {
        out = range.upperBound();
    }
    return out;
}

double refine_spun_u(const GeSpunSurf& surf, const GePoint3d& target, const GeInterval& rangeU, double u0, double vFixed)
{
    double span = rangeU.upperBound() - rangeU.lowerBound();
    if (span < 0.0) {
        span = -span;
    }
    double window = span / 16.0;
    if (window <= GeContext::gTol.equalPoint()) {
        window = span / 4.0;
    }

    double left = clamp_to_interval_spun(u0 - window, rangeU);
    double right = clamp_to_interval_spun(u0 + window, rangeU);
    if (right < left) {
        double t = left;
        left = right;
        right = t;
    }

    for (int i = 0; i < 20; ++i) {
        double m1 = left + (right - left) / 3.0;
        double m2 = right - (right - left) / 3.0;
        double d1 = surf.evalPoint(GePoint2d(m1, vFixed)).distanceTo(target);
        double d2 = surf.evalPoint(GePoint2d(m2, vFixed)).distanceTo(target);
        if (d1 <= d2) {
            right = m2;
        }
        else {
            left = m1;
        }
    }
    return (left + right) * 0.5;
}

double refine_spun_v(const GeSpunSurf& surf, const GePoint3d& target, const GeInterval& rangeV, double uFixed, double v0)
{
    double span = rangeV.upperBound() - rangeV.lowerBound();
    if (span < 0.0) {
        span = -span;
    }
    double window = span / 16.0;
    if (window <= GeContext::gTol.equalPoint()) {
        window = span / 4.0;
    }

    double left = clamp_to_interval_spun(v0 - window, rangeV);
    double right = clamp_to_interval_spun(v0 + window, rangeV);
    if (right < left) {
        double t = left;
        left = right;
        right = t;
    }

    for (int i = 0; i < 20; ++i) {
        double m1 = left + (right - left) / 3.0;
        double m2 = right - (right - left) / 3.0;
        double d1 = surf.evalPoint(GePoint2d(uFixed, m1)).distanceTo(target);
        double d2 = surf.evalPoint(GePoint2d(uFixed, m2)).distanceTo(target);
        if (d1 <= d2) {
            right = m2;
        }
        else {
            left = m1;
        }
    }
    return (left + right) * 0.5;
}
}

GeSpunSurf::GeSpunSurf()
    : m_pProfile(NULL)
    , m_isOwner(false)
    , m_base(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kZAxis)
    , m_ref(GeVector3d::kIdentity)
    , m_startAngle(0.0)
    , m_endAngle(PI * 2.0)
{
}

GeSpunSurf::GeSpunSurf(const GeCurve3d& pProfile, const GePoint3d& pBase, const GeVector3d& pAxis)
    : m_pProfile(NULL)
    , m_isOwner(false)
    , m_base(pBase)
    , m_axis(pAxis)
    , m_ref(GeVector3d::kIdentity)
    , m_startAngle(0.0)
    , m_endAngle(PI * 2.0)
{
    set(pProfile, pBase, pAxis);
}

GeSpunSurf::GeSpunSurf(const GeSpunSurf& spunSurf)
    : m_pProfile(NULL)
    , m_isOwner(false)
    , m_base(spunSurf.m_base)
    , m_axis(spunSurf.m_axis)
    , m_ref(spunSurf.m_ref)
    , m_startAngle(spunSurf.m_startAngle)
    , m_endAngle(spunSurf.m_endAngle)
{
    *this = spunSurf;
}

GeSpunSurf::~GeSpunSurf()
{
    clearOwnedProfile();
}

void GeSpunSurf::clearOwnedProfile()
{
    if (m_isOwner && m_pProfile != NULL) {
        delete m_pProfile;
    }
    m_pProfile = NULL;
    m_isOwner = false;
}

GeVector3d GeSpunSurf::axisUnit() const
{
    GeVector3d axis = m_axis;
    if (axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kZAxis;
    }
    axis.normalize();
    return axis;
}

bool GeSpunSurf::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == this->type();
}

Ge::EntityId GeSpunSurf::type() const
{
    return Ge::kSpunSurface;
}

GeSpunSurf* GeSpunSurf::copy() const
{
    return new GeSpunSurf(*this);
}

bool GeSpunSurf::operator == (const GeSpunSurf& entity) const
{
    return isEqualTo(entity);
}

bool GeSpunSurf::operator != (const GeSpunSurf& entity) const
{
    return !isEqualTo(entity);
}

bool GeSpunSurf::isEqualTo(const GeSpunSurf& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSpunSurf::isEqualTo(const GeSpunSurf& entity, const GeTol& tol) const
{
    if (!m_base.isEqualTo(entity.m_base, tol)) {
        return false;
    }
    if (!axisUnit().isEqualTo(entity.axisUnit(), tol)) {
        return false;
    }
    if (std::fabs(m_startAngle - entity.m_startAngle) > tol.equalPoint() || std::fabs(m_endAngle - entity.m_endAngle) > tol.equalPoint()) {
        return false;
    }

    if (m_pProfile == NULL || entity.m_pProfile == NULL) {
        return m_pProfile == entity.m_pProfile;
    }
    return m_pProfile->isEqualTo(*entity.m_pProfile, tol);
}

GeSpunSurf& GeSpunSurf::transformBy(const GeMatrix3d& xfm)
{
    m_base.transformBy(xfm);
    m_axis.transformBy(xfm);
    m_ref.transformBy(xfm);
    if (m_pProfile != NULL) {
        m_pProfile->transformBy(xfm);
    }
    return *this;
}

GeSpunSurf& GeSpunSurf::translateBy(const GeVector3d& translateVec)
{
    m_base += translateVec;
    if (m_pProfile != NULL) {
        m_pProfile->translateBy(translateVec);
    }
    return *this;
}

GeSpunSurf& GeSpunSurf::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeSpunSurf& GeSpunSurf::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_base.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_ref.rotateBy(angle, vec);
    if (m_pProfile != NULL) {
        m_pProfile->rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeSpunSurf& GeSpunSurf::mirror(const GePlane& plane)
{
    m_base.mirror(plane);
    GeMatrix3d xfm;
    xfm.setToMirroring(plane);
    m_axis.transformBy(xfm);
    m_ref.transformBy(xfm);
    if (m_pProfile != NULL) {
        m_pProfile->mirror(plane);
    }
    return *this;
}

GeSpunSurf& GeSpunSurf::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeSpunSurf& GeSpunSurf::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    m_base.scaleBy(scale, wrtPoint);
    if (m_pProfile != NULL) {
        m_pProfile->scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeSpunSurf::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeSpunSurf::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d uv = paramOf(pnt, tol);
    return evalPoint(uv).isEqualTo(pnt, tol);
}

GeSpunSurf& GeSpunSurf::operator = (const GeSpunSurf& spunSurface)
{
    if (this == &spunSurface) {
        return *this;
    }

    clearOwnedProfile();
    m_base = spunSurface.m_base;
    m_axis = spunSurface.m_axis;
    m_ref = spunSurface.m_ref;
    m_startAngle = spunSurface.m_startAngle;
    m_endAngle = spunSurface.m_endAngle;

    if (spunSurface.m_pProfile != NULL) {
        GeEntity3d* copied = spunSurface.m_pProfile->copy();
        m_pProfile = dynamic_cast<GeCurve3d*>(copied);
        if (m_pProfile == NULL) {
            delete copied;
            m_pProfile = const_cast<GeCurve3d*>(spunSurface.m_pProfile);
            m_isOwner = false;
        }
        else {
            m_isOwner = true;
        }
    }

    return *this;
}

const GeCurve3d* GeSpunSurf::getProfile() const
{
    return m_pProfile;
}

GePoint3d GeSpunSurf::getBasePoint() const
{
    return m_base;
}

GeVector3d GeSpunSurf::getAxis() const
{
    return m_axis;
}

GeVector3d GeSpunSurf::getRef() const
{
    return m_ref;
}

GeSpunSurf& GeSpunSurf::setAngles(double startAngle, double endAngle)
{
    m_startAngle = startAngle;
    m_endAngle = endAngle;
    return *this;
}

GeSpunSurf& GeSpunSurf::setRef(const GeVector3d& pRef)
{
    m_ref = pRef;
    return *this;
}

void GeSpunSurf::set(const GeCurve3d& pProfile, const GePoint3d& pBase, const GeVector3d& pAxis)
{
    clearOwnedProfile();
    m_base = pBase;
    m_axis = pAxis;

    GeEntity3d* copied = pProfile.copy();
    m_pProfile = dynamic_cast<GeCurve3d*>(copied);
    if (m_pProfile == NULL) {
        delete copied;
        m_pProfile = const_cast<GeCurve3d*>(&pProfile);
        m_isOwner = false;
    }
    else {
        m_isOwner = true;
    }
}

GePoint2d GeSpunSurf::paramOf(const GePoint3d& pnt, const GeTol&) const
{
    if (m_pProfile == NULL) {
        return GePoint2d::kOrigin;
    }

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);

    const int su = 80;
    const int sv = 80;
    double bestU = rangeU.lowerBound();
    double bestV = rangeV.lowerBound();
    double bestD = evalPoint(GePoint2d(bestU, bestV)).distanceTo(pnt);

    for (int i = 0; i <= su; ++i) {
        double ru = static_cast<double>(i) / static_cast<double>(su);
        double u = rangeU.lowerBound() + (rangeU.upperBound() - rangeU.lowerBound()) * ru;
        for (int j = 0; j <= sv; ++j) {
            double rv = static_cast<double>(j) / static_cast<double>(sv);
            double v = rangeV.lowerBound() + (rangeV.upperBound() - rangeV.lowerBound()) * rv;
            double d = evalPoint(GePoint2d(u, v)).distanceTo(pnt);
            if (d < bestD) {
                bestD = d;
                bestU = u;
                bestV = v;
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        bestU = refine_spun_u(*this, pnt, rangeU, bestU, bestV);
        bestV = refine_spun_v(*this, pnt, rangeV, bestU, bestV);
    }

    return GePoint2d(bestU, bestV);
}

GePoint3d GeSpunSurf::evalPoint(const GePoint2d& param) const
{
    if (m_pProfile == NULL) {
        return GePoint3d::kOrigin;
    }

    GePoint3d c = m_pProfile->evalPoint(param.x);
    GeVector3d axis = axisUnit();
    GeVector3d cp = c - m_base;
    GePoint3d z = m_base + axis * cp.dotProduct(axis);
    GeVector3d r = c - z;
    GeVector3d cross = axis.crossProduct(r);

    return z + r * std::cos(param.y) + cross * std::sin(param.y);
}

void GeSpunSurf::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    if (m_pProfile != NULL) {
        m_pProfile->getInterval(rangeU);
    }
    else {
        rangeU.set();
    }
    rangeV.set(m_startAngle, m_endAngle);
}