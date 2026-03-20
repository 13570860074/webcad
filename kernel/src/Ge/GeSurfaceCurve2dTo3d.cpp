#include "GeSurfaceCurve2dTo3d.h"

#include "GeEntity2d.h"
#include "GeEntity3d.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GeScale3d.h"

namespace {
double surface_curve2d_to3d_refine_param(const GeSurfaceCurve2dTo3d& curve, const GePoint3d& target, double lower, double upper)
{
    if (upper < lower) {
        double tmp = lower;
        lower = upper;
        upper = tmp;
    }

    double left = lower;
    double right = upper;
    for (int i = 0; i < 24; ++i) {
        double m1 = left + (right - left) / 3.0;
        double m2 = right - (right - left) / 3.0;
        double d1 = curve.evalPoint(m1).distanceTo(target);
        double d2 = curve.evalPoint(m2).distanceTo(target);
        if (d1 <= d2) {
            right = m2;
        }
        else {
            left = m1;
        }
    }
    return (left + right) * 0.5;
}
}

GeSurfaceCurve2dTo3d::GeSurfaceCurve2dTo3d()
    : m_pUvCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
}

GeSurfaceCurve2dTo3d::GeSurfaceCurve2dTo3d(const GeSurfaceCurve2dTo3d& source)
    : m_pUvCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    *this = source;
}

GeSurfaceCurve2dTo3d::GeSurfaceCurve2dTo3d(const GeCurve2d& uvc, const GeSurface& surf)
    : m_pUvCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    set(&uvc, &surf, kCopy);
}

GeSurfaceCurve2dTo3d::GeSurfaceCurve2dTo3d(const GeCurve2d* pUvCurve, const GeSurface* pSurface, OwnershipFlag ownership)
    : m_pUvCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    set(pUvCurve, pSurface, ownership);
}

GeSurfaceCurve2dTo3d::~GeSurfaceCurve2dTo3d()
{
    clearOwnedData();
}

void GeSurfaceCurve2dTo3d::clearOwnedData()
{
    if (m_pUvCurve != NULL) {
        int curvePolicy = static_cast<int>(m_ownership) & static_cast<int>(kCurveMask);
        if (curvePolicy == static_cast<int>(kCurveCopy) || curvePolicy == static_cast<int>(kCurveOwn)) {
            delete m_pUvCurve;
        }
    }

    if (m_pSurface != NULL) {
        int surfacePolicy = static_cast<int>(m_ownership) & static_cast<int>(kSurfaceMask);
        if (surfacePolicy == static_cast<int>(kSurfaceCopy) || surfacePolicy == static_cast<int>(kSurfaceOwn)) {
            delete m_pSurface;
        }
    }

    m_pUvCurve = NULL;
    m_pSurface = NULL;
    m_ownership = kReference;
}

bool GeSurfaceCurve2dTo3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == Ge::kCurve3d || entType == this->type();
}

Ge::EntityId GeSurfaceCurve2dTo3d::type() const
{
    return Ge::kSurfaceCurve2dTo3d;
}

GeSurfaceCurve2dTo3d* GeSurfaceCurve2dTo3d::copy() const
{
    return new GeSurfaceCurve2dTo3d(*this);
}

bool GeSurfaceCurve2dTo3d::operator == (const GeSurfaceCurve2dTo3d& entity) const
{
    return isEqualTo(entity);
}

bool GeSurfaceCurve2dTo3d::operator != (const GeSurfaceCurve2dTo3d& entity) const
{
    return !isEqualTo(entity);
}

bool GeSurfaceCurve2dTo3d::isEqualTo(const GeSurfaceCurve2dTo3d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSurfaceCurve2dTo3d::isEqualTo(const GeSurfaceCurve2dTo3d& entity, const GeTol& tol) const
{
    if (m_pUvCurve == NULL || m_pSurface == NULL || entity.m_pUvCurve == NULL || entity.m_pSurface == NULL) {
        return m_pUvCurve == entity.m_pUvCurve && m_pSurface == entity.m_pSurface;
    }

    GeInterval thisRange;
    GeInterval otherRange;
    m_pUvCurve->getInterval(thisRange);
    entity.m_pUvCurve->getInterval(otherRange);
    if (!thisRange.isBounded() || !otherRange.isBounded()) {
        return false;
    }

    GePoint3d thisStart = evalPoint(thisRange.lowerBound());
    GePoint3d thisEnd = evalPoint(thisRange.upperBound());
    GePoint3d otherStart = entity.evalPoint(otherRange.lowerBound());
    GePoint3d otherEnd = entity.evalPoint(otherRange.upperBound());

    return thisStart.isEqualTo(otherStart, tol) && thisEnd.isEqualTo(otherEnd, tol);
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::transformBy(const GeMatrix3d& xfm)
{
    if (m_pSurface != NULL) {
        m_pSurface->transformBy(xfm);
    }
    return *this;
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::translateBy(const GeVector3d& translateVec)
{
    if (m_pSurface != NULL) {
        m_pSurface->translateBy(translateVec);
    }
    return *this;
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::rotateBy(double angle, const GeVector3d& vec)
{
    if (m_pSurface != NULL) {
        m_pSurface->rotateBy(angle, vec, GePoint3d::kOrigin);
    }
    return *this;
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    if (m_pSurface != NULL) {
        m_pSurface->rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::mirror(const GePlane& plane)
{
    if (m_pSurface != NULL) {
        m_pSurface->mirror(plane);
    }
    return *this;
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::scaleBy(double scaleFactor)
{
    if (m_pSurface != NULL) {
        m_pSurface->scaleBy(scaleFactor, GePoint3d::kOrigin);
    }
    return *this;
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    if (m_pSurface != NULL) {
        m_pSurface->scaleBy(scaleFactor, wrtPoint);
    }
    return *this;
}

bool GeSurfaceCurve2dTo3d::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeSurfaceCurve2dTo3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    double param = 0.0;
    return isOn(pnt, param, tol);
}

GeSurfaceCurve2dTo3d& GeSurfaceCurve2dTo3d::operator = (const GeSurfaceCurve2dTo3d& src)
{
    if (this == &src) {
        return *this;
    }

    clearOwnedData();
    if (src.m_pUvCurve == NULL || src.m_pSurface == NULL) {
        return *this;
    }

    set(src.m_pUvCurve, src.m_pSurface, kCopy);
    return *this;
}

const GeCurve2d* GeSurfaceCurve2dTo3d::curve() const
{
    return m_pUvCurve;
}

const GeSurface* GeSurfaceCurve2dTo3d::surface() const
{
    return m_pSurface;
}

void GeSurfaceCurve2dTo3d::getGeomExtents(GeExtents3d& extents) const
{
    extents = GeExtents3d::kInvalid;
    if (m_pUvCurve == NULL || m_pSurface == NULL) {
        return;
    }

    GeInterval range;
    m_pUvCurve->getInterval(range);
    if (!range.isBounded()) {
        return;
    }

    GePoint3dArray points;
    GeDoubleArray params;
    double span = range.upperBound() - range.lowerBound();
    if (span < 0.0) {
        span = -span;
    }
    double step = span / 64.0;
    if (step <= GeContext::gTol.equalPoint()) {
        step = 1.0;
    }

    getSamplePoints(range.lowerBound(), range.upperBound(), step, points, params, false);
    if (points.length() == 0) {
        points.append(evalPoint(range.lowerBound()));
        points.append(evalPoint(range.upperBound()));
    }

    for (int i = 0; i < points.length(); ++i) {
        extents.addPoint(points[i]);
    }
}

bool GeSurfaceCurve2dTo3d::set(const GeCurve2d* pUvCurve, const GeSurface* pSurface, OwnershipFlag ownership)
{
    clearOwnedData();
    if (pUvCurve == NULL || pSurface == NULL) {
        return false;
    }

    m_ownership = ownership;

    int curvePolicy = static_cast<int>(ownership) & static_cast<int>(kCurveMask);
    if (curvePolicy == static_cast<int>(kCurveCopy)) {
        GeEntity2d* copied = pUvCurve->copy();
        m_pUvCurve = dynamic_cast<GeCurve2d*>(copied);
        if (m_pUvCurve == NULL) {
            delete copied;
            return false;
        }
    }
    else {
        m_pUvCurve = const_cast<GeCurve2d*>(pUvCurve);
    }

    int surfacePolicy = static_cast<int>(ownership) & static_cast<int>(kSurfaceMask);
    if (surfacePolicy == static_cast<int>(kSurfaceCopy)) {
        GeEntity3d* copied = pSurface->copy();
        m_pSurface = dynamic_cast<GeSurface*>(copied);
        if (m_pSurface == NULL) {
            delete copied;
            clearOwnedData();
            return false;
        }
    }
    else {
        m_pSurface = const_cast<GeSurface*>(pSurface);
    }

    return true;
}

void GeSurfaceCurve2dTo3d::getInterval(GeInterval& range) const
{
    if (m_pUvCurve == NULL) {
        range.set();
        return;
    }
    m_pUvCurve->getInterval(range);
}

void GeSurfaceCurve2dTo3d::getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const
{
    getInterval(range);
    if (!range.isBounded()) {
        startPoint = GePoint3d::kOrigin;
        endPoint = GePoint3d::kOrigin;
        return;
    }

    startPoint = evalPoint(range.lowerBound());
    endPoint = evalPoint(range.upperBound());
}

GePoint3d GeSurfaceCurve2dTo3d::evalPoint(double param) const
{
    if (m_pUvCurve == NULL || m_pSurface == NULL) {
        return GePoint3d::kOrigin;
    }

    GePoint2d uv = m_pUvCurve->evalPoint(param);
    return m_pSurface->evalPoint(uv);
}

double GeSurfaceCurve2dTo3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    if (m_pUvCurve == NULL || m_pSurface == NULL) {
        return 0.0;
    }

    GeInterval range;
    m_pUvCurve->getInterval(range);
    if (!range.isBounded()) {
        return 0.0;
    }

    const int sampleCount = 256;
    double step = (range.upperBound() - range.lowerBound()) / static_cast<double>(sampleCount);

    double bestParam = range.lowerBound();
    double bestDist = evalPoint(bestParam).distanceTo(pnt);
    for (int i = 1; i <= sampleCount; ++i) {
        double param = range.lowerBound() + step * static_cast<double>(i);
        double dist = evalPoint(param).distanceTo(pnt);
        if (dist < bestDist) {
            bestDist = dist;
            bestParam = param;
        }
    }

    double localLower = bestParam - step;
    double localUpper = bestParam + step;
    if (localLower < range.lowerBound()) {
        localLower = range.lowerBound();
    }
    if (localUpper > range.upperBound()) {
        localUpper = range.upperBound();
    }
    if (localUpper <= localLower + tol.equalPoint()) {
        return bestParam;
    }

    return surface_curve2d_to3d_refine_param(*this, pnt, localLower, localUpper);
}

bool GeSurfaceCurve2dTo3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const
{
    param = paramOf(pnt, tol);
    GePoint3d onCurve = evalPoint(param);
    return onCurve.isEqualTo(pnt, tol);
}