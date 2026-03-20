#include "GeSurfaceCurve3dTo2d.h"

#include "GeEntity2d.h"
#include "GeEntity3d.h"
#include "GeInterval.h"
#include "GeLine2d.h"

namespace {
double surface_curve3d_to2d_refine_param(const GeSurfaceCurve3dTo2d& curve, const GePoint2d& target, double lower, double upper)
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

GeSurfaceCurve3dTo2d::GeSurfaceCurve3dTo2d()
    : m_pCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    m_uvXfm.setToIdentity();
}

GeSurfaceCurve3dTo2d::GeSurfaceCurve3dTo2d(const GeSurfaceCurve3dTo2d& source)
    : m_pCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    m_uvXfm.setToIdentity();
    *this = source;
}

GeSurfaceCurve3dTo2d::GeSurfaceCurve3dTo2d(const GeCurve3d& curve3d, const GeSurface& surf)
    : m_pCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    m_uvXfm.setToIdentity();
    set(&curve3d, &surf, kCopy);
}

GeSurfaceCurve3dTo2d::GeSurfaceCurve3dTo2d(const GeCurve3d* pCurve3d, const GeSurface* pSurface, OwnershipFlag ownership)
    : m_pCurve(NULL)
    , m_pSurface(NULL)
    , m_ownership(kReference)
{
    m_uvXfm.setToIdentity();
    set(pCurve3d, pSurface, ownership);
}

GeSurfaceCurve3dTo2d::~GeSurfaceCurve3dTo2d()
{
    clearOwnedData();
}

void GeSurfaceCurve3dTo2d::clearOwnedData()
{
    if (m_pCurve != NULL) {
        int curvePolicy = static_cast<int>(m_ownership) & static_cast<int>(kCurveMask);
        if (curvePolicy == static_cast<int>(kCurveCopy) || curvePolicy == static_cast<int>(kCurveOwn)) {
            delete m_pCurve;
        }
    }

    if (m_pSurface != NULL) {
        int surfacePolicy = static_cast<int>(m_ownership) & static_cast<int>(kSurfaceMask);
        if (surfacePolicy == static_cast<int>(kSurfaceCopy) || surfacePolicy == static_cast<int>(kSurfaceOwn)) {
            delete m_pSurface;
        }
    }

    m_pCurve = NULL;
    m_pSurface = NULL;
    m_ownership = kReference;
    m_uvXfm.setToIdentity();
}

bool GeSurfaceCurve3dTo2d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == this->type();
}

Ge::EntityId GeSurfaceCurve3dTo2d::type() const
{
    return Ge::kSurfaceCurve3dTo2d;
}

GeSurfaceCurve3dTo2d* GeSurfaceCurve3dTo2d::copy() const
{
    return new GeSurfaceCurve3dTo2d(*this);
}

bool GeSurfaceCurve3dTo2d::operator == (const GeSurfaceCurve3dTo2d& entity) const
{
    return isEqualTo(entity);
}

bool GeSurfaceCurve3dTo2d::operator != (const GeSurfaceCurve3dTo2d& entity) const
{
    return !isEqualTo(entity);
}

bool GeSurfaceCurve3dTo2d::isEqualTo(const GeSurfaceCurve3dTo2d& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSurfaceCurve3dTo2d::isEqualTo(const GeSurfaceCurve3dTo2d& entity, const GeTol& tol) const
{
    if (m_pCurve == NULL || m_pSurface == NULL || entity.m_pCurve == NULL || entity.m_pSurface == NULL) {
        return m_pCurve == entity.m_pCurve && m_pSurface == entity.m_pSurface;
    }

    GeInterval thisRange;
    GeInterval otherRange;
    m_pCurve->getInterval(thisRange);
    entity.m_pCurve->getInterval(otherRange);
    if (!thisRange.isBounded() || !otherRange.isBounded()) {
        return false;
    }

    GePoint2d thisStart = evalPoint(thisRange.lowerBound());
    GePoint2d thisEnd = evalPoint(thisRange.upperBound());
    GePoint2d otherStart = entity.evalPoint(otherRange.lowerBound());
    GePoint2d otherEnd = entity.evalPoint(otherRange.upperBound());
    return thisStart.isEqualTo(otherStart, tol) && thisEnd.isEqualTo(otherEnd, tol);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::transformBy(const GeMatrix2d& xfm)
{
    GeMatrix2d product;
    product.setToProduct(xfm, m_uvXfm);
    m_uvXfm = product;
    return *this;
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::translateBy(const GeVector2d& translateVec)
{
    GeMatrix2d xfm;
    xfm.setToTranslation(translateVec);
    return transformBy(xfm);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::rotateBy(double angle)
{
    GeMatrix2d xfm;
    xfm.setToRotation(angle);
    return transformBy(xfm);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
    GeMatrix2d xfm;
    xfm.setToRotation(angle, wrtPoint);
    return transformBy(xfm);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::mirror(const GeLine2d& line)
{
    GeMatrix2d xfm;
    xfm.setToMirroring(line);
    return transformBy(xfm);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::scaleBy(double scaleFactor)
{
    GeMatrix2d xfm;
    xfm.setToScaling(scaleFactor);
    return transformBy(xfm);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
    GeMatrix2d xfm;
    xfm.setToScaling(scaleFactor, wrtPoint);
    return transformBy(xfm);
}

bool GeSurfaceCurve3dTo2d::isOn(const GePoint2d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeSurfaceCurve3dTo2d::isOn(const GePoint2d& pnt, const GeTol& tol) const
{
    double param = 0.0;
    return isOn(pnt, param, tol);
}

GeSurfaceCurve3dTo2d& GeSurfaceCurve3dTo2d::operator = (const GeSurfaceCurve3dTo2d& src)
{
    if (this == &src) {
        return *this;
    }

    clearOwnedData();
    if (src.m_pCurve == NULL || src.m_pSurface == NULL) {
        return *this;
    }

    set(src.m_pCurve, src.m_pSurface, kCopy);
    m_uvXfm = src.m_uvXfm;
    return *this;
}

const GeCurve3d* GeSurfaceCurve3dTo2d::curve() const
{
    return m_pCurve;
}

const GeSurface* GeSurfaceCurve3dTo2d::surface() const
{
    return m_pSurface;
}

void GeSurfaceCurve3dTo2d::getGeomExtents(GeExtents2d& extents) const
{
    extents = GeExtents2d::kInvalid;
    if (m_pCurve == NULL || m_pSurface == NULL) {
        return;
    }

    GeInterval range;
    m_pCurve->getInterval(range);
    if (!range.isBounded()) {
        return;
    }

    GePoint2dArray points;
    const int sampleCount = 65;
    for (int i = 0; i <= sampleCount; ++i) {
        double r = static_cast<double>(i) / static_cast<double>(sampleCount);
        double param = range.lowerBound() + (range.upperBound() - range.lowerBound()) * r;
        points.append(evalPoint(param));
    }

    for (int i = 0; i < points.length(); ++i) {
        extents.addPoint(points[i]);
    }
}

bool GeSurfaceCurve3dTo2d::set(const GeCurve3d* pCurve3d, const GeSurface* pSurface, OwnershipFlag ownership)
{
    clearOwnedData();
    if (pCurve3d == NULL || pSurface == NULL) {
        return false;
    }

    m_ownership = ownership;

    int curvePolicy = static_cast<int>(ownership) & static_cast<int>(kCurveMask);
    if (curvePolicy == static_cast<int>(kCurveCopy)) {
        GeEntity3d* copied = pCurve3d->copy();
        m_pCurve = dynamic_cast<GeCurve3d*>(copied);
        if (m_pCurve == NULL) {
            delete copied;
            return false;
        }
    }
    else {
        m_pCurve = const_cast<GeCurve3d*>(pCurve3d);
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

    m_uvXfm.setToIdentity();
    return true;
}

void GeSurfaceCurve3dTo2d::getInterval(GeInterval& range) const
{
    if (m_pCurve == NULL) {
        range.set();
        return;
    }
    m_pCurve->getInterval(range);
}

void GeSurfaceCurve3dTo2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const
{
    getInterval(range);
    if (!range.isBounded()) {
        startPoint = GePoint2d::kOrigin;
        endPoint = GePoint2d::kOrigin;
        return;
    }

    startPoint = evalPoint(range.lowerBound());
    endPoint = evalPoint(range.upperBound());
}

GePoint2d GeSurfaceCurve3dTo2d::evalPoint(double param) const
{
    if (m_pCurve == NULL || m_pSurface == NULL) {
        return GePoint2d::kOrigin;
    }

    GePoint3d p3 = m_pCurve->evalPoint(param);
    GePoint2d uv = m_pSurface->paramOf(p3, GeContext::gTol);
    uv.transformBy(m_uvXfm);
    return uv;
}

double GeSurfaceCurve3dTo2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const
{
    if (m_pCurve == NULL || m_pSurface == NULL) {
        return 0.0;
    }

    GeInterval range;
    m_pCurve->getInterval(range);
    if (!range.isBounded()) {
        return 0.0;
    }

    const int sampleCount = 256;
    double step = (range.upperBound() - range.lowerBound()) / static_cast<double>(sampleCount);

    double bestParam = range.lowerBound();
    GePoint2d bestPoint = evalPoint(bestParam);
    double bestDist = bestPoint.distanceTo(pnt);
    for (int i = 1; i <= sampleCount; ++i) {
        double param = range.lowerBound() + step * static_cast<double>(i);
        GePoint2d uv = evalPoint(param);
        double dist = uv.distanceTo(pnt);
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

    return surface_curve3d_to2d_refine_param(*this, pnt, localLower, localUpper);
}

bool GeSurfaceCurve3dTo2d::isOn(const GePoint2d& pnt, double& param, const GeTol& tol) const
{
    param = paramOf(pnt, tol);
    GePoint2d onCurve = evalPoint(param);
    return onCurve.isEqualTo(pnt, tol);
}