#include "GeNurbSurface.h"

#include "GeInterval.h"
#include "GeNurbCurve3d.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

GeNurbSurface::GeNurbSurface()
    : m_degreeU(3)
    , m_degreeV(3)
    , m_propsU(Ge::kOpen)
    , m_propsV(Ge::kOpen)
    , m_numCtrlU(0)
    , m_numCtrlV(0)
    , m_tol(GeContext::gTol)
{
    ensureDefaults();
}

GeNurbSurface::GeNurbSurface(
    int degreeInU,
    int degreeInV,
    int propsInU,
    int propsInV,
    int numControlPointsInU,
    int numControlPointsInV,
    const GePoint3dArray& controlPoints,
    const GeDoubleArray& weights,
    const GeKnotVector& uKnots,
    const GeKnotVector& vKnots,
    const GeTol& tol)
    : m_degreeU(3)
    , m_degreeV(3)
    , m_propsU(Ge::kOpen)
    , m_propsV(Ge::kOpen)
    , m_numCtrlU(0)
    , m_numCtrlV(0)
    , m_tol(tol)
{
    set(degreeInU, degreeInV, propsInU, propsInV, numControlPointsInU, numControlPointsInV, controlPoints, weights, uKnots, vKnots, tol);
}

GeNurbSurface::GeNurbSurface(const GeNurbSurface& source)
{
    *this = source;
}

bool GeNurbSurface::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kSurface || entType == this->type();
}

Ge::EntityId GeNurbSurface::type() const
{
    return Ge::kNurbSurface;
}

GeNurbSurface* GeNurbSurface::copy() const
{
    return new GeNurbSurface(*this);
}

bool GeNurbSurface::operator == (const GeNurbSurface& entity) const
{
    return isEqualTo(entity);
}

bool GeNurbSurface::operator != (const GeNurbSurface& entity) const
{
    return !isEqualTo(entity);
}

bool GeNurbSurface::isEqualTo(const GeNurbSurface& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeNurbSurface::isEqualTo(const GeNurbSurface& entity, const GeTol& tol) const
{
    if (m_degreeU != entity.m_degreeU || m_degreeV != entity.m_degreeV || m_numCtrlU != entity.m_numCtrlU || m_numCtrlV != entity.m_numCtrlV) {
        return false;
    }
    if (m_controlPoints.length() != entity.m_controlPoints.length()) {
        return false;
    }
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        if (!m_controlPoints[i].isEqualTo(entity.m_controlPoints[i], tol)) {
            return false;
        }
    }
    return true;
}

GeNurbSurface& GeNurbSurface::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].transformBy(xfm);
    }
    return *this;
}

GeNurbSurface& GeNurbSurface::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i] += translateVec;
    }
    return *this;
}

GeNurbSurface& GeNurbSurface::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeNurbSurface& GeNurbSurface::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeNurbSurface& GeNurbSurface::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].mirror(plane);
    }
    return *this;
}

GeNurbSurface& GeNurbSurface::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeNurbSurface& GeNurbSurface::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeNurbSurface::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeNurbSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d uv = paramOf(pnt, tol);
    return evalPoint(uv).isEqualTo(pnt, tol);
}

GeNurbSurface& GeNurbSurface::operator = (const GeNurbSurface& nurb)
{
    if (this == &nurb) {
        return *this;
    }
    m_degreeU = nurb.m_degreeU;
    m_degreeV = nurb.m_degreeV;
    m_propsU = nurb.m_propsU;
    m_propsV = nurb.m_propsV;
    m_numCtrlU = nurb.m_numCtrlU;
    m_numCtrlV = nurb.m_numCtrlV;
    m_controlPoints = nurb.m_controlPoints;
    m_weights = nurb.m_weights;
    m_uKnots = nurb.m_uKnots;
    m_vKnots = nurb.m_vKnots;
    m_tol = nurb.m_tol;
    return *this;
}

bool GeNurbSurface::isRationalInU() const
{
    return m_weights.length() == m_controlPoints.length() && m_weights.length() > 0;
}

bool GeNurbSurface::isPeriodicInU(double& period) const
{
    period = 0.0;
    if ((m_propsU & Ge::kPeriodic) == 0 || m_uKnots.length() < 2) {
        return false;
    }
    period = m_uKnots[m_uKnots.length() - 1] - m_uKnots[0];
    return true;
}

bool GeNurbSurface::isRationalInV() const
{
    return isRationalInU();
}

bool GeNurbSurface::isPeriodicInV(double& period) const
{
    period = 0.0;
    if ((m_propsV & Ge::kPeriodic) == 0 || m_vKnots.length() < 2) {
        return false;
    }
    period = m_vKnots[m_vKnots.length() - 1] - m_vKnots[0];
    return true;
}

int GeNurbSurface::singularityInU() const
{
    return m_propsU & (Ge::kNoPoles | Ge::kPoleAtMin | Ge::kPoleAtMax | Ge::kPoleAtBoth);
}

int GeNurbSurface::singularityInV() const
{
    return m_propsV & (Ge::kNoPoles | Ge::kPoleAtMin | Ge::kPoleAtMax | Ge::kPoleAtBoth);
}

int GeNurbSurface::degreeInU() const
{
    return m_degreeU;
}

int GeNurbSurface::numControlPointsInU() const
{
    return m_numCtrlU;
}

int GeNurbSurface::degreeInV() const
{
    return m_degreeV;
}

int GeNurbSurface::numControlPointsInV() const
{
    return m_numCtrlV;
}

void GeNurbSurface::getControlPoints(GePoint3dArray& controlPoints) const
{
    controlPoints = m_controlPoints;
}

bool GeNurbSurface::getWeights(GeDoubleArray& weights) const
{
    weights = m_weights;
    return isRationalInU();
}

int GeNurbSurface::numKnotsInU() const
{
    return m_uKnots.length();
}

void GeNurbSurface::getUKnots(GeKnotVector& uKnots) const
{
    uKnots = m_uKnots;
}

int GeNurbSurface::numKnotsInV() const
{
    return m_vKnots.length();
}

void GeNurbSurface::getVKnots(GeKnotVector& vKnots) const
{
    vKnots = m_vKnots;
}

void GeNurbSurface::getDefinition(
    int& degreeInU,
    int& degreeInV,
    int& propsInU,
    int& propsInV,
    int& numControlPointsInU,
    int& numControlPointsInV,
    GePoint3dArray& controlPoints,
    GeDoubleArray& weights,
    GeKnotVector& uKnots,
    GeKnotVector& vKnots) const
{
    degreeInU = m_degreeU;
    degreeInV = m_degreeV;
    propsInU = m_propsU;
    propsInV = m_propsV;
    numControlPointsInU = m_numCtrlU;
    numControlPointsInV = m_numCtrlV;
    controlPoints = m_controlPoints;
    weights = m_weights;
    uKnots = m_uKnots;
    vKnots = m_vKnots;
}

GeNurbSurface& GeNurbSurface::set(
    int degreeInU,
    int degreeInV,
    int propsInU,
    int propsInV,
    int numControlPointsInU,
    int numControlPointsInV,
    const GePoint3dArray& controlPoints,
    const GeDoubleArray& weights,
    const GeKnotVector& uKnots,
    const GeKnotVector& vKnots,
    const GeTol& tol)
{
    m_degreeU = degreeInU;
    m_degreeV = degreeInV;
    m_propsU = propsInU;
    m_propsV = propsInV;
    m_numCtrlU = numControlPointsInU;
    m_numCtrlV = numControlPointsInV;
    m_controlPoints = controlPoints;
    m_weights = weights;
    m_uKnots = uKnots;
    m_vKnots = vKnots;
    m_tol = tol;
    ensureDefaults();
    return *this;
}

GeNurbSurface& GeNurbSurface::setFitData(
    const GePoint3dArray& fitPoints,
    const GeVector3dArray&,
    const GeVector3dArray&,
    const GeVector3dArray&,
    const GeKnotVector& uKnots,
    const GeKnotVector& vKnots,
    const GeTol& tol)
{
    int count = fitPoints.length();
    if (count < 4) {
        return *this;
    }

    int side = static_cast<int>(std::sqrt(static_cast<double>(count)) + 0.5);
    if (side * side != count) {
        return *this;
    }

    m_degreeU = 3;
    m_degreeV = 3;
    m_propsU = Ge::kOpen;
    m_propsV = Ge::kOpen;
    m_numCtrlU = side;
    m_numCtrlV = side;
    m_controlPoints = fitPoints;
    m_weights.removeAll();
    m_uKnots = uKnots;
    m_vKnots = vKnots;
    m_tol = tol;
    ensureDefaults();
    return *this;
}

void GeNurbSurface::computeVIsoLine(double V, GeNurbCurve3d& isoline) const
{
    if (m_numCtrlU < 2 || m_numCtrlV < 2) {
        return;
    }

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);
    double rv = (rangeV.length() > 0.0) ? (V - rangeV.lowerBound()) / rangeV.length() : 0.0;
    if (rv < 0.0) rv = 0.0;
    if (rv > 1.0) rv = 1.0;
    int j = static_cast<int>(rv * (m_numCtrlV - 1) + 0.5);
    if (j < 0) j = 0;
    if (j >= m_numCtrlV) j = m_numCtrlV - 1;

    GePoint3dArray cps;
    for (int i = 0; i < m_numCtrlU; ++i) {
        cps.append(controlPointAt(i, j));
    }

    GeKnotVector knots;
    for (int i = 0; i < m_numCtrlU + 2; ++i) {
        knots.append(static_cast<double>(i));
    }
    GeNurbCurve3d curve(1, knots, cps, false);
    isoline = curve;
}

void GeNurbSurface::computeUIsoLine(double U, GeNurbCurve3d& isoline) const
{
    if (m_numCtrlU < 2 || m_numCtrlV < 2) {
        return;
    }

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);
    double ru = (rangeU.length() > 0.0) ? (U - rangeU.lowerBound()) / rangeU.length() : 0.0;
    if (ru < 0.0) ru = 0.0;
    if (ru > 1.0) ru = 1.0;
    int i = static_cast<int>(ru * (m_numCtrlU - 1) + 0.5);
    if (i < 0) i = 0;
    if (i >= m_numCtrlU) i = m_numCtrlU - 1;

    GePoint3dArray cps;
    for (int j = 0; j < m_numCtrlV; ++j) {
        cps.append(controlPointAt(i, j));
    }

    GeKnotVector knots;
    for (int k = 0; k < m_numCtrlV + 2; ++k) {
        knots.append(static_cast<double>(k));
    }
    GeNurbCurve3d curve(1, knots, cps, false);
    isoline = curve;
}

GePoint2d GeNurbSurface::paramOfPrec(const GePoint3d& point, const GeTol& tol) const
{
    return paramOf(point, tol);
}

int GeNurbSurface::loc(int i, int j) const
{
    return j * m_numCtrlU + i;
}

GePoint2d GeNurbSurface::paramOf(const GePoint3d& pnt, const GeTol&) const
{
    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);

    const int sample = 40;
    double bestU = rangeU.lowerBound();
    double bestV = rangeV.lowerBound();
    double bestD = evalPoint(GePoint2d(bestU, bestV)).distanceTo(pnt);

    for (int i = 0; i <= sample; ++i) {
        double ru = static_cast<double>(i) / static_cast<double>(sample);
        double u = rangeU.lowerBound() + (rangeU.upperBound() - rangeU.lowerBound()) * ru;
        for (int j = 0; j <= sample; ++j) {
            double rv = static_cast<double>(j) / static_cast<double>(sample);
            double v = rangeV.lowerBound() + (rangeV.upperBound() - rangeV.lowerBound()) * rv;
            double d = evalPoint(GePoint2d(u, v)).distanceTo(pnt);
            if (d < bestD) {
                bestD = d;
                bestU = u;
                bestV = v;
            }
        }
    }
    return GePoint2d(bestU, bestV);
}

GePoint3d GeNurbSurface::evalPoint(const GePoint2d& param) const
{
    if (m_numCtrlU < 2 || m_numCtrlV < 2) {
        return GePoint3d::kOrigin;
    }

    GeInterval rangeU;
    GeInterval rangeV;
    getEnvelope(rangeU, rangeV);

    double tu = (rangeU.length() > 0.0) ? (param.x - rangeU.lowerBound()) / rangeU.length() : 0.0;
    double tv = (rangeV.length() > 0.0) ? (param.y - rangeV.lowerBound()) / rangeV.length() : 0.0;
    if (tu < 0.0) tu = 0.0;
    if (tu > 1.0) tu = 1.0;
    if (tv < 0.0) tv = 0.0;
    if (tv > 1.0) tv = 1.0;

    double fu = tu * (m_numCtrlU - 1);
    double fv = tv * (m_numCtrlV - 1);
    int i0 = static_cast<int>(fu);
    int j0 = static_cast<int>(fv);
    int i1 = (i0 + 1 < m_numCtrlU) ? i0 + 1 : i0;
    int j1 = (j0 + 1 < m_numCtrlV) ? j0 + 1 : j0;
    double a = fu - i0;
    double b = fv - j0;

    GePoint3d p00 = controlPointAt(i0, j0);
    GePoint3d p10 = controlPointAt(i1, j0);
    GePoint3d p01 = controlPointAt(i0, j1);
    GePoint3d p11 = controlPointAt(i1, j1);

    GeVector3d v00 = p00 - GePoint3d::kOrigin;
    GeVector3d v10 = p10 - GePoint3d::kOrigin;
    GeVector3d v01 = p01 - GePoint3d::kOrigin;
    GeVector3d v11 = p11 - GePoint3d::kOrigin;
    GeVector3d v0 = v00 * (1.0 - a) + v10 * a;
    GeVector3d v1 = v01 * (1.0 - a) + v11 * a;
    GeVector3d v = v0 * (1.0 - b) + v1 * b;
    return GePoint3d::kOrigin + v;
}

void GeNurbSurface::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    if (m_uKnots.length() >= 2) {
        rangeU.set(m_uKnots[0], m_uKnots[m_uKnots.length() - 1]);
    }
    else {
        rangeU.set(0.0, 1.0);
    }

    if (m_vKnots.length() >= 2) {
        rangeV.set(m_vKnots[0], m_vKnots[m_vKnots.length() - 1]);
    }
    else {
        rangeV.set(0.0, 1.0);
    }
}

void GeNurbSurface::ensureDefaults()
{
    if (m_numCtrlU < 2) m_numCtrlU = 2;
    if (m_numCtrlV < 2) m_numCtrlV = 2;
    int expect = m_numCtrlU * m_numCtrlV;

    if (m_controlPoints.length() != expect) {
        m_controlPoints.removeAll();
        for (int j = 0; j < m_numCtrlV; ++j) {
            for (int i = 0; i < m_numCtrlU; ++i) {
                m_controlPoints.append(GePoint3d(static_cast<double>(i), static_cast<double>(j), 0.0));
            }
        }
    }

    if (m_weights.length() > 0 && m_weights.length() != expect) {
        m_weights.removeAll();
    }

    if (m_uKnots.length() < 2) {
        m_uKnots.removeAll();
        for (int i = 0; i < m_numCtrlU + m_degreeU + 1; ++i) {
            m_uKnots.append(static_cast<double>(i));
        }
    }
    if (m_vKnots.length() < 2) {
        m_vKnots.removeAll();
        for (int i = 0; i < m_numCtrlV + m_degreeV + 1; ++i) {
            m_vKnots.append(static_cast<double>(i));
        }
    }
}

GePoint3d GeNurbSurface::controlPointAt(int i, int j) const
{
    int idx = loc(i, j);
    if (idx < 0 || idx >= m_controlPoints.length()) {
        return GePoint3d::kOrigin;
    }
    return m_controlPoints[idx];
}