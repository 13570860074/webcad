#include "GeCubicSplineCurve3d.h"

#include "GeInterval.h"
#include "GePlane.h"

GeCubicSplineCurve3d::GeCubicSplineCurve3d()
    : GeSplineEnt3d()
    , m_isPeriodic(false)
{
    m_fitPoints.append(GePoint3d::kOrigin);
    m_fitPoints.append(GePoint3d(1.0, 0.0, 0.0));
    m_firstDerivs.append(GeVector3d::kXAxis);
    m_firstDerivs.append(GeVector3d::kXAxis);
    rebuildBaseData();
}

GeCubicSplineCurve3d::GeCubicSplineCurve3d(const GeCubicSplineCurve3d& source)
    : GeSplineEnt3d(source)
{
    *this = source;
}

GeCubicSplineCurve3d::GeCubicSplineCurve3d(const GePoint3dArray& fitPnts, const GeTol&)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
{
    m_fitPoints = fitPnts;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint3d::kOrigin);
        m_fitPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }
    m_firstDerivs.removeAll();
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_firstDerivs.append(GeVector3d::kXAxis);
    }
    rebuildBaseData();
}

GeCubicSplineCurve3d::GeCubicSplineCurve3d(const GePoint3dArray& fitPnts, const GeVector3d& startDeriv, const GeVector3d& endDeriv, const GeTol&)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
{
    m_fitPoints = fitPnts;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint3d::kOrigin);
        m_fitPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }

    m_firstDerivs.removeAll();
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_firstDerivs.append(GeVector3d::kXAxis);
    }
    if (m_firstDerivs.length() > 0) {
        m_firstDerivs[0] = startDeriv;
        m_firstDerivs[m_firstDerivs.length() - 1] = endDeriv;
    }

    rebuildBaseData();
}

GeCubicSplineCurve3d::GeCubicSplineCurve3d(const GeCurve3d& curve, double epsilon)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
{
    GeInterval range;
    curve.getInterval(range);

    int sampleCount = 32;
    if (epsilon > GeContext::gTol.equalPoint()) {
        double curveLength = curve.length();
        if (curveLength > GeContext::gTol.equalPoint()) {
            sampleCount = static_cast<int>(curveLength / epsilon) + 1;
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
        m_fitPoints.append(GePoint3d::kOrigin);
        m_fitPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }

    m_firstDerivs.removeAll();
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_firstDerivs.append(GeVector3d::kXAxis);
    }
    rebuildBaseData();
}

GeCubicSplineCurve3d::GeCubicSplineCurve3d(const GeKnotVector& knots, const GePoint3dArray& fitPnts, const GeVector3dArray& firstDerivs, bool isPeriodic)
    : GeSplineEnt3d()
    , m_isPeriodic(isPeriodic)
{
    m_fitPoints = fitPnts;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint3d::kOrigin);
        m_fitPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }

    m_firstDerivs = firstDerivs;
    while (m_firstDerivs.length() < m_fitPoints.length()) {
        m_firstDerivs.append(GeVector3d::kXAxis);
    }

    m_knots = knots;
    rebuildBaseData();
}

bool GeCubicSplineCurve3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kCurve3d || entType == Ge::kSplineEnt3d || entType == this->type();
}

Ge::EntityId GeCubicSplineCurve3d::type() const
{
    return Ge::kCubicSplineCurve3d;
}

GeCubicSplineCurve3d* GeCubicSplineCurve3d::copy() const
{
    return new GeCubicSplineCurve3d(*this);
}

bool GeCubicSplineCurve3d::operator == (const GeCubicSplineCurve3d& entity) const
{
    return isEqualTo(entity);
}

bool GeCubicSplineCurve3d::operator != (const GeCubicSplineCurve3d& entity) const
{
    return !isEqualTo(entity);
}

bool GeCubicSplineCurve3d::isEqualTo(const GeCubicSplineCurve3d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeCubicSplineCurve3d::isEqualTo(const GeCubicSplineCurve3d& entity, const GeTol& tol) const
{
    if (!GeSplineEnt3d::isEqualTo(entity, tol) || m_isPeriodic != entity.m_isPeriodic) {
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

GeCubicSplineCurve3d& GeCubicSplineCurve3d::transformBy(const GeMatrix3d& xfm)
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

GeCubicSplineCurve3d& GeCubicSplineCurve3d::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i] += translateVec;
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        m_firstDerivs[i].rotateBy(angle, vec);
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].mirror(plane);
    }
    for (int i = 0; i < m_firstDerivs.length(); ++i) {
        m_firstDerivs[i].mirror(plane.normal());
    }
    rebuildBaseData();
    return *this;
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
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

bool GeCubicSplineCurve3d::isOn(const GePoint3d& pnt) const
{
    return GeSplineEnt3d::isOn(pnt);
}

bool GeCubicSplineCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return GeSplineEnt3d::isOn(pnt, tol);
}

int GeCubicSplineCurve3d::numFitPoints() const
{
    return m_fitPoints.length();
}

GePoint3d GeCubicSplineCurve3d::fitPointAt(int fitPointIndex) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_fitPoints.length()) {
        return GePoint3d::kOrigin;
    }
    return m_fitPoints[fitPointIndex];
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::setFitPointAt(int fitPointIndex, const GePoint3d& point)
{
    if (fitPointIndex >= 0 && fitPointIndex < m_fitPoints.length()) {
        m_fitPoints[fitPointIndex] = point;
        rebuildBaseData();
    }
    return *this;
}

GeVector3d GeCubicSplineCurve3d::firstDerivAt(int fitPointIndex) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_firstDerivs.length()) {
        return GeVector3d::kXAxis;
    }
    return m_firstDerivs[fitPointIndex];
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::setFirstDerivAt(int fitPointIndex, const GeVector3d& deriv)
{
    if (fitPointIndex >= 0 && fitPointIndex < m_firstDerivs.length()) {
        m_firstDerivs[fitPointIndex] = deriv;
    }
    return *this;
}

GeCubicSplineCurve3d& GeCubicSplineCurve3d::operator = (const GeCubicSplineCurve3d& spline)
{
    if (this == &spline) {
        return *this;
    }

    GeSplineEnt3d::operator=(spline);
    m_fitPoints = spline.m_fitPoints;
    m_firstDerivs = spline.m_firstDerivs;
    m_isPeriodic = spline.m_isPeriodic;
    return *this;
}

void GeCubicSplineCurve3d::rebuildBaseData()
{
    m_hasFitData = true;
    m_degree = 3;
    m_controlPoints = m_fitPoints;

    if (m_controlPoints.length() < 2) {
        m_controlPoints.removeAll();
        m_controlPoints.append(GePoint3d::kOrigin);
        m_controlPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }

    if (m_knots.length() != m_controlPoints.length()) {
        m_knots.removeAll();
        for (int i = 0; i < m_controlPoints.length(); ++i) {
            m_knots.append(static_cast<double>(i));
        }
    }
}