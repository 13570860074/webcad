#include "GeCone.h"

#include "GeInterval.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
static double cone_abs(double value)
{
    return value < 0.0 ? -value : value;
}

static double cone_clamp(double value, double minValue, double maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static double cone_normalize_0_2pi(double angle)
{
    double twoPi = PI * 2.0;
    double out = fmod(angle, twoPi);
    if (out < 0.0) {
        out += twoPi;
    }
    return out;
}

static double cone_adjust_to_interval(double angle, double start, double end)
{
    double twoPi = PI * 2.0;
    double out = angle;
    while (out < start) {
        out += twoPi;
    }
    while (out > end) {
        out -= twoPi;
    }

    if (out < start) {
        double a = out;
        double b = out + twoPi;
        out = (cone_abs(a - start) <= cone_abs(b - start)) ? a : b;
    }
    if (out > end) {
        double a = out;
        double b = out - twoPi;
        out = (cone_abs(a - end) <= cone_abs(b - end)) ? a : b;
    }

    return cone_clamp(out, start, end);
}
}

GeCone::GeCone()
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_outerNormal(true)
{
}

GeCone::GeCone(double cosineAngle, double sineAngle, const GePoint3d& baseOrigin, double baseRadius, const GeVector3d& axisOfSymmetry)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_outerNormal(true)
{
    set(cosineAngle, sineAngle, baseOrigin, baseRadius, axisOfSymmetry);
}

GeCone::GeCone(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& baseOrigin,
    double baseRadius,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& refAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_outerNormal(true)
{
    set(cosineAngle, sineAngle, baseOrigin, baseRadius, axisOfSymmetry, refAxis, height, startAng, endAng);
}

GeCone::GeCone(const GeCone& cone)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_outerNormal(true)
{
    *this = cone;
}

bool GeCone::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == type();
}

Ge::EntityId GeCone::type() const
{
    return Ge::kCone;
}

GeCone* GeCone::copy() const
{
    return new GeCone(*this);
}

bool GeCone::operator == (const GeCone& entity) const
{
    return isEqualTo(entity);
}

bool GeCone::operator != (const GeCone& entity) const
{
    return !isEqualTo(entity);
}

bool GeCone::isEqualTo(const GeCone& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeCone::isEqualTo(const GeCone& entity, const GeTol& tol) const
{
    if (cone_abs(m_baseRadius - entity.m_baseRadius) > tol.equalPoint()) {
        return false;
    }
    if (!m_baseCenter.isEqualTo(entity.m_baseCenter, tol)) {
        return false;
    }
    if (!axisUnit().isEqualTo(entity.axisUnit(), tol)) {
        return false;
    }
    if (!refUnit().isEqualTo(entity.refUnit(), tol)) {
        return false;
    }
    if (cone_abs(m_cosHalf - entity.m_cosHalf) > tol.equalPoint() || cone_abs(m_sinHalf - entity.m_sinHalf) > tol.equalPoint()) {
        return false;
    }
    if (!(m_height == entity.m_height)) {
        return false;
    }
    if (cone_abs(m_startAng - entity.m_startAng) > tol.equalPoint() || cone_abs(m_endAng - entity.m_endAng) > tol.equalPoint()) {
        return false;
    }
    return m_outerNormal == entity.m_outerNormal;
}

GeCone& GeCone::transformBy(const GeMatrix3d& xfm)
{
    m_baseCenter.transformBy(xfm);

    GeVector3d axisTransformed = axisUnit();
    axisTransformed.transformBy(xfm);
    double axisScale = axisTransformed.length();
    if (axisScale <= GeContext::gTol.equalVector()) {
        axisScale = 1.0;
    }
    axisTransformed.normalize();

    GeVector3d refTransformed = refUnit();
    refTransformed.transformBy(xfm);
    if (refTransformed.length() <= GeContext::gTol.equalVector() || refTransformed.isParallelTo(axisTransformed, GeContext::gTol)) {
        GeVector3d fallback = (cone_abs(axisTransformed.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
        refTransformed = axisTransformed.crossProduct(fallback);
    }
    refTransformed.normalize();

    m_axis = axisTransformed;
    m_ref = refTransformed;
    m_baseRadius *= axisScale;
    if (m_height.isBounded()) {
        m_height.set(m_height.lowerBound() * axisScale, m_height.upperBound() * axisScale);
    }

    return *this;
}

GeCone& GeCone::translateBy(const GeVector3d& translateVec)
{
    m_baseCenter += translateVec;
    return *this;
}

GeCone& GeCone::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeCone& GeCone::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_baseCenter.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_ref.rotateBy(angle, vec);
    return *this;
}

GeCone& GeCone::mirror(const GePlane& plane)
{
    m_baseCenter.mirror(plane);
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    m_axis.transformBy(mirrorXfm);
    m_ref.transformBy(mirrorXfm);
    m_outerNormal = !m_outerNormal;
    return *this;
}

GeCone& GeCone::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCone& GeCone::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    m_baseCenter.scaleBy(scale, wrtPoint);
    m_baseRadius *= scaleFactor;
    if (m_height.isBounded()) {
        m_height.set(m_height.lowerBound() * scaleFactor, m_height.upperBound() * scaleFactor);
    }
    return *this;
}

bool GeCone::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeCone::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d uv = paramOf(pnt, tol);
    return evalPoint(uv).isEqualTo(pnt, tol);
}

double GeCone::baseRadius() const
{
    return m_baseRadius;
}

GePoint3d GeCone::baseCenter() const
{
    return m_baseCenter;
}

void GeCone::getAngles(double& startAng, double& endAng) const
{
    startAng = m_startAng;
    endAng = m_endAng;
}

double GeCone::halfAngle() const
{
    return atan2(cone_abs(m_sinHalf), cone_abs(m_cosHalf));
}

void GeCone::getHalfAngle(double& cosineAngle, double& sineAngle) const
{
    cosineAngle = cone_abs(m_cosHalf);
    sineAngle = cone_abs(m_sinHalf);
}

void GeCone::getHalfAngleSigned(double& cosineAngle, double& sineAngle) const
{
    cosineAngle = m_cosHalf;
    sineAngle = m_sinHalf;
}

void GeCone::getHeight(GeInterval& height) const
{
    height = m_height;
}

double GeCone::heightAt(double u) const
{
    return u;
}

GeVector3d GeCone::axisOfSymmetry() const
{
    return axisUnit();
}

GeVector3d GeCone::refAxis() const
{
    return refUnit();
}

GePoint3d GeCone::apex() const
{
    double slope = signedTanHalfAngle();
    if (cone_abs(slope) <= GeContext::gTol.equalPoint()) {
        return m_baseCenter;
    }
    double uApex = -m_baseRadius / slope;
    return m_baseCenter + axisUnit() * uApex;
}

bool GeCone::isClosed(const GeTol& tol) const
{
    return (m_endAng - m_startAng) >= (PI * 2.0 - tol.equalPoint());
}

bool GeCone::isOuterNormal() const
{
    return m_outerNormal;
}

GeCone& GeCone::setBaseRadius(double baseRadius)
{
    m_baseRadius = baseRadius > GeContext::gTol.equalPoint() ? baseRadius : GeContext::gTol.equalPoint();
    return *this;
}

GeCone& GeCone::setBaseCenter(const GePoint3d& baseOrigin)
{
    m_baseCenter = baseOrigin;
    return *this;
}

GeCone& GeCone::setAngles(double startAng, double endAng)
{
    double twoPi = PI * 2.0;
    m_startAng = startAng;
    m_endAng = endAng;
    while (m_endAng < m_startAng) {
        m_endAng += twoPi;
    }
    if (m_endAng - m_startAng > twoPi) {
        m_endAng = m_startAng + twoPi;
    }
    return *this;
}

GeCone& GeCone::setHeight(const GeInterval& height)
{
    if (height.isBounded()) {
        m_height = height;
    }
    else {
        m_height.set(0.0, 1.0);
    }
    return *this;
}

GeCone& GeCone::set(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& baseOrigin,
    double baseRadius,
    const GeVector3d& axisOfSymmetry)
{
    GeInterval defaultHeight(0.0, 1.0);
    return set(cosineAngle, sineAngle, baseOrigin, baseRadius, axisOfSymmetry, GeVector3d::kXAxis, defaultHeight, -PI, PI);
}

GeCone& GeCone::set(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& baseOrigin,
    double baseRadius,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& refAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
{
    m_cosHalf = cosineAngle;
    m_sinHalf = sineAngle;
    if (cone_abs(m_cosHalf) <= GeContext::gTol.equalPoint() && cone_abs(m_sinHalf) <= GeContext::gTol.equalPoint()) {
        m_cosHalf = std::sqrt(0.5);
        m_sinHalf = std::sqrt(0.5);
    }

    m_baseCenter = baseOrigin;
    setBaseRadius(baseRadius);

    m_axis = axisOfSymmetry.length() > GeContext::gTol.equalVector() ? axisOfSymmetry.normal() : GeVector3d::kYAxis;

    GeVector3d ref = refAxis;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(m_axis, GeContext::gTol)) {
        GeVector3d fallback = (cone_abs(m_axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        ref = m_axis.crossProduct(fallback);
    }
    ref -= m_axis * ref.dotProduct(m_axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis;
    }
    ref.normalize();
    m_ref = ref;

    setHeight(height);
    setAngles(startAng, endAng);
    return *this;
}

GeCone& GeCone::operator = (const GeCone& cone)
{
    if (this == &cone) {
        return *this;
    }

    m_cosHalf = cone.m_cosHalf;
    m_sinHalf = cone.m_sinHalf;
    m_baseCenter = cone.m_baseCenter;
    m_baseRadius = cone.m_baseRadius;
    m_axis = cone.m_axis;
    m_ref = cone.m_ref;
    m_height = cone.m_height;
    m_startAng = cone.m_startAng;
    m_endAng = cone.m_endAng;
    m_outerNormal = cone.m_outerNormal;
    return *this;
}

GePoint2d GeCone::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GeVector3d delta = pnt - m_baseCenter;
    double u = delta.dotProduct(axis);
    if (m_height.isBounded()) {
        u = cone_clamp(u, m_height.lowerBound(), m_height.upperBound());
    }

    GeVector3d radial = delta - axis * u;
    double v = m_startAng;
    if (radial.length() > tol.equalVector()) {
        v = atan2(radial.dotProduct(ortho), radial.dotProduct(ref));
    }

    v = cone_adjust_to_interval(v, m_startAng, m_endAng);
    return GePoint2d(u, v);
}

GePoint3d GeCone::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double u = param.x;
    if (m_height.isBounded()) {
        u = cone_clamp(u, m_height.lowerBound(), m_height.upperBound());
    }

    double v = cone_adjust_to_interval(param.y, m_startAng, m_endAng);
    double r = radiusAt(u);
    GeVector3d radialDir = ref * cos(v) + ortho * sin(v);
    return m_baseCenter + axis * u + radialDir * r;
}

void GeCone::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    rangeU = m_height;
    rangeV.set(m_startAng, m_endAng);
}

GeVector3d GeCone::axisUnit() const
{
    if (m_axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kYAxis;
    }
    GeVector3d axis = m_axis;
    axis.normalize();
    return axis;
}

GeVector3d GeCone::refUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = m_ref;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(axis, GeContext::gTol)) {
        GeVector3d fallback = (cone_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        ref = axis.crossProduct(fallback);
    }
    ref -= axis * ref.dotProduct(axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis;
    }
    ref.normalize();
    return ref;
}

GeVector3d GeCone::orthoRefUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = axis.crossProduct(ref);
    if (ortho.length() <= GeContext::gTol.equalVector()) {
        GeVector3d fallback = (cone_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        ortho = axis.crossProduct(fallback);
    }
    ortho.normalize();
    return ortho;
}

double GeCone::signedTanHalfAngle() const
{
    if (cone_abs(m_cosHalf) <= GeContext::gTol.equalPoint()) {
        return (m_sinHalf >= 0.0) ? 1.0e12 : -1.0e12;
    }
    return m_sinHalf / m_cosHalf;
}

double GeCone::radiusAt(double u) const
{
    return m_baseRadius + u * signedTanHalfAngle();
}
