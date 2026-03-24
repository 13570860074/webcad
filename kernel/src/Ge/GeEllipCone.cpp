#include "GeEllipCone.h"

#include "GeLinearEnt3d.h"
#include "GePlane.h"
#include "GePointOnSurface.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
static double ellipcone_abs(double value)
{
    return value < 0.0 ? -value : value;
}

static double ellipcone_clamp(double value, double minValue, double maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static double ellipcone_adjust_to_interval(double angle, double start, double end)
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
        out = (ellipcone_abs(a - start) <= ellipcone_abs(b - start)) ? a : b;
    }
    if (out > end) {
        double a = out;
        double b = out - twoPi;
        out = (ellipcone_abs(a - end) <= ellipcone_abs(b - end)) ? a : b;
    }

    return ellipcone_clamp(out, start, end);
}

static double ellipcone_move_into_valid_interval(double angle, double start, double end)
{
    double out = angle;
    double period = PI * 2.0;

    if (out < start) {
        out = start - std::fmod(start - out, period);
        if (out < start) {
            out += period;
        }
    }
    if (out > end) {
        out = end + std::fmod(out - end, period);
        if (out > end) {
            out -= period;
        }
    }
    if (out < start) {
        double d1 = start - out;
        double d2 = std::fabs(end - out - period);
        if (d2 < d1) {
            out += period;
        }
    }
    if (out > end) {
        double d1 = out - end;
        double d2 = std::fabs(out - period - start);
        if (d2 < d1) {
            out -= period;
        }
    }

    return out;
}

static bool ellipcone_is_uni_scaled_ortho(const GeMatrix3d& xfm, double& scaleFactor, const GeTol& tol = GeContext::gTol)
{
    if (xfm.isPerspective()) {
        return false;
    }

    GeVector3d axisX = GeVector3d::kXAxis;
    GeVector3d axisY = GeVector3d::kYAxis;
    GeVector3d axisZ = GeVector3d::kZAxis;
    axisX.transformBy(xfm);
    axisY.transformBy(xfm);
    axisZ.transformBy(xfm);

    double lengthX = axisX.length();
    double lengthY = axisY.length();
    double lengthZ = axisZ.length();
    if (lengthX <= tol.equalVector() || lengthY <= tol.equalVector() || lengthZ <= tol.equalVector()) {
        return false;
    }

    if (std::fabs(lengthX - lengthY) > tol.equalVector() || std::fabs(lengthX - lengthZ) > tol.equalVector()) {
        return false;
    }

    GeVector3d unitX = axisX.normal();
    GeVector3d unitY = axisY.normal();
    GeVector3d unitZ = axisZ.normal();
    if (std::fabs(unitX.dotProduct(unitY)) > tol.equalVector()
        || std::fabs(unitX.dotProduct(unitZ)) > tol.equalVector()
        || std::fabs(unitY.dotProduct(unitZ)) > tol.equalVector()) {
        return false;
    }

    scaleFactor = (lengthX + lengthY + lengthZ) / 3.0;
    return true;
}

static bool ellipcone_interval_contains_with_tol(const GeInterval& range, double value, const GeTol& tol)
{
    if (range.isBoundedBelow() && value < range.lowerBound() - tol.equalPoint()) {
        return false;
    }
    if (range.isBoundedAbove() && value > range.upperBound() + tol.equalPoint()) {
        return false;
    }
    return true;
}

static bool ellipcone_linear_entity_contains_point(const GeLinearEnt3d& linEnt, const GePoint3d& point, const GeTol& tol)
{
    if (linEnt.isOn(point, tol)) {
        double param = linEnt.paramOf(point, tol);
        GeInterval range;
        linEnt.getInterval(range);
        return ellipcone_interval_contains_with_tol(range, param, tol);
    }
    return false;
}

static bool ellipcone_surface_contains_point(const GeEllipCone& cone, const GePoint3d& point, const GeTol& tol)
{
    return cone.isOn(point, tol);
}

static void ellipcone_append_unique_point(GePoint3d* points, int& count, const GePoint3d& point, const GeTol& tol)
{
    for (int i = 0; i < count; ++i) {
        if (points[i].isEqualTo(point, tol)) {
            return;
        }
    }

    if (count < 2) {
        points[count] = point;
        ++count;
    }
}

static double ellipcone_default_u_param_scale(double majorRadius)
{
    return ellipcone_abs(majorRadius) > GeContext::gTol.equalPoint() ? majorRadius : 1.0;
}

static double ellipcone_effective_u_param_scale(double scale, double majorRadius)
{
    return ellipcone_abs(scale) > GeContext::gTol.equalPoint() ? scale : ellipcone_default_u_param_scale(majorRadius);
}

static void ellipcone_scale_interval(GeInterval& interval, double scale)
{
    if (interval.isBounded()) {
        interval.set(interval.lowerBound() * scale, interval.upperBound() * scale);
        return;
    }

    if (interval.isBoundedBelow() && interval.isBoundedAbove()) {
        interval.set(interval.lowerBound() * scale, interval.upperBound() * scale);
        return;
    }

    if (interval.isBoundedBelow()) {
        interval.set(scale >= 0.0, interval.lowerBound() * scale);
        return;
    }

    if (interval.isBoundedAbove()) {
        interval.set(scale < 0.0, interval.upperBound() * scale);
    }
}

static GeVector3d ellipcone_default_major_axis(const GeVector3d& axisOfSymmetry)
{
    GeVector3d axis = axisOfSymmetry;
    if (axis.length() <= GeContext::gTol.equalVector()) {
        axis = GeVector3d::kYAxis;
    }
    axis.normalize();

    GeVector3d fallback = GeVector3d::kZAxis;
    if (axis.isParallelTo(fallback, GeContext::gTol)) {
        fallback = GeVector3d::kYAxis;
    }
    GeVector3d major = fallback.crossProduct(axis);
    if (major.length() <= GeContext::gTol.equalVector()) {
        major = GeVector3d::kXAxis.crossProduct(axis);
    }
    if (major.length() <= GeContext::gTol.equalVector()) {
        major = GeVector3d::kXAxis;
    }
    major.normalize();
    return major;
}

static GePoint3d ellipcone_segment_closest_point(const GePoint3d& start, const GePoint3d& end, const GePoint3d& point, double& blend)
{
    GeVector3d seg = end - start;
    double lengthSq = seg.lengthSqrd();
    if (lengthSq <= GeContext::gTol.equalPoint()) {
        blend = 0.0;
        return start;
    }

    blend = (point - start).dotProduct(seg) / lengthSq;
    if (blend < 0.0) {
        blend = 0.0;
    }
    else if (blend > 1.0) {
        blend = 1.0;
    }
    return start + seg * blend;
}

}

GeEllipCone::GeEllipCone()
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_minorRadius(1.0)
    , m_majorRadius(1.0)
    , m_baseCenter(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(1.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(true)
{
}

GeEllipCone::GeEllipCone(double cosineAngle, double sineAngle, const GePoint3d& origin, double minorRadius, double majorRadius, const GeVector3d& axisOfSymmetry)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_minorRadius(1.0)
    , m_majorRadius(1.0)
    , m_baseCenter(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(1.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(true)
{
    set(cosineAngle, sineAngle, origin, minorRadius, majorRadius, axisOfSymmetry);
}

GeEllipCone::GeEllipCone(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& baseOrigin,
    double minorRadius,
    double majorRadius,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& majorAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_minorRadius(1.0)
    , m_majorRadius(1.0)
    , m_baseCenter(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(1.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(true)
{
    set(cosineAngle, sineAngle, baseOrigin, minorRadius, majorRadius, axisOfSymmetry, majorAxis, height, startAng, endAng);
}

GeEllipCone::GeEllipCone(const GeEllipCone& cone)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_minorRadius(1.0)
    , m_majorRadius(1.0)
    , m_baseCenter(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(1.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(true)
{
    *this = cone;
}

bool GeEllipCone::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == type();
}

Ge::EntityId GeEllipCone::type() const
{
    return Ge::kEllipCone;
}

GeEllipCone* GeEllipCone::copy() const
{
    return new GeEllipCone(*this);
}

bool GeEllipCone::operator == (const GeEllipCone& entity) const
{
    return isEqualTo(entity);
}

bool GeEllipCone::operator != (const GeEllipCone& entity) const
{
    return !isEqualTo(entity);
}

bool GeEllipCone::isEqualTo(const GeEllipCone& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeEllipCone::isEqualTo(const GeEllipCone& entity, const GeTol& tol) const
{
    GeInterval intrvlU;
    GeInterval intrvlV;
    GeInterval intrvlU1;
    GeInterval intrvlV1;
    getEnvelope(intrvlU, intrvlV);
    entity.getEnvelope(intrvlU1, intrvlV1);
    double bh1 = 0.0;
    double bh2 = 0.0;
    double rvBh1 = 0.0;
    double rvBh2 = 0.0;
    double ba1 = 0.0;
    double ba2 = 0.0;
    double rvBa1 = 0.0;
    double rvBa2 = 0.0;
    intrvlU.getBounds(bh1, bh2);
    intrvlU1.getBounds(rvBh1, rvBh2);
    intrvlV.getBounds(ba1, ba2);
    intrvlV1.getBounds(rvBa1, rvBa2);

    if (ellipcone_abs(m_minorRadius - entity.m_minorRadius) > tol.equalPoint()) {
        return false;
    }
    if (ellipcone_abs(m_majorRadius - entity.m_majorRadius) > tol.equalPoint()) {
        return false;
    }
    if (!m_baseCenter.isEqualTo(entity.m_baseCenter, tol)) {
        return false;
    }
    if (!axisUnit().isEqualTo(entity.axisUnit(), tol)) {
        return false;
    }
    if (!majorAxisUnit().isEqualTo(entity.majorAxisUnit(), tol)) {
        return false;
    }
    if (ellipcone_abs(m_cosHalf - entity.m_cosHalf) > tol.equalPoint() || ellipcone_abs(m_sinHalf - entity.m_sinHalf) > tol.equalPoint()) {
        return false;
    }
    if (ellipcone_abs(ba1 - rvBa1) > tol.equalPoint() || ellipcone_abs(ba2 - rvBa2) > tol.equalPoint()) {
        return false;
    }
    if (ellipcone_abs(bh1 - rvBh1) > tol.equalPoint() || ellipcone_abs(bh2 - rvBh2) > tol.equalPoint()) {
        return false;
    }
    return ellipcone_abs(getUParamScale() - entity.getUParamScale()) <= tol.equalPoint();
}

GeEllipCone& GeEllipCone::transformBy(const GeMatrix3d& xfm)
{
    double scaleFactor = 1.0;
    if (!ellipcone_is_uni_scaled_ortho(xfm, scaleFactor)) {
        return *this;
    }

    GeVector3d axisRef = axisUnit();
    GeVector3d majorRef = majorAxisUnit();

    GeVector3d axisTransformed = axisRef;
    GeVector3d majorTransformed = majorRef;
    axisTransformed.transformBy(xfm);
    majorTransformed.transformBy(xfm);
    if (axisTransformed.length() <= GeContext::gTol.equalVector() || majorTransformed.length() <= GeContext::gTol.equalVector()) {
        return *this;
    }

    axisTransformed.normalize();
    majorTransformed.normalize();

    if (std::fabs(axisTransformed.dotProduct(majorTransformed)) > GeContext::gTol.equalVector()) {
        return *this;
    }

    m_baseCenter.transformBy(xfm);
    m_minorRadius *= scaleFactor;
    m_majorRadius *= scaleFactor;
    m_uParamScale *= scaleFactor;
    ellipcone_scale_interval(m_height, scaleFactor);

    m_axis = axisTransformed;
    m_majorAxis = majorTransformed;

    if (xfm.det() < 0.0) {
        m_uParamScale = -m_uParamScale;
        ellipcone_scale_interval(m_height, -1.0);
        m_axis.negate();
        m_sinHalf = -m_sinHalf;
    }

    return *this;
}

GeEllipCone& GeEllipCone::translateBy(const GeVector3d& translateVec)
{
    m_baseCenter += translateVec;
    return *this;
}

GeEllipCone& GeEllipCone::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeEllipCone& GeEllipCone::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_baseCenter.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_majorAxis.rotateBy(angle, vec);
    return *this;
}

GeEllipCone& GeEllipCone::mirror(const GePlane& plane)
{
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    return transformBy(mirrorXfm);
}

GeEllipCone& GeEllipCone::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeEllipCone& GeEllipCone::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeMatrix3d scaleXfm;
    scaleXfm.setToScaling(scaleFactor, wrtPoint);
    return transformBy(scaleXfm);
}

bool GeEllipCone::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeEllipCone::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return GeSurface::isOn(pnt, tol);
}

double GeEllipCone::radiusRatio() const
{
    return m_minorRadius / m_majorRadius;
}

double GeEllipCone::minorRadius() const
{
    return m_minorRadius;
}

double GeEllipCone::majorRadius() const
{
    return m_majorRadius;
}

GePoint3d GeEllipCone::baseCenter() const
{
    return m_baseCenter;
}

void GeEllipCone::getAngles(double& startAng, double& endAng) const
{
    startAng = m_startAng;
    endAng = m_endAng;
}

double GeEllipCone::halfAngle() const
{
    return std::asin(m_sinHalf);
}

void GeEllipCone::getHalfAngle(double& cosineAngle, double& sineAngle) const
{
    cosineAngle = m_cosHalf;
    sineAngle = m_sinHalf;
}

void GeEllipCone::getHeight(GeInterval& height) const
{
    height = m_height;
}

double GeEllipCone::heightAt(double u) const
{
    return u * getUParamScale() * std::fabs(m_cosHalf);
}

double GeEllipCone::getUParamScale() const
{
    return m_uParamScale;
}

GeVector3d GeEllipCone::axisOfSymmetry() const
{
    return axisUnit();
}

GeVector3d GeEllipCone::majorAxis() const
{
    return majorAxisUnit() * m_majorRadius;
}

GeVector3d GeEllipCone::minorAxis() const
{
    return minorAxisUnit() * m_minorRadius;
}

GePoint3d GeEllipCone::apex() const
{
    return m_baseCenter - axisUnit() * (m_majorRadius * m_cosHalf / m_sinHalf);
}

bool GeEllipCone::isClosed(const GeTol& tol) const
{
    return std::fabs((m_endAng - m_startAng) - (PI * 2.0)) <= tol.equalPoint();
}

bool GeEllipCone::isOuterNormal() const
{
    if (m_outerNormal) {
        return m_cosHalf >= 0.0;
    }
    return m_cosHalf < 0.0;
}

bool GeEllipCone::isNormalReversed() const
{
    return !m_outerNormal;
}

bool GeEllipCone::isLeftHanded() const
{
    return isOuterNormal() ^ (m_uParamScale < 0.0);
}

GeSurface& GeEllipCone::reverseNormal()
{
    m_outerNormal = !m_outerNormal;
    return *this;
}

void GeEllipCone::setUParamScale(double uScale)
{
    if (ellipcone_abs(uScale) <= GeContext::gTol.equalPoint()) {
        uScale = m_majorRadius;
    }

    if (uScale * m_uParamScale < 0.0) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }

    m_uParamScale = uScale;
    m_hasCustomUParamScale = true;
}

GeEllipCone& GeEllipCone::setMinorRadius(double minorRadius)
{
    m_minorRadius = std::fabs(minorRadius);
    return *this;
}

GeEllipCone& GeEllipCone::setMajorRadius(double majorRadius)
{
    m_majorRadius = std::fabs(majorRadius);
    m_uParamScale = m_majorRadius;
    m_hasCustomUParamScale = false;
    if (m_height.isBounded() && m_height.upperBound() < m_height.lowerBound()) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }
    return *this;
}

GeEllipCone& GeEllipCone::setAngles(double startAng, double endAng)
{
    if (startAng < endAng) {
        m_startAng = startAng;
        m_endAng = endAng;
    }
    else {
        m_startAng = endAng;
        m_endAng = startAng;
    }
    if (m_endAng - m_startAng > PI * 2.0) {
        m_startAng = -PI;
        m_endAng = PI;
    }
    return *this;
}

GeEllipCone& GeEllipCone::setHeight(const GeInterval& height)
{
    double apexValue = -m_majorRadius * m_cosHalf / m_sinHalf;

    if (height == GeInterval()) {
        if (apexValue < 0.0) {
            m_height.set(apexValue, 0.0);
        }
        else {
            m_height.set(0.0, apexValue);
        }
    }
    else if (height.isBounded()) {
        if ((getUParamScale() > 0.0) ^ (height.lowerBound() < height.upperBound())) {
            m_height.set(height.upperBound(), height.lowerBound());
        }
        else {
            m_height = height;
        }

        if (apexValue < 0.0) {
            if (m_height.lowerBound() < apexValue) {
                m_height.setLower(apexValue);
            }
            if (m_height.upperBound() < apexValue) {
                m_height.setUpper(apexValue);
                if (m_height.lowerBound() == apexValue) {
                    m_height.set(heightAt(1.0), heightAt(0.0));
                }
            }
        }
        else {
            if (m_height.lowerBound() > apexValue) {
                m_height.setLower(apexValue);
            }
            if (m_height.upperBound() > apexValue) {
                m_height.setUpper(apexValue);
                if (m_height.lowerBound() == apexValue) {
                    m_height.set(heightAt(1.0), heightAt(0.0));
                }
            }
        }
    }

    return *this;
}

double GeEllipCone::getPoleParam() const
{
    double cosSign = (m_cosHalf > 0.0) ? 1.0 : -1.0;
    return -(cosSign / m_sinHalf) * m_majorRadius / m_uParamScale;
}

bool GeEllipCone::project(const GePoint3d& pnt, GePoint3d& projPnt) const
{
    return project(pnt, projPnt, GeContext::gTol);
}

bool GeEllipCone::project(const GePoint3d& pnt, GePoint3d& projPnt, const GeTol& tol) const
{
    GePoint3d apexPoint = apex();
    if (pnt.isEqualTo(apexPoint)) {
        projPnt = pnt;
        return true;
    }

    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = minorAxisUnit();

    GeVector3d apexToPoint = pnt - apexPoint;
    GeVector3d apexToPointPerp = apexToPoint - axis * axis.dotProduct(apexToPoint);
    if (apexToPointPerp.length() <= tol.equalVector()) {
        return false;
    }
    apexToPointPerp.normalize();

    double cosine = apexToPointPerp.dotProduct(major);
    double sine = apexToPointPerp.dotProduct(minor);
    if (axis.dotProduct(apexToPoint) < 0.0) {
        cosine = -cosine;
        sine = -sine;
    }

    GeVector3d rulingDir = (m_baseCenter + major * (cosine * m_majorRadius) + minor * (sine * m_minorRadius)) - apexPoint;
    if (rulingDir.length() <= tol.equalVector()) {
        return false;
    }
    rulingDir.normalize();

    projPnt = apexPoint + rulingDir * rulingDir.dotProduct(apexToPoint);
    return true;
}

GeEllipCone& GeEllipCone::set(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& origin,
    double minorRadius,
    double majorRadius,
    const GeVector3d& axisOfSymmetry)
{
    double invAbsSin = 1.0 / std::fabs(sineAngle);
    GeInterval defaultHeight(0.0, invAbsSin);

    GeVector3d defaultMajor = ellipcone_default_major_axis(axisOfSymmetry);

    return set(cosineAngle, sineAngle, origin, minorRadius, majorRadius, axisOfSymmetry, defaultMajor, defaultHeight, -PI, PI);
}

GeEllipCone& GeEllipCone::set(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& baseOrigin,
    double minorRadius,
    double majorRadius,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& majorAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
{
    m_outerNormal = true;
    m_cosHalf = cosineAngle;
    m_sinHalf = sineAngle;

    setMinorRadius(minorRadius);
    setMajorRadius(majorRadius);
    setAngles(startAng, endAng);

    m_baseCenter = baseOrigin;
    GeVector3d major = majorAxis;
    if (major.length() <= GeContext::gTol.equalVector()) {
        major = GeVector3d::kXAxis;
    }
    major.normalize();
    m_majorAxis = major;

    GeVector3d axis = axisOfSymmetry;
    if (axis.length() <= GeContext::gTol.equalVector()) {
        axis = GeVector3d::kYAxis;
    }

    if (axis.isParallelTo(m_majorAxis, GeContext::gTol)) {
        GeVector3d fallback = (std::fabs(m_majorAxis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        axis = m_majorAxis.crossProduct(fallback);
        if (axis.length() <= GeContext::gTol.equalVector()) {
            axis = m_majorAxis.crossProduct(GeVector3d::kYAxis);
        }
    }
    else {
        GeVector3d tmp = m_majorAxis.crossProduct(axis);
        axis = tmp.crossProduct(m_majorAxis);
    }
    if (axis.length() <= GeContext::gTol.equalVector()) {
        axis = GeVector3d::kYAxis;
    }
    axis.normalize();
    m_axis = axis;

    setHeight(height);
    return *this;
}

GeEllipCone& GeEllipCone::operator = (const GeEllipCone& cone)
{
    if (this == &cone) {
        return *this;
    }

    m_cosHalf = cone.m_cosHalf;
    m_sinHalf = cone.m_sinHalf;
    m_minorRadius = cone.m_minorRadius;
    m_majorRadius = cone.m_majorRadius;
    m_baseCenter = cone.m_baseCenter;
    m_axis = cone.m_axis;
    m_majorAxis = cone.m_majorAxis;
    m_height = cone.m_height;
    m_startAng = cone.m_startAng;
    m_endAng = cone.m_endAng;
    m_uParamScale = cone.m_uParamScale;
    m_hasCustomUParamScale = cone.m_hasCustomUParamScale;
    m_outerNormal = cone.m_outerNormal;
    return *this;
}

bool GeEllipCone::intersectWith(
    const GeLinearEnt3d& linEnt,
    int& numInt,
    GePoint3d& p1,
    GePoint3d& p2,
    const GeTol& tol) const
{
    numInt = 0;
    p1 = GePoint3d::kOrigin;
    p2 = GePoint3d::kOrigin;

    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = minorAxisUnit();
    GePoint3d linePoint = linEnt.pointOnLine();
    GeVector3d lineDir = linEnt.direction();
    if (lineDir.length() <= tol.equalVector()) {
        return false;
    }

    GeVector3d delta = linePoint - m_baseCenter;
    double x0 = delta.dotProduct(major);
    double y0 = delta.dotProduct(minor);
    double u0 = delta.dotProduct(axis);
    double dx = lineDir.dotProduct(major);
    double dy = lineDir.dotProduct(minor);
    double du = lineDir.dotProduct(axis);

    double ratio = radiusRatio();
    if (ellipcone_abs(ratio) <= tol.equalPoint()) {
        ratio = (ratio >= 0.0) ? tol.equalPoint() : -tol.equalPoint();
    }
    double invRatio = 1.0 / ratio;

    double y0Scaled = y0 * invRatio;
    double dyScaled = dy * invRatio;

    double k = signedTanHalfAngle();
    double majorAtU0 = m_majorRadius + k * u0;
    double dMajor = k * du;

    double a = (dx * dx + dyScaled * dyScaled) - (dMajor * dMajor);
    double b = 2.0 * (x0 * dx + y0Scaled * dyScaled - majorAtU0 * dMajor);
    double c = (x0 * x0 + y0Scaled * y0Scaled) - (majorAtU0 * majorAtU0);

    GePoint3d hits[2];
    int hitCount = 0;

    if (std::fabs(a) <= tol.equalPoint()) {
        if (std::fabs(b) <= tol.equalPoint()) {
            return false;
        }

        double t = -c / b;
        GePoint3d point = linePoint + lineDir * t;
        if (ellipcone_linear_entity_contains_point(linEnt, point, tol) && ellipcone_surface_contains_point(*this, point, tol)) {
            ellipcone_append_unique_point(hits, hitCount, point, tol);
        }
    }
    else {
        double disc = b * b - 4.0 * a * c;
        if (disc < -tol.equalPoint()) {
            return false;
        }

        if (std::fabs(disc) <= tol.equalPoint()) {
            double t = -b / (2.0 * a);
            GePoint3d point = linePoint + lineDir * t;
            if (ellipcone_linear_entity_contains_point(linEnt, point, tol) && ellipcone_surface_contains_point(*this, point, tol)) {
                ellipcone_append_unique_point(hits, hitCount, point, tol);
            }
        }
        else {
            double sqrtDisc = std::sqrt(disc);
            double t1 = (-b - sqrtDisc) / (2.0 * a);
            double t2 = (-b + sqrtDisc) / (2.0 * a);

            GePoint3d point1 = linePoint + lineDir * t1;
            if (ellipcone_linear_entity_contains_point(linEnt, point1, tol) && ellipcone_surface_contains_point(*this, point1, tol)) {
                ellipcone_append_unique_point(hits, hitCount, point1, tol);
            }

            GePoint3d point2 = linePoint + lineDir * t2;
            if (ellipcone_linear_entity_contains_point(linEnt, point2, tol) && ellipcone_surface_contains_point(*this, point2, tol)) {
                ellipcone_append_unique_point(hits, hitCount, point2, tol);
            }
        }
    }

    numInt = hitCount;
    if (hitCount >= 1) {
        p1 = hits[0];
    }
    if (hitCount >= 2) {
        p2 = hits[1];
    }
    return hitCount > 0;
}

GePoint2d GeEllipCone::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = axis.crossProduct(major);
    if (minor.length() <= GeContext::gTol.equalVector()) {
        minor = minorAxisUnit();
    }

    GeVector3d delta = pnt - m_baseCenter;
    double localX = delta.dotProduct(major);
    double localY = delta.dotProduct(minor) * m_majorRadius / m_minorRadius;
    double localZ = delta.dotProduct(axis);

    double v = std::atan2(localY, localX);
    v = ellipcone_adjust_to_interval(v, m_startAng, m_endAng);

    double d = localX * localX + localY * localY;
    if (d < 0.0) {
        d = 0.0;
    }
    double uScaled = localZ * m_cosHalf - (m_majorRadius - std::sqrt(d)) * m_sinHalf;
    if (m_cosHalf < 0.0) {
        uScaled = -uScaled;
    }
    double u = uScaled / m_uParamScale;

    return GePoint2d(u, v);
}

GePoint3d GeEllipCone::closestPointTo(const GePoint3d& pnt) const
{
    return closestPointTo(pnt, GeContext::gTol);
}

GePoint3d GeEllipCone::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePointOnSurface result;
    getClosestPointTo(pnt, result, tol);
    return result.point();
}

void GeEllipCone::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const
{
    getClosestPointTo(pnt, result, GeContext::gTol);
}

void GeEllipCone::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = minorAxisUnit();

    GeVector3d delta = pnt - m_baseCenter;
    double localX = delta.dotProduct(major);
    double localY = delta.dotProduct(minor) * m_majorRadius / m_minorRadius;
    double v = std::atan2(localY, localX);

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);

    v = ellipcone_move_into_valid_interval(v, rangeV.lowerBound(), rangeV.upperBound());

    double u = rangeU.lowerBound();
    if (!ellipcone_interval_contains_with_tol(rangeV, v, tol)) {
        double blend1 = 0.0;
        double blend2 = 0.0;
        GePoint3d line1Start = evalPoint(GePoint2d(rangeU.lowerBound(), rangeV.lowerBound()));
        GePoint3d line1End = evalPoint(GePoint2d(rangeU.upperBound(), rangeV.lowerBound()));
        GePoint3d line2Start = evalPoint(GePoint2d(rangeU.lowerBound(), rangeV.upperBound()));
        GePoint3d line2End = evalPoint(GePoint2d(rangeU.upperBound(), rangeV.upperBound()));
        GePoint3d point1 = ellipcone_segment_closest_point(line1Start, line1End, pnt, blend1);
        GePoint3d point2 = ellipcone_segment_closest_point(line2Start, line2End, pnt, blend2);
        if (pnt.distanceTo(point1) < pnt.distanceTo(point2)) {
            v = rangeV.lowerBound();
            u = rangeU.lowerBound() * (1.0 - blend1) + rangeU.upperBound() * blend1;
        }
        else {
            v = rangeV.upperBound();
            u = rangeU.lowerBound() * (1.0 - blend2) + rangeU.upperBound() * blend2;
        }
    }
    else {
        double blend = 0.0;
        GePoint3d lineStart = evalPoint(GePoint2d(rangeU.lowerBound(), v));
        GePoint3d lineEnd = evalPoint(GePoint2d(rangeU.upperBound(), v));
        ellipcone_segment_closest_point(lineStart, lineEnd, pnt, blend);
        u = rangeU.lowerBound() * (1.0 - blend) + rangeU.upperBound() * blend;
    }

    result.setSurface(*this);
    result.setParameter(GePoint2d(u, v));
}

GePoint3d GeEllipCone::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = minorAxisUnit();

    double sa = m_sinHalf;
    double ca = m_cosHalf;
    if (ca < 0.0) {
        sa = -sa;
        ca = -ca;
    }

    double u = param.x;
    double v = param.y;
    double ratio = (std::fabs(m_majorRadius) > GeContext::gTol.equalPoint()) ? (m_minorRadius / m_majorRadius) : 1.0;
    double radiusMajor = m_majorRadius + getUParamScale() * u * sa;
    double radiusMinor = radiusMajor * ratio;
    GePoint3d center = m_baseCenter + axis * (u * getUParamScale() * ca);
    if (std::fabs(radiusMajor) <= GeContext::gTol.equalPoint()) {
        return center;
    }
    return center + major * (radiusMajor * std::cos(v)) + minor * (radiusMinor * std::sin(v));
}

void GeEllipCone::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    double convertScale = m_uParamScale * std::fabs(m_cosHalf);
    if (m_height.isBounded()) {
        double lower = m_height.lowerBound() / convertScale;
        double upper = m_height.upperBound() / convertScale;
        if (lower <= upper) {
            rangeU.set(lower, upper);
        }
        else {
            rangeU.set(upper, lower);
        }
    }
    else {
        rangeU = m_height;
    }
    rangeV.set(m_startAng, m_endAng);
}

GeVector3d GeEllipCone::axisUnit() const
{
    if (m_axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kYAxis;
    }
    GeVector3d axis = m_axis;
    axis.normalize();
    return axis;
}

GeVector3d GeEllipCone::majorAxisUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = m_majorAxis;
    if (major.length() <= GeContext::gTol.equalVector() || major.isParallelTo(axis, GeContext::gTol)) {
        GeVector3d fallback = (ellipcone_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        major = axis.crossProduct(fallback);
    }
    major -= axis * major.dotProduct(axis);
    if (major.length() <= GeContext::gTol.equalVector()) {
        major = GeVector3d::kXAxis;
    }
    major.normalize();
    return major;
}

GeVector3d GeEllipCone::minorAxisUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = axis.crossProduct(major);
    if (minor.length() <= GeContext::gTol.equalVector()) {
        GeVector3d fallback = (ellipcone_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        minor = axis.crossProduct(fallback);
    }
    minor.normalize();
    return minor;
}

double GeEllipCone::signedTanHalfAngle() const
{
    if (ellipcone_abs(m_cosHalf) <= GeContext::gTol.equalPoint()) {
        return (m_sinHalf >= 0.0) ? 1.0e12 : -1.0e12;
    }
    return m_sinHalf / m_cosHalf;
}

double GeEllipCone::majorRadiusAt(double u) const
{
    return m_majorRadius + u * signedTanHalfAngle();
}

double GeEllipCone::minorRadiusAt(double u) const
{
    return m_minorRadius + u * signedTanHalfAngle() * radiusRatio();
}
