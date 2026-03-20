#include "GeCompositeCurve2d.h"
#include "GeInterval.h"
#include "GeLineSeg2d.h"
#include "GeScale2d.h"
#include <cmath>

namespace {
double composite2d_abs(double v)
{
    return v < 0.0 ? 0.0 - v : v;
}
}

GeCompositeCurve2d::GeCompositeCurve2d()
    : m_totalLength(0.0)
{
    this->ensureDefaultCurve();
    this->rebuildParamMap();
}

GeCompositeCurve2d::GeCompositeCurve2d(const GeCompositeCurve2d& source)
    : m_totalLength(0.0)
{
    *this = source;
}

GeCompositeCurve2d::GeCompositeCurve2d(const GeCurve2dPtrArray& curveList)
    : m_totalLength(0.0)
{
    this->setCurveList(curveList);
}

GeCompositeCurve2d::~GeCompositeCurve2d()
{
    this->clearOwnedCurves();
}

void GeCompositeCurve2d::clearOwnedCurves()
{
    for (int i = m_curves.length() - 1; i >= 0; i--) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        delete pCurve;
    }
    m_curves.removeAll();
    m_curveList.removeAll();
    m_curveStarts.removeAll();
    m_curveSpans.removeAll();
}

void GeCompositeCurve2d::appendOwnedCurve(const GeCurve2d& curve)
{
    GeCurve2d* pCurve = dynamic_cast<GeCurve2d*>(curve.copy());
    if (pCurve != NULL) {
        m_curves.append(pCurve);
        m_curveList.append(pCurve);
    }
}

void GeCompositeCurve2d::ensureDefaultCurve()
{
    if (m_curves.length() > 0) {
        return;
    }

    GeLineSeg2d lineSeg(GePoint2d::kOrigin, GeVector2d::kXAxis);
    this->appendOwnedCurve(lineSeg);
}

void GeCompositeCurve2d::rebuildParamMap()
{
    m_curveStarts.removeAll();
    m_curveSpans.removeAll();
    m_totalLength = 0.0;

    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        m_curveStarts.append(m_totalLength);

        double span = 0.0;
        if (pCurve != NULL) {
            span = pCurve->length();
            if (span <= GeContext::gTol.equalPoint()) {
                GeInterval range;
                pCurve->getInterval(range);
                if (range.isBounded() == true) {
                    span = composite2d_abs(range.upperBound() - range.lowerBound());
                }
            }
        }

        if (span <= GeContext::gTol.equalPoint()) {
            span = 1.0;
        }

        m_curveSpans.append(span);
        m_totalLength += span;
    }

    if (m_curves.length() == 0) {
        m_totalLength = 0.0;
    }
}

int GeCompositeCurve2d::findCurveIndex(double globalParam) const
{
    int count = m_curves.length();
    if (count <= 0) {
        return -1;
    }

    if (globalParam <= 0.0) {
        return 0;
    }
    if (globalParam >= m_totalLength) {
        return count - 1;
    }

    for (int i = 0; i < count; i++) {
        double start = m_curveStarts[i];
        double end = start + m_curveSpans[i];
        if (globalParam <= end || i == count - 1) {
            return i;
        }
    }

    return count - 1;
}

bool GeCompositeCurve2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity2d
        || entType == Ge::EntityId::kCurve2d
        || entType == this->type();
}

Ge::EntityId GeCompositeCurve2d::type() const
{
    return Ge::EntityId::kCompositeCrv2d;
}

GeCompositeCurve2d* GeCompositeCurve2d::copy() const
{
    return new GeCompositeCurve2d(*this);
}

bool GeCompositeCurve2d::operator == (const GeCompositeCurve2d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeCompositeCurve2d::operator != (const GeCompositeCurve2d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeCompositeCurve2d::isEqualTo(const GeCompositeCurve2d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeCompositeCurve2d::isEqualTo(const GeCompositeCurve2d& entity, const GeTol& tol) const
{
    if (m_curves.length() != entity.m_curves.length()) {
        return false;
    }

    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pA = static_cast<GeCurve2d*>(m_curves[i]);
        GeCurve2d* pB = static_cast<GeCurve2d*>(entity.m_curves[i]);
        if (pA == NULL || pB == NULL) {
            if (pA != pB) {
                return false;
            }
            continue;
        }
        if (pA->isEqualTo(*pB, tol) == false) {
            return false;
        }
    }

    return true;
}

GeCompositeCurve2d& GeCompositeCurve2d::transformBy(const GeMatrix2d& xfm)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->transformBy(xfm);
        }
    }
    this->rebuildParamMap();
    return *this;
}

GeCompositeCurve2d& GeCompositeCurve2d::translateBy(const GeVector2d& translateVec)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->translateBy(translateVec);
        }
    }
    return *this;
}

GeCompositeCurve2d& GeCompositeCurve2d::rotateBy(double angle)
{
    return this->rotateBy(angle, GePoint2d::kOrigin);
}

GeCompositeCurve2d& GeCompositeCurve2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->rotateBy(angle, wrtPoint);
        }
    }
    return *this;
}

GeCompositeCurve2d& GeCompositeCurve2d::mirror(const GeLine2d& line)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->mirror(line);
        }
    }
    return *this;
}

GeCompositeCurve2d& GeCompositeCurve2d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}

GeCompositeCurve2d& GeCompositeCurve2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->scaleBy(scaleFactor, wrtPoint);
        }
    }
    this->rebuildParamMap();
    return *this;
}

bool GeCompositeCurve2d::isOn(const GePoint2d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeCompositeCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL && pCurve->isOn(pnt, tol) == true) {
            return true;
        }
    }
    return false;
}

void GeCompositeCurve2d::getCurveList(GeCurve2dPtrArray& curveList) const
{
    curveList = m_curveList;
}

const GeCurve2dPtrArray& GeCompositeCurve2d::getCurveList() const
{
    return m_curveList;
}

GeCompositeCurve2d& GeCompositeCurve2d::setCurveList(const GeCurve2dPtrArray& curveList)
{
    this->clearOwnedCurves();

    for (int i = 0; i < curveList.length(); i++) {
        if (curveList[i] != NULL) {
            this->appendOwnedCurve(*curveList[i]);
        }
    }

    this->ensureDefaultCurve();
    this->rebuildParamMap();
    return *this;
}

double GeCompositeCurve2d::globalToLocalParam(double param, int& crvNum) const
{
    crvNum = this->findCurveIndex(param);
    if (crvNum < 0) {
        return 0.0;
    }

    const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[crvNum]);
    if (pCurve == NULL) {
        return 0.0;
    }

    GeInterval range;
    pCurve->getInterval(range);
    if (range.isBounded() == false) {
        return param - m_curveStarts[crvNum];
    }

    double span = m_curveSpans[crvNum];
    if (span <= GeContext::gTol.equalPoint()) {
        return range.lowerBound();
    }

    double localRatio = (param - m_curveStarts[crvNum]) / span;
    if (localRatio < 0.0) {
        localRatio = 0.0;
    }
    if (localRatio > 1.0) {
        localRatio = 1.0;
    }

    return range.lowerBound() + (range.upperBound() - range.lowerBound()) * localRatio;
}

double GeCompositeCurve2d::localToGlobalParam(double param, int crvNum) const
{
    if (crvNum < 0 || crvNum >= m_curves.length()) {
        return 0.0;
    }

    const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[crvNum]);
    if (pCurve == NULL) {
        return 0.0;
    }

    GeInterval range;
    pCurve->getInterval(range);
    if (range.isBounded() == false) {
        return m_curveStarts[crvNum] + param;
    }

    double localSpan = range.upperBound() - range.lowerBound();
    if (composite2d_abs(localSpan) <= GeContext::gTol.equalPoint()) {
        return m_curveStarts[crvNum];
    }

    double ratio = (param - range.lowerBound()) / localSpan;
    return m_curveStarts[crvNum] + ratio * m_curveSpans[crvNum];
}

GeCompositeCurve2d& GeCompositeCurve2d::operator = (const GeCompositeCurve2d& compCurve)
{
    if (this == &compCurve) {
        return *this;
    }

    this->clearOwnedCurves();

    for (int i = 0; i < compCurve.m_curves.length(); i++) {
        const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(compCurve.m_curves[i]);
        if (pCurve != NULL) {
            this->appendOwnedCurve(*pCurve);
        }
    }

    this->ensureDefaultCurve();
    this->rebuildParamMap();
    return *this;
}

void GeCompositeCurve2d::getInterval(GeInterval& range) const
{
    range.set(0.0, m_totalLength);
}

void GeCompositeCurve2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const
{
    this->getInterval(range);
    startPoint = this->evalPoint(range.lowerBound());
    endPoint = this->evalPoint(range.upperBound());
}

GeCurve2d& GeCompositeCurve2d::reverseParam()
{
    int count = m_curves.length();
    for (int i = 0; i < count; i++) {
        GeCurve2d* pCurve = static_cast<GeCurve2d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->reverseParam();
        }
    }

    for (int i = 0; i < count / 2; i++) {
        void* left = m_curves[i];
        m_curves[i] = m_curves[count - i - 1];
        m_curves[count - i - 1] = left;
    }

    this->rebuildParamMap();
    return *this;
}

double GeCompositeCurve2d::distanceTo(const GePoint2d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GeCompositeCurve2d::distanceTo(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d closest = this->closestPointTo(pnt, tol);
    return closest.distanceTo(pnt);
}

double GeCompositeCurve2d::distanceTo(const GeCurve2d& curve) const
{
    return this->distanceTo(curve, GeContext::gTol);
}

double GeCompositeCurve2d::distanceTo(const GeCurve2d& curve, const GeTol& tol) const
{
    return GeCurve2d::distanceTo(curve, tol);
}

GePoint2d GeCompositeCurve2d::closestPointTo(const GePoint2d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint2d GeCompositeCurve2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const
{
    GePoint2d best = GePoint2d::kOrigin;
    bool hasBest = false;

    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[i]);
        if (pCurve == NULL) {
            continue;
        }

        GePoint2d point = pCurve->closestPointTo(pnt, tol);
        if (hasBest == false || point.distanceTo(pnt) < best.distanceTo(pnt)) {
            best = point;
            hasBest = true;
        }
    }

    return best;
}

GePoint2d GeCompositeCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const
{
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}

GePoint2d GeCompositeCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const
{
    GePoint2d best = GePoint2d::kOrigin;
    GePoint2d bestOther = GePoint2d::kOrigin;
    bool hasBest = false;

    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[i]);
        if (pCurve == NULL) {
            continue;
        }

        GePoint2d other;
        GePoint2d point = pCurve->closestPointTo(curve2d, other, tol);
        if (hasBest == false || point.distanceTo(other) < best.distanceTo(bestOther)) {
            best = point;
            bestOther = other;
            hasBest = true;
        }
    }

    pntOnOtherCrv = bestOther;
    return best;
}

double GeCompositeCurve2d::paramOf(const GePoint2d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

double GeCompositeCurve2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const
{
    double bestGlobal = 0.0;
    double bestDist = 0.0;
    bool hasBest = false;

    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[i]);
        if (pCurve == NULL) {
            continue;
        }

        GePoint2d point = pCurve->closestPointTo(pnt, tol);
        double dist = point.distanceTo(pnt);
        if (hasBest == false || dist < bestDist) {
            double local = pCurve->paramOf(point, tol);
            bestGlobal = this->localToGlobalParam(local, i);
            bestDist = dist;
            hasBest = true;
        }
    }

    return bestGlobal;
}

bool GeCompositeCurve2d::isClosed() const
{
    return this->isClosed(GeContext::gTol);
}

bool GeCompositeCurve2d::isClosed(const GeTol& tol) const
{
    GePoint2d startPoint;
    GePoint2d endPoint;
    if (this->hasStartPoint(startPoint) == false || this->hasEndPoint(endPoint) == false) {
        return false;
    }
    return startPoint.isEqualTo(endPoint, tol);
}

double GeCompositeCurve2d::length() const
{
    return m_totalLength;
}

double GeCompositeCurve2d::length(double fromParam, double toParam) const
{
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}

double GeCompositeCurve2d::length(double fromParam, double toParam, double tol) const
{
    double start = fromParam;
    double end = toParam;

    if (start < 0.0) {
        start = 0.0;
    }
    if (end < 0.0) {
        end = 0.0;
    }
    if (start > m_totalLength) {
        start = m_totalLength;
    }
    if (end > m_totalLength) {
        end = m_totalLength;
    }

    return composite2d_abs(end - start);
}

double GeCompositeCurve2d::paramAtLength(double datumParam, double length) const
{
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}

double GeCompositeCurve2d::paramAtLength(double datumParam, double length, double tol) const
{
    double param = datumParam + length;
    if (param < 0.0) {
        param = 0.0;
    }
    if (param > m_totalLength) {
        param = m_totalLength;
    }
    return param;
}

bool GeCompositeCurve2d::hasStartPoint(GePoint2d& startPoint) const
{
    if (m_curves.length() <= 0) {
        return false;
    }

    const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[0]);
    if (pCurve == NULL) {
        return false;
    }

    return pCurve->hasStartPoint(startPoint);
}

bool GeCompositeCurve2d::hasEndPoint(GePoint2d& endPoint) const
{
    int count = m_curves.length();
    if (count <= 0) {
        return false;
    }

    const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[count - 1]);
    if (pCurve == NULL) {
        return false;
    }

    return pCurve->hasEndPoint(endPoint);
}

GePoint2d GeCompositeCurve2d::evalPoint(double param) const
{
    int curveIndex = -1;
    double localParam = this->globalToLocalParam(param, curveIndex);
    if (curveIndex < 0 || curveIndex >= m_curves.length()) {
        return GePoint2d::kOrigin;
    }

    const GeCurve2d* pCurve = static_cast<const GeCurve2d*>(m_curves[curveIndex]);
    if (pCurve == NULL) {
        return GePoint2d::kOrigin;
    }

    return pCurve->evalPoint(localParam);
}
