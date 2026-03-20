#include "GeCurveSurfInt.h"

#include "GeCurve2d.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GePointOnCurve3d.h"
#include "GePointOnSurface.h"
#include "GeScale3d.h"

GeCurveSurfInt::GeCurveSurfInt()
    : m_curve(NULL)
    , m_surface(NULL)
    , m_tol(GeContext::gTol)
{
}

GeCurveSurfInt::GeCurveSurfInt(const GeCurve3d& curve, const GeSurface& surf, const GeTol& tol)
    : m_curve(NULL)
    , m_surface(NULL)
    , m_tol(tol)
{
    set(curve, surf, tol);
}

GeCurveSurfInt::GeCurveSurfInt(const GeCurveSurfInt& source)
    : m_curve(source.m_curve)
    , m_surface(source.m_surface)
    , m_tol(source.m_tol)
    , m_intPoints(source.m_intPoints)
    , m_curveParams(source.m_curveParams)
    , m_surfaceParams(source.m_surfaceParams)
{
}

bool GeCurveSurfInt::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == this->type();
}

Ge::EntityId GeCurveSurfInt::type() const
{
    return Ge::kCurveSurfaceInt;
}

GeCurveSurfInt* GeCurveSurfInt::copy() const
{
    return new GeCurveSurfInt(*this);
}

bool GeCurveSurfInt::operator == (const GeCurveSurfInt& entity) const
{
    return isEqualTo(entity);
}

bool GeCurveSurfInt::operator != (const GeCurveSurfInt& entity) const
{
    return !isEqualTo(entity);
}

bool GeCurveSurfInt::isEqualTo(const GeCurveSurfInt& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeCurveSurfInt::isEqualTo(const GeCurveSurfInt& entity, const GeTol& tol) const
{
    if (m_curve != entity.m_curve || m_surface != entity.m_surface) {
        return false;
    }

    if (m_intPoints.length() != entity.m_intPoints.length()) {
        return false;
    }

    for (int i = 0; i < m_intPoints.length(); ++i) {
        if (!m_intPoints[i].isEqualTo(entity.m_intPoints[i], tol)) {
            return false;
        }
    }

    return true;
}

GeCurveSurfInt& GeCurveSurfInt::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].transformBy(xfm);
    }
    return *this;
}

GeCurveSurfInt& GeCurveSurfInt::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i] += translateVec;
    }
    return *this;
}

GeCurveSurfInt& GeCurveSurfInt::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeCurveSurfInt& GeCurveSurfInt::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeCurveSurfInt& GeCurveSurfInt::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].mirror(plane);
    }
    return *this;
}

GeCurveSurfInt& GeCurveSurfInt::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeCurveSurfInt& GeCurveSurfInt::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].scaleBy(scale, wrtPoint);
    }
    return *this;
}

bool GeCurveSurfInt::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeCurveSurfInt::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        if (m_intPoints[i].isEqualTo(pnt, tol)) {
            return true;
        }
    }
    return false;
}

const GeCurve3d* GeCurveSurfInt::curve() const
{
    return m_curve;
}

const GeSurface* GeCurveSurfInt::surface() const
{
    return m_surface;
}

GeTol GeCurveSurfInt::tolerance() const
{
    return m_tol;
}

int GeCurveSurfInt::numResults(Ge::GeIntersectError& status) const
{
    if (m_curve == NULL || m_surface == NULL) {
        status = Ge::kXXUnknown;
        return 0;
    }

    status = Ge::kXXOk;
    return m_intPoints.length();
}

int GeCurveSurfInt::getDimension(int intNum, Ge::GeIntersectError& status) const
{
    if (m_curve == NULL || m_surface == NULL) {
        status = Ge::kXXUnknown;
        return 0;
    }

    if (intNum < 0 || intNum >= m_intPoints.length()) {
        status = Ge::kXXIndexOutOfRange;
        return 0;
    }

    status = Ge::kXXOk;
    return 0;
}

int GeCurveSurfInt::numIntPoints(Ge::GeIntersectError& status) const
{
    return numResults(status);
}

GePoint3d GeCurveSurfInt::intPoint(int intNum, Ge::GeIntersectError& status) const
{
    if (getDimension(intNum, status) != 0 || status != Ge::kXXOk) {
        return GePoint3d::kOrigin;
    }

    return m_intPoints[intNum];
}

void GeCurveSurfInt::getIntParams(int intNum, double& param1, GePoint2d& param2, Ge::GeIntersectError& status) const
{
    param1 = 0.0;
    param2 = GePoint2d::kOrigin;

    if (getDimension(intNum, status) != 0 || status != Ge::kXXOk) {
        return;
    }

    param1 = m_curveParams[intNum];
    param2 = m_surfaceParams[intNum];
}

void GeCurveSurfInt::getPointOnCurve(int intNum, GePointOnCurve3d& intPnt, Ge::GeIntersectError& status) const
{
    double param1 = 0.0;
    GePoint2d param2 = GePoint2d::kOrigin;
    getIntParams(intNum, param1, param2, status);
    if (status != Ge::kXXOk || m_curve == NULL) {
        return;
    }

    intPnt.setCurve(*m_curve);
    intPnt.setParameter(param1);
}

void GeCurveSurfInt::getPointOnSurface(int intNum, GePointOnSurface& intPnt, Ge::GeIntersectError& status) const
{
    double param1 = 0.0;
    GePoint2d param2 = GePoint2d::kOrigin;
    getIntParams(intNum, param1, param2, status);
    if (status != Ge::kXXOk || m_surface == NULL) {
        return;
    }

    intPnt.setSurface(*m_surface);
    intPnt.setParameter(param2);
}

void GeCurveSurfInt::getIntConfigs(int intNum, Ge::csiConfig& lower, Ge::csiConfig& higher, bool& smallAngle, Ge::GeIntersectError& status) const
{
    lower = Ge::kXUnknown;
    higher = Ge::kXUnknown;
    smallAngle = false;
    getDimension(intNum, status);
}

int GeCurveSurfInt::overlapCount(Ge::GeIntersectError& status) const
{
    if (m_curve == NULL || m_surface == NULL) {
        status = Ge::kXXUnknown;
        return 0;
    }

    status = Ge::kXXOk;
    return 0;
}

void GeCurveSurfInt::getOverlapRange(int intNum, GeInterval& range, Ge::GeIntersectError& status) const
{
    range.set();
    if (m_curve == NULL || m_surface == NULL) {
        status = Ge::kXXUnknown;
        return;
    }

    if (intNum < 0) {
        status = Ge::kXXIndexOutOfRange;
        return;
    }

    status = Ge::kXXWrongDimensionAtIndex;
}

GeCurve2d* GeCurveSurfInt::intParamCurve(int intNum, bool isExternal, Ge::GeIntersectError& status) const
{
    if (m_curve == NULL || m_surface == NULL) {
        status = Ge::kXXUnknown;
        return NULL;
    }

    if (intNum < 0 || intNum >= m_intPoints.length()) {
        status = Ge::kXXIndexOutOfRange;
        return NULL;
    }

    status = Ge::kXXWrongDimensionAtIndex;
    return NULL;
}

GeCurveSurfInt& GeCurveSurfInt::set(const GeCurve3d& crv, const GeSurface& surface, const GeTol& tol)
{
    m_curve = &crv;
    m_surface = &surface;
    m_tol = tol;
    recalcIntersections();
    return *this;
}

GeCurveSurfInt& GeCurveSurfInt::operator = (const GeCurveSurfInt& crvSurfInt)
{
    if (this == &crvSurfInt) {
        return *this;
    }

    m_curve = crvSurfInt.m_curve;
    m_surface = crvSurfInt.m_surface;
    m_tol = crvSurfInt.m_tol;
    m_intPoints = crvSurfInt.m_intPoints;
    m_curveParams = crvSurfInt.m_curveParams;
    m_surfaceParams = crvSurfInt.m_surfaceParams;
    return *this;
}

void GeCurveSurfInt::appendIntersection(double curveParam, const GePoint2d& surfParam, const GePoint3d& point)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        if (m_intPoints[i].isEqualTo(point, m_tol)) {
            return;
        }
    }

    m_intPoints.append(point);
    m_curveParams.append(curveParam);
    m_surfaceParams.append(surfParam);
}

void GeCurveSurfInt::recalcIntersections()
{
    m_intPoints.removeAll();
    m_curveParams.removeAll();
    m_surfaceParams.removeAll();

    if (m_curve == NULL || m_surface == NULL) {
        return;
    }

    GeInterval range;
    m_curve->getInterval(range);
    if (!range.isBounded()) {
        return;
    }

    const int sampleCount = 256;
    double lower = range.lowerBound();
    double upper = range.upperBound();
    for (int i = 0; i <= sampleCount; ++i) {
        double ratio = static_cast<double>(i) / static_cast<double>(sampleCount);
        double param = lower + (upper - lower) * ratio;
        GePoint3d point = m_curve->evalPoint(param);
        if (!m_surface->isOn(point, m_tol)) {
            continue;
        }

        GePoint2d surfParam = m_surface->paramOf(point, m_tol);
        appendIntersection(param, surfParam, point);
    }
}