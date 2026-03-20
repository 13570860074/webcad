#include "GeNurbCurve3d.h"

#include "GeEllipArc3d.h"
#include "GeLineSeg3d.h"
#include "GePlane.h"
#include "GePolyline3d.h"
#include <cmath>

GeNurbCurve3d::GeNurbCurve3d()
    : GeSplineEnt3d()
    , m_isPeriodic(false)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector3d::kXAxis)
    , m_endTangent(GeVector3d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    ensureDefaultData();
}

GeNurbCurve3d::GeNurbCurve3d(const GeNurbCurve3d& source)
    : GeSplineEnt3d(source)
{
    *this = source;
}

GeNurbCurve3d::GeNurbCurve3d(int degree, const GeKnotVector& knots, const GePoint3dArray& controlPoints, bool isPeriodic)
    : GeSplineEnt3d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector3d::kXAxis)
    , m_endTangent(GeVector3d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    m_degree = degree;
    m_controlPoints = controlPoints;
    m_knots = knots;
    m_isRational = false;
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}

GeNurbCurve3d::GeNurbCurve3d(int degree, const GeKnotVector& knots, const GePoint3dArray& controlPoints, const GeDoubleArray& weights, bool isPeriodic)
    : GeNurbCurve3d(degree, knots, controlPoints, isPeriodic)
{
    m_weights = weights;
    while (m_weights.length() < m_controlPoints.length()) {
        m_weights.append(1.0);
    }
    m_isRational = m_weights.length() > 0;
}

GeNurbCurve3d::GeNurbCurve3d(int degree, const GeKnotVector& knots, const GePoint3d* controlPoints, unsigned int numControlPoints, const double* weights, unsigned int numWeights, bool isPeriodic)
    : GeSplineEnt3d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector3d::kXAxis)
    , m_endTangent(GeVector3d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    m_degree = degree;
    for (unsigned int i = 0; i < numControlPoints; ++i) {
        m_controlPoints.append(controlPoints[i]);
    }
    for (unsigned int i = 0; i < numWeights; ++i) {
        m_weights.append(weights[i]);
    }
    m_knots = knots;
    m_isRational = numWeights > 0;
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}

GeNurbCurve3d::GeNurbCurve3d(int degree, const GePolyline3d& fitPolyline, bool isPeriodic)
    : GeSplineEnt3d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
{
    m_degree = degree;
    for (int i = 0; i < fitPolyline.numFitPoints(); ++i) {
        m_fitPoints.append(fitPolyline.fitPointAt(i));
    }
    setFitMeta(m_fitPoints, GeContext::gTol, false, GeVector3d::kXAxis, GeVector3d::kXAxis, false, false, Ge::kChord);
}

GeNurbCurve3d::GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeVector3d& startTangent, const GeVector3d& endTangent, bool startTangentDefined, bool endTangentDefined, const GeTol& fitTol)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
    , m_evalMode(false)
{
    setFitMeta(fitPoints, fitTol, true, startTangent, endTangent, startTangentDefined, endTangentDefined, Ge::kChord);
}

GeNurbCurve3d::GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeTol& fitTolerance)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
    , m_evalMode(false)
{
    setFitMeta(fitPoints, fitTolerance, false, GeVector3d::kXAxis, GeVector3d::kXAxis, false, false, Ge::kChord);
}

GeNurbCurve3d::GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeVector3dArray& fitTangents, const GeTol& fitTolerance, bool isPeriodic)
    : GeSplineEnt3d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
{
    GeVector3d startTangent = fitTangents.length() > 0 ? fitTangents[0] : GeVector3d::kXAxis;
    GeVector3d endTangent = fitTangents.length() > 0 ? fitTangents[fitTangents.length() - 1] : GeVector3d::kXAxis;
    setFitMeta(fitPoints, fitTolerance, fitTangents.length() > 0, startTangent, endTangent, fitTangents.length() > 0, fitTangents.length() > 0, Ge::kChord);
}

GeNurbCurve3d::GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeVector3d& startTangent, const GeVector3d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam, const GeTol& fitTolerance)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
    , m_evalMode(false)
{
    setFitMeta(fitPoints, fitTolerance, true, startTangent, endTangent, startTangentDefined, endTangentDefined, knotParam);
}

GeNurbCurve3d::GeNurbCurve3d(const GeEllipArc3d& ellipse, int numSpans)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector3d::kXAxis)
    , m_endTangent(GeVector3d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    if (numSpans <= 0) {
        numSpans = 16;
    }
    m_degree = 2;
    m_controlPoints = GeEllipArc3d::toLineSegment(ellipse.center(), ellipse.majorAxis(), ellipse.minorAxis(), ellipse.startAng(), ellipse.endAng(), numSpans + 1);
    m_isRational = false;
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}

GeNurbCurve3d::GeNurbCurve3d(const GeLineSeg3d& lineSeg)
    : GeSplineEnt3d()
    , m_isPeriodic(false)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector3d::kXAxis)
    , m_endTangent(GeVector3d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    m_degree = 1;
    m_controlPoints.append(lineSeg.startPoint());
    m_controlPoints.append(lineSeg.endPoint());
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}

bool GeNurbCurve3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kCurve3d || entType == Ge::kSplineEnt3d || entType == this->type();
}

Ge::EntityId GeNurbCurve3d::type() const
{
    return Ge::kNurbCurve3d;
}

GeNurbCurve3d* GeNurbCurve3d::copy() const
{
    return new GeNurbCurve3d(*this);
}

bool GeNurbCurve3d::operator == (const GeNurbCurve3d& entity) const
{
    return isEqualTo(entity);
}

bool GeNurbCurve3d::operator != (const GeNurbCurve3d& entity) const
{
    return !isEqualTo(entity);
}

bool GeNurbCurve3d::isEqualTo(const GeNurbCurve3d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeNurbCurve3d::isEqualTo(const GeNurbCurve3d& entity, const GeTol& tol) const
{
    if (!GeSplineEnt3d::isEqualTo(entity, tol) || m_isPeriodic != entity.m_isPeriodic || m_evalMode != entity.m_evalMode || m_knotParam != entity.m_knotParam) {
        return false;
    }
    if (m_weights.length() != entity.m_weights.length()) {
        return false;
    }
    for (int i = 0; i < m_weights.length(); ++i) {
        if (std::fabs(m_weights[i] - entity.m_weights[i]) > tol.equalPoint()) {
            return false;
        }
    }
    return true;
}

GeNurbCurve3d& GeNurbCurve3d::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].transformBy(xfm);
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].transformBy(xfm);
    }
    if (m_fitTangentsExist) {
        m_startTangent.transformBy(xfm);
        m_endTangent.transformBy(xfm);
    }
    return *this;
}

GeNurbCurve3d& GeNurbCurve3d::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i] += translateVec;
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i] += translateVec;
    }
    return *this;
}

GeNurbCurve3d& GeNurbCurve3d::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeNurbCurve3d& GeNurbCurve3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    if (m_fitTangentsExist) {
        m_startTangent.rotateBy(angle, vec);
        m_endTangent.rotateBy(angle, vec);
    }
    return *this;
}

GeNurbCurve3d& GeNurbCurve3d::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].mirror(plane);
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].mirror(plane);
    }
    if (m_fitTangentsExist) {
        m_startTangent.mirror(plane.normal());
        m_endTangent.mirror(plane.normal());
    }
    return *this;
}

GeNurbCurve3d& GeNurbCurve3d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeNurbCurve3d& GeNurbCurve3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].scaleBy(scaleFactor, wrtPoint);
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].scaleBy(scaleFactor, wrtPoint);
    }
    if (m_fitTangentsExist) {
        m_startTangent *= scaleFactor;
        m_endTangent *= scaleFactor;
    }
    return *this;
}

bool GeNurbCurve3d::isOn(const GePoint3d& pnt) const
{
    return GeSplineEnt3d::isOn(pnt);
}

bool GeNurbCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return GeSplineEnt3d::isOn(pnt, tol);
}

int GeNurbCurve3d::numFitPoints() const
{
    return m_fitPoints.length();
}

bool GeNurbCurve3d::getFitKnotParameterization(Ge::GeKnotParameterization& knotParam) const
{
    if (!m_hasFitData) {
        return false;
    }
    knotParam = m_knotParam;
    return true;
}

bool GeNurbCurve3d::getFitPointAt(int fitPointIndex, GePoint3d& point) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_fitPoints.length()) {
        return false;
    }
    point = m_fitPoints[fitPointIndex];
    return true;
}

bool GeNurbCurve3d::getFitTolerance(GeTol& fitTolerance) const
{
    if (!m_hasFitData) {
        return false;
    }
    fitTolerance = m_fitTolerance;
    return true;
}

bool GeNurbCurve3d::getFitTangents(GeVector3d& startTangent, GeVector3d& endTangent) const
{
    if (!m_hasFitData || !m_fitTangentsExist) {
        return false;
    }
    startTangent = m_startTangent;
    endTangent = m_endTangent;
    return true;
}

bool GeNurbCurve3d::getFitTangents(GeVector3d& startTangent, GeVector3d& endTangent, bool& startTangentDefined, bool& endTangentDefined) const
{
    if (!getFitTangents(startTangent, endTangent)) {
        return false;
    }
    startTangentDefined = m_startTangentDefined;
    endTangentDefined = m_endTangentDefined;
    return true;
}

bool GeNurbCurve3d::getFitData(GePoint3dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector3d& startTangent, GeVector3d& endTangent) const
{
    Ge::GeKnotParameterization knotParam;
    return getFitData(fitPoints, fitTolerance, tangentsExist, startTangent, endTangent, knotParam);
}

bool GeNurbCurve3d::getFitData(GePoint3dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector3d& startTangent, GeVector3d& endTangent, Ge::GeKnotParameterization& knotParam) const
{
    if (!m_hasFitData) {
        return false;
    }
    fitPoints = m_fitPoints;
    fitTolerance = m_fitTolerance;
    tangentsExist = m_fitTangentsExist;
    startTangent = m_startTangent;
    endTangent = m_endTangent;
    knotParam = m_knotParam;
    return true;
}

void GeNurbCurve3d::getDefinitionData(int& degree, bool& rational, bool& periodic, GeKnotVector& knots, GePoint3dArray& controlPoints, GeDoubleArray& weights) const
{
    degree = m_degree;
    rational = m_isRational;
    periodic = m_isPeriodic;
    knots = m_knots;
    controlPoints = m_controlPoints;
    weights = m_weights;
}

int GeNurbCurve3d::numWeights() const
{
    return m_isRational ? m_weights.length() : 0;
}

double GeNurbCurve3d::weightAt(int weightIndex) const
{
    if (weightIndex < 0 || weightIndex >= m_weights.length()) {
        return 1.0;
    }
    return m_weights[weightIndex];
}

GeNurbCurve3d& GeNurbCurve3d::setWeightAt(int weightIndex, double weight)
{
    while (m_weights.length() < m_controlPoints.length()) {
        m_weights.append(1.0);
    }
    if (weightIndex >= 0 && weightIndex < m_weights.length()) {
        m_weights[weightIndex] = weight;
        m_isRational = true;
    }
    return *this;
}

bool GeNurbCurve3d::evalMode() const
{
    return m_evalMode;
}

bool GeNurbCurve3d::getParamsOfC1Discontinuity(GeDoubleArray& params, const GeTol&) const
{
    params.removeAll();
    return false;
}

bool GeNurbCurve3d::getParamsOfG1Discontinuity(GeDoubleArray& params, const GeTol&) const
{
    params.removeAll();
    return false;
}

GeNurbCurve3d& GeNurbCurve3d::operator = (const GeNurbCurve3d& spline)
{
    if (this == &spline) {
        return *this;
    }
    GeSplineEnt3d::operator=(spline);
    m_weights = spline.m_weights;
    m_isPeriodic = spline.m_isPeriodic;
    m_evalMode = spline.m_evalMode;
    m_fitPoints = spline.m_fitPoints;
    m_fitTolerance = spline.m_fitTolerance;
    m_fitTangentsExist = spline.m_fitTangentsExist;
    m_startTangent = spline.m_startTangent;
    m_endTangent = spline.m_endTangent;
    m_startTangentDefined = spline.m_startTangentDefined;
    m_endTangentDefined = spline.m_endTangentDefined;
    m_knotParam = spline.m_knotParam;
    return *this;
}

void GeNurbCurve3d::ensureDefaultData()
{
    if (m_controlPoints.length() < 2) {
        m_controlPoints.removeAll();
        m_controlPoints.append(GePoint3d::kOrigin);
        m_controlPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }
    rebuildKnotsIfNeeded();
    if (m_isRational && m_weights.length() < m_controlPoints.length()) {
        while (m_weights.length() < m_controlPoints.length()) {
            m_weights.append(1.0);
        }
    }
}

void GeNurbCurve3d::rebuildKnotsIfNeeded()
{
    int expectedLength = m_isPeriodic ? m_controlPoints.length() : (m_controlPoints.length() + m_degree + 1);
    if (expectedLength < 2) {
        expectedLength = 2;
    }
    if (m_knots.length() == expectedLength) {
        return;
    }
    m_knots.removeAll();
    for (int i = 0; i < expectedLength; ++i) {
        m_knots.append(static_cast<double>(i));
    }
}

void GeNurbCurve3d::setFitMeta(const GePoint3dArray& fitPoints, const GeTol& fitTolerance, bool tangentsExist, const GeVector3d& startTangent, const GeVector3d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam)
{
    m_fitPoints = fitPoints;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint3d::kOrigin);
        m_fitPoints.append(GePoint3d(1.0, 0.0, 0.0));
    }
    m_fitTolerance = fitTolerance;
    m_fitTangentsExist = tangentsExist;
    m_startTangent = startTangent;
    m_endTangent = endTangent;
    m_startTangentDefined = startTangentDefined;
    m_endTangentDefined = endTangentDefined;
    m_knotParam = knotParam;
    m_hasFitData = true;
    m_isRational = false;
    m_controlPoints = m_fitPoints;
    if (m_degree < 1) {
        m_degree = 3;
    }
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}