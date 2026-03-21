#include "GeCylinder.h"

#include "GePlane.h"
#include "GeScale3d.h"
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
}

GeCylinder::GeCylinder()
    : m_radius(2.0)
    , m_origin(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kYAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_height(0.0, 1.0)
    , m_startAng(-PI)
    , m_endAng(PI)
    , m_outerNormal(true)
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
    , m_outerNormal(true)
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
    , m_outerNormal(true)
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
    , m_outerNormal(true)
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
    if (!(m_height == entity.m_height)) {
        return false;
    }
    if (cyl_abs(m_startAng - entity.m_startAng) > tol.equalPoint() || cyl_abs(m_endAng - entity.m_endAng) > tol.equalPoint()) {
        return false;
    }
    return m_outerNormal == entity.m_outerNormal;
}

GeCylinder& GeCylinder::transformBy(const GeMatrix3d& xfm)
{
    m_origin.transformBy(xfm);

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
        GeVector3d fallback = (cyl_abs(axisTransformed.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
        refTransformed = axisTransformed.crossProduct(fallback);
    }
    refTransformed.normalize();

    m_axis = axisTransformed;
    m_ref = refTransformed;
    m_radius *= axisScale;
    if (m_height.isBounded()) {
        m_height.set(m_height.lowerBound() * axisScale, m_height.upperBound() * axisScale);
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
    m_origin.mirror(plane);
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    m_axis.transformBy(mirrorXfm);
    m_ref.transformBy(mirrorXfm);
    m_outerNormal = !m_outerNormal;
    return *this;
}

GeCylinder& GeCylinder::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCylinder& GeCylinder::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    m_origin.scaleBy(scale, wrtPoint);
    m_radius *= scaleFactor;
    if (m_height.isBounded()) {
        m_height.set(m_height.lowerBound() * scaleFactor, m_height.upperBound() * scaleFactor);
    }
    return *this;
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
    return m_radius;
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
    return u;
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
    return m_outerNormal;
}

bool GeCylinder::isClosed(const GeTol& tol) const
{
    return (m_endAng - m_startAng) >= (PI * 2.0 - tol.equalPoint());
}

GeCylinder& GeCylinder::setRadius(double radius)
{
    m_radius = radius > GeContext::gTol.equalPoint() ? radius : GeContext::gTol.equalPoint();
    return *this;
}

GeCylinder& GeCylinder::setAngles(double startAng, double endAng)
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

GeCylinder& GeCylinder::setHeight(const GeInterval& height)
{
    m_height = height;
    return *this;
}

GeCylinder& GeCylinder::set(double radius, const GePoint3d& origin, const GeVector3d& axisOfSym)
{
    GeInterval defaultHeight(0.0, 1.0);
    return set(radius, origin, axisOfSym, GeVector3d::kXAxis, defaultHeight, -PI, PI);
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
    m_outerNormal = cylinder.m_outerNormal;
    return *this;
}

GePoint2d GeCylinder::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GeVector3d delta = pnt - m_origin;
    double u = delta.dotProduct(axis);
    if (m_height.isBounded()) {
        u = cyl_clamp(u, m_height.lowerBound(), m_height.upperBound());
    }

    GeVector3d radial = delta - axis * u;
    double v = m_startAng;
    if (radial.length() > tol.equalVector()) {
        v = atan2(radial.dotProduct(ortho), radial.dotProduct(ref));
    }

    v = cyl_adjust_to_interval(v, m_startAng, m_endAng);
    return GePoint2d(u, v);
}

GePoint3d GeCylinder::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double u = param.x;
    if (m_height.isBounded()) {
        u = cyl_clamp(u, m_height.lowerBound(), m_height.upperBound());
    }

    double v = cyl_adjust_to_interval(param.y, m_startAng, m_endAng);
    GeVector3d radialDir = ref * cos(v) + ortho * sin(v);
    return m_origin + axis * u + radialDir * m_radius;
}

void GeCylinder::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    rangeU = m_height;
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
