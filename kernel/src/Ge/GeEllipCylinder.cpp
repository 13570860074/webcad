#include "GeEllipCylinder.h"

#include "GeLinearEnt3d.h"
#include "GePlane.h"
#include <cmath>

namespace {
static double ellipcyl_abs(double value)
{
    return value < 0.0 ? -value : value;
}

static double ellipcyl_clamp(double value, double minValue, double maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static double ellipcyl_adjust_to_interval(double angle, double start, double end)
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

static bool ellipcyl_is_uni_scaled_ortho(const GeMatrix3d& xfm, double& scaleFactor, const GeTol& tol = GeContext::gTol)
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

static bool ellipcyl_interval_contains_with_tol(const GeInterval& range, double value, const GeTol& tol)
{
    if (range.isBoundedBelow() && value < range.lowerBound() - tol.equalPoint()) {
        return false;
    }
    if (range.isBoundedAbove() && value > range.upperBound() + tol.equalPoint()) {
        return false;
    }
    return true;
}

static bool ellipcyl_linear_entity_contains_point(const GeLinearEnt3d& linEnt, const GePoint3d& point, const GeTol& tol)
{
    if (linEnt.isOn(point, tol)) {
        double param = linEnt.paramOf(point, tol);
        GeInterval range;
        linEnt.getInterval(range);
        return ellipcyl_interval_contains_with_tol(range, param, tol);
    }
    return false;
}

static bool ellipcyl_surface_contains_point(const GeEllipCylinder& cylinder, const GePoint3d& point, const GeTol& tol)
{
    return cylinder.isOn(point, tol);
}

static void ellipcyl_append_unique_point(GePoint3d* points, int& count, const GePoint3d& point, const GeTol& tol)
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

static GeVector3d ellipcyl_default_major_axis(const GeVector3d& axisOfSymmetry)
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

static GeVector3d ellipcyl_perp_axis(const GeVector3d& vec)
{
    GeVector3d base = vec;
    if (base.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kXAxis;
    }
    base.normalize();

    GeVector3d fallback = GeVector3d::kZAxis;
    if (base.isParallelTo(fallback, GeContext::gTol)) {
        fallback = GeVector3d::kYAxis;
    }
    GeVector3d perp = fallback.crossProduct(base);
    if (perp.length() <= GeContext::gTol.equalVector()) {
        perp = GeVector3d::kXAxis.crossProduct(base);
    }
    if (perp.length() <= GeContext::gTol.equalVector()) {
        perp = GeVector3d::kXAxis;
    }
    perp.normalize();
    return perp;
}

static void ellipcyl_scale_interval(GeInterval& interval, double scale)
{
    if (interval.isBounded()) {
        interval.set(interval.lowerBound() * scale, interval.upperBound() * scale);
        return;
    }

    if (interval.isBoundedBelow() && !interval.isBoundedAbove()) {
        double bound = interval.lowerBound() * scale;
        interval.set(scale >= 0.0, bound);
        return;
    }

    if (!interval.isBoundedBelow() && interval.isBoundedAbove()) {
        double bound = interval.upperBound() * scale;
        interval.set(scale < 0.0, bound);
        return;
    }

    interval.set();
}

}

GeEllipCylinder::GeEllipCylinder()
    : m_minorRadius(2.0)
    , m_majorRadius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(GeInterval())
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
}

GeEllipCylinder::GeEllipCylinder(double minorRadius, double majorRadius, const GePoint3d& origin, const GeVector3d& axisOfSymmetry)
    : m_minorRadius(2.0)
    , m_majorRadius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(GeInterval())
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    set(minorRadius, majorRadius, origin, axisOfSymmetry);
}

GeEllipCylinder::GeEllipCylinder(
    double minorRadius,
    double majorRadius,
    const GePoint3d& origin,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& majorAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
    : m_minorRadius(2.0)
    , m_majorRadius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(GeInterval())
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    set(minorRadius, majorRadius, origin, axisOfSymmetry, majorAxis, height, startAng, endAng);
}

GeEllipCylinder::GeEllipCylinder(const GeEllipCylinder& cylinder)
    : m_minorRadius(2.0)
    , m_majorRadius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_majorAxis(GeVector3d::kXAxis)
    , m_height(GeInterval())
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    *this = cylinder;
}

bool GeEllipCylinder::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == type();
}

Ge::EntityId GeEllipCylinder::type() const
{
    return Ge::kEllipCylinder;
}

GeEllipCylinder* GeEllipCylinder::copy() const
{
    return new GeEllipCylinder(*this);
}

bool GeEllipCylinder::operator == (const GeEllipCylinder& entity) const
{
    return isEqualTo(entity);
}

bool GeEllipCylinder::operator != (const GeEllipCylinder& entity) const
{
    return !isEqualTo(entity);
}

bool GeEllipCylinder::isEqualTo(const GeEllipCylinder& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeEllipCylinder::isEqualTo(const GeEllipCylinder& entity, const GeTol& tol) const
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

    if (ellipcyl_abs(m_minorRadius - entity.m_minorRadius) > tol.equalPoint()) {
        return false;
    }
    if (ellipcyl_abs(m_majorRadius - entity.m_majorRadius) > tol.equalPoint()) {
        return false;
    }
    if (!m_origin.isEqualTo(entity.m_origin, tol)) {
        return false;
    }
    if (!axisUnit().isEqualTo(entity.axisUnit(), tol)) {
        return false;
    }
    if (!majorAxisUnit().isEqualTo(entity.majorAxisUnit(), tol)) {
        return false;
    }
    if (ellipcyl_abs(ba1 - rvBa1) > tol.equalPoint() || ellipcyl_abs(ba2 - rvBa2) > tol.equalPoint()) {
        return false;
    }
    if (ellipcyl_abs(bh1 - rvBh1) > tol.equalPoint() || ellipcyl_abs(bh2 - rvBh2) > tol.equalPoint()) {
        return false;
    }
    return ellipcyl_abs(m_uParamScale - entity.m_uParamScale) <= tol.equalPoint();
}

GeEllipCylinder& GeEllipCylinder::transformBy(const GeMatrix3d& xfm)
{
    double scaleFactor = 1.0;
    if (!ellipcyl_is_uni_scaled_ortho(xfm, scaleFactor)) {
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

    m_origin.transformBy(xfm);
    m_majorRadius *= scaleFactor;
    m_minorRadius *= scaleFactor;
    m_uParamScale *= scaleFactor;
    ellipcyl_scale_interval(m_height, scaleFactor);

    m_axis = axisTransformed;
    m_majorAxis = majorTransformed;

    if (xfm.det() < 0.0) {
        m_uParamScale = -m_uParamScale;
        ellipcyl_scale_interval(m_height, -1.0);
        m_axis.negate();
    }

    return *this;
}

GeEllipCylinder& GeEllipCylinder::translateBy(const GeVector3d& translateVec)
{
    m_origin += translateVec;
    return *this;
}

GeEllipCylinder& GeEllipCylinder::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeEllipCylinder& GeEllipCylinder::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_origin.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_majorAxis.rotateBy(angle, vec);
    return *this;
}

GeEllipCylinder& GeEllipCylinder::mirror(const GePlane& plane)
{
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    return transformBy(mirrorXfm);
}

GeEllipCylinder& GeEllipCylinder::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeEllipCylinder& GeEllipCylinder::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeMatrix3d scaleXfm;
    scaleXfm.setToScaling(scaleFactor, wrtPoint);
    return transformBy(scaleXfm);
}

bool GeEllipCylinder::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeEllipCylinder::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return GeSurface::isOn(pnt, tol);
}

double GeEllipCylinder::radiusRatio() const
{
    if (m_majorRadius <= GeContext::gTol.equalPoint()) {
        return 0.0;
    }
    return m_minorRadius / m_majorRadius;
}

double GeEllipCylinder::minorRadius() const
{
    return m_minorRadius;
}

double GeEllipCylinder::majorRadius() const
{
    return m_majorRadius;
}

GePoint3d GeEllipCylinder::origin() const
{
    return m_origin;
}

void GeEllipCylinder::getAngles(double& startAng, double& endAng) const
{
    startAng = m_startAng;
    endAng = m_endAng;
}

void GeEllipCylinder::getHeight(GeInterval& height) const
{
    height = m_height;
}

double GeEllipCylinder::heightAt(double u) const
{
    return u * getUParamScale();
}

double GeEllipCylinder::getUParamScale() const
{
    return m_uParamScale;
}

GeVector3d GeEllipCylinder::axisOfSymmetry() const
{
    return axisUnit();
}

GeVector3d GeEllipCylinder::majorAxis() const
{
    return majorAxisUnit() * m_majorRadius;
}

GeVector3d GeEllipCylinder::minorAxis() const
{
    return minorAxisUnit() * m_minorRadius;
}

bool GeEllipCylinder::isOuterNormal() const
{
    if (m_outerNormal) {
        return m_majorRadius < 0.0;
    }
    return m_majorRadius >= 0.0;
}

bool GeEllipCylinder::isNormalReversed() const
{
    return m_outerNormal;
}

bool GeEllipCylinder::isLeftHanded() const
{
    return isOuterNormal() != (m_uParamScale < 0.0);
}

bool GeEllipCylinder::isClosed(const GeTol& tol) const
{
    return std::fabs((m_endAng - m_startAng) - (PI * 2.0)) <= tol.equalPoint();
}

GeSurface& GeEllipCylinder::reverseNormal()
{
    m_outerNormal = !m_outerNormal;
    return *this;
}

void GeEllipCylinder::setIsOuterNormal(bool isOuterNormal)
{
    m_majorRadius = std::fabs(m_majorRadius);
    if (!isOuterNormal) {
        m_majorRadius = -m_majorRadius;
    }
}

void GeEllipCylinder::setUParamScale(double uScale)
{
    if (uScale == 0.0) {
        uScale = std::fabs(m_majorRadius);
    }
    if (uScale * m_uParamScale < 0.0 && m_height.isBounded()) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }

    m_uParamScale = uScale;
    m_hasCustomUParamScale = true;
}

GeEllipCylinder& GeEllipCylinder::setMinorRadius(double minorRadius)
{
    m_minorRadius = std::fabs(minorRadius);
    return *this;
}

GeEllipCylinder& GeEllipCylinder::setMajorRadius(double majorRadius)
{
    double r = std::fabs(majorRadius);
    m_majorRadius = (m_majorRadius < 0.0) ? -r : r;
    m_uParamScale = r;
    m_hasCustomUParamScale = false;
    if (m_height.isBounded() && m_height.upperBound() < m_height.lowerBound()) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }
    return *this;
}

GeEllipCylinder& GeEllipCylinder::setAngles(double startAng, double endAng)
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

GeEllipCylinder& GeEllipCylinder::setHeight(const GeInterval& height)
{
    if (height.isBounded() && ((m_uParamScale > 0.0) ^ (height.lowerBound() < height.upperBound()))) {
        m_height.set(height.upperBound(), height.lowerBound());
    }
    else {
        m_height = height;
    }
    return *this;
}

GeEllipCylinder& GeEllipCylinder::set(double minorRadius, double majorRadius, const GePoint3d& origin, const GeVector3d& axisOfSymmetry)
{
    GeInterval defaultHeight;
    GeVector3d defaultMajorAxis = ellipcyl_default_major_axis(axisOfSymmetry);
    return set(minorRadius, majorRadius, origin, axisOfSymmetry, defaultMajorAxis, defaultHeight, -PI, PI);
}

GeEllipCylinder& GeEllipCylinder::set(
    double minorRadius,
    double majorRadius,
    const GePoint3d& origin,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& majorAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
{
    m_outerNormal = false;
    setMajorRadius(majorRadius);
    setMinorRadius(minorRadius);
    m_origin = origin;

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
        axis = ellipcyl_perp_axis(m_majorAxis);
    }
    else {
        GeVector3d tmp = m_majorAxis.crossProduct(axis);
        axis = tmp.crossProduct(m_majorAxis);
    }
    if (axis.length() <= GeContext::gTol.equalVector()) {
        axis = ellipcyl_perp_axis(m_majorAxis);
    }
    axis.normalize();
    m_axis = axis;

    setHeight(height);
    setAngles(startAng, endAng);
    return *this;
}

bool GeEllipCylinder::project(const GePoint3d& pnt, GePoint3d& projPnt) const
{
    return project(pnt, projPnt, GeContext::gTol);
}

bool GeEllipCylinder::project(const GePoint3d& pnt, GePoint3d& projPnt, const GeTol& tol) const
{
    GePoint2d paramUV = paramOf(pnt, tol);

    GeInterval intrvlU;
    GeInterval intrvlV;
    getEnvelope(intrvlU, intrvlV);
    if (!intrvlU.contains(paramUV.x)) {
        return false;
    }

    projPnt = evalPoint(paramUV);
    return true;
}

GeEllipCylinder& GeEllipCylinder::operator = (const GeEllipCylinder& cylinder)
{
    if (this == &cylinder) {
        return *this;
    }

    m_minorRadius = cylinder.m_minorRadius;
    m_majorRadius = cylinder.m_majorRadius;
    m_origin = cylinder.m_origin;
    m_axis = cylinder.m_axis;
    m_majorAxis = cylinder.m_majorAxis;
    m_height = cylinder.m_height;
    m_startAng = cylinder.m_startAng;
    m_endAng = cylinder.m_endAng;
    m_uParamScale = cylinder.m_uParamScale;
    m_hasCustomUParamScale = cylinder.m_hasCustomUParamScale;
    m_outerNormal = cylinder.m_outerNormal;
    return *this;
}

bool GeEllipCylinder::intersectWith(
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

    GeVector3d delta = linePoint - m_origin;
    double x0 = delta.dotProduct(major);
    double y0 = delta.dotProduct(minor);
    double dx = lineDir.dotProduct(major);
    double dy = lineDir.dotProduct(minor);

    double majorRadius = (m_majorRadius > tol.equalPoint()) ? m_majorRadius : tol.equalPoint();
    double minorRadius = (m_minorRadius > tol.equalPoint()) ? m_minorRadius : tol.equalPoint();
    double invMajor2 = 1.0 / (majorRadius * majorRadius);
    double invMinor2 = 1.0 / (minorRadius * minorRadius);

    double a = dx * dx * invMajor2 + dy * dy * invMinor2;
    double b = 2.0 * (x0 * dx * invMajor2 + y0 * dy * invMinor2);
    double c = x0 * x0 * invMajor2 + y0 * y0 * invMinor2 - 1.0;

    GePoint3d hits[2];
    int hitCount = 0;

    if (std::fabs(a) <= tol.equalPoint()) {
        return false;
    }

    double disc = b * b - 4.0 * a * c;
    if (disc < -tol.equalPoint()) {
        return false;
    }

    if (std::fabs(disc) <= tol.equalPoint()) {
        double t = -b / (2.0 * a);
        GePoint3d point = linePoint + lineDir * t;
        if (ellipcyl_linear_entity_contains_point(linEnt, point, tol) && ellipcyl_surface_contains_point(*this, point, tol)) {
            ellipcyl_append_unique_point(hits, hitCount, point, tol);
        }
    }
    else {
        double sqrtDisc = std::sqrt(disc);
        double t1 = (-b - sqrtDisc) / (2.0 * a);
        double t2 = (-b + sqrtDisc) / (2.0 * a);

        GePoint3d point1 = linePoint + lineDir * t1;
        if (ellipcyl_linear_entity_contains_point(linEnt, point1, tol) && ellipcyl_surface_contains_point(*this, point1, tol)) {
            ellipcyl_append_unique_point(hits, hitCount, point1, tol);
        }

        GePoint3d point2 = linePoint + lineDir * t2;
        if (ellipcyl_linear_entity_contains_point(linEnt, point2, tol) && ellipcyl_surface_contains_point(*this, point2, tol)) {
            ellipcyl_append_unique_point(hits, hitCount, point2, tol);
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

GePoint2d GeEllipCylinder::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    (void)tol;
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = axis.crossProduct(major);

    GeVector3d delta = pnt - m_origin;
    double localX = delta.dotProduct(major) / std::fabs(m_majorRadius);
    double localY = delta.dotProduct(minor) / m_minorRadius;
    double localZ = delta.dotProduct(axis);

    double v = std::atan2(localY, localX);
    v = ellipcyl_adjust_to_interval(v, m_startAng, m_endAng);

    double u = localZ / getUParamScale();

    return GePoint2d(u, v);
}

GePoint3d GeEllipCylinder::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = minorAxisUnit();

    double u = param.x;
    double uHeight = heightAt(u);
    double v = param.y;
    return m_origin + axis * uHeight + major * (std::fabs(m_majorRadius) * std::cos(v)) + minor * (m_minorRadius * std::sin(v));
}

void GeEllipCylinder::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    double uScale = getUParamScale();
    if (m_height.isBounded()) {
        double lower = m_height.lowerBound() / uScale;
        double upper = m_height.upperBound() / uScale;
        rangeU.set(lower, upper);
    }
    else {
        rangeU = GeInterval();
    }
    rangeV.set(m_startAng, m_endAng);
}

GeVector3d GeEllipCylinder::axisUnit() const
{
    if (m_axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kYAxis;
    }
    GeVector3d axis = m_axis;
    axis.normalize();
    return axis;
}

GeVector3d GeEllipCylinder::majorAxisUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = m_majorAxis;
    if (major.length() <= GeContext::gTol.equalVector() || major.isParallelTo(axis, GeContext::gTol)) {
        GeVector3d fallback = (ellipcyl_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        major = axis.crossProduct(fallback);
    }
    major -= axis * major.dotProduct(axis);
    if (major.length() <= GeContext::gTol.equalVector()) {
        major = GeVector3d::kXAxis;
    }
    major.normalize();
    return major;
}

GeVector3d GeEllipCylinder::minorAxisUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d major = majorAxisUnit();
    GeVector3d minor = axis.crossProduct(major);
    if (minor.length() <= GeContext::gTol.equalVector()) {
        GeVector3d fallback = (ellipcyl_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        minor = axis.crossProduct(fallback);
    }
    minor.normalize();
    return minor;
}
