#include "GeSplineEnt2d.h"

#include "GeLine2d.h"
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

GeSplineEnt2d::GeSplineEnt2d()
    : m_isRational(false)
    , m_degree(3)
    , m_hasFitData(false)
{
    m_knots.append(0.0);
    m_knots.append(1.0);
    m_controlPoints.append(GePoint2d::kOrigin);
    m_controlPoints.append(GePoint2d(1.0, 0.0));
}

GeSplineEnt2d::GeSplineEnt2d(const GeSplineEnt2d& spline)
{
    *this = spline;
}

bool GeSplineEnt2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == this->type();
}

Ge::EntityId GeSplineEnt2d::type() const
{
    return Ge::kSplineEnt2d;
}

GeSplineEnt2d* GeSplineEnt2d::copy() const
{
    return new GeSplineEnt2d(*this);
}

bool GeSplineEnt2d::operator == (const GeSplineEnt2d& entity) const
{
    return isEqualTo(entity);
}

bool GeSplineEnt2d::operator != (const GeSplineEnt2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeSplineEnt2d::isEqualTo(const GeSplineEnt2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSplineEnt2d::isEqualTo(const GeSplineEnt2d& entity, const GeTol& tol) const
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

GeSplineEnt2d& GeSplineEnt2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].transformBy(xfm);
    }
    return *this;
}

GeSplineEnt2d& GeSplineEnt2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i] += translateVec;
    }
    return *this;
}

GeSplineEnt2d& GeSplineEnt2d::rotateBy(double angle)
{
    return rotateBy(angle, GePoint2d::kOrigin);
}

GeSplineEnt2d& GeSplineEnt2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].rotateBy(angle, wrtPoint);
    }
    return *this;
}

GeSplineEnt2d& GeSplineEnt2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].mirror(line);
    }
    return *this;
}

GeSplineEnt2d& GeSplineEnt2d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeSplineEnt2d& GeSplineEnt2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeSplineEnt2d::isOn(const GePoint2d& pnt) const
{
    return GeCurve2d::isOn(pnt);
}

bool GeSplineEnt2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    return GeCurve2d::isOn(pnt, tol);
}

bool GeSplineEnt2d::isRational() const
{
    return m_isRational;
}

int GeSplineEnt2d::degree() const
{
    return m_degree;
}

int GeSplineEnt2d::order() const
{
    return m_degree + 1;
}

int GeSplineEnt2d::numKnots() const
{
    return m_knots.length();
}

const GeKnotVector& GeSplineEnt2d::knots() const
{
    return m_knots;
}

int GeSplineEnt2d::numControlPoints() const
{
    return m_controlPoints.length();
}

int GeSplineEnt2d::continuityAtKnot(int knotIndex, const GeTol& tol) const
{
    int m = knotMultiplicityAt(m_knots, knotIndex, tol);
    int c = m_degree - m;
    return c < 0 ? 0 : c;
}

double GeSplineEnt2d::startParam() const
{
    return m_knots.length() > 0 ? m_knots[0] : 0.0;
}

double GeSplineEnt2d::endParam() const
{
    return m_knots.length() > 0 ? m_knots[m_knots.length() - 1] : 1.0;
}

GePoint2d GeSplineEnt2d::startPoint() const
{
    return m_controlPoints.length() > 0 ? m_controlPoints[0] : GePoint2d::kOrigin;
}

GePoint2d GeSplineEnt2d::endPoint() const
{
    return m_controlPoints.length() > 0 ? m_controlPoints[m_controlPoints.length() - 1] : GePoint2d::kOrigin;
}

bool GeSplineEnt2d::hasFitData() const
{
    return m_hasFitData;
}

double GeSplineEnt2d::knotAt(int knotIndex) const
{
    if (knotIndex < 0 || knotIndex >= m_knots.length()) {
        return 0.0;
    }
    return m_knots[knotIndex];
}

GeSplineEnt2d& GeSplineEnt2d::setKnotAt(int knotIndex, double val)
{
    if (knotIndex >= 0 && knotIndex < m_knots.length()) {
        m_knots[knotIndex] = val;
    }
    return *this;
}

GePoint2d GeSplineEnt2d::controlPointAt(int controlPointIndex) const
{
    if (controlPointIndex < 0 || controlPointIndex >= m_controlPoints.length()) {
        return GePoint2d::kOrigin;
    }
    return m_controlPoints[controlPointIndex];
}

GeSplineEnt2d& GeSplineEnt2d::setControlPointAt(int controlPointIndex, const GePoint2d& point)
{
    if (controlPointIndex >= 0 && controlPointIndex < m_controlPoints.length()) {
        m_controlPoints[controlPointIndex] = point;
    }
    return *this;
}

GeSplineEnt2d& GeSplineEnt2d::operator = (const GeSplineEnt2d& spline)
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
