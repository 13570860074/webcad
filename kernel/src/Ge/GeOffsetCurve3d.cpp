#include "GeOffsetCurve3d.h"
#include "GeInterval.h"
#include "GeLine3d.h"
#include "GePlane.h"
#include "GeScale3d.h"
#include <cmath>
#include <limits>

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

GePoint3d offset3d_invalid_point()
{
    double nanValue = std::numeric_limits<double>::quiet_NaN();
    return GePoint3d(nanValue, nanValue, nanValue);
}

bool offset3d_is_degenerate_curve(const GeCurve3d* curve, const GeTol& tol)
{
    if (curve == NULL) {
        return true;
    }

    if (curve->length() > tol.equalPoint()) {
        return false;
    }

    GeInterval range;
    curve->getInterval(range);
    if (range.isBounded()) {
        return std::fabs(range.upperBound() - range.lowerBound()) <= tol.equalPoint();
    }

    return true;
}

GeVector3d offset3d_matrix_column(const GeMatrix3d& matrix, int column)
{
    return GeVector3d(matrix(0, column), matrix(1, column), matrix(2, column));
}

bool offset3d_is_uni_scaled_ortho(const GeMatrix3d& matrix, double& scaleFactor, const GeTol& tol)
{
    GeVector3d xAxis = offset3d_matrix_column(matrix, 0);
    GeVector3d yAxis = offset3d_matrix_column(matrix, 1);
    GeVector3d zAxis = offset3d_matrix_column(matrix, 2);

    double xLength = xAxis.length();
    double yLength = yAxis.length();
    double zLength = zAxis.length();
    double scaleTol = tol.equalVector();

    if (xLength <= scaleTol || yLength <= scaleTol || zLength <= scaleTol) {
        return false;
    }

    if (std::fabs(xLength - yLength) > scaleTol || std::fabs(xLength - zLength) > scaleTol) {
        return false;
    }

    if (std::fabs(xAxis.dotProduct(yAxis)) > scaleTol * xLength * yLength) {
        return false;
    }
    if (std::fabs(xAxis.dotProduct(zAxis)) > scaleTol * xLength * zLength) {
        return false;
    }
    if (std::fabs(yAxis.dotProduct(zAxis)) > scaleTol * yLength * zLength) {
        return false;
    }

    scaleFactor = xLength;
    return true;
}
}

GeOffsetCurve3d::GeOffsetCurve3d()
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(false)
    , m_offsetDist(0.0)
    , m_planeNormal(GeVector3d::kIdentity)
    , m_interval()
    , m_hasCustomInterval(false)
{
    m_transform.setToIdentity();
}

GeOffsetCurve3d::GeOffsetCurve3d(const GeCurve3d& baseCurve, const GeVector3d& planeNormal, double offsetDistance, bool makeCopy)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(false)
    , m_offsetDist(0.0)
    , m_planeNormal(GeVector3d::kIdentity)
    , m_interval()
    , m_hasCustomInterval(false)
{
    m_transform.setToIdentity();
    this->setCurve(baseCurve, makeCopy);
    if (m_pCurve != NULL) {
        m_pCurve->getInterval(m_interval);
    }
    else {
        m_interval.set();
    }
    m_hasCustomInterval = false;

    GeVector3d normalizedNormal = planeNormal;
    if (planeNormal.length() > GeContext::gTol.equalVector()) {
        normalizedNormal = planeNormal.normal();
    }
    this->setNormal(normalizedNormal);
    this->setOffsetDistance(offsetDistance);
}

GeOffsetCurve3d::GeOffsetCurve3d(const GeOffsetCurve3d& source)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(source.m_paramDir)
    , m_offsetDist(source.m_offsetDist)
    , m_planeNormal(source.m_planeNormal)
    , m_transform(source.m_transform)
    , m_interval(source.m_interval)
    , m_hasCustomInterval(source.m_hasCustomInterval)
{
    if (source.m_pCurve != NULL) {
        this->setCurve(*source.m_pCurve, true);
        m_interval = source.m_interval;
        m_hasCustomInterval = source.m_hasCustomInterval;
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
    if (m_pCurve != NULL && m_isOwner == false) {
        GeCurve3d* copiedCurve = dynamic_cast<GeCurve3d*>(m_pCurve->copy());
        if (copiedCurve != NULL) {
            m_pCurve = copiedCurve;
            m_isOwner = true;
        }
    }
    return m_pCurve;
}

const GeCurve3d* GeOffsetCurve3d::curve() const
{
    return m_pCurve;
}

GeVector3d GeOffsetCurve3d::normal() const
{
    return m_planeNormal;
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
    m_interval = offsetCurve.m_interval;
    m_hasCustomInterval = offsetCurve.m_hasCustomInterval;

    if (offsetCurve.m_pCurve != NULL) {
        this->setCurve(*offsetCurve.m_pCurve, true);
        m_interval = offsetCurve.m_interval;
        m_hasCustomInterval = offsetCurve.m_hasCustomInterval;
    }

    return *this;
}

bool GeOffsetCurve3d::setInterval(const GeInterval& range)
{
    m_interval = range;
    m_hasCustomInterval = true;
    return m_pCurve != NULL;
}

void GeOffsetCurve3d::getInterval(GeInterval& range) const
{
    range = m_interval;
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
    GeCurve3d* baseCurve = this->mutableCurve();
    if (baseCurve != NULL) {
        baseCurve->reverseParam();
    }
    return *this;
}

GeOffsetCurve3d& GeOffsetCurve3d::transformBy(const GeMatrix3d& xfm)
{
    if (xfm.isEqualTo(GeMatrix3d::kIdentity)) {
        return *this;
    }

    double scaleFactor = 0.0;
    if (xfm.isPerspective() || offset3d_is_uni_scaled_ortho(xfm, scaleFactor, GeContext::gTol) == false) {
        return *this;
    }

    m_transform *= xfm;

    GeCurve3d* baseCurve = this->mutableCurve();
    if (baseCurve != NULL) {
        baseCurve->transformBy(xfm);
    }

    m_offsetDist *= scaleFactor;
    m_planeNormal.transformBy(xfm);
    if (m_planeNormal.length() > GeContext::gTol.equalVector()) {
        m_planeNormal.normalize();
    }

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

    if (m_planeNormal.isEqualTo(entity.m_planeNormal, tol) == false) {
        return false;
    }

    if ((m_interval == entity.m_interval) == false) {
        return false;
    }

    if (m_transform.isEqualTo(entity.m_transform, tol) == false) {
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
        return GePoint3d::kOrigin;
    }

    GeInterval range;
    this->getInterval(range);

    double bestParam = 0.0;
    double bestDist = 0.0;
    bool hasBest = false;

    if (range.isBounded() == true) {
        int sampleCount = 129;
        int bestIndex = 0;
        for (int i = 0; i < sampleCount; i++) {
            double ratio = 0.0;
            if (sampleCount > 1) {
                ratio = double(i) / double(sampleCount - 1);
            }
            double param = range.lowerBound() + (range.upperBound() - range.lowerBound()) * ratio;
            GePoint3d p = this->evalLocalPoint(param, tol);
            double dist = p.distanceTo(pnt);
            if (hasBest == false || dist < bestDist) {
                hasBest = true;
                bestDist = dist;
                bestParam = param;
                bestIndex = i;
            }
        }

        if (hasBest == true && sampleCount > 2) {
            double span = range.upperBound() - range.lowerBound();
            double sampleStep = span / double(sampleCount - 1);
            double left = bestParam;
            double right = bestParam;
            if (bestIndex > 0) {
                left = bestParam - sampleStep;
            }
            if (bestIndex + 1 < sampleCount) {
                right = bestParam + sampleStep;
            }

            if (right < left) {
                double tmp = left;
                left = right;
                right = tmp;
            }
            if (left < range.lowerBound()) {
                left = range.lowerBound();
            }
            if (right > range.upperBound()) {
                right = range.upperBound();
            }

            if (right > left) {
                for (int iter = 0; iter < 24; ++iter) {
                    double p1 = left + (right - left) / 3.0;
                    double p2 = right - (right - left) / 3.0;
                    double d1 = this->evalLocalPoint(p1, tol).distanceTo(pnt);
                    double d2 = this->evalLocalPoint(p2, tol).distanceTo(pnt);
                    if (d1 <= d2) {
                        right = p2;
                    }
                    else {
                        left = p1;
                    }
                }

                double refinedParam = (left + right) * 0.5;
                double refinedDist = this->evalLocalPoint(refinedParam, tol).distanceTo(pnt);
                if (refinedDist < bestDist) {
                    bestDist = refinedDist;
                    bestParam = refinedParam;
                }
            }
        }
    }
    else {
        GePoint3d baseClosest = m_pCurve->closestPointTo(pnt, tol);
        bestParam = m_pCurve->paramOf(baseClosest, tol);
        hasBest = true;
    }

    if (hasBest == false) {
        return GePoint3d::kOrigin;
    }

    return this->evalLocalPoint(bestParam, tol);
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
    if (m_pCurve == NULL) {
        return 0.0;
    }

    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        GePoint3d baseClosest = m_pCurve->closestPointTo(pnt, tol);
        return m_pCurve->paramOf(baseClosest, tol);
    }

    double bestParam = range.lowerBound();
    double bestDist = this->evalLocalPoint(bestParam, tol).distanceTo(pnt);
    int bestIndex = 0;

    int sampleCount = 129;
    for (int i = 1; i < sampleCount; i++) {
        double ratio = double(i) / double(sampleCount - 1);
        double param = range.lowerBound() + (range.upperBound() - range.lowerBound()) * ratio;
        double dist = this->evalLocalPoint(param, tol).distanceTo(pnt);
        if (dist < bestDist) {
            bestDist = dist;
            bestParam = param;
            bestIndex = i;
        }
    }

    if (sampleCount > 2) {
        double span = range.upperBound() - range.lowerBound();
        double sampleStep = span / double(sampleCount - 1);
        double left = bestParam;
        double right = bestParam;
        if (bestIndex > 0) {
            left = bestParam - sampleStep;
        }
        if (bestIndex + 1 < sampleCount) {
            right = bestParam + sampleStep;
        }

        if (right < left) {
            double tmp = left;
            left = right;
            right = tmp;
        }
        if (left < range.lowerBound()) {
            left = range.lowerBound();
        }
        if (right > range.upperBound()) {
            right = range.upperBound();
        }

        if (right > left) {
            for (int iter = 0; iter < 24; ++iter) {
                double p1 = left + (right - left) / 3.0;
                double p2 = right - (right - left) / 3.0;
                double d1 = this->evalLocalPoint(p1, tol).distanceTo(pnt);
                double d2 = this->evalLocalPoint(p2, tol).distanceTo(pnt);
                if (d1 <= d2) {
                    right = p2;
                }
                else {
                    left = p1;
                }
            }

            double refinedParam = (left + right) * 0.5;
            double refinedDist = this->evalLocalPoint(refinedParam, tol).distanceTo(pnt);
            if (refinedDist < bestDist) {
                bestParam = refinedParam;
            }
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
    return param;
}

GeVector3d GeOffsetCurve3d::tangentAt(double param, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return GeVector3d::kXAxis;
    }

    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        m_pCurve->getInterval(range);
    }
    double step = offset3d_param_step(range);

    double mapped = this->mapParam(param);
    double p0 = mapped - step;
    double p1 = mapped + step;

    if (range.isBounded()) {
        double lower = range.lowerBound();
        double upper = range.upperBound();
        if (p0 < lower) {
            p0 = lower;
        }
        if (p1 > upper) {
            p1 = upper;
        }
        if (p1 <= p0) {
            if (mapped <= lower) {
                p0 = lower;
                p1 = lower + step;
                if (p1 > upper) {
                    p1 = upper;
                }
            }
            else {
                p1 = upper;
                p0 = upper - step;
                if (p0 < lower) {
                    p0 = lower;
                }
            }
        }
    }

    GePoint3d a = m_pCurve->evalPoint(p0);
    GePoint3d b = m_pCurve->evalPoint(p1);
    GeVector3d tan = b - a;

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

    if (offset3d_is_degenerate_curve(m_pCurve, tol)) {
        return offset3d_invalid_point();
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
    return this->evalLocalPoint(param, GeContext::gTol);
}

void GeOffsetCurve3d::getSplitCurves(double param, GeCurve3d*& piece1, GeCurve3d*& piece2) const
{
    piece1 = NULL;
    piece2 = NULL;

    if (m_pCurve == NULL) {
        return;
    }

    GeInterval range;
    this->getInterval(range);
    GeInterval exactRange = range;
    exactRange.setTolerance(0.0);
    if (range.isBounded() == false || exactRange.contains(param) == false) {
        return;
    }

    GeOffsetCurve3d* offsetPiece1 = new GeOffsetCurve3d(*m_pCurve, m_planeNormal, m_offsetDist, true);
    offsetPiece1->m_paramDir = m_paramDir;
    offsetPiece1->m_transform = m_transform;
    offsetPiece1->setInterval(GeInterval(range.lowerBound(), param));
    piece1 = offsetPiece1;

    GeOffsetCurve3d* offsetPiece2 = new GeOffsetCurve3d(*m_pCurve, m_planeNormal, m_offsetDist, true);
    offsetPiece2->m_paramDir = m_paramDir;
    offsetPiece2->m_transform = m_transform;
    offsetPiece2->setInterval(GeInterval(param, range.upperBound()));
    piece2 = offsetPiece2;
}
