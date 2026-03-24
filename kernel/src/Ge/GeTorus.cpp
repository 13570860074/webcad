#include "GeTorus.h"

#include "GeInterval.h"
#include "GeLinearEnt3d.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <algorithm>
#include <complex>
#include <cmath>
#include <vector>

namespace {
static double torus_abs(double value)
{
    return value < 0.0 ? -value : value;
}

static double torus_clamp(double value, double minValue, double maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

static double torus_adjust_to_interval(double angle, double start, double end)
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
        out = (torus_abs(a - start) <= torus_abs(b - start)) ? a : b;
    }
    if (out > end) {
        double a = out;
        double b = out - twoPi;
        out = (torus_abs(a - end) <= torus_abs(b - end)) ? a : b;
    }

    return torus_clamp(out, start, end);
}

static bool torus_interval_contains_with_tol(const GeInterval& range, double value, const GeTol& tol)
{
    if (range.isBoundedBelow() && value < range.lowerBound() - tol.equalPoint()) {
        return false;
    }
    if (range.isBoundedAbove() && value > range.upperBound() + tol.equalPoint()) {
        return false;
    }
    return true;
}

static bool torus_linear_entity_contains_point(const GeLinearEnt3d& linEnt, const GePoint3d& point, const GeTol& tol)
{
    if (linEnt.isOn(point, tol)) {
        double param = linEnt.paramOf(point, tol);
        GeInterval range;
        linEnt.getInterval(range);
        return torus_interval_contains_with_tol(range, param, tol);
    }
    return false;
}

static bool torus_surface_contains_point(const GeTorus& torus, const GePoint3d& point, const GeTol& tol)
{
    GePoint2d uv = torus.paramOf(point, tol);
    GeInterval rangeU;
    GeInterval rangeV;
    torus.getEnvelope(rangeU, rangeV);
    if (!torus_interval_contains_with_tol(rangeU, uv.x, tol) || !torus_interval_contains_with_tol(rangeV, uv.y, tol)) {
        return false;
    }
    return torus.evalPoint(uv).distanceTo(point) <= tol.equalPoint() * 4.0;
}

static std::complex<double> torus_poly_eval(const std::vector<double>& coeffs, const std::complex<double>& z)
{
    std::complex<double> acc(0.0, 0.0);
    for (size_t i = 0; i < coeffs.size(); ++i) {
        acc = acc * z + coeffs[i];
    }
    return acc;
}

static bool torus_solve_real_polynomial_roots(const std::vector<double>& coeffIn, std::vector<double>& realRoots)
{
    realRoots.clear();
    if (coeffIn.empty()) {
        return false;
    }

    std::vector<double> coeffs = coeffIn;
    const double leadEps = 1.0e-14;
    while (coeffs.size() > 1 && std::fabs(coeffs[0]) <= leadEps) {
        coeffs.erase(coeffs.begin());
    }

    int degree = int(coeffs.size()) - 1;
    if (degree <= 0) {
        return false;
    }

    if (degree == 1) {
        realRoots.push_back(-coeffs[1] / coeffs[0]);
        return true;
    }

    double lead = coeffs[0];
    for (size_t i = 0; i < coeffs.size(); ++i) {
        coeffs[i] /= lead;
    }

    std::vector<std::complex<double>> roots(static_cast<size_t>(degree));
    const double twoPi = PI * 2.0;
    for (int i = 0; i < degree; ++i) {
        double ang = twoPi * double(i) / double(degree);
        roots[size_t(i)] = std::polar(1.0, ang);
    }

    for (int iter = 0; iter < 100; ++iter) {
        bool converged = true;
        for (int i = 0; i < degree; ++i) {
            std::complex<double> denom(1.0, 0.0);
            for (int j = 0; j < degree; ++j) {
                if (i != j) {
                    denom *= (roots[size_t(i)] - roots[size_t(j)]);
                }
            }

            if (std::abs(denom) <= 1.0e-16) {
                denom = std::complex<double>(1.0e-16, 0.0);
            }

            std::complex<double> delta = torus_poly_eval(coeffs, roots[size_t(i)]) / denom;
            roots[size_t(i)] -= delta;
            if (std::abs(delta) > 1.0e-12) {
                converged = false;
            }
        }
        if (converged) {
            break;
        }
    }

    for (size_t i = 0; i < roots.size(); ++i) {
        const std::complex<double>& r = roots[i];
        if (std::fabs(r.imag()) <= 1.0e-8) {
            realRoots.push_back(r.real());
        }
    }

    std::sort(realRoots.begin(), realRoots.end());
    std::vector<double> uniqueRoots;
    const double mergeTol = 1.0e-7;
    for (size_t i = 0; i < realRoots.size(); ++i) {
        if (uniqueRoots.empty() || std::fabs(realRoots[i] - uniqueRoots.back()) > mergeTol) {
            uniqueRoots.push_back(realRoots[i]);
        }
    }
    realRoots.swap(uniqueRoots);
    return !realRoots.empty();
}

static void torus_append_unique_point(GePoint3d* points, double* params, int& count, const GePoint3d& point, double param, const GeTol& tol)
{
    for (int i = 0; i < count; ++i) {
        if (points[i].isEqualTo(point, tol)) {
            return;
        }
    }

    if (count < 4) {
        points[count] = point;
        params[count] = param;
        ++count;
    }
}

static void torus_sort_points_by_param(GePoint3d* points, double* params, int count)
{
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (params[j] < params[i]) {
                double tp = params[i];
                params[i] = params[j];
                params[j] = tp;
                GePoint3d p = points[i];
                points[i] = points[j];
                points[j] = p;
            }
        }
    }
}

}

GeTorus::GeTorus()
    : m_majorRadius(1.0)
    , m_minorRadius(0.5)
    , m_center(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kZAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_startU(-PI)
    , m_endU(PI)
    , m_startV(-PI)
    , m_endV(PI)
    , m_outerNormal(true)
    , m_reverseV(false)
{
}

GeTorus::GeTorus(double majorRadius, double minorRadius, const GePoint3d& center, const GeVector3d& axisOfSymmetry)
    : m_majorRadius(1.0)
    , m_minorRadius(0.5)
    , m_center(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kZAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_startU(-PI)
    , m_endU(PI)
    , m_startV(-PI)
    , m_endV(PI)
    , m_outerNormal(true)
    , m_reverseV(false)
{
    set(majorRadius, minorRadius, center, axisOfSymmetry);
}

GeTorus::GeTorus(
    double majorRadius,
    double minorRadius,
    const GePoint3d& center,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& refAxis,
    double startAngleU,
    double endAngleU,
    double startAngleV,
    double endAngleV)
    : m_majorRadius(1.0)
    , m_minorRadius(0.5)
    , m_center(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kZAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_startU(-PI)
    , m_endU(PI)
    , m_startV(-PI)
    , m_endV(PI)
    , m_outerNormal(true)
    , m_reverseV(false)
{
    set(majorRadius, minorRadius, center, axisOfSymmetry, refAxis, startAngleU, endAngleU, startAngleV, endAngleV);
}

GeTorus::GeTorus(const GeTorus& source)
    : m_majorRadius(1.0)
    , m_minorRadius(0.5)
    , m_center(GePoint3d::kOrigin)
    , m_axis(GeVector3d::kZAxis)
    , m_ref(GeVector3d::kXAxis)
    , m_startU(-PI)
    , m_endU(PI)
    , m_startV(-PI)
    , m_endV(PI)
    , m_outerNormal(true)
    , m_reverseV(false)
{
    *this = source;
}

bool GeTorus::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == type();
}

Ge::EntityId GeTorus::type() const
{
    return Ge::kTorus;
}

GeTorus* GeTorus::copy() const
{
    return new GeTorus(*this);
}

bool GeTorus::operator == (const GeTorus& entity) const
{
    return isEqualTo(entity);
}

bool GeTorus::operator != (const GeTorus& entity) const
{
    return !isEqualTo(entity);
}

bool GeTorus::isEqualTo(const GeTorus& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeTorus::isEqualTo(const GeTorus& entity, const GeTol& tol) const
{
    GeInterval rangeU;
    GeInterval rangeV;
    GeInterval rangeU1;
    GeInterval rangeV1;
    getEnvelope(rangeU, rangeV);
    entity.getEnvelope(rangeU1, rangeV1);

    double u1 = 0.0;
    double u2 = 0.0;
    double ru1 = 0.0;
    double ru2 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double rv1 = 0.0;
    double rv2 = 0.0;
    rangeU.getBounds(u1, u2);
    rangeU1.getBounds(ru1, ru2);
    rangeV.getBounds(v1, v2);
    rangeV1.getBounds(rv1, rv2);

    if (torus_abs(m_majorRadius - entity.m_majorRadius) > tol.equalPoint()) {
        return false;
    }
    if (torus_abs(m_minorRadius - entity.m_minorRadius) > tol.equalPoint()) {
        return false;
    }
    if (!m_center.isEqualTo(entity.m_center, tol)) {
        return false;
    }
    if (!axisUnit().isEqualTo(entity.axisUnit(), tol)) {
        return false;
    }
    if (!refUnit().isEqualTo(entity.refUnit(), tol)) {
        return false;
    }
    if (torus_abs(u1 - ru1) > tol.equalPoint() || torus_abs(u2 - ru2) > tol.equalPoint()) {
        return false;
    }
    if (torus_abs(v1 - rv1) > tol.equalPoint() || torus_abs(v2 - rv2) > tol.equalPoint()) {
        return false;
    }
    return m_reverseV == entity.m_reverseV;
}

GeTorus& GeTorus::transformBy(const GeMatrix3d& xfm)
{
    if (xfm.isPerspective()) {
        return *this;
    }

    GeVector3d axisRef = axisUnit();
    GeVector3d refAxisRef = refUnit();
    GeVector3d orthoRef = orthoRefUnit();

    GeVector3d axisTransformed = axisRef;
    GeVector3d refTransformed = refAxisRef;
    GeVector3d orthoTransformed = orthoRef;
    axisTransformed.transformBy(xfm);
    refTransformed.transformBy(xfm);
    orthoTransformed.transformBy(xfm);

    double axisScale = axisTransformed.length();
    double refScale = refTransformed.length();
    double orthoScale = orthoTransformed.length();
    if (axisScale <= GeContext::gTol.equalVector() || refScale <= GeContext::gTol.equalVector() || orthoScale <= GeContext::gTol.equalVector()) {
        return *this;
    }

    axisTransformed.normalize();
    refTransformed.normalize();
    orthoTransformed.normalize();

    if (std::fabs(axisTransformed.dotProduct(refTransformed)) > GeContext::gTol.equalVector()) {
        return *this;
    }
    if (std::fabs(axisTransformed.dotProduct(orthoTransformed)) > GeContext::gTol.equalVector()) {
        return *this;
    }
    if (std::fabs(refTransformed.dotProduct(orthoTransformed)) > GeContext::gTol.equalVector()) {
        return *this;
    }
    if (std::fabs(refScale - axisScale) > GeContext::gTol.equalPoint() || std::fabs(orthoScale - axisScale) > GeContext::gTol.equalPoint()) {
        return *this;
    }

    m_center.transformBy(xfm);

    m_axis = axisTransformed;
    m_ref = refTransformed;
    m_majorRadius *= axisScale;
    m_minorRadius *= axisScale;

    if (xfm.det() < 0.0) {
        m_reverseV = !m_reverseV;
        double oldStart = m_startV;
        double oldEnd = m_endV;
        m_startV = -oldEnd;
        m_endV = -oldStart;
    }

    return *this;
}

GeTorus& GeTorus::translateBy(const GeVector3d& translateVec)
{
    m_center += translateVec;
    return *this;
}

GeTorus& GeTorus::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeTorus& GeTorus::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    m_center.rotateBy(angle, vec, wrtPoint);
    m_axis.rotateBy(angle, vec);
    m_ref.rotateBy(angle, vec);
    return *this;
}

GeTorus& GeTorus::mirror(const GePlane& plane)
{
    m_center.mirror(plane);
    GeMatrix3d mirrorXfm;
    mirrorXfm.setToMirroring(plane);
    m_axis.transformBy(mirrorXfm);
    m_ref.transformBy(mirrorXfm);
    m_outerNormal = !m_outerNormal;
    return *this;
}

GeTorus& GeTorus::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeTorus& GeTorus::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    m_center.scaleBy(scale, wrtPoint);
    m_majorRadius *= scaleFactor;
    m_minorRadius *= scaleFactor;
    return *this;
}

bool GeTorus::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeTorus::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d uv = paramOf(pnt, tol);
    return evalPoint(uv).isEqualTo(pnt, tol);
}

double GeTorus::majorRadius() const
{
    return m_majorRadius;
}

double GeTorus::minorRadius() const
{
    return m_outerNormal ? m_minorRadius : -m_minorRadius;
}

void GeTorus::getAnglesInU(double& startAngleU, double& endAngleU) const
{
    startAngleU = m_startU;
    endAngleU = m_endU;
}

void GeTorus::getAnglesInV(double& startAngleV, double& endAngleV) const
{
    startAngleV = m_startV;
    endAngleV = m_endV;
}

GePoint3d GeTorus::center() const
{
    return m_center;
}

GeVector3d GeTorus::axisOfSymmetry() const
{
    return axisUnit();
}

GeVector3d GeTorus::refAxis() const
{
    return refUnit();
}

bool GeTorus::isOuterNormal() const
{
    return minorRadius() >= 0.0;
}

GeTorus& GeTorus::setMajorRadius(double radius)
{
    m_majorRadius = radius;
    setAnglesInU(-PI, PI);
    return *this;
}

GeTorus& GeTorus::setMinorRadius(double radius)
{
    m_minorRadius = radius;
    setAnglesInU(-PI, PI);
    return *this;
}

GeTorus& GeTorus::setAnglesInU(double startAngleU, double endAngleU)
{
    double twoPi = PI * 2.0;
    m_startU = startAngleU;
    m_endU = endAngleU;
    while (m_endU < m_startU) {
        m_endU += twoPi;
    }
    if (m_endU - m_startU > twoPi) {
        m_endU = m_startU + twoPi;
    }

    if (!isDoughnut()) {
        double r = torus_abs(m_minorRadius);
        if (r > GeContext::gTol.equalPoint()) {
            double ratio = -m_majorRadius / r;
            ratio = torus_clamp(ratio, -1.0, 1.0);
            double naturalHalf = std::acos(ratio);
            double naturalStart = -naturalHalf;
            double naturalEnd = naturalHalf;

            double center = (m_startU + m_endU) * 0.5;
            while (center < -PI) {
                m_startU += twoPi;
                m_endU += twoPi;
                center += twoPi;
            }
            while (center > PI) {
                m_startU -= twoPi;
                m_endU -= twoPi;
                center -= twoPi;
            }

            double clippedStart = (m_startU > naturalStart) ? m_startU : naturalStart;
            double clippedEnd = (m_endU < naturalEnd) ? m_endU : naturalEnd;
            if (clippedEnd >= clippedStart) {
                m_startU = clippedStart;
                m_endU = clippedEnd;
            }
            else {
                m_startU = 1.0;
                m_endU = 0.0;
            }
        }
    }
    return *this;
}

GeTorus& GeTorus::setAnglesInV(double startAngleV, double endAngleV)
{
    double twoPi = PI * 2.0;
    m_startV = startAngleV;
    m_endV = endAngleV;
    while (m_endV < m_startV) {
        m_endV += twoPi;
    }
    if (m_endV - m_startV > twoPi) {
        m_endV = m_startV + twoPi;
    }
    return *this;
}

GeTorus& GeTorus::set(double majorRadius, double minorRadius, const GePoint3d& center, const GeVector3d& axisOfSymmetry)
{
    return set(majorRadius, minorRadius, center, axisOfSymmetry, GeVector3d::kXAxis, -PI, PI, -PI, PI);
}

GeTorus& GeTorus::set(
    double majorRadius,
    double minorRadius,
    const GePoint3d& center,
    const GeVector3d& axisOfSymmetry,
    const GeVector3d& refAxis,
    double startAngleU,
    double endAngleU,
    double startAngleV,
    double endAngleV)
{
    setMajorRadius(majorRadius);
    setMinorRadius(minorRadius);
    m_center = center;
    m_axis = axisOfSymmetry.length() > GeContext::gTol.equalVector() ? axisOfSymmetry.normal() : GeVector3d::kZAxis;

    GeVector3d ref = refAxis;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(m_axis, GeContext::gTol)) {
        GeVector3d fallback = (torus_abs(m_axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
        ref = m_axis.crossProduct(fallback);
    }
    ref -= m_axis * ref.dotProduct(m_axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis;
    }
    ref.normalize();
    m_ref = ref;

    setAnglesInU(startAngleU, endAngleU);
    setAnglesInV(startAngleV, endAngleV);
    return *this;
}

GeTorus& GeTorus::operator = (const GeTorus& torus)
{
    if (this == &torus) {
        return *this;
    }

    m_majorRadius = torus.m_majorRadius;
    m_minorRadius = torus.m_minorRadius;
    m_center = torus.m_center;
    m_axis = torus.m_axis;
    m_ref = torus.m_ref;
    m_startU = torus.m_startU;
    m_endU = torus.m_endU;
    m_startV = torus.m_startV;
    m_endV = torus.m_endV;
    m_outerNormal = torus.m_outerNormal;
    m_reverseV = torus.m_reverseV;
    return *this;
}

bool GeTorus::intersectWith(
    const GeLinearEnt3d& linEnt,
    int& numInt,
    GePoint3d& p1,
    GePoint3d& p2,
    GePoint3d& p3,
    GePoint3d& p4,
    const GeTol& tol) const
{
    numInt = 0;
    p1 = GePoint3d::kOrigin;
    p2 = GePoint3d::kOrigin;
    p3 = GePoint3d::kOrigin;
    p4 = GePoint3d::kOrigin;

    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GePoint3d basePoint = linEnt.pointOnLine();
    GeVector3d dir = linEnt.direction();
    if (dir.length() <= tol.equalVector()) {
        return false;
    }

    GeVector3d p = basePoint - m_center;
    double px = p.dotProduct(ref);
    double py = p.dotProduct(ortho);
    double pz = p.dotProduct(axis);
    double dx = dir.dotProduct(ref);
    double dy = dir.dotProduct(ortho);
    double dz = dir.dotProduct(axis);

    double R = m_majorRadius;
    double r = torus_abs(m_minorRadius);
    double c = R * R - r * r;

    double a2 = dx * dx + dy * dy + dz * dz;
    double a1 = 2.0 * (px * dx + py * dy + pz * dz);
    double a0 = px * px + py * py + pz * pz;

    double b2 = dx * dx + dy * dy;
    double b1 = 2.0 * (px * dx + py * dy);
    double b0 = px * px + py * py;

    double A2 = a2;
    double A1 = a1;
    double A0 = a0 + c;

    std::vector<double> coeffs(5, 0.0);
    coeffs[0] = A2 * A2;
    coeffs[1] = 2.0 * A2 * A1;
    coeffs[2] = A1 * A1 + 2.0 * A2 * A0 - 4.0 * R * R * b2;
    coeffs[3] = 2.0 * A1 * A0 - 4.0 * R * R * b1;
    coeffs[4] = A0 * A0 - 4.0 * R * R * b0;

    std::vector<double> roots;
    if (!torus_solve_real_polynomial_roots(coeffs, roots)) {
        return false;
    }

    GeInterval lineRange;
    linEnt.getInterval(lineRange);
    GePoint3d candidates[4];
    double candidateParams[4] = { 0.0, 0.0, 0.0, 0.0 };
    int candidateCount = 0;

    for (size_t i = 0; i < roots.size(); ++i) {
        double t = roots[i];
        if (!torus_interval_contains_with_tol(lineRange, t, tol)) {
            continue;
        }

        GePoint3d point = linEnt.evalPoint(t);
        if (!torus_linear_entity_contains_point(linEnt, point, tol)) {
            continue;
        }
        if (!torus_surface_contains_point(*this, point, tol)) {
            continue;
        }

        torus_append_unique_point(candidates, candidateParams, candidateCount, point, t, tol);
    }

    if (candidateCount <= 0) {
        return false;
    }

    torus_sort_points_by_param(candidates, candidateParams, candidateCount);
    numInt = candidateCount;
    if (numInt > 0) {
        p1 = candidates[0];
    }
    if (numInt > 1) {
        p2 = candidates[1];
    }
    if (numInt > 2) {
        p3 = candidates[2];
    }
    if (numInt > 3) {
        p4 = candidates[3];
    }
    return true;
}

bool GeTorus::isLemon() const
{
    return m_majorRadius <= 0.0;
}

bool GeTorus::isApple() const
{
    if (m_majorRadius <= 0.0) {
        return false;
    }
    return m_majorRadius < torus_abs(m_minorRadius) - GeContext::gTol.equalVector();
}

bool GeTorus::isVortex() const
{
    return torus_abs(m_majorRadius - torus_abs(m_minorRadius)) < GeContext::gTol.equalVector();
}

bool GeTorus::isDoughnut() const
{
    return m_majorRadius >= torus_abs(m_minorRadius) + GeContext::gTol.equalVector();
}

bool GeTorus::isDegenerate() const
{
    return !isDoughnut();
}

bool GeTorus::isHollow() const
{
    return minorRadius() < 0.0;
}

bool GeTorus::isReverseV() const
{
    return m_reverseV;
}

void GeTorus::setReverseV(bool isReverseV)
{
    m_reverseV = isReverseV;
}

GePoint2d GeTorus::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    GeVector3d delta = pnt - m_center;

    double v = atan2(delta.dotProduct(ortho), delta.dotProduct(ref));
    if (m_reverseV) {
        v = -v;
    }
    v = torus_adjust_to_interval(v, m_startV, m_endV);

    double ringAngle = m_reverseV ? -v : v;
    GeVector3d ringDir = ref * cos(ringAngle) + ortho * sin(ringAngle);
    GePoint3d ringCenter = m_center + ringDir * m_majorRadius;
    GeVector3d tubeVec = pnt - ringCenter;

    double u = 0.0;
    double tubeLen = tubeVec.length();
    if (tubeLen > tol.equalVector()) {
        u = atan2(tubeVec.dotProduct(axis), tubeVec.dotProduct(ringDir));
    }
    u = torus_adjust_to_interval(u, m_startU, m_endU);

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);
    if (rangeU.isBounded()) {
        u = torus_clamp(u, rangeU.lowerBound(), rangeU.upperBound());
    }
    if (rangeV.isBounded()) {
        v = torus_clamp(v, rangeV.lowerBound(), rangeV.upperBound());
    }

    return GePoint2d(u, v);
}

GePoint3d GeTorus::evalPoint(const GePoint2d& param) const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = orthoRefUnit();

    double u = torus_adjust_to_interval(param.x, m_startU, m_endU);
    double v = torus_adjust_to_interval(param.y, m_startV, m_endV);
    double ringAngle = m_reverseV ? -v : v;

    GeVector3d ringDir = ref * cos(ringAngle) + ortho * sin(ringAngle);
    GeVector3d tubeCenterVec = ringDir * m_majorRadius;

    GeVector3d tubeOffset = ringDir * (m_minorRadius * cos(u)) + axis * (m_minorRadius * sin(u));
    return m_center + tubeCenterVec + tubeOffset;
}

void GeTorus::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    rangeU.set(m_startU, m_endU);
    rangeV.set(m_startV, m_endV);
}

GeVector3d GeTorus::axisUnit() const
{
    if (m_axis.length() <= GeContext::gTol.equalVector()) {
        return GeVector3d::kZAxis;
    }
    GeVector3d axis = m_axis;
    axis.normalize();
    return axis;
}

GeVector3d GeTorus::refUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = m_ref;
    if (ref.length() <= GeContext::gTol.equalVector() || ref.isParallelTo(axis, GeContext::gTol)) {
        GeVector3d fallback = (torus_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
        ref = axis.crossProduct(fallback);
    }
    ref -= axis * ref.dotProduct(axis);
    if (ref.length() <= GeContext::gTol.equalVector()) {
        ref = GeVector3d::kXAxis;
    }
    ref.normalize();
    return ref;
}

GeVector3d GeTorus::orthoRefUnit() const
{
    GeVector3d axis = axisUnit();
    GeVector3d ref = refUnit();
    GeVector3d ortho = axis.crossProduct(ref);
    if (ortho.length() <= GeContext::gTol.equalVector()) {
        GeVector3d fallback = (torus_abs(axis.x) < 0.9) ? GeVector3d::kXAxis : GeVector3d::kYAxis;
        ortho = axis.crossProduct(fallback);
    }
    ortho.normalize();
    return ortho;
}
