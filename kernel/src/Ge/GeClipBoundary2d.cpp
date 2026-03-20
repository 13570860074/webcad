#include "GeClipBoundary2d.h"

#include "GeLine2d.h"
#include <algorithm>
#include <cmath>
#include <vector>

namespace {
double signedArea(const GePoint2dArray& polygon)
{
    if (polygon.length() < 3) {
        return 0.0;
    }

    double area2 = 0.0;
    for (int i = 0; i < polygon.length(); ++i) {
        const GePoint2d& a = polygon[i];
        const GePoint2d& b = polygon[(i + 1) % polygon.length()];
        area2 += a.x * b.y - a.y * b.x;
    }
    return area2 * 0.5;
}

bool pointOnSegment(const GePoint2d& p, const GePoint2d& a, const GePoint2d& b, const GeTol& tol)
{
    GeVector2d ab = b - a;
    GeVector2d ap = p - a;
    if (std::fabs(ab.crossProduct(ap)) > tol.equalPoint()) {
        return false;
    }

    double abLen2 = ab.lengthSqrd();
    if (abLen2 <= tol.equalPoint() * tol.equalPoint()) {
        return p.isEqualTo(a, tol);
    }

    double proj = ap.dotProduct(ab);
    return proj >= -tol.equalPoint() && proj <= abLen2 + tol.equalPoint();
}

bool isPointInsideOrOn(const GePoint2d& p, const GePoint2dArray& polygon, const GeTol& tol)
{
    if (polygon.length() < 3) {
        return false;
    }

    for (int i = 0; i < polygon.length(); ++i) {
        const GePoint2d& a = polygon[i];
        const GePoint2d& b = polygon[(i + 1) % polygon.length()];
        if (pointOnSegment(p, a, b, tol)) {
            return true;
        }
    }

    bool inside = false;
    for (int i = 0, j = polygon.length() - 1; i < polygon.length(); j = i++) {
        const GePoint2d& pi = polygon[i];
        const GePoint2d& pj = polygon[j];
        bool intersect = ((pi.y > p.y) != (pj.y > p.y)) &&
            (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y) + pi.x);
        if (intersect) {
            inside = !inside;
        }
    }
    return inside;
}

bool segmentIntersection(
    const GePoint2d& p0,
    const GePoint2d& p1,
    const GePoint2d& q0,
    const GePoint2d& q1,
    double& tOut,
    GePoint2d& pointOut,
    const GeTol& tol)
{
    GeVector2d r = p1 - p0;
    GeVector2d s = q1 - q0;
    double denom = r.crossProduct(s);
    if (std::fabs(denom) <= tol.equalPoint()) {
        return false;
    }

    GeVector2d qp = q0 - p0;
    double t = qp.crossProduct(s) / denom;
    double u = qp.crossProduct(r) / denom;
    if (t < -tol.equalPoint() || t > 1.0 + tol.equalPoint()) {
        return false;
    }
    if (u < -tol.equalPoint() || u > 1.0 + tol.equalPoint()) {
        return false;
    }

    tOut = t;
    pointOut = p0 + (r * t);
    return true;
}

bool isConvexPolygon(const GePoint2dArray& polygon, const GeTol& tol)
{
    if (polygon.length() < 3) {
        return false;
    }

    int sign = 0;
    for (int i = 0; i < polygon.length(); ++i) {
        const GePoint2d& a = polygon[i];
        const GePoint2d& b = polygon[(i + 1) % polygon.length()];
        const GePoint2d& c = polygon[(i + 2) % polygon.length()];
        GeVector2d ab = b - a;
        GeVector2d bc = c - b;
        double cross = ab.crossProduct(bc);

        if (std::fabs(cross) <= tol.equalPoint()) {
            continue;
        }

        int thisSign = (cross > 0.0) ? 1 : -1;
        if (sign == 0) {
            sign = thisSign;
        }
        else if (sign != thisSign) {
            return false;
        }
    }

    return true;
}

void appendUniquePoint(GePoint2dArray& points, const GePoint2d& p, const GeTol& tol)
{
    if (points.length() == 0 || !points[points.length() - 1].isEqualTo(p, tol)) {
        points.append(p);
    }
}

GePoint2d lineIntersection(
    const GePoint2d& s,
    const GePoint2d& e,
    const GePoint2d& cp1,
    const GePoint2d& cp2,
    const GeTol& tol)
{
    GeVector2d se = e - s;
    GeVector2d ce = cp2 - cp1;
    double denom = se.crossProduct(ce);
    if (std::fabs(denom) <= tol.equalPoint()) {
        return e;
    }

    GeVector2d cs = cp1 - s;
    double t = cs.crossProduct(ce) / denom;
    return s + (se * t);
}

bool insideHalfPlane(
    const GePoint2d& p,
    const GePoint2d& a,
    const GePoint2d& b,
    int orientationSign,
    const GeTol& tol)
{
    GeVector2d edge = b - a;
    GeVector2d toPoint = p - a;
    double cross = edge.crossProduct(toPoint);
    return orientationSign * cross >= -tol.equalPoint();
}
}

GeClipBoundary2d::GeClipBoundary2d()
    : m_initialized(false)
{
}

GeClipBoundary2d::GeClipBoundary2d(const GePoint2d& cornerA, const GePoint2d& cornerB)
    : m_initialized(false)
{
    set(cornerA, cornerB);
}

GeClipBoundary2d::GeClipBoundary2d(const GePoint2dArray& clipBoundary)
    : m_initialized(false)
{
    set(clipBoundary);
}

GeClipBoundary2d::GeClipBoundary2d(const GeClipBoundary2d& src)
    : m_initialized(false)
{
    *this = src;
}

bool GeClipBoundary2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == this->type();
}

Ge::EntityId GeClipBoundary2d::type() const
{
    return Ge::kEntity2d;
}

GeClipBoundary2d* GeClipBoundary2d::copy() const
{
    return new GeClipBoundary2d(*this);
}

bool GeClipBoundary2d::operator == (const GeClipBoundary2d& entity) const
{
    return isEqualTo(entity);
}

bool GeClipBoundary2d::operator != (const GeClipBoundary2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeClipBoundary2d::isEqualTo(const GeClipBoundary2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeClipBoundary2d::isEqualTo(const GeClipBoundary2d& entity, const GeTol& tol) const
{
    if (m_initialized != entity.m_initialized || m_boundary.length() != entity.m_boundary.length()) {
        return false;
    }
    for (int i = 0; i < m_boundary.length(); ++i) {
        if (!m_boundary[i].isEqualTo(entity.m_boundary[i], tol)) {
            return false;
        }
    }
    return true;
}

GeClipBoundary2d& GeClipBoundary2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i].transformBy(xfm);
    }
    return *this;
}

GeClipBoundary2d& GeClipBoundary2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i] += translateVec;
    }
    return *this;
}

GeClipBoundary2d& GeClipBoundary2d::rotateBy(double angle)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i].rotateBy(angle);
    }
    return *this;
}

GeClipBoundary2d& GeClipBoundary2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i].rotateBy(angle, wrtPoint);
    }
    return *this;
}

GeClipBoundary2d& GeClipBoundary2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i].mirror(line);
    }
    return *this;
}

GeClipBoundary2d& GeClipBoundary2d::scaleBy(double scaleFactor)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i].scaleBy(scaleFactor);
    }
    return *this;
}

GeClipBoundary2d& GeClipBoundary2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        m_boundary[i].scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeClipBoundary2d::isOn(const GePoint2d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeClipBoundary2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_boundary.length(); ++i) {
        const GePoint2d& a = m_boundary[i];
        const GePoint2d& b = m_boundary[(i + 1) % m_boundary.length()];
        if (pointOnSegment(pnt, a, b, tol)) {
            return true;
        }
    }
    return false;
}

Ge::ClipError GeClipBoundary2d::set(const GePoint2d& cornerA, const GePoint2d& cornerB)
{
    m_boundary.removeAll();

    GePoint2d pMin(std::min(cornerA.x, cornerB.x), std::min(cornerA.y, cornerB.y));
    GePoint2d pMax(std::max(cornerA.x, cornerB.x), std::max(cornerA.y, cornerB.y));

    if (pMin.isEqualTo(pMax, GeContext::gTol)) {
        m_initialized = false;
        return Ge::eInvalidClipBoundary;
    }

    m_boundary.append(GePoint2d(pMin.x, pMin.y));
    m_boundary.append(GePoint2d(pMax.x, pMin.y));
    m_boundary.append(GePoint2d(pMax.x, pMax.y));
    m_boundary.append(GePoint2d(pMin.x, pMax.y));

    m_initialized = true;
    return Ge::eOk;
}

Ge::ClipError GeClipBoundary2d::set(const GePoint2dArray& clipBoundary)
{
    m_boundary.removeAll();

    if (clipBoundary.length() < 3) {
        m_initialized = false;
        return Ge::eInvalidClipBoundary;
    }

    for (int i = 0; i < clipBoundary.length(); ++i) {
        m_boundary.append(clipBoundary[i]);
    }

    if (m_boundary.length() >= 2 && m_boundary[0].isEqualTo(m_boundary[m_boundary.length() - 1], GeContext::gTol)) {
        GePoint2dArray normalized;
        for (int i = 0; i < m_boundary.length() - 1; ++i) {
            normalized.append(m_boundary[i]);
        }
        m_boundary = normalized;
    }

    m_initialized = m_boundary.length() >= 3;
    return m_initialized ? Ge::eOk : Ge::eInvalidClipBoundary;
}

Ge::ClipError GeClipBoundary2d::clipPolygon(
    const GePoint2dArray& rawVertices,
    GePoint2dArray& clippedVertices,
    Ge::ClipCondition& clipCondition,
    GeIntArray* pClippedSegmentSourceLabel) const
{
    clippedVertices.removeAll();
    if (pClippedSegmentSourceLabel != NULL) {
        pClippedSegmentSourceLabel->removeAll();
    }

    if (!m_initialized || m_boundary.length() < 3) {
        clipCondition = Ge::kInvalid;
        return Ge::eNotInitialized;
    }

    if (rawVertices.length() < 3) {
        clipCondition = Ge::kInvalid;
        return Ge::eInvalidClipBoundary;
    }

    if (!isConvexPolygon(m_boundary, GeContext::gTol)) {
        clipCondition = Ge::kInvalid;
        return Ge::eInvalidClipBoundary;
    }

    GePoint2dArray output;
    for (int i = 0; i < rawVertices.length(); ++i) {
        output.append(rawVertices[i]);
    }

    int orientationSign = (signedArea(m_boundary) >= 0.0) ? 1 : -1;
    GeTol tol = GeContext::gTol;

    for (int edge = 0; edge < m_boundary.length(); ++edge) {
        GePoint2dArray input = output;
        output.removeAll();
        if (input.length() == 0) {
            break;
        }

        const GePoint2d& cp1 = m_boundary[edge];
        const GePoint2d& cp2 = m_boundary[(edge + 1) % m_boundary.length()];
        GePoint2d s = input[input.length() - 1];

        for (int i = 0; i < input.length(); ++i) {
            GePoint2d e = input[i];
            bool eInside = insideHalfPlane(e, cp1, cp2, orientationSign, tol);
            bool sInside = insideHalfPlane(s, cp1, cp2, orientationSign, tol);

            if (eInside) {
                if (!sInside) {
                    appendUniquePoint(output, lineIntersection(s, e, cp1, cp2, tol), tol);
                }
                appendUniquePoint(output, e, tol);
            }
            else if (sInside) {
                appendUniquePoint(output, lineIntersection(s, e, cp1, cp2, tol), tol);
            }

            s = e;
        }
    }

    bool allInside = true;
    for (int i = 0; i < rawVertices.length(); ++i) {
        if (!isPointInsideOrOn(rawVertices[i], m_boundary, GeContext::gTol)) {
            allInside = false;
            break;
        }
    }

    for (int i = 0; i < output.length(); ++i) {
        clippedVertices.append(output[i]);
        if (pClippedSegmentSourceLabel != NULL) {
            pClippedSegmentSourceLabel->append(-1);
        }
    }

    if (clippedVertices.length() == 0) {
        clipCondition = Ge::kAllSegmentsOutsideZeroWinds;
    }
    else if (allInside) {
        clipCondition = Ge::kAllSegmentsInside;
    }
    else {
        clipCondition = Ge::kSegmentsIntersect;
    }

    return Ge::eOk;
}

Ge::ClipError GeClipBoundary2d::clipPolyline(
    const GePoint2dArray& rawVertices,
    GePoint2dArray& clippedVertices,
    Ge::ClipCondition& clipCondition,
    GeIntArray* pClippedSegmentSourceLabel) const
{
    clippedVertices.removeAll();
    if (pClippedSegmentSourceLabel != NULL) {
        pClippedSegmentSourceLabel->removeAll();
    }

    if (!m_initialized || m_boundary.length() < 3) {
        clipCondition = Ge::kInvalid;
        return Ge::eNotInitialized;
    }

    if (rawVertices.length() < 2) {
        clipCondition = Ge::kInvalid;
        return Ge::eInvalidClipBoundary;
    }

    GeTol tol = GeContext::gTol;
    bool hadIntersection = false;
    bool allInside = true;

    for (int seg = 0; seg < rawVertices.length() - 1; ++seg) {
        GePoint2d a = rawVertices[seg];
        GePoint2d b = rawVertices[seg + 1];
        bool aInside = isPointInsideOrOn(a, m_boundary, tol);
        bool bInside = isPointInsideOrOn(b, m_boundary, tol);
        allInside = allInside && aInside && bInside;

        std::vector<double> ts;
        ts.push_back(0.0);
        ts.push_back(1.0);

        for (int edge = 0; edge < m_boundary.length(); ++edge) {
            const GePoint2d& c = m_boundary[edge];
            const GePoint2d& d = m_boundary[(edge + 1) % m_boundary.length()];
            double t;
            GePoint2d intersection;
            if (segmentIntersection(a, b, c, d, t, intersection, tol)) {
                ts.push_back(t);
                hadIntersection = true;
            }
        }

        std::sort(ts.begin(), ts.end());
        std::vector<double> uniqueTs;
        for (size_t i = 0; i < ts.size(); ++i) {
            if (uniqueTs.empty() || std::fabs(ts[i] - uniqueTs.back()) > tol.equalPoint()) {
                uniqueTs.push_back(ts[i]);
            }
        }

        GeVector2d ab = b - a;
        for (size_t i = 0; i + 1 < uniqueTs.size(); ++i) {
            double t0 = uniqueTs[i];
            double t1 = uniqueTs[i + 1];
            double tm = 0.5 * (t0 + t1);
            GePoint2d mid = a + (ab * tm);

            if (!isPointInsideOrOn(mid, m_boundary, tol)) {
                continue;
            }

            GePoint2d p0 = a + (ab * t0);
            GePoint2d p1 = a + (ab * t1);
            appendUniquePoint(clippedVertices, p0, tol);
            appendUniquePoint(clippedVertices, p1, tol);

            if (pClippedSegmentSourceLabel != NULL) {
                int label = seg;
                if (pClippedSegmentSourceLabel->length() < clippedVertices.length()) {
                    pClippedSegmentSourceLabel->append(label);
                }
                if (pClippedSegmentSourceLabel->length() < clippedVertices.length()) {
                    pClippedSegmentSourceLabel->append(label);
                }
            }
        }
    }

    if (clippedVertices.length() == 0) {
        clipCondition = Ge::kAllSegmentsOutsideZeroWinds;
    }
    else if (allInside && !hadIntersection) {
        clipCondition = Ge::kAllSegmentsInside;
    }
    else {
        clipCondition = Ge::kSegmentsIntersect;
    }

    return Ge::eOk;
}

GeClipBoundary2d& GeClipBoundary2d::operator = (const GeClipBoundary2d& src)
{
    if (this == &src) {
        return *this;
    }

    m_boundary = src.m_boundary;
    m_initialized = src.m_initialized;
    return *this;
}