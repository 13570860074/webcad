#include "GeOffsetCurve2d.h"
#include "GeCircArc2d.h"
#include "GeInterval.h"
#include "GeLine2d.h"
#include "GeScale2d.h"
#include <cmath>
#include <limits>

namespace {
double offset2d_pi()
{
    return std::acos(-1.0);
}

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

GePoint2d offset2d_invalid_point()
{
    double nanValue = std::numeric_limits<double>::quiet_NaN();
    return GePoint2d(nanValue, nanValue);
}

bool offset2d_is_degenerate_curve(const GeCurve2d* curve, const GeTol& tol)
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

GeVector2d offset2d_matrix_column(const GeMatrix2d& matrix, int column)
{
    return GeVector2d(matrix(0, column), matrix(1, column));
}

bool offset2d_is_uni_scaled_ortho(const GeMatrix2d& matrix, double& scaleFactor, const GeTol& tol)
{
    GeVector2d xAxis = offset2d_matrix_column(matrix, 0);
    GeVector2d yAxis = offset2d_matrix_column(matrix, 1);

    double xLength = xAxis.length();
    double yLength = yAxis.length();
    double scaleTol = tol.equalVector();

    if (xLength <= scaleTol || yLength <= scaleTol) {
        return false;
    }

    if (std::fabs(xLength - yLength) > scaleTol) {
        return false;
    }

    if (std::fabs(xAxis.dotProduct(yAxis)) > scaleTol * xLength * yLength) {
        return false;
    }

    scaleFactor = xLength;
    if (matrix.det() < 0.0) {
        scaleFactor = -scaleFactor;
    }
    return true;
}

double offset2d_move_into_valid_inter(double lower, double upper, double param)
{
    double period = 2.0 * offset2d_pi();
    double center = (lower + upper) * 0.5;
    double shift = std::floor((center - param) / period + 0.5);
    double bestParam = param + shift * period;
    double bestDelta = std::fabs(bestParam - center);

    for (int offset = -1; offset <= 1; ++offset) {
        double candidate = param + (shift + double(offset)) * period;
        double delta = std::fabs(candidate - center);
        if (delta < bestDelta) {
            bestDelta = delta;
            bestParam = candidate;
        }
    }

    return bestParam;
}

double offset2d_clamp_to_interval(const GeInterval& range, double value)
{
    if (range.isBoundedBelow() && value < range.lowerBound()) {
        return range.lowerBound();
    }
    if (range.isBoundedAbove() && value > range.upperBound()) {
        return range.upperBound();
    }
    return value;
}
}

GeOffsetCurve2d::GeOffsetCurve2d()
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(false)
    , m_offsetDist(0.0)
    , m_interval()
    , m_hasCustomInterval(false)
{
    m_transform.setToIdentity();
}

GeOffsetCurve2d::GeOffsetCurve2d(const GeCurve2d& baseCurve, double offsetDistance, bool makeCopy)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(false)
    , m_offsetDist(0.0)
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
    this->setOffsetDistance(offsetDistance);
}

GeOffsetCurve2d::GeOffsetCurve2d(const GeOffsetCurve2d& source)
    : m_pCurve(NULL)
    , m_isOwner(false)
    , m_paramDir(source.m_paramDir)
    , m_offsetDist(source.m_offsetDist)
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
    if (m_pCurve != NULL && m_isOwner == false) {
        GeCurve2d* copiedCurve = dynamic_cast<GeCurve2d*>(m_pCurve->copy());
        if (copiedCurve != NULL) {
            m_pCurve = copiedCurve;
            m_isOwner = true;
        }
    }
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
    m_interval = offsetCurve.m_interval;
    m_hasCustomInterval = offsetCurve.m_hasCustomInterval;

    if (offsetCurve.m_pCurve != NULL) {
        this->setCurve(*offsetCurve.m_pCurve, true);
        m_interval = offsetCurve.m_interval;
        m_hasCustomInterval = offsetCurve.m_hasCustomInterval;
    }

    return *this;
}

bool GeOffsetCurve2d::setInterval(const GeInterval& range)
{
    m_interval = range;
    m_hasCustomInterval = true;
    return true;
}

void GeOffsetCurve2d::getInterval(GeInterval& range) const
{
    range = m_interval;
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
    GeCurve2d* baseCurve = this->mutableCurve();
    if (baseCurve != NULL) {
        baseCurve->reverseParam();
    }
    return *this;
}

GeOffsetCurve2d& GeOffsetCurve2d::transformBy(const GeMatrix2d& xfm)
{
    if (xfm.isEqualTo(GeMatrix2d::kIdentity)) {
        return *this;
    }

    double scaleFactor = 0.0;
    if (offset2d_is_uni_scaled_ortho(xfm, scaleFactor, GeContext::gTol) == false) {
        return *this;
    }

    m_transform *= xfm;

    GeCurve2d* baseCurve = this->mutableCurve();
    if (baseCurve != NULL) {
        baseCurve->transformBy(xfm);
    }

    m_offsetDist *= scaleFactor;
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
        return GePoint2d::kOrigin;
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
            GePoint2d p = this->evalLocalPoint(param, tol);
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
        GePoint2d baseClosest = m_pCurve->closestPointTo(pnt, tol);
        bestParam = m_pCurve->paramOf(baseClosest, tol);
        hasBest = true;
    }

    if (hasBest == false) {
        return GePoint2d::kOrigin;
    }

    return this->evalLocalPoint(bestParam, tol);
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
    if (m_pCurve == NULL) {
        return 0.0;
    }

    if (m_pCurve->isKindOf(Ge::EntityId::kLinearEnt2d)) {
        return m_pCurve->paramOf(pnt, tol);
    }

    if (m_pCurve->type() == Ge::EntityId::kCircArc2d) {
        double param = m_pCurve->paramOf(pnt, tol);
        const GeCircArc2d* arc = static_cast<const GeCircArc2d*>(m_pCurve);
        if (arc->radius() < std::fabs(m_offsetDist) && arc->isClockWise() == false) {
            GeInterval arcInterval;
            arc->getInterval(arcInterval);
            param = offset2d_move_into_valid_inter(arcInterval.lowerBound(), arcInterval.upperBound(), param + offset2d_pi());
        }

        GeInterval range;
        this->getInterval(range);
        if (range.isBounded()) {
            return offset2d_clamp_to_interval(range, param);
        }
        return param;
    }

    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        GePoint2d baseClosest = m_pCurve->closestPointTo(pnt, tol);
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
    return param;
}

GeVector2d GeOffsetCurve2d::tangentAt(double param, const GeTol& tol) const
{
    if (m_pCurve == NULL) {
        return GeVector2d::kXAxis;
    }

    GeInterval range;
    this->getInterval(range);
    if (range.isBounded() == false) {
        m_pCurve->getInterval(range);
    }
    double step = offset2d_param_step(range);

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

    GePoint2d a = m_pCurve->evalPoint(p0);
    GePoint2d b = m_pCurve->evalPoint(p1);
    GeVector2d tan = b - a;

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

    if (offset2d_is_degenerate_curve(m_pCurve, tol)) {
        return offset2d_invalid_point();
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
    return this->evalLocalPoint(param, GeContext::gTol);
}

void GeOffsetCurve2d::getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const
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

    GeOffsetCurve2d* offsetPiece1 = new GeOffsetCurve2d(*m_pCurve, m_offsetDist, true);
    offsetPiece1->m_paramDir = m_paramDir;
    offsetPiece1->m_transform = m_transform;
    offsetPiece1->setInterval(GeInterval(range.lowerBound(), param));
    piece1 = offsetPiece1;

    GeOffsetCurve2d* offsetPiece2 = new GeOffsetCurve2d(*m_pCurve, m_offsetDist, true);
    offsetPiece2->m_paramDir = m_paramDir;
    offsetPiece2->m_transform = m_transform;
    offsetPiece2->setInterval(GeInterval(param, range.upperBound()));
    piece2 = offsetPiece2;
}
