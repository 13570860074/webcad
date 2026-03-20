#include "GePolyline2d.h"
#include "GeInterval.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeScale2d.h"
#include <cmath>

namespace {
double poly2d_abs(double v)
{
    return v < 0.0 ? 0.0 - v : v;
}

double poly2d_clamp(double v, double minV, double maxV)
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

GePolyline2d::GePolyline2d()
{
    this->setDefault();
}

GePolyline2d::GePolyline2d(const GePolyline2d& source)
{
    *this = source;
}

GePolyline2d::GePolyline2d(const GePoint2dArray& fitPoints)
{
    this->set(fitPoints);
}

GePolyline2d::GePolyline2d(const GeKnotVector& knots, const GePoint2dArray& points)
{
    this->set(knots, points);
}

GePolyline2d::GePolyline2d(const GeCurve2d& crv, double approxEps)
{
    GePoint2dArray points;
    GeInterval range;
    crv.getInterval(range);

    if (range.isBounded() == false) {
        points.append(GePoint2d::kOrigin);
        points.append(GePoint2d(1.0, 0.0));
        this->set(points);
        return;
    }

    double curveLength = crv.length();
    int sampleCount = 32;
    if (approxEps > GeContext::gTol.equalPoint() && curveLength > GeContext::gTol.equalPoint()) {
        sampleCount = static_cast<int>(curveLength / approxEps) + 1;
    }
    if (sampleCount < 2) {
        sampleCount = 2;
    }
    if (sampleCount > 512) {
        sampleCount = 512;
    }

    double lower = range.lowerBound();
    double upper = range.upperBound();
    for (int i = 0; i < sampleCount; i++) {
        double ratio = 0.0;
        if (sampleCount > 1) {
            ratio = static_cast<double>(i) / static_cast<double>(sampleCount - 1);
        }
        double param = lower + (upper - lower) * ratio;
        points.append(crv.evalPoint(param));
    }

    this->set(points);
}

void GePolyline2d::setDefault()
{
    m_points.removeAll();
    m_knots.removeAll();

    m_points.append(GePoint2d::kOrigin);
    m_points.append(GePoint2d(1.0, 0.0));
    this->rebuildKnotsFromPoints();
}

void GePolyline2d::rebuildKnotsFromPoints()
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

int GePolyline2d::segmentIndexAtParam(double param) const
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

GePoint2d GePolyline2d::closestPointOnSegment(const GePoint2d& pnt, int segIndex, double& segParam) const
{
    if (segIndex < 0 || segIndex >= m_points.length() - 1) {
        segParam = 0.0;
        return GePoint2d::kOrigin;
    }

    GePoint2d p0 = m_points[segIndex];
    GePoint2d p1 = m_points[segIndex + 1];
    GeVector2d seg = p1 - p0;
    double denom = seg.lengthSqrd();

    if (denom <= GeContext::gTol.equalPoint() * GeContext::gTol.equalPoint()) {
        segParam = m_knots[segIndex];
        return p0;
    }

    GeVector2d v = pnt - p0;
    double t = v.dotProduct(seg) / denom;
    t = poly2d_clamp(t, 0.0, 1.0);

    double k0 = m_knots[segIndex];
    double k1 = m_knots[segIndex + 1];
    segParam = k0 + (k1 - k0) * t;

    return p0 + seg * t;
}

bool GePolyline2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity2d
        || entType == Ge::EntityId::kCurve2d
        || entType == this->type();
}

Ge::EntityId GePolyline2d::type() const
{
    return Ge::EntityId::kPolyline2d;
}

GePolyline2d* GePolyline2d::copy() const
{
    return new GePolyline2d(*this);
}

bool GePolyline2d::operator == (const GePolyline2d& entity) const
{
    return this->isEqualTo(entity);
}

bool GePolyline2d::operator != (const GePolyline2d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GePolyline2d::isEqualTo(const GePolyline2d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GePolyline2d::isEqualTo(const GePolyline2d& entity, const GeTol& tol) const
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

GePolyline2d& GePolyline2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].transformBy(xfm);
    }
    this->rebuildKnotsFromPoints();
    return *this;
}

GePolyline2d& GePolyline2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i] += translateVec;
    }
    return *this;
}

GePolyline2d& GePolyline2d::rotateBy(double angle)
{
    return this->rotateBy(angle, GePoint2d::kOrigin);
}

GePolyline2d& GePolyline2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].rotateBy(angle, wrtPoint);
    }
    return *this;
}

GePolyline2d& GePolyline2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].mirror(line);
    }
    return *this;
}

GePolyline2d& GePolyline2d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GePolyline2d& GePolyline2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_points.length(); i++) {
        m_points[i].scaleBy(scaleFactor, wrtPoint);
    }
    this->rebuildKnotsFromPoints();
    return *this;
}

bool GePolyline2d::isOn(const GePoint2d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GePolyline2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d cp = this->closestPointTo(pnt, tol);
    return cp.distanceTo(pnt) <= tol.equalPoint();
}

int GePolyline2d::numFitPoints() const
{
    return m_points.length();
}

GePoint2d GePolyline2d::fitPointAt(int fitPointIndex) const
{
    if (fitPointIndex < 0 || fitPointIndex >= m_points.length()) {
        return GePoint2d::kOrigin;
    }
    return m_points[fitPointIndex];
}

GePolyline2d& GePolyline2d::setFitPointAt(int fitPointIndex, const GePoint2d& point)
{
    if (fitPointIndex >= 0 && fitPointIndex < m_points.length()) {
        m_points[fitPointIndex] = point;
        this->rebuildKnotsFromPoints();
    }
    return *this;
}

GePolyline2d& GePolyline2d::set(const GePoint2dArray& fitPoints)
{
    m_points = fitPoints;
    if (m_points.length() < 2) {
        this->setDefault();
        return *this;
    }

    this->rebuildKnotsFromPoints();
    return *this;
}

GePolyline2d& GePolyline2d::set(const GeKnotVector& knots, const GePoint2dArray& points)
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

GePolyline2d& GePolyline2d::operator = (const GePolyline2d& polyline)
{
    if (this == &polyline) {
        return *this;
    }

    m_points = polyline.m_points;
    m_knots = polyline.m_knots;
    return *this;
}

void GePolyline2d::getInterval(GeInterval& range) const
{
    if (m_knots.length() <= 0) {
        range.set();
        return;
    }

    range.set(m_knots[0], m_knots[m_knots.length() - 1]);
}

void GePolyline2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const
{
    this->getInterval(range);
    startPoint = m_points[0];
    endPoint = m_points[m_points.length() - 1];
}

GeCurve2d& GePolyline2d::reverseParam()
{
    int count = m_points.length();
    for (int i = 0; i < count / 2; i++) {
        GePoint2d temp = m_points[i];
        m_points[i] = m_points[count - i - 1];
        m_points[count - i - 1] = temp;
    }

    this->rebuildKnotsFromPoints();
    return *this;
}

double GePolyline2d::distanceTo(const GePoint2d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GePolyline2d::distanceTo(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d cp = this->closestPointTo(pnt, tol);
    return cp.distanceTo(pnt);
}

double GePolyline2d::distanceTo(const GeCurve2d& curve) const
{
    return this->distanceTo(curve, GeContext::gTol);
}

double GePolyline2d::distanceTo(const GeCurve2d& curve, const GeTol& tol) const
{
    return GeCurve2d::distanceTo(curve, tol);
}

GePoint2d GePolyline2d::closestPointTo(const GePoint2d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint2d GePolyline2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d best = GePoint2d::kOrigin;
    bool hasBest = false;

    for (int i = 0; i < m_points.length() - 1; i++) {
        double segParam = 0.0;
        GePoint2d cp = this->closestPointOnSegment(pnt, i, segParam);
        if (hasBest == false || cp.distanceTo(pnt) < best.distanceTo(pnt)) {
            best = cp;
            hasBest = true;
        }
    }

    return best;
}

GePoint2d GePolyline2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const
{
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}

GePoint2d GePolyline2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const
{
    return GeCurve2d::closestPointTo(curve2d, pntOnOtherCrv, tol);
}

double GePolyline2d::paramOf(const GePoint2d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

double GePolyline2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const
{
    double bestParam = 0.0;
    double bestDist = 0.0;
    bool hasBest = false;

    for (int i = 0; i < m_points.length() - 1; i++) {
        double segParam = 0.0;
        GePoint2d cp = this->closestPointOnSegment(pnt, i, segParam);
        double dist = cp.distanceTo(pnt);
        if (hasBest == false || dist < bestDist) {
            bestDist = dist;
            bestParam = segParam;
            hasBest = true;
        }
    }

    return bestParam;
}

bool GePolyline2d::isClosed() const
{
    return this->isClosed(GeContext::gTol);
}

bool GePolyline2d::isClosed(const GeTol& tol) const
{
    if (m_points.length() < 2) {
        return false;
    }

    return m_points[0].isEqualTo(m_points[m_points.length() - 1], tol);
}

double GePolyline2d::length() const
{
    if (m_knots.length() <= 0) {
        return 0.0;
    }
    return m_knots[m_knots.length() - 1] - m_knots[0];
}

double GePolyline2d::length(double fromParam, double toParam) const
{
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}

double GePolyline2d::length(double fromParam, double toParam, double tol) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return 0.0;
    }

    double f = poly2d_clamp(fromParam, range.lowerBound(), range.upperBound());
    double t = poly2d_clamp(toParam, range.lowerBound(), range.upperBound());
    return poly2d_abs(t - f);
}

double GePolyline2d::paramAtLength(double datumParam, double length) const
{
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}

double GePolyline2d::paramAtLength(double datumParam, double length, double tol) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return datumParam;
    }

    return poly2d_clamp(datumParam + length, range.lowerBound(), range.upperBound());
}

bool GePolyline2d::hasStartPoint(GePoint2d& startPoint) const
{
    if (m_points.length() <= 0) {
        return false;
    }

    startPoint = m_points[0];
    return true;
}

bool GePolyline2d::hasEndPoint(GePoint2d& endPoint) const
{
    if (m_points.length() <= 0) {
        return false;
    }

    endPoint = m_points[m_points.length() - 1];
    return true;
}

GePoint2d GePolyline2d::evalPoint(double param) const
{
    if (m_points.length() <= 0) {
        return GePoint2d::kOrigin;
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
    if (poly2d_abs(span) <= GeContext::gTol.equalPoint()) {
        return m_points[segIndex];
    }

    double t = (param - k0) / span;
    t = poly2d_clamp(t, 0.0, 1.0);
    return m_points[segIndex] + (m_points[segIndex + 1] - m_points[segIndex]) * t;
}
