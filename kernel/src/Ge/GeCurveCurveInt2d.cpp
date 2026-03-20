#include "GeCurveCurveInt2d.h"
#include "GeLine2d.h"
#include "GeScale2d.h"
#include <cmath>

namespace {
void curve_int2d_sort_by_param(GeDoubleArray& sortParams, GeDoubleArray& peer1, GeDoubleArray& peer2, GePoint2dArray& points)
{
    int count = sortParams.length();
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (sortParams[j] < sortParams[i]) {
                double temp = sortParams[i];
                sortParams[i] = sortParams[j];
                sortParams[j] = temp;

                temp = peer1[i];
                peer1[i] = peer1[j];
                peer1[j] = temp;

                temp = peer2[i];
                peer2[i] = peer2[j];
                peer2[j] = temp;

                GePoint2d point = points[i];
                points[i] = points[j];
                points[j] = point;
            }
        }
    }
}
}

GeCurveCurveInt2d::GeCurveCurveInt2d()
    : m_curve1(NULL)
    , m_curve2(NULL)
    , m_tol(GeContext::gTol)
{
    m_range1.set();
    m_range2.set();
}

GeCurveCurveInt2d::GeCurveCurveInt2d(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeTol& tol)
    : m_curve1(NULL)
    , m_curve2(NULL)
    , m_tol(tol)
{
    this->set(curve1, curve2, tol);
}

GeCurveCurveInt2d::GeCurveCurveInt2d(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeInterval& range1, const GeInterval& range2, const GeTol& tol)
    : m_curve1(NULL)
    , m_curve2(NULL)
    , m_tol(tol)
{
    this->set(curve1, curve2, range1, range2, tol);
}

GeCurveCurveInt2d::GeCurveCurveInt2d(const GeCurveCurveInt2d& source)
    : m_curve1(source.m_curve1)
    , m_curve2(source.m_curve2)
    , m_range1(source.m_range1)
    , m_range2(source.m_range2)
    , m_tol(source.m_tol)
    , m_intPoints(source.m_intPoints)
    , m_params1(source.m_params1)
    , m_params2(source.m_params2)
{
}

bool GeCurveCurveInt2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity2d || entType == this->type();
}

Ge::EntityId GeCurveCurveInt2d::type() const
{
    return Ge::EntityId::kCurveCurveInt2d;
}

GeCurveCurveInt2d* GeCurveCurveInt2d::copy() const
{
    return new GeCurveCurveInt2d(*this);
}

bool GeCurveCurveInt2d::operator == (const GeCurveCurveInt2d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeCurveCurveInt2d::operator != (const GeCurveCurveInt2d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeCurveCurveInt2d::isEqualTo(const GeCurveCurveInt2d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeCurveCurveInt2d::isEqualTo(const GeCurveCurveInt2d& entity, const GeTol& tol) const
{
    if (m_curve1 != entity.m_curve1 || m_curve2 != entity.m_curve2) {
        return false;
    }
    if (this->numIntPoints() != entity.numIntPoints()) {
        return false;
    }
    for (int i = 0; i < this->numIntPoints(); i++) {
        if (this->intPoint(i).isEqualTo(entity.intPoint(i), tol) == false) {
            return false;
        }
    }
    return true;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].transformBy(xfm);
    }
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i] += translateVec;
    }
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::rotateBy(double angle)
{
    return this->rotateBy(angle, GePoint2d::kOrigin);
}

GeCurveCurveInt2d& GeCurveCurveInt2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].rotateBy(angle, wrtPoint);
    }
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].mirror(line);
    }
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeCurveCurveInt2d& GeCurveCurveInt2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    GeScale2d scale(scaleFactor, scaleFactor);
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].scaleBy(scale, wrtPoint);
    }
    return *this;
}

bool GeCurveCurveInt2d::isOn(const GePoint2d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeCurveCurveInt2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        if (m_intPoints[i].isEqualTo(pnt, tol) == true) {
            return true;
        }
    }
    return false;
}

const GeCurve2d* GeCurveCurveInt2d::curve1() const
{
    return m_curve1;
}

const GeCurve2d* GeCurveCurveInt2d::curve2() const
{
    return m_curve2;
}

void GeCurveCurveInt2d::getIntRanges(GeInterval& range1, GeInterval& range2) const
{
    range1 = m_range1;
    range2 = m_range2;
}

GeTol GeCurveCurveInt2d::tolerance() const
{
    return m_tol;
}

int GeCurveCurveInt2d::numIntPoints() const
{
    return m_intPoints.length();
}

GePoint2d GeCurveCurveInt2d::intPoint(int intNum) const
{
    if (intNum < 0 || intNum >= m_intPoints.length()) {
        return GePoint2d::kOrigin;
    }
    return m_intPoints[intNum];
}

void GeCurveCurveInt2d::getIntParams(int intNum, double& param1, double& param2) const
{
    if (intNum < 0 || intNum >= m_params1.length() || intNum >= m_params2.length()) {
        param1 = 0.0;
        param2 = 0.0;
        return;
    }
    param1 = m_params1[intNum];
    param2 = m_params2[intNum];
}

void GeCurveCurveInt2d::getPointOnCurve1(int intNum, GePointOnCurve2d& intPnt) const
{
    if (m_curve1 == NULL) {
        return;
    }
    double p1 = 0.0;
    double p2 = 0.0;
    this->getIntParams(intNum, p1, p2);
    intPnt.setCurve(*m_curve1);
    intPnt.setParameter(p1);
}

void GeCurveCurveInt2d::getPointOnCurve2(int intNum, GePointOnCurve2d& intPnt) const
{
    if (m_curve2 == NULL) {
        return;
    }
    double p1 = 0.0;
    double p2 = 0.0;
    this->getIntParams(intNum, p1, p2);
    intPnt.setCurve(*m_curve2);
    intPnt.setParameter(p2);
}

void GeCurveCurveInt2d::getIntConfigs(int intNum, Ge::GeXConfig& config1wrt2, Ge::GeXConfig& config2wrt1) const
{
    config1wrt2 = Ge::kNotDefined;
    config2wrt1 = Ge::kNotDefined;
}

bool GeCurveCurveInt2d::isTangential(int intNum) const
{
    return false;
}

bool GeCurveCurveInt2d::isTransversal(int intNum) const
{
    return false;
}

double GeCurveCurveInt2d::intPointTol(int intNum) const
{
    return m_tol.equalPoint();
}

int GeCurveCurveInt2d::overlapCount() const
{
    return 0;
}

bool GeCurveCurveInt2d::overlapDirection(int overlapNum) const
{
    return false;
}

void GeCurveCurveInt2d::getOverlapRanges(int overlapNum, GeInterval& range1, GeInterval& range2) const
{
    range1.set();
    range2.set();
}

void GeCurveCurveInt2d::changeCurveOrder()
{
    const GeCurve2d* curve = m_curve1;
    m_curve1 = m_curve2;
    m_curve2 = curve;

    GeInterval range = m_range1;
    m_range1 = m_range2;
    m_range2 = range;

    GeDoubleArray params = m_params1;
    m_params1 = m_params2;
    m_params2 = params;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::orderWrt1()
{
    curve_int2d_sort_by_param(m_params1, m_params2, m_params1, m_intPoints);
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::orderWrt2()
{
    curve_int2d_sort_by_param(m_params2, m_params1, m_params2, m_intPoints);
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::set(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeTol& tol)
{
    GeInterval range1;
    GeInterval range2;
    curve1.getInterval(range1);
    curve2.getInterval(range2);
    return this->set(curve1, curve2, range1, range2, tol);
}

GeCurveCurveInt2d& GeCurveCurveInt2d::set(const GeCurve2d& curve1, const GeCurve2d& curve2, const GeInterval& range1, const GeInterval& range2, const GeTol& tol)
{
    m_curve1 = &curve1;
    m_curve2 = &curve2;
    m_range1 = range1;
    m_range2 = range2;
    m_tol = tol;
    this->recalcIntersections();
    return *this;
}

GeCurveCurveInt2d& GeCurveCurveInt2d::operator = (const GeCurveCurveInt2d& source)
{
    if (this == &source) {
        return *this;
    }

    m_curve1 = source.m_curve1;
    m_curve2 = source.m_curve2;
    m_range1 = source.m_range1;
    m_range2 = source.m_range2;
    m_tol = source.m_tol;
    m_intPoints = source.m_intPoints;
    m_params1 = source.m_params1;
    m_params2 = source.m_params2;
    return *this;
}

void GeCurveCurveInt2d::appendIntersection(double param1, double param2, const GePoint2d& pnt)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        if (m_intPoints[i].isEqualTo(pnt, m_tol) == true) {
            return;
        }
    }

    m_intPoints.append(pnt);
    m_params1.append(param1);
    m_params2.append(param2);
}

void GeCurveCurveInt2d::recalcIntersections()
{
    m_intPoints.removeAll();
    m_params1.removeAll();
    m_params2.removeAll();

    if (m_curve1 == NULL || m_curve2 == NULL) {
        return;
    }
    if (m_range1.isBounded() == false || m_range2.isBounded() == false) {
        return;
    }

    int sampleCount = 256;
    for (int i = 0; i <= sampleCount; i++) {
        double ratio = double(i) / double(sampleCount);
        double p1 = m_range1.lowerBound() + (m_range1.upperBound() - m_range1.lowerBound()) * ratio;
        GePoint2d c1 = m_curve1->evalPoint(p1);

        GePoint2d c2 = m_curve2->closestPointTo(c1, m_tol);
        double p2 = m_curve2->paramOf(c2, m_tol);
        if (m_range2.contains(p2) == false) {
            continue;
        }

        if (c1.distanceTo(c2) <= m_tol.equalPoint()) {
            GePoint2d p = GePoint2d((c1.x + c2.x) * 0.5, (c1.y + c2.y) * 0.5);
            this->appendIntersection(p1, p2, p);
        }
    }

    for (int i = 0; i <= sampleCount; i++) {
        double ratio = double(i) / double(sampleCount);
        double p2 = m_range2.lowerBound() + (m_range2.upperBound() - m_range2.lowerBound()) * ratio;
        GePoint2d c2 = m_curve2->evalPoint(p2);

        GePoint2d c1 = m_curve1->closestPointTo(c2, m_tol);
        double p1 = m_curve1->paramOf(c1, m_tol);
        if (m_range1.contains(p1) == false) {
            continue;
        }

        if (c1.distanceTo(c2) <= m_tol.equalPoint()) {
            GePoint2d p = GePoint2d((c1.x + c2.x) * 0.5, (c1.y + c2.y) * 0.5);
            this->appendIntersection(p1, p2, p);
        }
    }

    this->orderWrt1();
}
