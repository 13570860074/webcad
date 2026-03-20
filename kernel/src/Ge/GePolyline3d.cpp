#include "GePolyline3d.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
double poly3d_abs(double v)
{
    return v < 0.0 ? 0.0 - v : v;
}

double poly3d_clamp(double v, double minV, double maxV)
{
    if (v < minV) {
        return minV;
    }
    if (v > maxV) {
        return maxV;
    }
    return v;
}
}

GePolyline3d::GePolyline3d()
{
    this->setDefault();
}

GePolyline3d::GePolyline3d(const GePolyline3d& source)
{
    *this = source;
}

GePolyline3d::GePolyline3d(const GePoint3dArray& fitPoints)
{
    this->set(fitPoints);
}

GePolyline3d::GePolyline3d(const GeKnotVector& knots, const GePoint3dArray& points)
{
    this->set(knots, points);
}

GePolyline3d::GePolyline3d(const GeCurve3d& crv, double approxEps)
{
    GePoint3dArray points;
    GeDoubleArray params;

    int sampleCount = 32;
    double curveLength = crv.length();
    if (approxEps > GeContext::gTol.equalPoint() && curveLength > GeContext::gTol.equalPoint()) {
        sampleCount = static_cast<int>(curveLength / approxEps) + 1;
    }
    if (sampleCount < 2) {
        sampleCount = 2;
    }
    if (sampleCount > 512) {
        sampleCount = 512;
    }

    crv.getSamplePoints(sampleCount, points, params);

    if (points.length() < 2) {
        points.removeAll();
        points.append(GePoint3d::kOrigin);
        points.append(GePoint3d(1.0, 0.0, 0.0));
    }

    this->set(points);
}

GePolyline3d::GePolyline3d(int numPoints, const GePoint3d* pPoints)
{
    GePoint3dArray points;
    if (pPoints != NULL && numPoints > 0) {
        for (int i = 0; i < numPoints; i++) {
            points.append(pPoints[i]);
        }
    }

    this->set(points);
}

void GePolyline3d::setDefault()
{
    m_points.removeAll();
    m_knots.removeAll();

    m_points.append(GePoint3d::kOrigin);
    m_points.append(GePoint3d(1.0, 0.0, 0.0));
    this->rebuildKnotsFromPoints();
}

void GePolyline3d::rebuildKnotsFromPoints()
{
    m_knots.removeAll();

    if (m_points.length() <= 0) {
        m_knots.append(0.0);
        return;
    }

    double sum = 0.0;
    m_knots.append(sum);
    for (int i = 1; i < m_points.length(); i++) {
        sum += m_points[i].distanceTo(m_points[i - 1]);
        m_knots.append(sum);
    }
}

int GePolyline3d::segmentIndexAtParam(double param) const
{
    int count = m_points.length();
    if (count < 2) {
        return -1;
    }

    if (param <= m_knots[0]) {
        return 0;
    }
    if (param >= m_knots[count - 1]) {
        return count - 2;
    }

    for (int i = 0; i < count - 1; i++) {
        if (param <= m_knots[i + 1]) {
            return i;
        }
    }

    return count - 2;
}

GePoint3d GePolyline3d::closestPointOnSegment(const GePoint3d& pnt, int segIndex, double& segParam) const
{
    if (segIndex < 0 || segIndex >= m_points.length() - 1) {
        segParam = 0.0;
        return GePoint3d::kOrigin;
    }

    GePoint3d p0 = m_points[segIndex];
    GePoint3d p1 = m_points[segIndex + 1];
    GeVector3d seg = p1 - p0;
    double denom = seg.lengthSqrd();

    if (denom <= GeContext::gTol.equalPoint() * GeContext::gTol.equalPoint()) {
        segParam = m_knots[segIndex];
        return p0;
    }

    GeVector3d v = pnt - p0;
    double t = v.dotProduct(seg) / denom;
    t = poly3d_clamp(t, 0.0, 1.0);

    double k0 = m_knots[segIndex];
    double k1 = m_knots[segIndex + 1];
    segParam = k0 + (k1 - k0) * t;

    return p0 + seg * t;
}

bool GePolyline3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d
        || entType == Ge::EntityId::kCurve3d
        || entType == this->type();
}

Ge::EntityId GePolyline3d::type() const
{
    return Ge::EntityId::kPolyline3d;
}

GePolyline3d* GePolyline3d::copy() const
{
    return new GePolyline3d(*this);
}

bool GePolyline3d::operator == (const GePolyline3d& entity) const
{
    return this->isEqualTo(entity);
}

bool GePolyline3d::operator != (const GePolyline3d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GePolyline3d::isEqualTo(const GePolyline3d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GePolyline3d::isEqualTo(const GePolyline3d& entity, const GeTol& tol) const
{
    if (m_points.length() != entity.m_points.length()) {
        return false;
    }

    for (int i = 0; i < m_points.length(); i++) {
        if (m_points[i].isEqualTo(entity.m_points[i], tol) == false) {
            return false;
        }
    }

    return true;
}

GePolyline3d& GePolyline3d::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].transformBy(xfm);
    }
    this->rebuildKnotsFromPoints();
    return *this;
}

GePolyline3d& GePolyline3d::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i] += translateVec;
    }
    return *this;
}

GePolyline3d& GePolyline3d::rotateBy(double angle, const GeVector3d& vec)
{
    return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}

GePolyline3d& GePolyline3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GePolyline3d& GePolyline3d::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].mirror(plane);
    }
    return *this;
}

GePolyline3d& GePolyline3d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GePolyline3d& GePolyline3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].scaleBy(scaleFactor, wrtPoint);
    }
    this->rebuildKnotsFromPoints();
    return *this;
}

bool GePolyline3d::isOn(const GePoint3d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GePolyline3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d cp = this->closestPointTo(pnt, tol);
    return cp.distanceTo(pnt) <= tol.equalPoint();
}

int GePolyline3d::numFitPoints() const
{
    return m_points.length();
}

GePoint3d GePolyline3d::fitPointAt(int fitPointIndex) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_points.length()) {
        return GePoint3d::kOrigin;
    }
    return m_points[fitPointIndex];
}

GePolyline3d& GePolyline3d::setFitPointAt(int fitPointIndex, const GePoint3d& point)
{
    if (fitPointIndex >= 0 && fitPointIndex < m_points.length()) {
        m_points[fitPointIndex] = point;
        this->rebuildKnotsFromPoints();
    }
    return *this;
}

GePolyline3d& GePolyline3d::set(const GePoint3dArray& fitPoints)
{
    m_points = fitPoints;
    if (m_points.length() < 2) {
        this->setDefault();
        return *this;
    }

    this->rebuildKnotsFromPoints();
    return *this;
}

GePolyline3d& GePolyline3d::set(const GeKnotVector& knots, const GePoint3dArray& points)
{
    m_points = points;
    if (m_points.length() < 2) {
        this->setDefault();
        return *this;
    }

    if (knots.length() == m_points.length()) {
        bool mono = true;
        for (int i = 1; i < knots.length(); i++) {
            if (knots[i] < knots[i - 1]) {
                mono = false;
                break;
            }
        }

        if (mono == true) {
            m_knots = knots;
            return *this;
        }
    }

    this->rebuildKnotsFromPoints();
    return *this;
}

GePolyline3d& GePolyline3d::operator = (const GePolyline3d& polyline)
{
    if (this == &polyline) {
        return *this;
    }

    m_points = polyline.m_points;
    m_knots = polyline.m_knots;
    return *this;
}

GePoint3d GePolyline3d::evalPointSeg(double param, int& numSeg) const
{
    numSeg = this->segmentIndexAtParam(param);
    return this->evalPoint(param);
}

void GePolyline3d::getInterval(GeInterval& range) const
{
    if (m_knots.length() <= 0) {
        range.set();
        return;
    }

    range.set(m_knots[0], m_knots[m_knots.length() - 1]);
}

void GePolyline3d::getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const
{
    this->getInterval(range);
    startPoint = m_points[0];
    endPoint = m_points[m_points.length() - 1];
}

GeCurve3d& GePolyline3d::reverseParam()
{
    int count = m_points.length();
    for (int i = 0; i < count / 2; i++) {
        GePoint3d temp = m_points[i];
        m_points[i] = m_points[count - i - 1];
        m_points[count - i - 1] = temp;
    }

    this->rebuildKnotsFromPoints();
    return *this;
}

double GePolyline3d::distanceTo(const GePoint3d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GePolyline3d::distanceTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d cp = this->closestPointTo(pnt, tol);
    return cp.distanceTo(pnt);
}

double GePolyline3d::distanceTo(const GeCurve3d& curve) const
{
    return this->distanceTo(curve, GeContext::gTol);
}

double GePolyline3d::distanceTo(const GeCurve3d& curve, const GeTol& tol) const
{
    return GeCurve3d::distanceTo(curve, tol);
}

GePoint3d GePolyline3d::closestPointTo(const GePoint3d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint3d GePolyline3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d best = GePoint3d::kOrigin;
    bool hasBest = false;

    for (int i = 0; i < m_points.length() - 1; i++) {
        double segParam = 0.0;
        GePoint3d cp = this->closestPointOnSegment(pnt, i, segParam);
        if (hasBest == false || cp.distanceTo(pnt) < best.distanceTo(pnt)) {
            best = cp;
            hasBest = true;
        }
    }

    return best;
}

GePoint3d GePolyline3d::closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
    return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}

GePoint3d GePolyline3d::closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
    return GeCurve3d::closestPointTo(curve3d, pntOnOtherCrv, tol);
}

double GePolyline3d::paramOf(const GePoint3d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

double GePolyline3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    double bestParam = 0.0;
    double bestDist = 0.0;
    bool hasBest = false;

    for (int i = 0; i < m_points.length() - 1; i++) {
        double segParam = 0.0;
        GePoint3d cp = this->closestPointOnSegment(pnt, i, segParam);
        double dist = cp.distanceTo(pnt);
        if (hasBest == false || dist < bestDist) {
            bestDist = dist;
            bestParam = segParam;
            hasBest = true;
        }
    }

    return bestParam;
}

bool GePolyline3d::isClosed() const
{
    return this->isClosed(GeContext::gTol);
}

bool GePolyline3d::isClosed(const GeTol& tol) const
{
    if (m_points.length() < 2) {
        return false;
    }

    return m_points[0].isEqualTo(m_points[m_points.length() - 1], tol);
}

double GePolyline3d::length() const
{
    if (m_knots.length() <= 0) {
        return 0.0;
    }
    return m_knots[m_knots.length() - 1] - m_knots[0];
}

double GePolyline3d::length(double fromParam, double toParam) const
{
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}

double GePolyline3d::length(double fromParam, double toParam, double tol) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return 0.0;
    }

    double f = poly3d_clamp(fromParam, range.lowerBound(), range.upperBound());
    double t = poly3d_clamp(toParam, range.lowerBound(), range.upperBound());
    return poly3d_abs(t - f);
}

double GePolyline3d::paramAtLength(double datumParam, double length) const
{
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}

double GePolyline3d::paramAtLength(double datumParam, double length, double tol) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return datumParam;
    }

    return poly3d_clamp(datumParam + length, range.lowerBound(), range.upperBound());
}

bool GePolyline3d::hasStartPoint(GePoint3d& startPoint) const
{
    if (m_points.length() <= 0) {
        return false;
    }

    startPoint = m_points[0];
    return true;
}

bool GePolyline3d::hasEndPoint(GePoint3d& endPoint) const
{
    if (m_points.length() <= 0) {
        return false;
    }

    endPoint = m_points[m_points.length() - 1];
    return true;
}

GePoint3d GePolyline3d::evalPoint(double param) const
{
    if (m_points.length() <= 0) {
        return GePoint3d::kOrigin;
    }
    if (m_points.length() == 1) {
        return m_points[0];
    }

    int segIndex = this->segmentIndexAtParam(param);
    if (segIndex < 0) {
        return m_points[0];
    }

    double k0 = m_knots[segIndex];
    double k1 = m_knots[segIndex + 1];
    double span = k1 - k0;
    if (poly3d_abs(span) <= GeContext::gTol.equalPoint()) {
        return m_points[segIndex];
    }

    double t = (param - k0) / span;
    t = poly3d_clamp(t, 0.0, 1.0);
    return m_points[segIndex] + (m_points[segIndex + 1] - m_points[segIndex]) * t;
}
