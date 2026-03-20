#include "GeOffsetCurve3d.h"
#include "GeInterval.h"
#include "GeLine3d.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>

namespace {
double offset3d_param_step(const GeInterval& range)
{
    if (range.isBounded() == true) {
        double span = range.upperBound() - range.lowerBound();
        if (span < 0.0) {
            span = 0.0 - span;
        }
        if (span > 1.0e-6) {
            return span * 1.0e-5;
        }
    }
    return 1.0e-5;
}
}

GeOffsetCurve3d::GeOffsetCurve3d()
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(true)
    , m_offsetDist(0.0)
    , m_planeNormal(GeVector3d::kZAxis)
{
    m_transform.setToIdentity();
}

GeOffsetCurve3d::GeOffsetCurve3d(const GeCurve3d& baseCurve, const GeVector3d& planeNormal, double offsetDistance, bool makeCopy)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(true)
    , m_offsetDist(offsetDistance)
    , m_planeNormal(planeNormal)
{
    m_transform.setToIdentity();
    this->setCurve(baseCurve, makeCopy);
}

GeOffsetCurve3d::GeOffsetCurve3d(const GeOffsetCurve3d& source)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(source.m_paramDir)
    , m_offsetDist(source.m_offsetDist)
    , m_planeNormal(source.m_planeNormal)
    , m_transform(source.m_transform)
{
    if (source.m_pCurve != NULL) {
        this->setCurve(*source.m_pCurve, true);
    }
}

GeOffsetCurve3d::~GeOffsetCurve3d()
{
    this->clearOwnedCurve();
}

void GeOffsetCurve3d::clearOwnedCurve()
{
    if (m_isOwner == true && m_pCurve != NULL) {
        delete m_pCurve;
    }
    m_pCurve = NULL;
    m_isOwner = false;
}

GeCurve3d* GeOffsetCurve3d::mutableCurve()
{
    return m_pCurve;
}

const GeCurve3d* GeOffsetCurve3d::curve() const
{
    return m_pCurve;
}

GeVector3d GeOffsetCurve3d::normal() const
{
    GeVector3d n = m_planeNormal;
    n.transformBy(m_transform);
    if (n.length() > GeContext::gTol.equalPoint()) {
        n.normalize();
    }
    return n;
}

double GeOffsetCurve3d::offsetDistance() const
{
    return m_offsetDist;
}

bool GeOffsetCurve3d::paramDirection() const
{
    return m_paramDir;
}

GeMatrix3d GeOffsetCurve3d::transformation() const
{
    return m_transform;
}

GeOffsetCurve3d& GeOffsetCurve3d::setCurve(const GeCurve3d& baseCurve, bool makeCopy)
{
    this->clearOwnedCurve();

    if (makeCopy == true) {
        m_pCurve = dynamic_cast<GeCurve3d*>(baseCurve.copy());
        m_isOwner = (m_pCurve != NULL);
        if (m_pCurve == NULL) {
            m_pCurve = const_cast<GeCurve3d*>(&baseCurve);
        }
    }
    else {
        m_pCurve = const_cast<GeCurve3d*>(&baseCurve);
        m_isOwner = false;
    }

    return *this;
}

GeOffsetCurve3d& GeOffsetCurve3d::setNormal(const GeVector3d& planeNormal)
{
    m_planeNormal = planeNormal;
    return *this;
}

GeOffsetCurve3d& GeOffsetCurve3d::setOffsetDistance(double distance)
{
    m_offsetDist = distance;
    return *this;
}

GeOffsetCurve3d& GeOffsetCurve3d::operator = (const GeOffsetCurve3d& offsetCurve)
{
    if (this == &offsetCurve) {
        return *this;
    }

    this->clearOwnedCurve();
    m_paramDir = offsetCurve.m_paramDir;
    m_offsetDist = offsetCurve.m_offsetDist;
    m_planeNormal = offsetCurve.m_planeNormal;
    m_transform = offsetCurve.m_transform;

    if (offsetCurve.m_pCurve != NULL) {
        this->setCurve(*offsetCurve.m_pCurve, true);
    }

    return *this;
}

void GeOffsetCurve3d::getInterval(GeInterval& range) const
{
    if (m_pCurve == NULL) {
        range.set();
        return;
    }
    m_pCurve->getInterval(range);
}

void GeOffsetCurve3d::getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const
{
    this->getInterval(range);
    if (range.isBounded() == false) {
        startPoint = GePoint3d::kOrigin;
        endPoint = GePoint3d::kOrigin;
        return;
    }

    startPoint = this->evalPoint(range.lowerBound());
    endPoint = this->evalPoint(range.upperBound());
}

GeCurve3d& GeOffsetCurve3d::reverseParam()
{
    m_paramDir = !m_paramDir;
    m_offsetDist = 0.0 - m_offsetDist;
    return *this;
}

GeOffsetCurve3d& GeOffsetCurve3d::transformBy(const GeMatrix3d& xfm)
{
    GeMatrix3d product;
    product.setToProduct(xfm, m_transform);
    m_transform = product;
    return *this;
}

GeOffsetCurve3d& GeOffsetCurve3d::translateBy(const GeVector3d& translateVec)
{
    GeMatrix3d mat;
    mat.setToTranslation(translateVec);
    return this->transformBy(mat);
}

GeOffsetCurve3d& GeOffsetCurve3d::rotateBy(double angle, const GeVector3d& vec)
{
    return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeOffsetCurve3d& GeOffsetCurve3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    GeMatrix3d mat;
    mat.setToRotation(angle, vec, wrtPoint);
    return this->transformBy(mat);
}

GeOffsetCurve3d& GeOffsetCurve3d::mirror(const GePlane& plane)
{
    GeMatrix3d mat;
    mat.setToMirroring(plane);
    return this->transformBy(mat);
}

GeOffsetCurve3d& GeOffsetCurve3d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeOffsetCurve3d& GeOffsetCurve3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeMatrix3d mat;
    mat.setToScaling(scaleFactor, wrtPoint);
    return this->transformBy(mat);
}

bool GeOffsetCurve3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d
        || entType == Ge::EntityId::kCurve3d
        || entType == this->type();
}

Ge::EntityId GeOffsetCurve3d::type() const
{
    return Ge::EntityId::kOffsetCurve3d;
}

GeOffsetCurve3d* GeOffsetCurve3d::copy() const
{
    return new GeOffsetCurve3d(*this);
}

bool GeOffsetCurve3d::operator == (const GeOffsetCurve3d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeOffsetCurve3d::operator != (const GeOffsetCurve3d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeOffsetCurve3d::isEqualTo(const GeOffsetCurve3d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeOffsetCurve3d::isEqualTo(const GeOffsetCurve3d& entity, const GeTol& tol) const
{
    if (std::fabs(m_offsetDist - entity.m_offsetDist) > tol.equalPoint()) {
        return false;
    }

    if (m_paramDir != entity.m_paramDir) {
        return false;
    }

    if (this->normal().isEqualTo(entity.normal(), tol) == false) {
        return false;
    }

    if (m_pCurve == NULL || entity.m_pCurve == NULL) {
        return m_pCurve == entity.m_pCurve;
    }

    return m_pCurve->isEqualTo(*entity.m_pCurve, tol);
}

bool GeOffsetCurve3d::isOn(const GePoint3d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeOffsetCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d closest = this->closestPointTo(pnt, tol);
    return closest.distanceTo(pnt) <= tol.equalPoint();
}

double GeOffsetCurve3d::distanceTo(const GePoint3d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GeOffsetCurve3d::distanceTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d closest = this->closestPointTo(pnt, tol);
    return closest.distanceTo(pnt);
}

double GeOffsetCurve3d::distanceTo(const GeCurve3d& curve) const
{
    return this->distanceTo(curve, GeContext::gTol);
}

double GeOffsetCurve3d::distanceTo(const GeCurve3d& curve, const GeTol& tol) const
{
    return GeCurve3d::distanceTo(curve, tol);
}

GePoint3d GeOffsetCurve3d::closestPointTo(const GePoint3d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint3d GeOffsetCurve3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        GePoint3d zero = GePoint3d::kOrigin;
        zero.transformBy(m_transform);
        return zero;
    }

    GeMatrix3d inv = m_transform.inverse();
    GePoint3d local = pnt;
    local.transformBy(inv);

    GeInterval range;
    this->getInterval(range);

    double bestParam = 0.0;
    double bestDist = 0.0;
    bool hasBest = false;

    if (range.isBounded() == true) {
        int sampleCount = 129;
        for (int i = 0; i < sampleCount; i++) {
            double ratio = 0.0;
            if (sampleCount > 1) {
                ratio = double(i) / double(sampleCount - 1);
            }
            double param = range.lowerBound() + (range.upperBound() - range.lowerBound()) * ratio;
            GePoint3d p = this->evalLocalPoint(param, tol);
            double dist = p.distanceTo(local);
            if (hasBest == false || dist < bestDist) {
                hasBest = true;
                bestDist = dist;
                bestParam = param;
            }
        }
    }
    else {
        GePoint3d baseClosest = m_pCurve->closestPointTo(local, tol);
        bestParam = m_pCurve->paramOf(baseClosest, tol);
        hasBest = true;
    }

    if (hasBest == false) {
        GePoint3d zero = GePoint3d::kOrigin;
        zero.transformBy(m_transform);
        return zero;
    }

    GePoint3d result = this->evalLocalPoint(bestParam, tol);
    result.transformBy(m_transform);
    return result;
}

GePoint3d GeOffsetCurve3d::closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
    return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}

GePoint3d GeOffsetCurve3d::closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
    return GeCurve3d::closestPointTo(curve3d, pntOnOtherCrv, tol);
}

double GeOffsetCurve3d::paramOf(const GePoint3d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

double GeOffsetCurve3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        GePoint3d on = this->closestPointTo(pnt, tol);
        GeMatrix3d inv = m_transform.inverse();
        on.transformBy(inv);
        return m_pCurve != NULL ? m_pCurve->paramOf(on, tol) : 0.0;
    }

    GeMatrix3d inv = m_transform.inverse();
    GePoint3d local = pnt;
    local.transformBy(inv);

    double bestParam = range.lowerBound();
    double bestDist = this->evalLocalPoint(bestParam, tol).distanceTo(local);

    int sampleCount = 129;
    for (int i = 1; i < sampleCount; i++) {
        double ratio = double(i) / double(sampleCount - 1);
        double param = range.lowerBound() + (range.upperBound() - range.lowerBound()) * ratio;
        double dist = this->evalLocalPoint(param, tol).distanceTo(local);
        if (dist < bestDist) {
            bestDist = dist;
            bestParam = param;
        }
    }
    return bestParam;
}

bool GeOffsetCurve3d::isClosed() const
{
    return this->isClosed(GeContext::gTol);
}

bool GeOffsetCurve3d::isClosed(const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return false;
    }
    return m_pCurve->isClosed(tol);
}

double GeOffsetCurve3d::length() const
{
    return GeCurve3d::length();
}

double GeOffsetCurve3d::length(double fromParam, double toParam) const
{
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}

double GeOffsetCurve3d::length(double fromParam, double toParam, double tol) const
{
    return GeCurve3d::length(fromParam, toParam, tol);
}

double GeOffsetCurve3d::paramAtLength(double datumParam, double length) const
{
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}

double GeOffsetCurve3d::paramAtLength(double datumParam, double length, double tol) const
{
    return GeCurve3d::paramAtLength(datumParam, length, tol);
}

bool GeOffsetCurve3d::hasStartPoint(GePoint3d& startPoint) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return false;
    }
    startPoint = this->evalPoint(range.lowerBound());
    return true;
}

bool GeOffsetCurve3d::hasEndPoint(GePoint3d& endPoint) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return false;
    }
    endPoint = this->evalPoint(range.upperBound());
    return true;
}

double GeOffsetCurve3d::mapParam(double param) const
{
    if (m_paramDir == true) {
        return param;
    }

    GeInterval range;
    if (m_pCurve != NULL) {
        m_pCurve->getInterval(range);
    }

    if (range.isBounded() == true) {
        return range.lowerBound() + range.upperBound() - param;
    }

    return 0.0 - param;
}

GeVector3d GeOffsetCurve3d::tangentAt(double param, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return GeVector3d::kXAxis;
    }

    GeInterval range;
    m_pCurve->getInterval(range);
    double step = offset3d_param_step(range);

    double mapped = this->mapParam(param);
    double p0 = mapped - step;
    double p1 = mapped + step;

    GePoint3d a = m_pCurve->evalPoint(p0);
    GePoint3d b = m_pCurve->evalPoint(p1);
    GeVector3d tan = b - a;

    if (m_paramDir == false) {
        tan = tan * (-1.0);
    }

    if (tan.length() <= tol.equalPoint()) {
        return GeVector3d::kXAxis;
    }

    tan.normalize();
    return tan;
}

GePoint3d GeOffsetCurve3d::evalLocalPoint(double param, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return GePoint3d::kOrigin;
    }

    double mapped = this->mapParam(param);
    GePoint3d base = m_pCurve->evalPoint(mapped);

    GeVector3d tan = this->tangentAt(param, tol);
    GeVector3d n = m_planeNormal;
    if (n.length() > tol.equalPoint()) {
        n.normalize();
    }
    else {
        n = GeVector3d::kZAxis;
    }

    GeVector3d offsetDir = n.crossProduct(tan);
    if (offsetDir.length() <= tol.equalPoint()) {
        return base;
    }

    offsetDir.normalize();
    return base + offsetDir * m_offsetDist;
}

GePoint3d GeOffsetCurve3d::evalPoint(double param) const
{
    GePoint3d p = this->evalLocalPoint(param, GeContext::gTol);
    p.transformBy(m_transform);
    return p;
}
