#include "GeCurveCurveInt3d.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
void curve_int3d_sort_by_param(GeDoubleArray& sortParams, GeDoubleArray& peer1, GeDoubleArray& peer2, GePoint3dArray& points)
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

                GePoint3d point = points[i];
                points[i] = points[j];
                points[j] = point;
            }
        }
    }
}
}

GeCurveCurveInt3d::GeCurveCurveInt3d()
    : m_curve1(NULL)
    , m_curve2(NULL)
    , m_planeNormal(GeVector3d::kIdentity)
    , m_tol(GeContext::gTol)
{
    m_range1.set();
    m_range2.set();
}

GeCurveCurveInt3d::GeCurveCurveInt3d(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeVector3d& planeNormal, const GeTol& tol)
    : m_curve1(NULL)
    , m_curve2(NULL)
    , m_planeNormal(planeNormal)
    , m_tol(tol)
{
    this->set(curve1, curve2, planeNormal, tol);
}

GeCurveCurveInt3d::GeCurveCurveInt3d(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeInterval& range1, const GeInterval& range2, const GeVector3d& planeNormal, const GeTol& tol)
    : m_curve1(NULL)
    , m_curve2(NULL)
    , m_planeNormal(planeNormal)
    , m_tol(tol)
{
    this->set(curve1, curve2, range1, range2, planeNormal, tol);
}

GeCurveCurveInt3d::GeCurveCurveInt3d(const GeCurveCurveInt3d& source)
    : m_curve1(source.m_curve1)
    , m_curve2(source.m_curve2)
    , m_range1(source.m_range1)
    , m_range2(source.m_range2)
    , m_planeNormal(source.m_planeNormal)
    , m_tol(source.m_tol)
    , m_intPoints(source.m_intPoints)
    , m_params1(source.m_params1)
    , m_params2(source.m_params2)
{
}

bool GeCurveCurveInt3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d || entType == this->type();
}

Ge::EntityId GeCurveCurveInt3d::type() const
{
    return Ge::EntityId::kCurveCurveInt3d;
}

GeCurveCurveInt3d* GeCurveCurveInt3d::copy() const
{
    return new GeCurveCurveInt3d(*this);
}

bool GeCurveCurveInt3d::operator == (const GeCurveCurveInt3d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeCurveCurveInt3d::operator != (const GeCurveCurveInt3d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeCurveCurveInt3d::isEqualTo(const GeCurveCurveInt3d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeCurveCurveInt3d::isEqualTo(const GeCurveCurveInt3d& entity, const GeTol& tol) const
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

GeCurveCurveInt3d& GeCurveCurveInt3d::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].transformBy(xfm);
    }
    m_planeNormal.transformBy(xfm);
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i] += translateVec;
    }
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::rotateBy(double angle, const GeVector3d& vec)
{
    return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeCurveCurveInt3d& GeCurveCurveInt3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    m_planeNormal.rotateBy(angle, vec);
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].mirror(plane);
    }
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCurveCurveInt3d& GeCurveCurveInt3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    for (int i = 0; i < m_intPoints.length(); i++) {
        m_intPoints[i].scaleBy(scale, wrtPoint);
    }
    return *this;
}

bool GeCurveCurveInt3d::isOn(const GePoint3d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeCurveCurveInt3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_intPoints.length(); i++) {
        if (m_intPoints[i].isEqualTo(pnt, tol) == true) {
            return true;
        }
    }
    return false;
}

const GeCurve3d* GeCurveCurveInt3d::curve1() const
{
    return m_curve1;
}

const GeCurve3d* GeCurveCurveInt3d::curve2() const
{
    return m_curve2;
}

void GeCurveCurveInt3d::getIntRanges(GeInterval& range1, GeInterval& range2) const
{
    range1 = m_range1;
    range2 = m_range2;
}

GeVector3d GeCurveCurveInt3d::planeNormal() const
{
    return m_planeNormal;
}

GeTol GeCurveCurveInt3d::tolerance() const
{
    return m_tol;
}

int GeCurveCurveInt3d::numIntPoints() const
{
    return m_intPoints.length();
}

GePoint3d GeCurveCurveInt3d::intPoint(int intNum) const
{
    if (intNum < 0 || intNum >= m_intPoints.length()) {
        return GePoint3d::kOrigin;
    }
    return m_intPoints[intNum];
}

void GeCurveCurveInt3d::getIntParams(int intNum, double& param1, double& param2) const
{
    if (intNum < 0 || intNum >= m_params1.length() || intNum >= m_params2.length()) {
        param1 = 0.0;
        param2 = 0.0;
        return;
    }
    param1 = m_params1[intNum];
    param2 = m_params2[intNum];
}

void GeCurveCurveInt3d::getPointOnCurve1(int intNum, GePointOnCurve3d& intPnt) const
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

void GeCurveCurveInt3d::getPointOnCurve2(int intNum, GePointOnCurve3d& intPnt) const
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

void GeCurveCurveInt3d::getIntConfigs(int intNum, Ge::GeXConfig& config1wrt2, Ge::GeXConfig& config2wrt1) const
{
    config1wrt2 = Ge::kNotDefined;
    config2wrt1 = Ge::kNotDefined;
}

bool GeCurveCurveInt3d::isTangential(int intNum) const
{
    return false;
}

bool GeCurveCurveInt3d::isTransversal(int intNum) const
{
    return false;
}

double GeCurveCurveInt3d::intPointTol(int intNum) const
{
    return m_tol.equalPoint();
}

int GeCurveCurveInt3d::overlapCount() const
{
    return 0;
}

bool GeCurveCurveInt3d::overlapDirection(int overlapNum) const
{
    return false;
}

void GeCurveCurveInt3d::getOverlapRanges(int overlapNum, GeInterval& range1, GeInterval& range2) const
{
    range1.set();
    range2.set();
}

void GeCurveCurveInt3d::changeCurveOrder()
{
    const GeCurve3d* curve = m_curve1;
    m_curve1 = m_curve2;
    m_curve2 = curve;

    GeInterval range = m_range1;
    m_range1 = m_range2;
    m_range2 = range;

    GeDoubleArray params = m_params1;
    m_params1 = m_params2;
    m_params2 = params;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::orderWrt1()
{
    curve_int3d_sort_by_param(m_params1, m_params2, m_params1, m_intPoints);
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::orderWrt2()
{
    curve_int3d_sort_by_param(m_params2, m_params1, m_params2, m_intPoints);
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::set(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeVector3d& planeNormal, const GeTol& tol)
{
    GeInterval range1;
    GeInterval range2;
    curve1.getInterval(range1);
    curve2.getInterval(range2);
    return this->set(curve1, curve2, range1, range2, planeNormal, tol);
}

GeCurveCurveInt3d& GeCurveCurveInt3d::set(const GeCurve3d& curve1, const GeCurve3d& curve2, const GeInterval& range1, const GeInterval& range2, const GeVector3d& planeNormal, const GeTol& tol)
{
    m_curve1 = &curve1;
    m_curve2 = &curve2;
    m_range1 = range1;
    m_range2 = range2;
    m_planeNormal = planeNormal;
    m_tol = tol;
    this->recalcIntersections();
    return *this;
}

GeCurveCurveInt3d& GeCurveCurveInt3d::operator = (const GeCurveCurveInt3d& source)
{
    if (this == &source) {
        return *this;
    }

    m_curve1 = source.m_curve1;
    m_curve2 = source.m_curve2;
    m_range1 = source.m_range1;
    m_range2 = source.m_range2;
    m_planeNormal = source.m_planeNormal;
    m_tol = source.m_tol;
    m_intPoints = source.m_intPoints;
    m_params1 = source.m_params1;
    m_params2 = source.m_params2;
    return *this;
}

void GeCurveCurveInt3d::appendIntersection(double param1, double param2, const GePoint3d& pnt)
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

void GeCurveCurveInt3d::recalcIntersections()
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
        GePoint3d c1 = m_curve1->evalPoint(p1);

        GePoint3d c2 = m_curve2->closestPointTo(c1, m_tol);
        double p2 = m_curve2->paramOf(c2, m_tol);
        if (m_range2.contains(p2) == false) {
            continue;
        }

        if (c1.distanceTo(c2) <= m_tol.equalPoint()) {
            GePoint3d p((c1.x + c2.x) * 0.5, (c1.y + c2.y) * 0.5, (c1.z + c2.z) * 0.5);
            this->appendIntersection(p1, p2, p);
        }
    }

    for (int i = 0; i <= sampleCount; i++) {
        double ratio = double(i) / double(sampleCount);
        double p2 = m_range2.lowerBound() + (m_range2.upperBound() - m_range2.lowerBound()) * ratio;
        GePoint3d c2 = m_curve2->evalPoint(p2);

        GePoint3d c1 = m_curve1->closestPointTo(c2, m_tol);
        double p1 = m_curve1->paramOf(c1, m_tol);
        if (m_range1.contains(p1) == false) {
            continue;
        }

        if (c1.distanceTo(c2) <= m_tol.equalPoint()) {
            GePoint3d p((c1.x + c2.x) * 0.5, (c1.y + c2.y) * 0.5, (c1.z + c2.z) * 0.5);
            this->appendIntersection(p1, p2, p);
        }
    }

    this->orderWrt1();
}
