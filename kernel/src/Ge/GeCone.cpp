#include "GeCone.h"

#include "GeInterval.h"
#include "GeLinearEnt3d.h"
#include "GePlane.h"
#include "GePointOnSurface.h"
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

static double cone_move_into_valid_interval(double angle, double start, double end)
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

static bool cone_interval_contains_with_tol(const GeInterval& range, double value, const GeTol& tol)
{
    if (range.isBoundedBelow() && value < range.lowerBound() - tol.equalPoint()) {
        return false;
    }
    if (range.isBoundedAbove() && value > range.upperBound() + tol.equalPoint()) {
        return false;
    }
    return true;
}

static bool cone_linear_entity_contains_point(const GeLinearEnt3d& linEnt, const GePoint3d& point, const GeTol& tol)
{
    if (linEnt.isOn(point, tol)) {
        double param = linEnt.paramOf(point, tol);
        GeInterval range;
        linEnt.getInterval(range);
        return cone_interval_contains_with_tol(range, param, tol);
    }
    return false;
}

static bool cone_surface_contains_point(const GeCone& cone, const GePoint3d& point, const GeTol& tol)
{
    return cone.isOn(point, tol);
}

static void cone_append_unique_point(GePoint3d* points, int& count, const GePoint3d& point, const GeTol& tol)
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

static double cone_default_u_param_scale(double baseRadius)
{
    return cone_abs(baseRadius) > GeContext::gTol.equalPoint() ? baseRadius : 1.0;
}

static double cone_effective_u_param_scale(double scale, double baseRadius)
{
    return cone_abs(scale) > GeContext::gTol.equalPoint() ? scale : cone_default_u_param_scale(baseRadius);
}

static GeVector3d cone_default_ref_axis(const GeVector3d& axisOfSymmetry)
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

    GeVector3d ref = fallback.crossProduct(axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis.crossProduct(axis);
    }
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis;
    }
    ref.normalize();
    return ref;
}

static GePoint3d cone_segment_closest_point(const GePoint3d& start, const GePoint3d& end, const GePoint3d& point, double& blend)
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

static bool cone_is_uni_scaled_ortho(const GeMatrix3d& xfm, double& scaleFactor, const GeTol& tol = GeContext::gTol)
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

}

GeCone::GeCone()
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
}

GeCone::GeCone(double cosineAngle, double sineAngle, const GePoint3d& baseOrigin, double baseRadius, const GeVector3d& axisOfSymmetry)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
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
    , m_sinHalf(-std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    set(cosineAngle, sineAngle, baseOrigin, baseRadius, axisOfSymmetry, refAxis, height, startAng, endAng);
}

GeCone::GeCone(const GeCone& cone)
    : m_cosHalf(std::sqrt(0.5))
    , m_sinHalf(-std::sqrt(0.5))
    , m_baseCenter(GePoint3d::kOrigin)
    , m_baseRadius(2.0)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
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
    if (cone_abs(ba1 - rvBa1) > tol.equalPoint() || cone_abs(ba2 - rvBa2) > tol.equalPoint()) {
        return false;
    }
    if (cone_abs(bh1 - rvBh1) > tol.equalPoint() || cone_abs(bh2 - rvBh2) > tol.equalPoint()) {
        return false;
    }
    return cone_abs(getUParamScale() - entity.getUParamScale()) <= tol.equalPoint();
}

GeCone& GeCone::transformBy(const GeMatrix3d& xfm)
{
    double scaleFactor = 1.0;
    if (!cone_is_uni_scaled_ortho(xfm, scaleFactor)) {
        return *this;
    }

    GeVector3d axisRef = axisUnit();
    GeVector3d refAxisRef = refUnit();

    GeVector3d axisTransformed = axisRef;
    GeVector3d refTransformed = refAxisRef;
    axisTransformed.transformBy(xfm);
    refTransformed.transformBy(xfm);
    if (axisTransformed.length() <= GeContext::gTol.equalVector() || refTransformed.length() <= GeContext::gTol.equalVector()) {
        return *this;
    }

    axisTransformed.normalize();
    refTransformed.normalize();

    if (std::fabs(axisTransformed.dotProduct(refTransformed)) > GeContext::gTol.equalVector()) {
        return *this;
    }

    m_baseCenter.transformBy(xfm);

    m_baseRadius *= scaleFactor;
    m_uParamScale *= scaleFactor;
    if (m_height.isBounded()) {
        m_height.set(m_height.lowerBound() * scaleFactor, m_height.upperBound() * scaleFactor);
    }

    m_axis = axisTransformed;
    m_ref = refTransformed;

    if (xfm.det() < 0.0) {
        m_uParamScale = -m_uParamScale;
        if (m_height.isBounded()) {
            double lower = m_height.lowerBound();
            double upper = m_height.upperBound();
            m_height.set(-lower, -upper);
        }
        m_axis.negate();
        m_sinHalf = -m_sinHalf;
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
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    return transformBy(mirrorXfm);
}

GeCone& GeCone::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCone& GeCone::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeMatrix3d scaleXfm;
    scaleXfm.setToScaling(scaleFactor, wrtPoint);
    return transformBy(scaleXfm);
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
    return std::fabs(std::asin(m_sinHalf));
}

void GeCone::getHalfAngle(double& cosineAngle, double& sineAngle) const
{
    cosineAngle = std::fabs(m_cosHalf);
    sineAngle = std::fabs(m_sinHalf);
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
    return u * getUParamScale() * std::fabs(m_cosHalf);
}

double GeCone::getUParamScale() const
{
    return m_uParamScale;
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
    return m_baseCenter - axisUnit() * (m_baseRadius * m_cosHalf / m_sinHalf);
}

bool GeCone::isClosed(const GeTol& tol) const
{
    return std::fabs((m_endAng - m_startAng) - (PI * 2.0)) <= tol.equalPoint();
}

bool GeCone::isOuterNormal() const
{
    if (m_outerNormal) {
        return m_cosHalf < 0.0;
    }
    return m_cosHalf >= 0.0;
}

bool GeCone::isNormalReversed() const
{
    return m_outerNormal;
}

bool GeCone::isLeftHanded() const
{
    return isOuterNormal() != (m_uParamScale < 0.0);
}

GeSurface& GeCone::reverseNormal()
{
    m_outerNormal = !m_outerNormal;
    return *this;
}

void GeCone::setUParamScale(double uScale)
{
    if (cone_abs(uScale) <= GeContext::gTol.equalPoint()) {
        uScale = m_baseRadius;
    }

    if (uScale * m_uParamScale < 0.0) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }

    m_uParamScale = uScale;
    m_hasCustomUParamScale = true;
}

GeCone& GeCone::setBaseRadius(double baseRadius)
{
    m_baseRadius = std::fabs(baseRadius);
    m_uParamScale = m_baseRadius;
    m_hasCustomUParamScale = false;
    if (m_height.isBounded() && m_height.upperBound() < m_height.lowerBound()) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }
    return *this;
}

GeCone& GeCone::setBaseCenter(const GePoint3d& baseOrigin)
{
    m_baseCenter = baseOrigin;
    return *this;
}

bool GeCone::project(const GePoint3d& pnt, GePoint3d& projPnt) const
{
    return project(pnt, projPnt, GeContext::gTol);
}

bool GeCone::project(const GePoint3d& pnt, GePoint3d& projPnt, const GeTol& tol) const
{
    GePoint3d apexPoint = apex();
    if (pnt.isEqualTo(apexPoint)) {
        projPnt = pnt;
        return true;
    }

    GePoint2d param = paramOf(pnt, tol);
    GePoint3d pointOnCone = evalPoint(param);

    GeVector3d surfaceRay = pointOnCone - apexPoint;
    GeVector3d pointRay = pnt - apexPoint;

    if (surfaceRay.length() <= tol.equalVector()) {
        GeVector3d radial = (pnt - m_baseCenter) / m_baseRadius;
        double v = 0.0;
        if (radial.length() > tol.equalVector()) {
            v = refUnit().angleToCCW(radial, axisUnit());
            if (v > PI) {
                v -= PI * 2.0;
            }
        }

        param.x = 0.0;
        param.y = v;
        surfaceRay = evalPoint(param) - apexPoint;
    }

    if (surfaceRay.length() <= tol.equalVector()) {
        return false;
    }

    surfaceRay.normalize();
    double projectedLength = surfaceRay.dotProduct(pointRay);
    if (projectedLength < 0.0) {
        return false;
    }

    projPnt = apexPoint + surfaceRay * projectedLength;
    return true;
}

GeCone& GeCone::setAngles(double startAng, double endAng)
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

GeCone& GeCone::setHeight(const GeInterval& height)
{
    double apexValue = -m_baseRadius * m_cosHalf / m_sinHalf;

    if (height == GeInterval()) {
        if (apexValue < 0.0) {
            m_height.set(apexValue, 0.0);
        }
        else {
            m_height.set(0.0, apexValue);
        }
    }
    else if (height.isBounded()) {
        if ((m_uParamScale > 0.0) ^ (height.lowerBound() < height.upperBound())) {
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

GeCone& GeCone::set(
    double cosineAngle,
    double sineAngle,
    const GePoint3d& baseOrigin,
    double baseRadius,
    const GeVector3d& axisOfSymmetry)
{
    GeInterval defaultHeight;
    GeVector3d defaultRefAxis = cone_default_ref_axis(axisOfSymmetry);
    return set(cosineAngle, sineAngle, baseOrigin, baseRadius, axisOfSymmetry, defaultRefAxis, defaultHeight, -PI, PI);
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
    m_outerNormal = false;
    m_cosHalf = cosineAngle;
    m_sinHalf = sineAngle;

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
    m_uParamScale = cone.m_uParamScale;
    m_hasCustomUParamScale = cone.m_hasCustomUParamScale;
    m_outerNormal = cone.m_outerNormal;
    return *this;
}

bool GeCone::intersectWith(
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
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();
    GePoint3d linePoint = linEnt.pointOnLine();
    GeVector3d lineDir = linEnt.direction();
    if (lineDir.length() <= tol.equalVector()) {
        return false;
    }

    GeVector3d delta = linePoint - m_baseCenter;
    double x0 = delta.dotProduct(ref);
    double y0 = delta.dotProduct(ortho);
    double u0 = delta.dotProduct(axis);
    double dx = lineDir.dotProduct(ref);
    double dy = lineDir.dotProduct(ortho);
    double du = lineDir.dotProduct(axis);

    double k = signedTanHalfAngle();
    double rAtU0 = m_baseRadius + k * u0;

    double a = (dx * dx + dy * dy) - (k * du) * (k * du);
    double b = 2.0 * (x0 * dx + y0 * dy) - 2.0 * rAtU0 * k * du;
    double c = (x0 * x0 + y0 * y0) - (rAtU0 * rAtU0);

    GePoint3d hits[2];
    int hitCount = 0;

    if (std::fabs(a) <= tol.equalPoint()) {
        if (std::fabs(b) <= tol.equalPoint()) {
            return false;
        }

        double t = -c / b;
        GePoint3d point = linePoint + lineDir * t;
        if (cone_linear_entity_contains_point(linEnt, point, tol) && cone_surface_contains_point(*this, point, tol)) {
            cone_append_unique_point(hits, hitCount, point, tol);
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
            if (cone_linear_entity_contains_point(linEnt, point, tol) && cone_surface_contains_point(*this, point, tol)) {
                cone_append_unique_point(hits, hitCount, point, tol);
            }
        }
        else {
            double sqrtDisc = std::sqrt(disc);
            double t1 = (-b - sqrtDisc) / (2.0 * a);
            double t2 = (-b + sqrtDisc) / (2.0 * a);

            GePoint3d point1 = linePoint + lineDir * t1;
            if (cone_linear_entity_contains_point(linEnt, point1, tol) && cone_surface_contains_point(*this, point1, tol)) {
                cone_append_unique_point(hits, hitCount, point1, tol);
            }

            GePoint3d point2 = linePoint + lineDir * t2;
            if (cone_linear_entity_contains_point(linEnt, point2, tol) && cone_surface_contains_point(*this, point2, tol)) {
                cone_append_unique_point(hits, hitCount, point2, tol);
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

GePoint2d GeCone::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GeVector3d delta = pnt - m_baseCenter;
    double localX = delta.dotProduct(ref);
    double localY = delta.dotProduct(ortho);
    double localZ = delta.dotProduct(axis);

    double d = localX * localX + localY * localY;
    if (d < 0.0) {
        d = 0.0;
    }

    double v = std::atan2(localY, localX);

    v = cone_move_into_valid_interval(v, m_startAng, m_endAng);

    double uScaled = localZ * m_cosHalf - (m_baseRadius - std::sqrt(d)) * m_sinHalf;
    if (m_cosHalf < 0.0) {
        uScaled = -uScaled;
    }
    double u = uScaled / getUParamScale();

    return GePoint2d(u, v);
}

GePoint3d GeCone::closestPointTo(const GePoint3d& pnt) const
{
    return closestPointTo(pnt, GeContext::gTol);
}

GePoint3d GeCone::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePointOnSurface result;
    getClosestPointTo(pnt, result, tol);
    return result.point();
}

void GeCone::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const
{
    getClosestPointTo(pnt, result, GeContext::gTol);
}

void GeCone::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double h = axis.dotProduct(pnt - m_baseCenter);
    GeVector3d planar = (pnt - axis * h) - m_baseCenter;

    double localX = planar.dotProduct(ref);
    double localY = planar.dotProduct(ortho);
    double v = std::atan2(localY, localX);
    if (v > PI) {
        v -= PI * 2.0;
    }

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);

    v = cone_move_into_valid_interval(v, rangeV.lowerBound(), rangeV.upperBound());

    double u = rangeU.lowerBound();
    if (!rangeV.contains(v)) {
        double blend1 = 0.0;
        double blend2 = 0.0;
        GePoint3d line1Start = evalPoint(GePoint2d(rangeU.lowerBound(), rangeV.lowerBound()));
        GePoint3d line1End = evalPoint(GePoint2d(rangeU.upperBound(), rangeV.lowerBound()));
        GePoint3d line2Start = evalPoint(GePoint2d(rangeU.lowerBound(), rangeV.upperBound()));
        GePoint3d line2End = evalPoint(GePoint2d(rangeU.upperBound(), rangeV.upperBound()));
        GePoint3d point1 = cone_segment_closest_point(line1Start, line1End, pnt, blend1);
        GePoint3d point2 = cone_segment_closest_point(line2Start, line2End, pnt, blend2);
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
        cone_segment_closest_point(lineStart, lineEnd, pnt, blend);
        u = rangeU.lowerBound() * (1.0 - blend) + rangeU.upperBound() * blend;
    }

    GePoint2d param(u, v);
    result.setSurface(*this);
    result.setParameter(param);
}

GePoint3d GeCone::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double sa = m_sinHalf;
    double ca = m_cosHalf;
    if (ca < 0.0) {
        sa = -sa;
        ca = -ca;
    }

    double u = param.x;
    double v = cone_adjust_to_interval(param.y, m_startAng, m_endAng);
    double uHeight = u * getUParamScale() * ca;
    double r = m_baseRadius + u * getUParamScale() * sa;
    GeVector3d radialDir = ref * cos(v) + ortho * sin(v);
    return m_baseCenter + axis * uHeight + radialDir * r;
}

void GeCone::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    double convertScale = getUParamScale() * std::fabs(m_cosHalf);
    if (std::fabs(convertScale) > GeContext::gTol.equalPoint()) {
        rangeU.set(m_height.lowerBound() / convertScale, m_height.upperBound() / convertScale);
    }
    else {
        rangeU = GeInterval();
    }
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
