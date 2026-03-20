#include "GeCompositeCurve3d.h"
#include "GeInterval.h"
#include "GeLineSeg3d.h"
#include <cmath>

namespace {
double composite3d_abs(double v)
{
    return v < 0.0 ? 0.0 - v : v;
}
}

GeCompositeCurve3d::GeCompositeCurve3d()
    : m_totalLength(0.0)
{
    this->ensureDefaultCurve();
    this->rebuildParamMap();
}

GeCompositeCurve3d::GeCompositeCurve3d(const GeCompositeCurve3d& source)
    : m_totalLength(0.0)
{
    *this = source;
}

GeCompositeCurve3d::GeCompositeCurve3d(const GeCurve3dPtrArray& curveList)
    : m_totalLength(0.0)
{
    this->setCurveList(curveList);
}

GeCompositeCurve3d::~GeCompositeCurve3d()
{
    this->clearOwnedCurves();
}

void GeCompositeCurve3d::clearOwnedCurves()
{
    for (int i = m_curves.length() - 1; i >= 0; i--) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        delete pCurve;
    }
    m_curves.removeAll();
    m_curveList.removeAll();
    m_curveStarts.removeAll();
    m_curveSpans.removeAll();
}

void GeCompositeCurve3d::appendOwnedCurve(const GeCurve3d& curve)
{
    GeCurve3d* pCurve = dynamic_cast<GeCurve3d*>(curve.copy());
    if (pCurve != NULL) {
        m_curves.append(pCurve);
        m_curveList.append(pCurve);
    }
}

void GeCompositeCurve3d::ensureDefaultCurve()
{
    if (m_curves.length() > 0) {
        return;
    }

    GeLineSeg3d lineSeg(GePoint3d::kOrigin, GeVector3d::kXAxis);
    this->appendOwnedCurve(lineSeg);
}

void GeCompositeCurve3d::rebuildParamMap()
{
    m_curveStarts.removeAll();
    m_curveSpans.removeAll();
    m_totalLength = 0.0;

    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        m_curveStarts.append(m_totalLength);

        double span = 0.0;
        if (pCurve != NULL) {
            span = pCurve->length();
            if (span <= GeContext::gTol.equalPoint()) {
                GeInterval range;
                pCurve->getInterval(range);
                if (range.isBounded() == true) {
                    span = composite3d_abs(range.upperBound() - range.lowerBound());
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

int GeCompositeCurve3d::findCurveIndex(double globalParam) const
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

bool GeCompositeCurve3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d
        || entType == Ge::EntityId::kCurve3d
        || entType == this->type();
}

Ge::EntityId GeCompositeCurve3d::type() const
{
    return Ge::EntityId::kCompositeCrv3d;
}

GeCompositeCurve3d* GeCompositeCurve3d::copy() const
{
    return new GeCompositeCurve3d(*this);
}

bool GeCompositeCurve3d::operator == (const GeCompositeCurve3d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeCompositeCurve3d::operator != (const GeCompositeCurve3d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeCompositeCurve3d::isEqualTo(const GeCompositeCurve3d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeCompositeCurve3d::isEqualTo(const GeCompositeCurve3d& entity, const GeTol& tol) const
{
    if (m_curves.length() != entity.m_curves.length()) {
        return false;
    }

    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pA = static_cast<GeCurve3d*>(m_curves[i]);
        GeCurve3d* pB = static_cast<GeCurve3d*>(entity.m_curves[i]);
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

GeCompositeCurve3d& GeCompositeCurve3d::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->transformBy(xfm);
        }
    }
    this->rebuildParamMap();
    return *this;
}

GeCompositeCurve3d& GeCompositeCurve3d::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->translateBy(translateVec);
        }
    }
    return *this;
}

GeCompositeCurve3d& GeCompositeCurve3d::rotateBy(double angle, const GeVector3d& vec)
{
    return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeCompositeCurve3d& GeCompositeCurve3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->rotateBy(angle, vec, wrtPoint);
        }
    }
    return *this;
}

GeCompositeCurve3d& GeCompositeCurve3d::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->mirror(plane);
        }
    }
    return *this;
}

GeCompositeCurve3d& GeCompositeCurve3d::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCompositeCurve3d& GeCompositeCurve3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_curves.length(); i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
        if (pCurve != NULL) {
            pCurve->scaleBy(scaleFactor, wrtPoint);
        }
    }
    this->rebuildParamMap();
    return *this;
}

bool GeCompositeCurve3d::isOn(const GePoint3d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeCompositeCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[i]);
        if (pCurve != NULL && pCurve->isOn(pnt, tol) == true) {
            return true;
        }
    }
    return false;
}

void GeCompositeCurve3d::getCurveList(GeCurve3dPtrArray& curveList) const
{
    curveList = m_curveList;
}

const GeCurve3dPtrArray& GeCompositeCurve3d::getCurveList() const
{
    return m_curveList;
}

GeCompositeCurve3d& GeCompositeCurve3d::setCurveList(const GeCurve3dPtrArray& curveList)
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

double GeCompositeCurve3d::globalToLocalParam(double param, int& crvNum) const
{
    crvNum = this->findCurveIndex(param);
    if (crvNum < 0) {
        return 0.0;
    }

    const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[crvNum]);
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

double GeCompositeCurve3d::localToGlobalParam(double param, int crvNum) const
{
    if (crvNum < 0 || crvNum >= m_curves.length()) {
        return 0.0;
    }

    const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[crvNum]);
    if (pCurve == NULL) {
        return 0.0;
    }

    GeInterval range;
    pCurve->getInterval(range);
    if (range.isBounded() == false) {
        return m_curveStarts[crvNum] + param;
    }

    double localSpan = range.upperBound() - range.lowerBound();
    if (composite3d_abs(localSpan) <= GeContext::gTol.equalPoint()) {
        return m_curveStarts[crvNum];
    }

    double ratio = (param - range.lowerBound()) / localSpan;
    return m_curveStarts[crvNum] + ratio * m_curveSpans[crvNum];
}

GeCompositeCurve3d& GeCompositeCurve3d::operator = (const GeCompositeCurve3d& compCurve)
{
    if (this == &compCurve) {
        return *this;
    }

    this->clearOwnedCurves();

    for (int i = 0; i < compCurve.m_curves.length(); i++) {
        const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(compCurve.m_curves[i]);
        if (pCurve != NULL) {
            this->appendOwnedCurve(*pCurve);
        }
    }

    this->ensureDefaultCurve();
    this->rebuildParamMap();
    return *this;
}

void GeCompositeCurve3d::getInterval(GeInterval& range) const
{
    range.set(0.0, m_totalLength);
}

void GeCompositeCurve3d::getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const
{
    this->getInterval(range);
    startPoint = this->evalPoint(range.lowerBound());
    endPoint = this->evalPoint(range.upperBound());
}

GeCurve3d& GeCompositeCurve3d::reverseParam()
{
    int count = m_curves.length();
    for (int i = 0; i < count; i++) {
        GeCurve3d* pCurve = static_cast<GeCurve3d*>(m_curves[i]);
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

double GeCompositeCurve3d::distanceTo(const GePoint3d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GeCompositeCurve3d::distanceTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d closest = this->closestPointTo(pnt, tol);
    return closest.distanceTo(pnt);
}

double GeCompositeCurve3d::distanceTo(const GeCurve3d& curve) const
{
    return this->distanceTo(curve, GeContext::gTol);
}

double GeCompositeCurve3d::distanceTo(const GeCurve3d& curve, const GeTol& tol) const
{
    return GeCurve3d::distanceTo(curve, tol);
}

GePoint3d GeCompositeCurve3d::closestPointTo(const GePoint3d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint3d GeCompositeCurve3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint3d best = GePoint3d::kOrigin;
    bool hasBest = false;

    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[i]);
        if (pCurve == NULL) {
            continue;
        }

        GePoint3d point = pCurve->closestPointTo(pnt, tol);
        if (hasBest == false || point.distanceTo(pnt) < best.distanceTo(pnt)) {
            best = point;
            hasBest = true;
        }
    }

    return best;
}

GePoint3d GeCompositeCurve3d::closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv) const
{
    return this->closestPointTo(curve3d, pntOnOtherCrv, GeContext::gTol);
}

GePoint3d GeCompositeCurve3d::closestPointTo(const GeCurve3d& curve3d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const
{
    GePoint3d best = GePoint3d::kOrigin;
    GePoint3d bestOther = GePoint3d::kOrigin;
    bool hasBest = false;

    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[i]);
        if (pCurve == NULL) {
            continue;
        }

        GePoint3d other;
        GePoint3d point = pCurve->closestPointTo(curve3d, other, tol);
        if (hasBest == false || point.distanceTo(other) < best.distanceTo(bestOther)) {
            best = point;
            bestOther = other;
            hasBest = true;
        }
    }

    pntOnOtherCrv = bestOther;
    return best;
}

double GeCompositeCurve3d::paramOf(const GePoint3d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

double GeCompositeCurve3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    double bestGlobal = 0.0;
    double bestDist = 0.0;
    bool hasBest = false;

    for (int i = 0; i < m_curves.length(); i++) {
        const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[i]);
        if (pCurve == NULL) {
            continue;
        }

        GePoint3d point = pCurve->closestPointTo(pnt, tol);
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

bool GeCompositeCurve3d::isClosed() const
{
    return this->isClosed(GeContext::gTol);
}

bool GeCompositeCurve3d::isClosed(const GeTol& tol) const
{
    GePoint3d startPoint;
    GePoint3d endPoint;
    if (this->hasStartPoint(startPoint) == false || this->hasEndPoint(endPoint) == false) {
        return false;
    }
    return startPoint.isEqualTo(endPoint, tol);
}

double GeCompositeCurve3d::length() const
{
    return m_totalLength;
}

double GeCompositeCurve3d::length(double fromParam, double toParam) const
{
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}

double GeCompositeCurve3d::length(double fromParam, double toParam, double tol) const
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

    return composite3d_abs(end - start);
}

double GeCompositeCurve3d::paramAtLength(double datumParam, double length) const
{
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}

double GeCompositeCurve3d::paramAtLength(double datumParam, double length, double tol) const
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

bool GeCompositeCurve3d::hasStartPoint(GePoint3d& startPoint) const
{
    if (m_curves.length() <= 0) {
        return false;
    }

    const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[0]);
    if (pCurve == NULL) {
        return false;
    }

    return pCurve->hasStartPoint(startPoint);
}

bool GeCompositeCurve3d::hasEndPoint(GePoint3d& endPoint) const
{
    int count = m_curves.length();
    if (count <= 0) {
        return false;
    }

    const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[count - 1]);
    if (pCurve == NULL) {
        return false;
    }

    return pCurve->hasEndPoint(endPoint);
}

GePoint3d GeCompositeCurve3d::evalPoint(double param) const
{
    int curveIndex = -1;
    double localParam = this->globalToLocalParam(param, curveIndex);
    if (curveIndex < 0 || curveIndex >= m_curves.length()) {
        return GePoint3d::kOrigin;
    }

    const GeCurve3d* pCurve = static_cast<const GeCurve3d*>(m_curves[curveIndex]);
    if (pCurve == NULL) {
        return GePoint3d::kOrigin;
    }

    return pCurve->evalPoint(localParam);
}
