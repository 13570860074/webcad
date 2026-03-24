#include "GeCylinder.h"

#include "GeLinearEnt3d.h"
#include "GePlane.h"
#include <cmath>

namespace {
static double cyl_abs(double value)
{
    return value < 0.0 ? -value : value;
}

static double cyl_clamp(double value, double minValue, double maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static double cyl_adjust_to_interval(double angle, double start, double end)
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
        out = (cyl_abs(a - start) <= cyl_abs(b - start)) ? a : b;
    }
    if (out > end) {
        double a = out;
        double b = out - twoPi;
        out = (cyl_abs(a - end) <= cyl_abs(b - end)) ? a : b;
    }

    return cyl_clamp(out, start, end);
}

static bool cyl_interval_contains_with_tol(const GeInterval& range, double value, const GeTol& tol)
{
    if (range.isBoundedBelow() && value < range.lowerBound() - tol.equalPoint()) {
        return false;
    }
    if (range.isBoundedAbove() && value > range.upperBound() + tol.equalPoint()) {
        return false;
    }
    return true;
}

static bool cyl_linear_entity_contains_point(const GeLinearEnt3d& linEnt, const GePoint3d& point, const GeTol& tol)
{
    if (linEnt.isOn(point, tol)) {
        double param = linEnt.paramOf(point, tol);
        GeInterval range;
        linEnt.getInterval(range);
        return cyl_interval_contains_with_tol(range, param, tol);
    }
    return false;
}

static bool cyl_surface_contains_point(const GeCylinder& cylinder, const GePoint3d& point, const GeTol& tol)
{
    return cylinder.isOn(point, tol);
}

static void cyl_append_unique_point(GePoint3d* points, int& count, const GePoint3d& point, const GeTol& tol)
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

static double cyl_default_u_param_scale(double radius)
{
    return cyl_abs(radius) > GeContext::gTol.equalPoint() ? radius : 1.0;
}

static double cyl_effective_u_param_scale(double scale, double radius)
{
    return cyl_abs(scale) > GeContext::gTol.equalPoint() ? scale : cyl_default_u_param_scale(radius);
}

static GeVector3d cyl_default_ref_axis(const GeVector3d& axisOfSymmetry)
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

static bool cyl_is_uni_scaled_ortho(const GeMatrix3d& xfm, double& scaleFactor, const GeTol& tol = GeContext::gTol)
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

GeCylinder::GeCylinder()
    : m_radius(2.0)
    , m_origin(GePoint3d::kOrigin)
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

GeCylinder::GeCylinder(double radius, const GePoint3d& origin, const GeVector3d& axisOfSymmetry)
    : m_radius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    set(radius, origin, axisOfSymmetry);
}

GeCylinder::GeCylinder(
    double radius,
    const GePoint3d& origin,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& refAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
    : m_radius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    set(radius, origin, axisOfSymmetry, refAxis, height, startAng, endAng);
}

GeCylinder::GeCylinder(const GeCylinder& cylinder)
    : m_radius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_uParamScale(2.0)
    , m_hasCustomUParamScale(false)
    , m_outerNormal(false)
{
    *this = cylinder;
}

bool GeCylinder::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == type();
}

Ge::EntityId GeCylinder::type() const
{
    return Ge::kCylinder;
}

GeCylinder* GeCylinder::copy() const
{
    return new GeCylinder(*this);
}

bool GeCylinder::operator == (const GeCylinder& entity) const
{
    return isEqualTo(entity);
}

bool GeCylinder::operator != (const GeCylinder& entity) const
{
    return !isEqualTo(entity);
}

bool GeCylinder::isEqualTo(const GeCylinder& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeCylinder::isEqualTo(const GeCylinder& entity, const GeTol& tol) const
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

    if (cyl_abs(m_radius - entity.m_radius) > tol.equalPoint()) {
        return false;
    }
    if (!m_origin.isEqualTo(entity.m_origin, tol)) {
        return false;
    }
    if (!axisUnit().isEqualTo(entity.axisUnit(), tol)) {
        return false;
    }
    if (!refUnit().isEqualTo(entity.refUnit(), tol)) {
        return false;
    }
    if (cyl_abs(ba1 - rvBa1) > tol.equalPoint() || cyl_abs(ba2 - rvBa2) > tol.equalPoint()) {
        return false;
    }
    if (cyl_abs(bh1 - rvBh1) > tol.equalPoint() || cyl_abs(bh2 - rvBh2) > tol.equalPoint()) {
        return false;
    }
    return cyl_abs(getUParamScale() - entity.getUParamScale()) <= tol.equalPoint();
}

GeCylinder& GeCylinder::transformBy(const GeMatrix3d& xfm)
{
    double scaleFactor = 1.0;
    if (!cyl_is_uni_scaled_ortho(xfm, scaleFactor)) {
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

    m_origin.transformBy(xfm);

    m_axis = axisTransformed;
    m_ref = refTransformed;
    m_radius *= scaleFactor;
    m_uParamScale *= scaleFactor;
    if (m_height.isBounded()) {
        m_height.set(m_height.lowerBound() * scaleFactor, m_height.upperBound() * scaleFactor);
    }

    if (xfm.det() < 0.0) {
        m_uParamScale = -m_uParamScale;
        if (m_height.isBounded()) {
            double lower = m_height.lowerBound();
            double upper = m_height.upperBound();
            m_height.set(-lower, -upper);
        }
        m_axis.negate();
    }

    return *this;
}

GeCylinder& GeCylinder::translateBy(const GeVector3d& translateVec)
{
    m_origin += translateVec;
    return *this;
}

GeCylinder& GeCylinder::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeCylinder& GeCylinder::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_origin.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_ref.rotateBy(angle, vec);
    return *this;
}

GeCylinder& GeCylinder::mirror(const GePlane& plane)
{
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    return transformBy(mirrorXfm);
}

GeCylinder& GeCylinder::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCylinder& GeCylinder::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeMatrix3d scaleXfm;
    scaleXfm.setToScaling(scaleFactor, wrtPoint);
    return transformBy(scaleXfm);
}

bool GeCylinder::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeCylinder::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d uv = paramOf(pnt, tol);
    return evalPoint(uv).isEqualTo(pnt, tol);
}

double GeCylinder::radius() const
{
    return std::fabs(m_radius);
}

GePoint3d GeCylinder::origin() const
{
    return m_origin;
}

void GeCylinder::getAngles(double& startAng, double& endAng) const
{
    startAng = m_startAng;
    endAng = m_endAng;
}

void GeCylinder::getHeight(GeInterval& height) const
{
    height = m_height;
}

double GeCylinder::heightAt(double u) const
{
    return u * getUParamScale();
}

double GeCylinder::getUParamScale() const
{
    return m_uParamScale;
}

GeVector3d GeCylinder::axisOfSymmetry() const
{
    return axisUnit();
}

GeVector3d GeCylinder::refAxis() const
{
    return refUnit();
}

bool GeCylinder::isOuterNormal() const
{
    if (m_outerNormal) {
        return m_radius < 0.0;
    }
    return m_radius >= 0.0;
}

bool GeCylinder::isNormalReversed() const
{
    return m_outerNormal;
}

bool GeCylinder::isLeftHanded() const
{
    return isOuterNormal() != (m_uParamScale < 0.0);
}

bool GeCylinder::isClosed(const GeTol& tol) const
{
    return std::fabs((m_endAng - m_startAng) - (PI * 2.0)) <= tol.equalPoint();
}

GeSurface& GeCylinder::reverseNormal()
{
    m_outerNormal = !m_outerNormal;
    return *this;
}

void GeCylinder::setIsOuterNormal(bool isOuterNormal)
{
    m_radius = std::fabs(m_radius);
    if (isOuterNormal == m_outerNormal) {
        m_radius = -m_radius;
    }
}

void GeCylinder::setUParamScale(double uScale)
{
    if (cyl_abs(uScale) <= GeContext::gTol.equalPoint()) {
        uScale = std::fabs(m_radius);
    }

    if (uScale * m_uParamScale < 0.0 && m_height.isBounded()) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }

    m_uParamScale = uScale;
    m_hasCustomUParamScale = true;
}

GeCylinder& GeCylinder::setRadius(double radius)
{
    double r = std::fabs(radius);
    m_radius = (m_radius < 0.0) ? -r : r;
    m_uParamScale = r;
    m_hasCustomUParamScale = false;
    if (m_height.isBounded() && m_height.upperBound() < m_height.lowerBound()) {
        m_height.set(m_height.upperBound(), m_height.lowerBound());
    }
    return *this;
}

GeCylinder& GeCylinder::setAngles(double startAng, double endAng)
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

GeCylinder& GeCylinder::setHeight(const GeInterval& height)
{
    if (height.isBounded() && ((m_uParamScale > 0.0) ^ (height.lowerBound() < height.upperBound()))) {
        m_height.set(height.upperBound(), height.lowerBound());
    }
    else {
        m_height = height;
    }
    return *this;
}

GeCylinder& GeCylinder::set(double radius, const GePoint3d& origin, const GeVector3d& axisOfSym)
{
    GeInterval defaultHeight;
    GeVector3d defaultRefAxis = cyl_default_ref_axis(axisOfSym);
    return set(radius, origin, axisOfSym, defaultRefAxis, defaultHeight, -PI, PI);
}

GeCylinder& GeCylinder::set(
    double radius,
    const GePoint3d& origin,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& refAxis,
    const GeInterval& height,
    double startAng,
    double endAng)
{
    m_outerNormal = false;
    setRadius(radius);
    m_origin = origin;
    m_axis = axisOfSymmetry.length() > GeContext::gTol.equalVector() ? axisOfSymmetry.normal() : GeVector3d::kYAxis;

    GeVector3d ref = refAxis;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(m_axis, GeContext::gTol)) {
        GeVector3d fallback = (cyl_abs(m_axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
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

GeCylinder& GeCylinder::operator = (const GeCylinder& cylinder)
{
    if (this == &cylinder) {
        return *this;
    }

    m_radius = cylinder.m_radius;
    m_origin = cylinder.m_origin;
    m_axis = cylinder.m_axis;
    m_ref = cylinder.m_ref;
    m_height = cylinder.m_height;
    m_startAng = cylinder.m_startAng;
    m_endAng = cylinder.m_endAng;
    m_uParamScale = cylinder.m_uParamScale;
    m_hasCustomUParamScale = cylinder.m_hasCustomUParamScale;
    m_outerNormal = cylinder.m_outerNormal;
    return *this;
}

bool GeCylinder::intersectWith(
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

    GeVector3d delta = linePoint - m_origin;
    double x0 = delta.dotProduct(ref);
    double y0 = delta.dotProduct(ortho);
    double dx = lineDir.dotProduct(ref);
    double dy = lineDir.dotProduct(ortho);

    double a = dx * dx + dy * dy;
    double b = 2.0 * (x0 * dx + y0 * dy);
    double c = x0 * x0 + y0 * y0 - m_radius * m_radius;

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
        if (cyl_linear_entity_contains_point(linEnt, point, tol) && cyl_surface_contains_point(*this, point, tol)) {
            cyl_append_unique_point(hits, hitCount, point, tol);
        }
    }
    else {
        double sqrtDisc = std::sqrt(disc);
        double t1 = (-b - sqrtDisc) / (2.0 * a);
        double t2 = (-b + sqrtDisc) / (2.0 * a);

        GePoint3d point1 = linePoint + lineDir * t1;
        if (cyl_linear_entity_contains_point(linEnt, point1, tol) && cyl_surface_contains_point(*this, point1, tol)) {
            cyl_append_unique_point(hits, hitCount, point1, tol);
        }

        GePoint3d point2 = linePoint + lineDir * t2;
        if (cyl_linear_entity_contains_point(linEnt, point2, tol) && cyl_surface_contains_point(*this, point2, tol)) {
            cyl_append_unique_point(hits, hitCount, point2, tol);
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

GePoint2d GeCylinder::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GeVector3d delta = pnt - m_origin;
    double localX = delta.dotProduct(ref);
    double localY = delta.dotProduct(ortho);
    double localZ = delta.dotProduct(axis);

    double uHeight = localZ;
    double u = uHeight / getUParamScale();

    double v = std::atan2(localY, localX);

    v = cyl_adjust_to_interval(v, m_startAng, m_endAng);

    return GePoint2d(u, v);
}

GePoint3d GeCylinder::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double u = param.x;
    double uHeight = heightAt(u);
    if (m_height.isBounded()) {
        uHeight = cyl_clamp(uHeight, m_height.lowerBound(), m_height.upperBound());
    }

    double v = cyl_adjust_to_interval(param.y, m_startAng, m_endAng);
    GeVector3d radialDir = ref * cos(v) + ortho * sin(v);
    return m_origin + axis * uHeight + radialDir * std::fabs(m_radius);
}

void GeCylinder::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    double uScale = getUParamScale();
    if (m_height.isBounded()) {
        rangeU.set(m_height.lowerBound() / uScale, m_height.upperBound() / uScale);
    }
    else {
        rangeU = GeInterval();
    }
    rangeV.set(m_startAng, m_endAng);
}

GeVector3d GeCylinder::axisUnit() const
{
    if (m_axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kYAxis;
    }
    GeVector3d axis = m_axis;
    axis.normalize();
    return axis;
}

GeVector3d GeCylinder::refUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = m_ref;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(axis, GeContext::gTol)) {
        GeVector3d fallback = (cyl_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        ref = axis.crossProduct(fallback);
    }
    ref -= axis * ref.dotProduct(axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis;
    }
    ref.normalize();
    return ref;
}

GeVector3d GeCylinder::orthoRefUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = axis.crossProduct(ref);
    if (ortho.length() <= GeContext::gTol.equalVector()) {
        GeVector3d fallback = (cyl_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        ortho = axis.crossProduct(fallback);
    }
    ortho.normalize();
    return ortho;
}
