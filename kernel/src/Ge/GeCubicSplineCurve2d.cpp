#include "GeCubicSplineCurve2d.h"

#include "GeInterval.h"
#include "GeLine2d.h"

GeCubicSplineCurve2d::GeCubicSplineCurve2d()
    : GeSplineEnt2d()
    , m_isPeriodic(false)
{
    m_fitPoints.append(GePoint2d::kOrigin);
    m_fitPoints.append(GePoint2d(1.0, 0.0));
    m_firstDerivs.append(GeVector2d::kXAxis);
    m_firstDerivs.append(GeVector2d::kXAxis);
    rebuildBaseData();
}

GeCubicSplineCurve2d::GeCubicSplineCurve2d(const GeCubicSplineCurve2d& source)
    : GeSplineEnt2d(source)
{
    *this = source;
}

GeCubicSplineCurve2d::GeCubicSplineCurve2d(const GePoint2dArray& fitPnts, const GeTol&)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
{
    m_fitPoints = fitPnts;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint2d::kOrigin);
        m_fitPoints.append(GePoint2d(1.0, 0.0));
    }
    m_firstDerivs.removeAll();
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_firstDerivs.append(GeVector2d::kXAxis);
    }
    rebuildBaseData();
}

GeCubicSplineCurve2d::GeCubicSplineCurve2d(const GePoint2dArray& fitPnts, const GeVector2d& startDeriv, const GeVector2d& endDeriv, const GeTol&)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
{
    m_fitPoints = fitPnts;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint2d::kOrigin);
        m_fitPoints.append(GePoint2d(1.0, 0.0));
    }

    m_firstDerivs.removeAll();
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_firstDerivs.append(GeVector2d::kXAxis);
    }
    if (m_firstDerivs.length() > 0) {
        m_firstDerivs[0] = startDeriv;
        m_firstDerivs[m_firstDerivs.length() - 1] = endDeriv;
    }

    rebuildBaseData();
}

GeCubicSplineCurve2d::GeCubicSplineCurve2d(const GeCurve2d& curve, double tol)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
{
    GeInterval range;
    curve.getInterval(range);

    int sampleCount = 32;
    if (tol > GeContext::gTol.equalPoint()) {
        double curveLength = curve.length();
        if (curveLength > GeContext::gTol.equalPoint()) {
            sampleCount = static_cast<int>(curveLength / tol) + 1;
        }
    }
    if (sampleCount < 2) {
        sampleCount = 2;
    }
    if (sampleCount > 512) {
        sampleCount = 512;
    }

    double lo = range.lowerBound();
    double hi = range.upperBound();
    for (int i = 0; i < sampleCount; ++i) {
        double r = static_cast<double>(i) / static_cast<double>(sampleCount - 1);
        m_fitPoints.append(curve.evalPoint(lo + (hi - lo) * r));
    }

    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint2d::kOrigin);
        m_fitPoints.append(GePoint2d(1.0, 0.0));
    }

    m_firstDerivs.removeAll();
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_firstDerivs.append(GeVector2d::kXAxis);
    }
    rebuildBaseData();
}

GeCubicSplineCurve2d::GeCubicSplineCurve2d(const GeKnotVector& knots, const GePoint2dArray& fitPnts, const GeVector2dArray& firstDerivs, bool isPeriodic)
    : GeSplineEnt2d()
    , m_isPeriodic(isPeriodic)
{
    m_fitPoints = fitPnts;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint2d::kOrigin);
        m_fitPoints.append(GePoint2d(1.0, 0.0));
    }

    m_firstDerivs = firstDerivs;
    while (m_firstDerivs.length() < m_fitPoints.length()) {
        m_firstDerivs.append(GeVector2d::kXAxis);
    }

    m_knots = knots;
    rebuildBaseData();
}

bool GeCubicSplineCurve2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == Ge::kSplineEnt2d || entType == this->type();
}

Ge::EntityId GeCubicSplineCurve2d::type() const
{
    return Ge::kCubicSplineCurve2d;
}

GeCubicSplineCurve2d* GeCubicSplineCurve2d::copy() const
{
    return new GeCubicSplineCurve2d(*this);
}

bool GeCubicSplineCurve2d::operator == (const GeCubicSplineCurve2d& entity) const
{
    return isEqualTo(entity);
}

bool GeCubicSplineCurve2d::operator != (const GeCubicSplineCurve2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeCubicSplineCurve2d::isEqualTo(const GeCubicSplineCurve2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeCubicSplineCurve2d::isEqualTo(const GeCubicSplineCurve2d& entity, const GeTol& tol) const
{
    if (!GeSplineEnt2d::isEqualTo(entity, tol) || m_isPeriodic != entity.m_isPeriodic) {
        return false;
    }
    if (m_fitPoints.length() != entity.m_fitPoints.length() || m_firstDerivs.length() != entity.m_firstDerivs.length()) {
        return false;
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        if (!m_fitPoints[i].isEqualTo(entity.m_fitPoints[i], tol)) {
            return false;
        }
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        if (!m_firstDerivs[i].isEqualTo(entity.m_firstDerivs[i], tol)) {
            return false;
        }
    }
    return true;
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].transformBy(xfm);
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        m_firstDerivs[i].transformBy(xfm);
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i] += translateVec;
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::rotateBy(double angle)
{
    return rotateBy(angle, GePoint2d::kOrigin);
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].rotateBy(angle, wrtPoint);
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        m_firstDerivs[i].rotateBy(angle);
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].mirror(line);
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        m_firstDerivs[i].mirror(line.direction());
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].scaleBy(scaleFactor, wrtPoint);
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        m_firstDerivs[i] *= scaleFactor;
    }
    rebuildBaseData();
    return *this;
}

bool GeCubicSplineCurve2d::isOn(const GePoint2d& pnt) const
{
    return GeSplineEnt2d::isOn(pnt);
}

bool GeCubicSplineCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    return GeSplineEnt2d::isOn(pnt, tol);
}

int GeCubicSplineCurve2d::numFitPoints() const
{
    return m_fitPoints.length();
}

GePoint2d GeCubicSplineCurve2d::fitPointAt(int fitPointIndex) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_fitPoints.length()) {
        return GePoint2d::kOrigin;
    }
    return m_fitPoints[fitPointIndex];
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::setFitPointAt(int fitPointIndex, const GePoint2d& point)
{
    if (fitPointIndex >= 0 && fitPointIndex < m_fitPoints.length()) {
        m_fitPoints[fitPointIndex] = point;
        rebuildBaseData();
    }
    return *this;
}

GeVector2d GeCubicSplineCurve2d::firstDerivAt(int fitPointIndex) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_firstDerivs.length()) {
        return GeVector2d::kXAxis;
    }
    return m_firstDerivs[fitPointIndex];
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::setFirstDerivAt(int fitPointIndex, const GeVector2d& deriv)
{
    if (fitPointIndex >= 0 && fitPointIndex < m_firstDerivs.length()) {
        m_firstDerivs[fitPointIndex] = deriv;
    }
    return *this;
}

GeCubicSplineCurve2d& GeCubicSplineCurve2d::operator = (const GeCubicSplineCurve2d& spline)
{
    if (this == &spline) {
        return *this;
    }

    GeSplineEnt2d::operator=(spline);
    m_fitPoints = spline.m_fitPoints;
    m_firstDerivs = spline.m_firstDerivs;
    m_isPeriodic = spline.m_isPeriodic;
    return *this;
}

void GeCubicSplineCurve2d::rebuildBaseData()
{
    m_hasFitData = true;
    m_degree = 3;
    m_controlPoints = m_fitPoints;

    if (m_controlPoints.length() < 2) {
        m_controlPoints.removeAll();
        m_controlPoints.append(GePoint2d::kOrigin);
        m_controlPoints.append(GePoint2d(1.0, 0.0));
    }

    if (m_knots.length() != m_controlPoints.length()) {
        m_knots.removeAll();
        for (int i = 0; i < m_controlPoints.length(); ++i) {
            m_knots.append(static_cast<double>(i));
        }
    }
}