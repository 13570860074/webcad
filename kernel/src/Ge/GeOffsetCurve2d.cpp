#include "GeOffsetCurve2d.h"
#include "GeInterval.h"
#include "GeLine2d.h"
#include "GeScale2d.h"
#include <cmath>

namespace {
double offset2d_param_step(const GeInterval& range)
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

GeOffsetCurve2d::GeOffsetCurve2d()
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(true)
    , m_offsetDist(0.0)
{
    m_transform.setToIdentity();
}

GeOffsetCurve2d::GeOffsetCurve2d(const GeCurve2d& baseCurve, double offsetDistance, bool makeCopy)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(true)
    , m_offsetDist(offsetDistance)
{
    m_transform.setToIdentity();
    this->setCurve(baseCurve, makeCopy);
}

GeOffsetCurve2d::GeOffsetCurve2d(const GeOffsetCurve2d& source)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(source.m_paramDir)
    , m_offsetDist(source.m_offsetDist)
    , m_transform(source.m_transform)
{
    if (source.m_pCurve != NULL) {
        this->setCurve(*source.m_pCurve, true);
    }
}

GeOffsetCurve2d::~GeOffsetCurve2d()
{
    this->clearOwnedCurve();
}

void GeOffsetCurve2d::clearOwnedCurve()
{
    if (m_isOwner == true && m_pCurve != NULL) {
        delete m_pCurve;
    }
    m_pCurve = NULL;
    m_isOwner = false;
}

GeCurve2d* GeOffsetCurve2d::mutableCurve()
{
    return m_pCurve;
}

const GeCurve2d* GeOffsetCurve2d::curve() const
{
    return m_pCurve;
}

double GeOffsetCurve2d::offsetDistance() const
{
    return m_offsetDist;
}

bool GeOffsetCurve2d::paramDirection() const
{
    return m_paramDir;
}

GeMatrix2d GeOffsetCurve2d::transformation() const
{
    return m_transform;
}

GeOffsetCurve2d& GeOffsetCurve2d::setCurve(const GeCurve2d& baseCurve, bool makeCopy)
{
    this->clearOwnedCurve();

    if (makeCopy == true) {
        m_pCurve = dynamic_cast<GeCurve2d*>(baseCurve.copy());
        m_isOwner = (m_pCurve != NULL);
        if (m_pCurve == NULL) {
            m_pCurve = const_cast<GeCurve2d*>(&baseCurve);
        }
    }
    else {
        m_pCurve = const_cast<GeCurve2d*>(&baseCurve);
        m_isOwner = false;
    }

    return *this;
}

GeOffsetCurve2d& GeOffsetCurve2d::setOffsetDistance(double distance)
{
    m_offsetDist = distance;
    return *this;
}

GeOffsetCurve2d& GeOffsetCurve2d::operator = (const GeOffsetCurve2d& offsetCurve)
{
    if (this == &offsetCurve) {
        return *this;
    }

    this->clearOwnedCurve();
    m_paramDir = offsetCurve.m_paramDir;
    m_offsetDist = offsetCurve.m_offsetDist;
    m_transform = offsetCurve.m_transform;

    if (offsetCurve.m_pCurve != NULL) {
        this->setCurve(*offsetCurve.m_pCurve, true);
    }

    return *this;
}

void GeOffsetCurve2d::getInterval(GeInterval& range) const
{
    if (m_pCurve == NULL) {
        range.set();
        return;
    }
    m_pCurve->getInterval(range);
}

void GeOffsetCurve2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const
{
    this->getInterval(range);
    if (range.isBounded() == false) {
        startPoint = GePoint2d::kOrigin;
        endPoint = GePoint2d::kOrigin;
        return;
    }

    startPoint = this->evalPoint(range.lowerBound());
    endPoint = this->evalPoint(range.upperBound());
}

GeCurve2d& GeOffsetCurve2d::reverseParam()
{
    m_paramDir = !m_paramDir;
    m_offsetDist = 0.0 - m_offsetDist;
    return *this;
}

GeOffsetCurve2d& GeOffsetCurve2d::transformBy(const GeMatrix2d& xfm)
{
    GeMatrix2d product;
    product.setToProduct(xfm, m_transform);
    m_transform = product;
    return *this;
}

GeOffsetCurve2d& GeOffsetCurve2d::translateBy(const GeVector2d& translateVec)
{
    GeMatrix2d mat;
    mat.setToTranslation(translateVec);
    return this->transformBy(mat);
}

GeOffsetCurve2d& GeOffsetCurve2d::rotateBy(double angle)
{
    return this->rotateBy(angle, GePoint2d::kOrigin);
}

GeOffsetCurve2d& GeOffsetCurve2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    GeMatrix2d mat;
    mat.setToRotation(angle, wrtPoint);
    return this->transformBy(mat);
}

GeOffsetCurve2d& GeOffsetCurve2d::mirror(const GeLine2d& line)
{
    GeMatrix2d mat;
    mat.setToMirroring(line);
    return this->transformBy(mat);
}

GeOffsetCurve2d& GeOffsetCurve2d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeOffsetCurve2d& GeOffsetCurve2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    GeMatrix2d mat;
    mat.setToScaling(scaleFactor, wrtPoint);
    return this->transformBy(mat);
}

bool GeOffsetCurve2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity2d
        || entType == Ge::EntityId::kCurve2d
        || entType == this->type();
}

Ge::EntityId GeOffsetCurve2d::type() const
{
    return Ge::EntityId::kOffsetCurve2d;
}

GeOffsetCurve2d* GeOffsetCurve2d::copy() const
{
    return new GeOffsetCurve2d(*this);
}

bool GeOffsetCurve2d::operator == (const GeOffsetCurve2d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeOffsetCurve2d::operator != (const GeOffsetCurve2d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeOffsetCurve2d::isEqualTo(const GeOffsetCurve2d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeOffsetCurve2d::isEqualTo(const GeOffsetCurve2d& entity, const GeTol& tol) const
{
    if (std::fabs(m_offsetDist - entity.m_offsetDist) > tol.equalPoint()) {
        return false;
    }

    if (m_paramDir != entity.m_paramDir) {
        return false;
    }

    if (m_pCurve == NULL || entity.m_pCurve == NULL) {
        return m_pCurve == entity.m_pCurve;
    }

    return m_pCurve->isEqualTo(*entity.m_pCurve, tol);
}

bool GeOffsetCurve2d::isOn(const GePoint2d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeOffsetCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d closest = this->closestPointTo(pnt, tol);
    return closest.distanceTo(pnt) <= tol.equalPoint();
}

double GeOffsetCurve2d::distanceTo(const GePoint2d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GeOffsetCurve2d::distanceTo(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d closest = this->closestPointTo(pnt, tol);
    return closest.distanceTo(pnt);
}

double GeOffsetCurve2d::distanceTo(const GeCurve2d& curve) const
{
    return this->distanceTo(curve, GeContext::gTol);
}

double GeOffsetCurve2d::distanceTo(const GeCurve2d& curve, const GeTol& tol) const
{
    return GeCurve2d::distanceTo(curve, tol);
}

GePoint2d GeOffsetCurve2d::closestPointTo(const GePoint2d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint2d GeOffsetCurve2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        GePoint2d zero = GePoint2d::kOrigin;
        zero.transformBy(m_transform);
        return zero;
    }

    GeMatrix2d inv = m_transform.inverse();
    GePoint2d local = pnt;
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
            GePoint2d p = this->evalLocalPoint(param, tol);
            double dist = p.distanceTo(local);
            if (hasBest == false || dist < bestDist) {
                hasBest = true;
                bestDist = dist;
                bestParam = param;
            }
        }
    }
    else {
        GePoint2d baseClosest = m_pCurve->closestPointTo(local, tol);
        bestParam = m_pCurve->paramOf(baseClosest, tol);
        hasBest = true;
    }

    if (hasBest == false) {
        GePoint2d zero = GePoint2d::kOrigin;
        zero.transformBy(m_transform);
        return zero;
    }

    GePoint2d result = this->evalLocalPoint(bestParam, tol);
    result.transformBy(m_transform);
    return result;
}

GePoint2d GeOffsetCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const
{
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}

GePoint2d GeOffsetCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const
{
    return GeCurve2d::closestPointTo(curve2d, pntOnOtherCrv, tol);
}

double GeOffsetCurve2d::paramOf(const GePoint2d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

double GeOffsetCurve2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        GePoint2d on = this->closestPointTo(pnt, tol);
        GeMatrix2d inv = m_transform.inverse();
        on.transformBy(inv);
        return m_pCurve != NULL ? m_pCurve->paramOf(on, tol) : 0.0;
    }

    GeMatrix2d inv = m_transform.inverse();
    GePoint2d local = pnt;
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

bool GeOffsetCurve2d::isClosed() const
{
    return this->isClosed(GeContext::gTol);
}

bool GeOffsetCurve2d::isClosed(const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return false;
    }
    return m_pCurve->isClosed(tol);
}

double GeOffsetCurve2d::length() const
{
    return GeCurve2d::length();
}

double GeOffsetCurve2d::length(double fromParam, double toParam) const
{
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}

double GeOffsetCurve2d::length(double fromParam, double toParam, double tol) const
{
    return GeCurve2d::length(fromParam, toParam, tol);
}

double GeOffsetCurve2d::paramAtLength(double datumParam, double length) const
{
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}

double GeOffsetCurve2d::paramAtLength(double datumParam, double length, double tol) const
{
    return GeCurve2d::paramAtLength(datumParam, length, tol);
}

bool GeOffsetCurve2d::hasStartPoint(GePoint2d& startPoint) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return false;
    }
    startPoint = this->evalPoint(range.lowerBound());
    return true;
}

bool GeOffsetCurve2d::hasEndPoint(GePoint2d& endPoint) const
{
    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        return false;
    }
    endPoint = this->evalPoint(range.upperBound());
    return true;
}

double GeOffsetCurve2d::mapParam(double param) const
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

GeVector2d GeOffsetCurve2d::tangentAt(double param, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return GeVector2d::kXAxis;
    }

    GeInterval range;
    m_pCurve->getInterval(range);
    double step = offset2d_param_step(range);

    double mapped = this->mapParam(param);
    double p0 = mapped - step;
    double p1 = mapped + step;

    GePoint2d a = m_pCurve->evalPoint(p0);
    GePoint2d b = m_pCurve->evalPoint(p1);
    GeVector2d tan = b - a;

    if (m_paramDir == false) {
        tan = tan * (-1.0);
    }

    if (tan.length() <= tol.equalPoint()) {
        return GeVector2d::kXAxis;
    }

    tan.normalize();
    return tan;
}

GePoint2d GeOffsetCurve2d::evalLocalPoint(double param, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return GePoint2d::kOrigin;
    }

    double mapped = this->mapParam(param);
    GePoint2d base = m_pCurve->evalPoint(mapped);
    GeVector2d tan = this->tangentAt(param, tol);
    GeVector2d normal = tan.perpVector();

    if (normal.length() > tol.equalPoint()) {
        normal.normalize();
    }
    else {
        normal = GeVector2d::kYAxis;
    }

    return base + normal * m_offsetDist;
}

GePoint2d GeOffsetCurve2d::evalPoint(double param) const
{
    GePoint2d p = this->evalLocalPoint(param, GeContext::gTol);
    p.transformBy(m_transform);
    return p;
}
