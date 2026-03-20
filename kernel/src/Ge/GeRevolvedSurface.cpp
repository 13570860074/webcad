#include "GeRevolvedSurface.h"

#include "GeCurve3d.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
double normalize_angle_0_2pi(double angle)
{
    double twoPi = PI * 2.0;
    double result = fmod(angle, twoPi);
    if (result < 0.0) {
        result += twoPi;
    }
    return result;
}

double clamp_to_interval(double value, const GeInterval& range)
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

double refine_revolved_u(const GeRevolvedSurface& surf, const GePoint3d& target, const GeInterval& rangeU, double u0, double vFixed)
{
    if (!rangeU.isBounded()) {
        return u0;
    }

    double span = rangeU.upperBound() - rangeU.lowerBound();
    if (span < 0.0) {
        span = -span;
    }
    double window = span / 16.0;
    if (window <= GeContext::gTol.equalPoint()) {
        window = span / 4.0;
    }

    double left = clamp_to_interval(u0 - window, rangeU);
    double right = clamp_to_interval(u0 + window, rangeU);
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

double refine_revolved_v(const GeRevolvedSurface& surf, const GePoint3d& target, const GeInterval& rangeV, double uFixed, double v0)
{
    if (!rangeV.isBounded()) {
        return v0;
    }

    double span = rangeV.upperBound() - rangeV.lowerBound();
    if (span < 0.0) {
        span = -span;
    }
    double window = span / 16.0;
    if (window <= GeContext::gTol.equalPoint()) {
        window = span / 4.0;
    }

    double left = clamp_to_interval(v0 - window, rangeV);
    double right = clamp_to_interval(v0 + window, rangeV);
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

GeRevolvedSurface::GeRevolvedSurface()
    : m_pProfile(NULL)
    , m_isOwner(false)
    , m_base(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kZAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_startAngle(0.0)
    , m_endAngle(PI * 2.0)
{
}

GeRevolvedSurface::GeRevolvedSurface(
    const GeCurve3d& pProfile,
    const GePoint3d& pBase,
    const GeVector3d& pAxis,
    const GeVector3d& pRef,
    double startAngle,
    double endAngle)
    : m_pProfile(NULL)
    , m_isOwner(false)
    , m_base(pBase)
    , m_axis(pAxis)
    , m_ref(pRef)
    , m_startAngle(startAngle)
    , m_endAngle(endAngle)
{
    set(pProfile, pBase, pAxis, pRef);
    setAngles(startAngle, endAngle);
}

GeRevolvedSurface::GeRevolvedSurface(const GeRevolvedSurface& revolvedSurf)
    : m_pProfile(NULL)
    , m_isOwner(false)
    , m_base(revolvedSurf.m_base)
    , m_axis(revolvedSurf.m_axis)
    , m_ref(revolvedSurf.m_ref)
    , m_startAngle(revolvedSurf.m_startAngle)
    , m_endAngle(revolvedSurf.m_endAngle)
{
    *this = revolvedSurf;
}

GeRevolvedSurface::~GeRevolvedSurface()
{
    clearOwnedProfile();
}

void GeRevolvedSurface::clearOwnedProfile()
{
    if (m_isOwner && m_pProfile != NULL) {
        delete m_pProfile;
    }
    m_pProfile = NULL;
    m_isOwner = false;
}

GeVector3d GeRevolvedSurface::axisUnit() const
{
    GeVector3d axis = m_axis;
    if (axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kZAxis;
    }
    axis.normalize();
    return axis;
}

GeVector3d GeRevolvedSurface::refUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = m_ref;
    GeVector3d fallback = (std::fabs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(axis, GeContext::gTol)) {
        ref = axis.crossProduct(fallback);
    }
    ref -= axis * ref.dotProduct(axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        GeVector3d secondFallback = GeVector3d::kZAxis;
        if (axis.isParallelTo(secondFallback, GeContext::gTol)) {
            secondFallback = GeVector3d::kXAxis;
        }
        ref = axis.crossProduct(secondFallback);
    }
    ref.normalize();
    return ref;
}

GeVector3d GeRevolvedSurface::orthoRefUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = axis.crossProduct(ref);
    if (ortho.length() <= GeContext::gTol.equalVector()) {
        GeVector3d fallback = (std::fabs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
        ortho = axis.crossProduct(fallback);
        if (ortho.length() <= GeContext::gTol.equalVector()) {
            ortho = GeVector3d::kZAxis;
        }
    }
    ortho.normalize();
    return ortho;
}

bool GeRevolvedSurface::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == this->type();
}

Ge::EntityId GeRevolvedSurface::type() const
{
    return Ge::kRevolvedSurface;
}

GeRevolvedSurface* GeRevolvedSurface::copy() const
{
    return new GeRevolvedSurface(*this);
}

bool GeRevolvedSurface::operator == (const GeRevolvedSurface& entity) const
{
    return isEqualTo(entity);
}

bool GeRevolvedSurface::operator != (const GeRevolvedSurface& entity) const
{
    return !isEqualTo(entity);
}

bool GeRevolvedSurface::isEqualTo(const GeRevolvedSurface& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeRevolvedSurface::isEqualTo(const GeRevolvedSurface& entity, const GeTol& tol) const
{
    if (!m_base.isEqualTo(entity.m_base, tol)) {
        return false;
    }

    if (!axisUnit().isEqualTo(entity.axisUnit(), tol) || !refUnit().isEqualTo(entity.refUnit(), tol)) {
        return false;
    }

    if (std::fabs(m_startAngle - entity.m_startAngle) > tol.equalPoint()
        || std::fabs(m_endAngle - entity.m_endAngle) > tol.equalPoint()) {
        return false;
    }

    if (m_pProfile == NULL || entity.m_pProfile == NULL) {
        return m_pProfile == entity.m_pProfile;
    }

    return m_pProfile->isEqualTo(*entity.m_pProfile, tol);
}

GeRevolvedSurface& GeRevolvedSurface::transformBy(const GeMatrix3d& xfm)
{
    m_base.transformBy(xfm);
    m_axis.transformBy(xfm);
    m_ref.transformBy(xfm);
    if (m_pProfile != NULL) {
        m_pProfile->transformBy(xfm);
    }
    return *this;
}

GeRevolvedSurface& GeRevolvedSurface::translateBy(const GeVector3d& translateVec)
{
    m_base += translateVec;
    if (m_pProfile != NULL) {
        m_pProfile->translateBy(translateVec);
    }
    return *this;
}

GeRevolvedSurface& GeRevolvedSurface::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeRevolvedSurface& GeRevolvedSurface::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_base.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_ref.rotateBy(angle, vec);
    if (m_pProfile != NULL) {
        m_pProfile->rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeRevolvedSurface& GeRevolvedSurface::mirror(const GePlane& plane)
{
    m_base.mirror(plane);
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    m_axis.transformBy(mirrorXfm);
    m_ref.transformBy(mirrorXfm);
    if (m_pProfile != NULL) {
        m_pProfile->mirror(plane);
    }
    return *this;
}

GeRevolvedSurface& GeRevolvedSurface::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeRevolvedSurface& GeRevolvedSurface::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    m_base.scaleBy(scale, wrtPoint);
    if (m_pProfile != NULL) {
        m_pProfile->scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeRevolvedSurface::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeRevolvedSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d uv = paramOf(pnt, tol);
    return evalPoint(uv).isEqualTo(pnt, tol);
}

GeRevolvedSurface& GeRevolvedSurface::operator = (const GeRevolvedSurface& revolvedSurf)
{
    if (this == &revolvedSurf) {
        return *this;
    }

    clearOwnedProfile();
    m_base = revolvedSurf.m_base;
    m_axis = revolvedSurf.m_axis;
    m_ref = revolvedSurf.m_ref;
    m_startAngle = revolvedSurf.m_startAngle;
    m_endAngle = revolvedSurf.m_endAngle;

    if (revolvedSurf.m_pProfile != NULL) {
        GeEntity3d* copied = revolvedSurf.m_pProfile->copy();
        m_pProfile = dynamic_cast<GeCurve3d*>(copied);
        if (m_pProfile == NULL) {
            delete copied;
            m_pProfile = const_cast<GeCurve3d*>(revolvedSurf.m_pProfile);
            m_isOwner = false;
        }
        else {
            m_isOwner = true;
        }
    }

    return *this;
}

const GeCurve3d& GeRevolvedSurface::getProfile() const
{
    return *m_pProfile;
}

GePoint3d GeRevolvedSurface::getBasePoint() const
{
    return m_base;
}

GeVector3d GeRevolvedSurface::getAxis() const
{
    return m_axis;
}

GeVector3d GeRevolvedSurface::getRef() const
{
    return m_ref;
}

void GeRevolvedSurface::getAngles(double& startAngle, double& endAngle) const
{
    startAngle = m_startAngle;
    endAngle = m_endAngle;
}

GeRevolvedSurface& GeRevolvedSurface::setAngles(double startAngle, double endAngle)
{
    m_startAngle = startAngle;
    m_endAngle = endAngle;
    return *this;
}

GeRevolvedSurface& GeRevolvedSurface::setRef(const GeVector3d& pRef)
{
    m_ref = pRef;
    return *this;
}

void GeRevolvedSurface::set(const GeCurve3d& pProfile, const GePoint3d& pBase, const GeVector3d& pAxis, const GeVector3d& pRef)
{
    clearOwnedProfile();
    m_base = pBase;
    m_axis = pAxis;
    m_ref = pRef;

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

GePoint2d GeRevolvedSurface::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    if (m_pProfile == NULL) {
        return GePoint2d::kOrigin;
    }

    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GeVector3d delta = pnt - m_base;
    double z = delta.dotProduct(axis);
    GeVector3d radial = delta - axis * z;
    double x = radial.length();

    double angle = 0.0;
    if (radial.length() > tol.equalVector()) {
        GeVector3d radialDir = radial.normal();
        double c = radialDir.dotProduct(ref);
        double s = radialDir.dotProduct(ortho);
        angle = atan2(s, c);
        angle = normalize_angle_0_2pi(angle);
    }

    GePoint3d profilePoint(x, 0.0, z);
    double u = m_pProfile->paramOf(profilePoint, tol);

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);
    if (!rangeU.isBounded() || !rangeV.isBounded()) {
        return GePoint2d(u, angle);
    }

    u = clamp_to_interval(u, rangeU);
    double v = clamp_to_interval(angle, rangeV);

    for (int i = 0; i < 3; ++i) {
        u = refine_revolved_u(*this, pnt, rangeU, u, v);
        v = refine_revolved_v(*this, pnt, rangeV, u, v);
    }

    return GePoint2d(u, v);
}

GePoint3d GeRevolvedSurface::evalPoint(const GePoint2d& param) const
{
    if (m_pProfile == NULL) {
        return GePoint3d::kOrigin;
    }

    GePoint3d c = m_pProfile->evalPoint(param.x);
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double v = param.y;
    GeVector3d radialDir = ref * cos(v) + ortho * sin(v);
    GePoint3d point = m_base + radialDir * c.x + axis * c.z;
    return point;
}

void GeRevolvedSurface::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    if (m_pProfile != NULL) {
        m_pProfile->getInterval(rangeU);
    }
    else {
        rangeU.set();
    }
    rangeV.set(m_startAngle, m_endAngle);
}