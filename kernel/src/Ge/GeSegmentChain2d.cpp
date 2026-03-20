#include "GeSegmentChain2d.h"

#include "GeInterval.h"
#include "GeLine2d.h"
#include <cmath>

namespace {
double seg_abs(double v)
{
    return v < 0.0 ? 0.0 - v : v;
}

void appendUnique(GePoint2dArray& points, const GePoint2d& p, const GeTol& tol)
{
    if (points.length() == 0 || !points[points.length() - 1].isEqualTo(p, tol)) {
        points.append(p);
    }
}

void appendArcSample(
    const GePoint2d& p0,
    const GePoint2d& p1,
    double bulge,
    int numSampleForEachArc,
    GePoint2dArray& out)
{
    if (seg_abs(bulge) <= GeContext::gTol.equalPoint()) {
        appendUnique(out, p0, GeContext::gTol);
        appendUnique(out, p1, GeContext::gTol);
        return;
    }

    GeVector2d chord = p1 - p0;
    double chordLen = chord.length();
    if (chordLen <= GeContext::gTol.equalPoint()) {
        appendUnique(out, p0, GeContext::gTol);
        return;
    }

    double theta = 4.0 * std::atan(bulge);
    double sinHalf = std::sin(seg_abs(theta) * 0.5);
    if (seg_abs(sinHalf) <= GeContext::gTol.equalPoint()) {
        appendUnique(out, p0, GeContext::gTol);
        appendUnique(out, p1, GeContext::gTol);
        return;
    }

    double radius = chordLen / (2.0 * sinHalf);
    double halfChord = chordLen * 0.5;
    double h2 = radius * radius - halfChord * halfChord;
    if (h2 < 0.0) {
        h2 = 0.0;
    }
    double h = std::sqrt(h2);

    GePoint2d mid((p0.x + p1.x) * 0.5, (p0.y + p1.y) * 0.5);
    GeVector2d dir = chord / chordLen;
    GeVector2d left(-dir.y, dir.x);
    double sign = bulge >= 0.0 ? 1.0 : -1.0;
    GePoint2d center = mid + left * (h * sign);

    double startAng = std::atan2(p0.y - center.y, p0.x - center.x);
    int sampleCount = static_cast<int>(std::ceil((seg_abs(theta) / (2.0 * PI)) * numSampleForEachArc));
    if (sampleCount < 2) {
        sampleCount = 2;
    }

    for (int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / static_cast<double>(sampleCount - 1);
        double a = startAng + theta * t;
        GePoint2d p(center.x + radius * std::cos(a), center.y + radius * std::sin(a));
        appendUnique(out, p, GeContext::gTol);
    }
}
}

GeSegmentChain2d::GeSegmentChain2d()
    : GePolyline2d()
    , m_closed(false)
{
    m_vertices.append(GePoint2d::kOrigin);
    m_vertices.append(GePoint2d(1.0, 0.0));
    m_bulges.append(0.0);
    syncPolyline();
}

GeSegmentChain2d::GeSegmentChain2d(const GeSegmentChain2d& source)
    : GePolyline2d(source)
{
    *this = source;
}

GeSegmentChain2d::GeSegmentChain2d(const GePoint2dArray& fitpoints)
    : GePolyline2d(fitpoints)
    , m_closed(false)
{
    m_vertices = fitpoints;
    if (m_vertices.length() < 2) {
        m_vertices.removeAll();
        m_vertices.append(GePoint2d::kOrigin);
        m_vertices.append(GePoint2d(1.0, 0.0));
    }
    m_bulges.removeAll();
    for (int i = 0; i < m_vertices.length() - 1; ++i) {
        m_bulges.append(0.0);
    }
    syncPolyline();
}

GeSegmentChain2d::GeSegmentChain2d(const GeKnotVector& knots, const GePoint2dArray& points)
    : GePolyline2d(knots, points)
    , m_closed(false)
{
    m_vertices = points;
    if (m_vertices.length() < 2) {
        m_vertices.removeAll();
        m_vertices.append(GePoint2d::kOrigin);
        m_vertices.append(GePoint2d(1.0, 0.0));
    }
    m_bulges.removeAll();
    for (int i = 0; i < m_vertices.length() - 1; ++i) {
        m_bulges.append(0.0);
    }
    syncPolyline();
}

GeSegmentChain2d::GeSegmentChain2d(const GeCurve2d& crv, double approxEps)
    : GePolyline2d(crv, approxEps)
    , m_closed(false)
{
    GePoint2dArray points;
    GeInterval range;
    crv.getInterval(range);

    if (range.isBounded()) {
        int sampleCount = 32;
        if (approxEps > GeContext::gTol.equalPoint()) {
            double len = crv.length();
            if (len > GeContext::gTol.equalPoint()) {
                sampleCount = static_cast<int>(len / approxEps) + 1;
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
            double t = static_cast<double>(i) / static_cast<double>(sampleCount - 1);
            points.append(crv.evalPoint(lo + (hi - lo) * t));
        }
    }

    if (points.length() < 2) {
        points.removeAll();
        points.append(GePoint2d::kOrigin);
        points.append(GePoint2d(1.0, 0.0));
    }

    m_vertices = points;
    m_bulges.removeAll();
    for (int i = 0; i < m_vertices.length() - 1; ++i) {
        m_bulges.append(0.0);
    }
    syncPolyline();
}

bool GeSegmentChain2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == Ge::kPolyline2d || entType == this->type();
}

Ge::EntityId GeSegmentChain2d::type() const
{
    return Ge::kPolyline2d;
}

GeSegmentChain2d* GeSegmentChain2d::copy() const
{
    return new GeSegmentChain2d(*this);
}

bool GeSegmentChain2d::operator == (const GeSegmentChain2d& entity) const
{
    return isEqualTo(entity);
}

bool GeSegmentChain2d::operator != (const GeSegmentChain2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeSegmentChain2d::isEqualTo(const GeSegmentChain2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSegmentChain2d::isEqualTo(const GeSegmentChain2d& entity, const GeTol& tol) const
{
    if (m_closed != entity.m_closed) {
        return false;
    }
    if (m_vertices.length() != entity.m_vertices.length() || m_bulges.length() != entity.m_bulges.length()) {
        return false;
    }

    for (int i = 0; i < m_vertices.length(); ++i) {
        if (!m_vertices[i].isEqualTo(entity.m_vertices[i], tol)) {
            return false;
        }
    }
    for (int i = 0; i < m_bulges.length(); ++i) {
        if (seg_abs(m_bulges[i] - entity.m_bulges[i]) > tol.equalPoint()) {
            return false;
        }
    }
    return true;
}

GeSegmentChain2d& GeSegmentChain2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_vertices.length(); ++i) {
        m_vertices[i].transformBy(xfm);
    }
    syncPolyline();
    return *this;
}

GeSegmentChain2d& GeSegmentChain2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_vertices.length(); ++i) {
        m_vertices[i] += translateVec;
    }
    syncPolyline();
    return *this;
}

GeSegmentChain2d& GeSegmentChain2d::rotateBy(double angle)
{
    return rotateBy(angle, GePoint2d::kOrigin);
}

GeSegmentChain2d& GeSegmentChain2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_vertices.length(); ++i) {
        m_vertices[i].rotateBy(angle, wrtPoint);
    }
    syncPolyline();
    return *this;
}

GeSegmentChain2d& GeSegmentChain2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_vertices.length(); ++i) {
        m_vertices[i].mirror(line);
    }
    for (int i = 0; i < m_bulges.length(); ++i) {
        m_bulges[i] = -m_bulges[i];
    }
    syncPolyline();
    return *this;
}

GeSegmentChain2d& GeSegmentChain2d::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeSegmentChain2d& GeSegmentChain2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_vertices.length(); ++i) {
        m_vertices[i].scaleBy(scaleFactor, wrtPoint);
    }
    syncPolyline();
    return *this;
}

bool GeSegmentChain2d::isOn(const GePoint2d& pnt) const
{
    return GePolyline2d::isOn(pnt);
}

bool GeSegmentChain2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    return GePolyline2d::isOn(pnt, tol);
}

GeSegmentChain2d& GeSegmentChain2d::operator = (const GeSegmentChain2d& pline)
{
    if (this == &pline) {
        return *this;
    }

    m_vertices = pline.m_vertices;
    m_bulges = pline.m_bulges;
    m_closed = pline.m_closed;
    syncPolyline();
    return *this;
}

GeSegmentChain2d& GeSegmentChain2d::setClosed(bool flag)
{
    m_closed = flag;

    if (m_vertices.length() < 2) {
        syncPolyline();
        return *this;
    }

    if (flag) {
        if (!m_vertices[0].isEqualTo(m_vertices[m_vertices.length() - 1], GeContext::gTol)) {
            m_vertices.append(m_vertices[0]);
        }
    }
    else {
        if (m_vertices.length() >= 2 && m_vertices[0].isEqualTo(m_vertices[m_vertices.length() - 1], GeContext::gTol)) {
            GePoint2dArray tmp;
            for (int i = 0; i < m_vertices.length() - 1; ++i) {
                tmp.append(m_vertices[i]);
            }
            m_vertices = tmp;
        }
    }

    while (m_bulges.length() > m_vertices.length() - 1) {
        m_bulges.removeAt(m_bulges.length() - 1);
    }
    while (m_bulges.length() < m_vertices.length() - 1) {
        m_bulges.append(0.0);
    }

    syncPolyline();
    return *this;
}

GeDoubleArray& GeSegmentChain2d::bulges()
{
    while (m_bulges.length() > m_vertices.length() - 1) {
        m_bulges.removeAt(m_bulges.length() - 1);
    }
    while (m_bulges.length() < m_vertices.length() - 1) {
        m_bulges.append(0.0);
    }
    return m_bulges;
}

const GeDoubleArray& GeSegmentChain2d::getBulges() const
{
    return m_bulges;
}

GePoint2dArray& GeSegmentChain2d::vertices()
{
    return m_vertices;
}

const GePoint2dArray& GeSegmentChain2d::getVertices() const
{
    return m_vertices;
}

bool GeSegmentChain2d::hasBulges() const
{
    for (int i = 0; i < m_bulges.length(); ++i) {
        if (seg_abs(m_bulges[i]) > GeContext::gTol.equalPoint()) {
            return true;
        }
    }
    return false;
}

void GeSegmentChain2d::appendSamplePointsOptimal(int numSampleForEachArc, GePoint2dArray& pointArray) const
{
    if (numSampleForEachArc < 4) {
        numSampleForEachArc = 4;
    }
    if (m_vertices.length() < 2) {
        return;
    }

    int segCount = m_vertices.length() - 1;
    for (int i = 0; i < segCount; ++i) {
        double bulge = (i < m_bulges.length()) ? m_bulges[i] : 0.0;
        appendArcSample(m_vertices[i], m_vertices[i + 1], bulge, numSampleForEachArc, pointArray);
    }
}

void GeSegmentChain2d::syncPolyline()
{
    if (m_vertices.length() < 2) {
        GePoint2dArray points;
        points.append(GePoint2d::kOrigin);
        points.append(GePoint2d(1.0, 0.0));
        GePolyline2d::set(points);
        return;
    }

    GePolyline2d::set(m_vertices);
}