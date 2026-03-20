#include "GeSplineEnt3d.h"

#include "GePlane.h"
#include <cmath>

namespace {
int knotMultiplicityAt(const GeKnotVector& knots, int knotIndex, const GeTol& tol)
{
    if (knotIndex < 0 || knotIndex >= knots.length()) {
        return 0;
    }

    int multiplicity = 1;
    double value = knots[knotIndex];
    for (int i = knotIndex - 1; i >= 0; --i) {
        if (std::fabs(value - knots[i]) <= tol.equalPoint()) {
            multiplicity++;
        }
        else {
            break;
        }
    }
    for (int i = knotIndex + 1; i < knots.length(); ++i) {
        if (std::fabs(knots[i] - value) <= tol.equalPoint()) {
            multiplicity++;
        }
        else {
            break;
        }
    }
    return multiplicity;
}
}

GeSplineEnt3d::GeSplineEnt3d()
    : m_isRational(false)
    , m_degree(3)
    , m_hasFitData(false)
{
    m_knots.append(0.0);
    m_knots.append(1.0);
    m_controlPoints.append(GePoint3d::kOrigin);
    m_controlPoints.append(GePoint3d(1.0, 0.0, 0.0));
}

GeSplineEnt3d::GeSplineEnt3d(const GeSplineEnt3d& spline)
{
    *this = spline;
}

bool GeSplineEnt3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kCurve3d || entType == this->type();
}

Ge::EntityId GeSplineEnt3d::type() const
{
    return Ge::kSplineEnt3d;
}

GeSplineEnt3d* GeSplineEnt3d::copy() const
{
    return new GeSplineEnt3d(*this);
}

bool GeSplineEnt3d::operator == (const GeSplineEnt3d& entity) const
{
    return isEqualTo(entity);
}

bool GeSplineEnt3d::operator != (const GeSplineEnt3d& entity) const
{
    return !isEqualTo(entity);
}

bool GeSplineEnt3d::isEqualTo(const GeSplineEnt3d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSplineEnt3d::isEqualTo(const GeSplineEnt3d& entity, const GeTol& tol) const
{
    if (m_isRational != entity.m_isRational || m_degree != entity.m_degree || m_hasFitData != entity.m_hasFitData) {
        return false;
    }
    if (m_knots.length() != entity.m_knots.length() || m_controlPoints.length() != entity.m_controlPoints.length()) {
        return false;
    }

    for (int i = 0; i < m_knots.length(); ++i) {
        if (std::fabs(m_knots[i] - entity.m_knots[i]) > tol.equalPoint()) {
            return false;
        }
    }
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        if (!m_controlPoints[i].isEqualTo(entity.m_controlPoints[i], tol)) {
            return false;
        }
    }

    return true;
}

GeSplineEnt3d& GeSplineEnt3d::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].transformBy(xfm);
    }
    return *this;
}

GeSplineEnt3d& GeSplineEnt3d::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i] += translateVec;
    }
    return *this;
}

GeSplineEnt3d& GeSplineEnt3d::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeSplineEnt3d& GeSplineEnt3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeSplineEnt3d& GeSplineEnt3d::mirror(const GePlane& plane)
{
    GeVector3d normal = plane.normal();
    GePoint3d origin = plane.pointOnPlane();
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        GeVector3d offset = m_controlPoints[i] - origin;
        double d = offset.dotProduct(normal);
        m_controlPoints[i] -= normal * (2.0 * d);
    }
    return *this;
}

GeSplineEnt3d& GeSplineEnt3d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeSplineEnt3d& GeSplineEnt3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        GeVector3d offset = m_controlPoints[i] - wrtPoint;
        m_controlPoints[i] = wrtPoint + offset * scaleFactor;
    }
    return *this;
}

bool GeSplineEnt3d::isOn(const GePoint3d& pnt) const
{
    return GeCurve3d::isOn(pnt);
}

bool GeSplineEnt3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return GeCurve3d::isOn(pnt, tol);
}

bool GeSplineEnt3d::isRational() const
{
    return m_isRational;
}

int GeSplineEnt3d::degree() const
{
    return m_degree;
}

int GeSplineEnt3d::order() const
{
    return m_degree + 1;
}

int GeSplineEnt3d::numKnots() const
{
    return m_knots.length();
}

const GeKnotVector& GeSplineEnt3d::knots() const
{
    return m_knots;
}

int GeSplineEnt3d::numControlPoints() const
{
    return m_controlPoints.length();
}

int GeSplineEnt3d::continuityAtKnot(int knotIndex, const GeTol& tol) const
{
    int m = knotMultiplicityAt(m_knots, knotIndex, tol);
    int c = m_degree - m;
    return c < 0 ? 0 : c;
}

double GeSplineEnt3d::startParam() const
{
    return m_knots.length() > 0 ? m_knots[0] : 0.0;
}

double GeSplineEnt3d::endParam() const
{
    return m_knots.length() > 0 ? m_knots[m_knots.length() - 1] : 1.0;
}

GePoint3d GeSplineEnt3d::startPoint() const
{
    return m_controlPoints.length() > 0 ? m_controlPoints[0] : GePoint3d::kOrigin;
}

GePoint3d GeSplineEnt3d::endPoint() const
{
    return m_controlPoints.length() > 0 ? m_controlPoints[m_controlPoints.length() - 1] : GePoint3d::kOrigin;
}

bool GeSplineEnt3d::hasFitData() const
{
    return m_hasFitData;
}

double GeSplineEnt3d::knotAt(int knotIndex) const
{
    if (knotIndex < 0 || knotIndex >= m_knots.length()) {
        return 0.0;
    }
    return m_knots[knotIndex];
}

GeSplineEnt3d& GeSplineEnt3d::setKnotAt(int knotIndex, double val)
{
    if (knotIndex >= 0 && knotIndex < m_knots.length()) {
        m_knots[knotIndex] = val;
    }
    return *this;
}

GePoint3d GeSplineEnt3d::controlPointAt(int controlPointIndex) const
{
    if (controlPointIndex < 0 || controlPointIndex >= m_controlPoints.length()) {
        return GePoint3d::kOrigin;
    }
    return m_controlPoints[controlPointIndex];
}

GeSplineEnt3d& GeSplineEnt3d::setControlPointAt(int controlPointIndex, const GePoint3d& point)
{
    if (controlPointIndex >= 0 && controlPointIndex < m_controlPoints.length()) {
        m_controlPoints[controlPointIndex] = point;
    }
    return *this;
}

GeSplineEnt3d& GeSplineEnt3d::operator = (const GeSplineEnt3d& spline)
{
    if (this == &spline) {
        return *this;
    }

    m_isRational = spline.m_isRational;
    m_degree = spline.m_degree;
    m_hasFitData = spline.m_hasFitData;
    m_knots = spline.m_knots;
    m_controlPoints = spline.m_controlPoints;
    return *this;
}
