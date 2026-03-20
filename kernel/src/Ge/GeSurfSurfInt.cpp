#include "GeSurfSurfInt.h"

#include "GeInterval.h"
#include "GePlane.h"
#include "GeScale3d.h"

GeSurfSurfInt::GeSurfSurfInt()
    : m_surface1(NULL)
    , m_surface2(NULL)
    , m_tol(GeContext::gTol)
{
}

GeSurfSurfInt::GeSurfSurfInt(const GeSurface& srf1, const GeSurface& srf2, const GeTol& tol)
    : m_surface1(NULL)
    , m_surface2(NULL)
    , m_tol(tol)
{
    set(srf1, srf2, tol);
}

GeSurfSurfInt::GeSurfSurfInt(const GeSurfSurfInt& source)
    : m_surface1(source.m_surface1)
    , m_surface2(source.m_surface2)
    , m_tol(source.m_tol)
    , m_intPoints(source.m_intPoints)
    , m_params1(source.m_params1)
    , m_params2(source.m_params2)
{
}

bool GeSurfSurfInt::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::kEntity3d || entType == this->type();
}

Ge::EntityId GeSurfSurfInt::type() const
{
    return Ge::kSurfaceSurfaceInt;
}

GeSurfSurfInt* GeSurfSurfInt::copy() const
{
    return new GeSurfSurfInt(*this);
}

bool GeSurfSurfInt::operator == (const GeSurfSurfInt& entity) const
{
    return isEqualTo(entity);
}

bool GeSurfSurfInt::operator != (const GeSurfSurfInt& entity) const
{
    return !isEqualTo(entity);
}

bool GeSurfSurfInt::isEqualTo(const GeSurfSurfInt& entity) const
{
    return isEqualTo(entity, GeContext::gTol);
}

bool GeSurfSurfInt::isEqualTo(const GeSurfSurfInt& entity, const GeTol& tol) const
{
    if (m_surface1 != entity.m_surface1 || m_surface2 != entity.m_surface2) {
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

GeSurfSurfInt& GeSurfSurfInt::transformBy(const GeMatrix3d& xfm)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].transformBy(xfm);
    }
    return *this;
}

GeSurfSurfInt& GeSurfSurfInt::translateBy(const GeVector3d& translateVec)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i] += translateVec;
    }
    return *this;
}

GeSurfSurfInt& GeSurfSurfInt::rotateBy(double angle, const GeVector3d& vec)
{
    return rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeSurfSurfInt& GeSurfSurfInt::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].rotateBy(angle, vec, wrtPoint);
    }
    return *this;
}

GeSurfSurfInt& GeSurfSurfInt::mirror(const GePlane& plane)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].mirror(plane);
    }
    return *this;
}

GeSurfSurfInt& GeSurfSurfInt::scaleBy(double scaleFactor)
{
    return scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeSurfSurfInt& GeSurfSurfInt::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeScale3d scale(scaleFactor, scaleFactor, scaleFactor);
    for (int i = 0; i < m_intPoints.length(); ++i) {
        m_intPoints[i].scaleBy(scale, wrtPoint);
    }
    return *this;
}

bool GeSurfSurfInt::isOn(const GePoint3d& pnt) const
{
    return isOn(pnt, GeContext::gTol);
}

bool GeSurfSurfInt::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        if (m_intPoints[i].isEqualTo(pnt, tol)) {
            return true;
        }
    }
    return false;
}

const GeSurface* GeSurfSurfInt::surface1() const
{
    return m_surface1;
}

const GeSurface* GeSurfSurfInt::surface2() const
{
    return m_surface2;
}

GeTol GeSurfSurfInt::tolerance() const
{
    return m_tol;
}

int GeSurfSurfInt::numResults(Ge::GeIntersectError& status) const
{
    if (m_surface1 == NULL || m_surface2 == NULL) {
        status = Ge::kXXUnknown;
        return 0;
    }

    status = Ge::kXXOk;
    return m_intPoints.length();
}

int GeSurfSurfInt::numIntPoints(Ge::GeIntersectError& status) const
{
    return numResults(status);
}

GeCurve3d* GeSurfSurfInt::intCurve(int intNum, bool isExternal, Ge::GeIntersectError& status) const
{
    if (m_surface1 == NULL || m_surface2 == NULL) {
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

GeCurve2d* GeSurfSurfInt::intParamCurve(int intNum, bool isExternal, bool isFirst, Ge::GeIntersectError& status) const
{
    if (m_surface1 == NULL || m_surface2 == NULL) {
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

GePoint3d GeSurfSurfInt::intPoint(int intNum, Ge::GeIntersectError& status) const
{
    if (getDimension(intNum, status) != 0 || status != Ge::kXXOk) {
        return GePoint3d::kOrigin;
    }

    return m_intPoints[intNum];
}

void GeSurfSurfInt::getIntPointParams(int intNum, GePoint2d& param1, GePoint2d& param2, Ge::GeIntersectError& status) const
{
    param1 = GePoint2d::kOrigin;
    param2 = GePoint2d::kOrigin;

    if (getDimension(intNum, status) != 0 || status != Ge::kXXOk) {
        return;
    }

    param1 = m_params1[intNum];
    param2 = m_params2[intNum];
}

void GeSurfSurfInt::getIntConfigs(
    int intNum,
    Ge::ssiConfig& surf1Left,
    Ge::ssiConfig& surf1Right,
    Ge::ssiConfig& surf2Left,
    Ge::ssiConfig& surf2Right,
    Ge::ssiType& intType,
    int& dim,
    Ge::GeIntersectError& status) const
{
    surf1Left = Ge::kSSIUnknown;
    surf1Right = Ge::kSSIUnknown;
    surf2Left = Ge::kSSIUnknown;
    surf2Right = Ge::kSSIUnknown;
    intType = Ge::kSSITransverse;
    dim = getDimension(intNum, status);
}

int GeSurfSurfInt::numIntCurves(Ge::GeIntersectError& status) const
{
    if (m_surface1 == NULL || m_surface2 == NULL) {
        status = Ge::kXXUnknown;
        return 0;
    }

    status = Ge::kXXOk;
    return 0;
}

int GeSurfSurfInt::getDimension(int intNum, Ge::GeIntersectError& status) const
{
    if (m_surface1 == NULL || m_surface2 == NULL) {
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

Ge::ssiType GeSurfSurfInt::getType(int intNum, Ge::GeIntersectError& status) const
{
    if (getDimension(intNum, status) != 0 || status != Ge::kXXOk) {
        return Ge::kSSITransverse;
    }

    return Ge::kSSITransverse;
}

bool GeSurfSurfInt::haveOverlap(Ge::GeIntersectError& status) const
{
    if (m_surface1 == NULL || m_surface2 == NULL) {
        status = Ge::kXXUnknown;
        return false;
    }

    status = Ge::kXXOk;
    return false;
}

GeSurfSurfInt& GeSurfSurfInt::set(const GeSurface& srf1, const GeSurface& srf2, const GeTol& tol)
{
    m_surface1 = &srf1;
    m_surface2 = &srf2;
    m_tol = tol;
    recalcIntersections();
    return *this;
}

GeSurfSurfInt& GeSurfSurfInt::operator = (const GeSurfSurfInt& surfSurfInt)
{
    if (this == &surfSurfInt) {
        return *this;
    }

    m_surface1 = surfSurfInt.m_surface1;
    m_surface2 = surfSurfInt.m_surface2;
    m_tol = surfSurfInt.m_tol;
    m_intPoints = surfSurfInt.m_intPoints;
    m_params1 = surfSurfInt.m_params1;
    m_params2 = surfSurfInt.m_params2;
    return *this;
}

void GeSurfSurfInt::appendIntersection(const GePoint3d& point, const GePoint2d& param1, const GePoint2d& param2)
{
    for (int i = 0; i < m_intPoints.length(); ++i) {
        if (m_intPoints[i].isEqualTo(point, m_tol)) {
            return;
        }
    }

    m_intPoints.append(point);
    m_params1.append(param1);
    m_params2.append(param2);
}

void GeSurfSurfInt::recalcIntersections()
{
    m_intPoints.removeAll();
    m_params1.removeAll();
    m_params2.removeAll();

    if (m_surface1 == NULL || m_surface2 == NULL) {
        return;
    }

    GeInterval rangeU;
    GeInterval rangeV;
    m_surface1->getEnvelope(rangeU, rangeV);
    if (!rangeU.isBounded() || !rangeV.isBounded()) {
        return;
    }

    const int sampleCountU = 32;
    const int sampleCountV = 32;
    double u0 = rangeU.lowerBound();
    double u1 = rangeU.upperBound();
    double v0 = rangeV.lowerBound();
    double v1 = rangeV.upperBound();

    for (int i = 0; i <= sampleCountU; ++i) {
        double ru = static_cast<double>(i) / static_cast<double>(sampleCountU);
        double u = u0 + (u1 - u0) * ru;
        for (int j = 0; j <= sampleCountV; ++j) {
            double rv = static_cast<double>(j) / static_cast<double>(sampleCountV);
            double v = v0 + (v1 - v0) * rv;

            GePoint2d param1(u, v);
            GePoint3d point = m_surface1->evalPoint(param1);
            if (!m_surface2->isOn(point, m_tol)) {
                continue;
            }

            GePoint2d param2 = m_surface2->paramOf(point, m_tol);
            appendIntersection(point, param1, param2);
        }
    }
}