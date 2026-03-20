#include "GeNurbCurve2d.h"

#include "GeEllipArc2d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GePolyline2d.h"
#include <cmath>

GeNurbCurve2d::GeNurbCurve2d()
    : GeSplineEnt2d()
    , m_isPeriodic(false)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector2d::kXAxis)
    , m_endTangent(GeVector2d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    ensureDefaultData();
}

GeNurbCurve2d::GeNurbCurve2d(const GeNurbCurve2d& source)
    : GeSplineEnt2d(source)
{
    *this = source;
}

GeNurbCurve2d::GeNurbCurve2d(int degree, const GeKnotVector& knots, const GePoint2dArray& controlPoints, bool isPeriodic)
    : GeSplineEnt2d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector2d::kXAxis)
    , m_endTangent(GeVector2d::kXAxis)
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

GeNurbCurve2d::GeNurbCurve2d(int degree, const GeKnotVector& knots, const GePoint2dArray& controlPoints, const GeDoubleArray& weights, bool isPeriodic)
    : GeNurbCurve2d(degree, knots, controlPoints, isPeriodic)
{
    m_weights = weights;
    while (m_weights.length() < m_controlPoints.length()) {
        m_weights.append(1.0);
    }
    m_isRational = m_weights.length() > 0;
}

GeNurbCurve2d::GeNurbCurve2d(int degree, const GePolyline2d& fitPolyline, bool isPeriodic)
    : GeSplineEnt2d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector2d::kXAxis)
    , m_endTangent(GeVector2d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kChord)
{
    m_degree = degree;
    for (int i = 0; i < fitPolyline.numFitPoints(); ++i) {
        m_fitPoints.append(fitPolyline.fitPointAt(i));
    }
    setFitMeta(m_fitPoints, GeContext::gTol, false, GeVector2d::kXAxis, GeVector2d::kXAxis, false, false, Ge::kChord);
}

GeNurbCurve2d::GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeVector2d& startTangent, const GeVector2d& endTangent, bool startTangentDefined, bool endTangentDefined, const GeTol& fitTolerance)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
    , m_evalMode(false)
{
    setFitMeta(fitPoints, fitTolerance, true, startTangent, endTangent, startTangentDefined, endTangentDefined, Ge::kChord);
}

GeNurbCurve2d::GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeTol& fitTolerance)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
    , m_evalMode(false)
{
    setFitMeta(fitPoints, fitTolerance, false, GeVector2d::kXAxis, GeVector2d::kXAxis, false, false, Ge::kChord);
}

GeNurbCurve2d::GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeVector2dArray& fitTangents, const GeTol& fitTolerance, bool isPeriodic)
    : GeSplineEnt2d()
    , m_isPeriodic(isPeriodic)
    , m_evalMode(false)
{
    GeVector2d startTangent = fitTangents.length() > 0 ? fitTangents[0] : GeVector2d::kXAxis;
    GeVector2d endTangent = fitTangents.length() > 0 ? fitTangents[fitTangents.length() - 1] : GeVector2d::kXAxis;
    setFitMeta(fitPoints, fitTolerance, fitTangents.length() > 0, startTangent, endTangent, fitTangents.length() > 0, fitTangents.length() > 0, Ge::kChord);
}

GeNurbCurve2d::GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeVector2d& startTangent, const GeVector2d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam, const GeTol& fitTol)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
    , m_evalMode(false)
{
    setFitMeta(fitPoints, fitTol, true, startTangent, endTangent, startTangentDefined, endTangentDefined, knotParam);
}

GeNurbCurve2d::GeNurbCurve2d(const GeEllipArc2d& ellipse)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector2d::kXAxis)
    , m_endTangent(GeVector2d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    m_degree = 2;
    m_controlPoints = GeEllipArc2d::toLineSegment(ellipse.center(), ellipse.majorAxis(), ellipse.minorAxis(), ellipse.startAng(), ellipse.endAng(), 16);
    m_isRational = false;
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}

GeNurbCurve2d::GeNurbCurve2d(const GeLineSeg2d& linSeg)
    : GeSplineEnt2d()
    , m_isPeriodic(false)
    , m_evalMode(false)
    , m_fitTolerance(GeContext::gTol)
    , m_fitTangentsExist(false)
    , m_startTangent(GeVector2d::kXAxis)
    , m_endTangent(GeVector2d::kXAxis)
    , m_startTangentDefined(false)
    , m_endTangentDefined(false)
    , m_knotParam(Ge::kNotDefinedKnotParam)
{
    m_degree = 1;
    m_controlPoints.append(linSeg.startPoint());
    m_controlPoints.append(linSeg.endPoint());
    rebuildKnotsIfNeeded();
    ensureDefaultData();
}

bool GeNurbCurve2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == Ge::kSplineEnt2d || entType == this->type();
}

Ge::EntityId GeNurbCurve2d::type() const
{
    return Ge::kNurbCurve2d;
}

GeNurbCurve2d* GeNurbCurve2d::copy() const
{
    return new GeNurbCurve2d(*this);
}

bool GeNurbCurve2d::operator == (const GeNurbCurve2d& entity) const
{
    return isEqualTo(entity);
}

bool GeNurbCurve2d::operator != (const GeNurbCurve2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeNurbCurve2d::isEqualTo(const GeNurbCurve2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeNurbCurve2d::isEqualTo(const GeNurbCurve2d& entity, const GeTol& tol) const
{
    if (!GeSplineEnt2d::isEqualTo(entity, tol) || m_isPeriodic != entity.m_isPeriodic || m_evalMode != entity.m_evalMode || m_knotParam != entity.m_knotParam) {
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

GeNurbCurve2d& GeNurbCurve2d::transformBy(const GeMatrix2d& xfm)
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

GeNurbCurve2d& GeNurbCurve2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i] += translateVec;
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i] += translateVec;
    }
    return *this;
}

GeNurbCurve2d& GeNurbCurve2d::rotateBy(double angle)
{
    return rotateBy(angle, GePoint2d::kOrigin);
}

GeNurbCurve2d& GeNurbCurve2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].rotateBy(angle, wrtPoint);
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].rotateBy(angle, wrtPoint);
    }
    if (m_fitTangentsExist) {
        m_startTangent.rotateBy(angle);
        m_endTangent.rotateBy(angle);
    }
    return *this;
}

GeNurbCurve2d& GeNurbCurve2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_controlPoints.length(); ++i) {
        m_controlPoints[i].mirror(line);
    }
    for (int i = 0; i < m_fitPoints.length(); ++i) {
        m_fitPoints[i].mirror(line);
    }
    if (m_fitTangentsExist) {
        m_startTangent.mirror(line.direction());
        m_endTangent.mirror(line.direction());
    }
    return *this;
}

GeNurbCurve2d& GeNurbCurve2d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeNurbCurve2d& GeNurbCurve2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
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

bool GeNurbCurve2d::isOn(const GePoint2d& pnt) const
{
    return GeSplineEnt2d::isOn(pnt);
}

bool GeNurbCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    return GeSplineEnt2d::isOn(pnt, tol);
}

int GeNurbCurve2d::numFitPoints() const
{
    return m_fitPoints.length();
}

bool GeNurbCurve2d::getFitKnotParameterization(Ge::GeKnotParameterization& knotParam) const
{
    if (!m_hasFitData) {
        return false;
    }
    knotParam = m_knotParam;
    return true;
}

bool GeNurbCurve2d::getFitPointAt(int fitPointIndex, GePoint2d& point) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_fitPoints.length()) {
        return false;
    }
    point = m_fitPoints[fitPointIndex];
    return true;
}

bool GeNurbCurve2d::getFitTolerance(GeTol& fitTolerance) const
{
    if (!m_hasFitData) {
        return false;
    }
    fitTolerance = m_fitTolerance;
    return true;
}

bool GeNurbCurve2d::getFitTangents(GeVector2d& startTangent, GeVector2d& endTangent) const
{
    if (!m_hasFitData || !m_fitTangentsExist) {
        return false;
    }
    startTangent = m_startTangent;
    endTangent = m_endTangent;
    return true;
}

bool GeNurbCurve2d::getFitData(GePoint2dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector2d& startTangent, GeVector2d& endTangent) const
{
    Ge::GeKnotParameterization knotParam;
    return getFitData(fitPoints, fitTolerance, tangentsExist, startTangent, endTangent, knotParam);
}

bool GeNurbCurve2d::getFitData(GePoint2dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector2d& startTangent, GeVector2d& endTangent, Ge::GeKnotParameterization& knotParam) const
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

void GeNurbCurve2d::getDefinitionData(int& degree, bool& rational, bool& periodic, GeKnotVector& knots, GePoint2dArray& controlPoints, GeDoubleArray& weights) const
{
    degree = m_degree;
    rational = m_isRational;
    periodic = m_isPeriodic;
    knots = m_knots;
    controlPoints = m_controlPoints;
    weights = m_weights;
}

int GeNurbCurve2d::numWeights() const
{
    return m_isRational ? m_weights.length() : 0;
}

double GeNurbCurve2d::weightAt(int weightIndex) const
{
    if (weightIndex < 0 || weightIndex >= m_weights.length()) {
        return 1.0;
    }
    return m_weights[weightIndex];
}

GeNurbCurve2d& GeNurbCurve2d::setWeightAt(int weightIndex, double weight)
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

bool GeNurbCurve2d::evalMode() const
{
    return m_evalMode;
}

bool GeNurbCurve2d::getParamsOfC1Discontinuity(GeDoubleArray& params, const GeTol&) const
{
    params.removeAll();
    return false;
}

bool GeNurbCurve2d::getParamsOfG1Discontinuity(GeDoubleArray& params, const GeTol&) const
{
    params.removeAll();
    return false;
}

GeNurbCurve2d& GeNurbCurve2d::operator = (const GeNurbCurve2d& spline)
{
    if (this == &spline) {
        return *this;
    }
    GeSplineEnt2d::operator=(spline);
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

void GeNurbCurve2d::ensureDefaultData()
{
    if (m_controlPoints.length() < 2) {
        m_controlPoints.removeAll();
        m_controlPoints.append(GePoint2d::kOrigin);
        m_controlPoints.append(GePoint2d(1.0, 0.0));
    }
    rebuildKnotsIfNeeded();
    if (m_isRational && m_weights.length() < m_controlPoints.length()) {
        while (m_weights.length() < m_controlPoints.length()) {
            m_weights.append(1.0);
        }
    }
}

void GeNurbCurve2d::rebuildKnotsIfNeeded()
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

void GeNurbCurve2d::setFitMeta(const GePoint2dArray& fitPoints, const GeTol& fitTolerance, bool tangentsExist, const GeVector2d& startTangent, const GeVector2d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam)
{
    m_fitPoints = fitPoints;
    if (m_fitPoints.length() < 2) {
        m_fitPoints.removeAll();
        m_fitPoints.append(GePoint2d::kOrigin);
        m_fitPoints.append(GePoint2d(1.0, 0.0));
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