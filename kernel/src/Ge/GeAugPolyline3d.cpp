#include "GeAugPolyline3d.h"
#include "GeInterval.h"

namespace {
double aug3d_clamp(double v, double minV, double maxV)
{
    if (v < minV) {
        return minV;
    }
    if (v > maxV) {
        return maxV;
    }
    return v;
}

void aug3d_resizeVectors(GeVector3dArray& vecs, int size, const GeVector3d& fillValue)
{
    while (vecs.length() < size) {
        vecs.append(fillValue);
    }
}
}

GeAugPolyline3d::GeAugPolyline3d()
    : GePolyline3d()
    , m_approxTol(GeContext::gTol.equalPoint())
{
    aug3d_resizeVectors(m_vecBundle, this->numFitPoints(), GeVector3d::kIdentity);
    aug3d_resizeVectors(m_d2Vectors, this->numFitPoints(), GeVector3d::kIdentity);
}

GeAugPolyline3d::GeAugPolyline3d(const GeAugPolyline3d& apline)
    : GePolyline3d(apline)
{
    *this = apline;
}

GeAugPolyline3d::GeAugPolyline3d(const GeKnotVector& knots, const GePoint3dArray& controlPoints, const GeVector3dArray& vecBundle)
    : GePolyline3d(knots, controlPoints)
    , m_approxTol(GeContext::gTol.equalPoint())
{
    m_vecBundle = vecBundle;
    aug3d_resizeVectors(m_vecBundle, this->numFitPoints(), GeVector3d::kIdentity);
    aug3d_resizeVectors(m_d2Vectors, this->numFitPoints(), GeVector3d::kIdentity);
}

GeAugPolyline3d::GeAugPolyline3d(const GePoint3dArray& controlPoints, const GeVector3dArray& vecBundle)
    : GePolyline3d(controlPoints)
    , m_approxTol(GeContext::gTol.equalPoint())
{
    m_vecBundle = vecBundle;
    aug3d_resizeVectors(m_vecBundle, this->numFitPoints(), GeVector3d::kIdentity);
    aug3d_resizeVectors(m_d2Vectors, this->numFitPoints(), GeVector3d::kIdentity);
}

GeAugPolyline3d::GeAugPolyline3d(const GeCurve3d& curve, double fromParam, double toParam, double approxEps)
    : GePolyline3d()
    , m_approxTol(approxEps)
{
    GeInterval range;
    curve.getInterval(range);

    if (range.isBounded() == false) {
        GePoint3dArray points;
        points.append(GePoint3d::kOrigin);
        points.append(GePoint3d(1.0, 0.0, 0.0));
        this->set(points);
    } else {
        double start = aug3d_clamp(fromParam, range.lowerBound(), range.upperBound());
        double end = aug3d_clamp(toParam, range.lowerBound(), range.upperBound());

        int sampleCount = 32;
        double segLength = curve.length(start, end);
        if (approxEps > GeContext::gTol.equalPoint() && segLength > GeContext::gTol.equalPoint()) {
            sampleCount = static_cast<int>(segLength / approxEps) + 1;
        }
        if (sampleCount < 2) {
            sampleCount = 2;
        }
        if (sampleCount > 512) {
            sampleCount = 512;
        }

        GePoint3dArray points;
        for (int i = 0; i < sampleCount; i++) {
            double ratio = 0.0;
            if (sampleCount > 1) {
                ratio = static_cast<double>(i) / static_cast<double>(sampleCount - 1);
            }
            double param = start + (end - start) * ratio;
            points.append(curve.evalPoint(param));
        }
        this->set(points);
    }

    aug3d_resizeVectors(m_vecBundle, this->numFitPoints(), GeVector3d::kIdentity);
    aug3d_resizeVectors(m_d2Vectors, this->numFitPoints(), GeVector3d::kIdentity);
}

bool GeAugPolyline3d::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d
        || entType == Ge::EntityId::kCurve3d
        || entType == Ge::EntityId::kPolyline3d
        || entType == this->type();
}

Ge::EntityId GeAugPolyline3d::type() const
{
    return Ge::EntityId::kAugPolyline3d;
}

GeAugPolyline3d* GeAugPolyline3d::copy() const
{
    return new GeAugPolyline3d(*this);
}

bool GeAugPolyline3d::operator == (const GeAugPolyline3d& entity) const
{
    return this->isEqualTo(entity);
}

bool GeAugPolyline3d::operator != (const GeAugPolyline3d& entity) const
{
    return !this->isEqualTo(entity);
}

bool GeAugPolyline3d::isEqualTo(const GeAugPolyline3d& entity) const
{
    return this->isEqualTo(entity, GeContext::gTol);
}

bool GeAugPolyline3d::isEqualTo(const GeAugPolyline3d& entity, const GeTol& tol) const
{
    if (GePolyline3d::isEqualTo(entity, tol) == false) {
        return false;
    }

    if (m_vecBundle.length() != entity.m_vecBundle.length()) {
        return false;
    }

    for (int i = 0; i < m_vecBundle.length(); i++) {
        if (m_vecBundle[i].isEqualTo(entity.m_vecBundle[i], tol) == false) {
            return false;
        }
    }

    return true;
}

GeAugPolyline3d& GeAugPolyline3d::transformBy(const GeMatrix3d& xfm)
{
    GePolyline3d::transformBy(xfm);
    for (int i = 0; i < m_vecBundle.length(); i++) {
        m_vecBundle[i].transformBy(xfm);
    }
    for (int i = 0; i < m_d2Vectors.length(); i++) {
        m_d2Vectors[i].transformBy(xfm);
    }
    return *this;
}

GeAugPolyline3d& GeAugPolyline3d::translateBy(const GeVector3d& translateVec)
{
    GePolyline3d::translateBy(translateVec);
    return *this;
}

GeAugPolyline3d& GeAugPolyline3d::rotateBy(double angle, const GeVector3d& vec)
{
    GePolyline3d::rotateBy(angle, vec);
    return *this;
}

GeAugPolyline3d& GeAugPolyline3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    GePolyline3d::rotateBy(angle, vec, wrtPoint);
    return *this;
}

GeAugPolyline3d& GeAugPolyline3d::mirror(const GePlane& plane)
{
    GePolyline3d::mirror(plane);
    return *this;
}

GeAugPolyline3d& GeAugPolyline3d::scaleBy(double scaleFactor)
{
    GePolyline3d::scaleBy(scaleFactor);
    return *this;
}

GeAugPolyline3d& GeAugPolyline3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GePolyline3d::scaleBy(scaleFactor, wrtPoint);
    return *this;
}

bool GeAugPolyline3d::isOn(const GePoint3d& pnt) const
{
    return GePolyline3d::isOn(pnt);
}

bool GeAugPolyline3d::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    return GePolyline3d::isOn(pnt, tol);
}

GeAugPolyline3d& GeAugPolyline3d::operator = (const GeAugPolyline3d& apline)
{
    if (this == &apline) {
        return *this;
    }

    GePolyline3d::operator=(apline);
    m_vecBundle = apline.m_vecBundle;
    m_d2Vectors = apline.m_d2Vectors;
    m_approxTol = apline.m_approxTol;
    return *this;
}

GePoint3d GeAugPolyline3d::getPoint(int index) const
{
    return this->fitPointAt(index);
}

GeAugPolyline3d& GeAugPolyline3d::setPoint(int controlpointIndex, const GePoint3d& point)
{
    this->setFitPointAt(controlpointIndex, point);
    return *this;
}

void GeAugPolyline3d::getPoints(GePoint3dArray& controlPoints) const
{
    controlPoints.removeAll();
    int count = this->numFitPoints();
    for (int i = 0; i < count; i++) {
        controlPoints.append(this->fitPointAt(i));
    }
}

GeVector3d GeAugPolyline3d::getVector(int vectorIndex) const
{
    if (vectorIndex < 0 || vectorIndex >= m_vecBundle.length()) {
        return GeVector3d::kIdentity;
    }
    return m_vecBundle[vectorIndex];
}

GeAugPolyline3d& GeAugPolyline3d::setVector(int vectorIndex, const GeVector3d& vect)
{
    if (vectorIndex < 0) {
        return *this;
    }

    aug3d_resizeVectors(m_vecBundle, vectorIndex + 1, GeVector3d::kIdentity);
    m_vecBundle[vectorIndex] = vect;
    return *this;
}

void GeAugPolyline3d::getD1Vectors(GeVector3dArray& tangents) const
{
    tangents = m_vecBundle;
}

GeVector3d GeAugPolyline3d::getD2Vector(int vectorIndex) const
{
    if (vectorIndex < 0 || vectorIndex >= m_d2Vectors.length()) {
        return GeVector3d::kIdentity;
    }
    return m_d2Vectors[vectorIndex];
}

GeAugPolyline3d& GeAugPolyline3d::setD2Vector(int vectorIndex, const GeVector3d& vect)
{
    if (vectorIndex < 0) {
        return *this;
    }

    aug3d_resizeVectors(m_d2Vectors, vectorIndex + 1, GeVector3d::kIdentity);
    m_d2Vectors[vectorIndex] = vect;
    return *this;
}

void GeAugPolyline3d::getD2Vectors(GeVector3dArray& d2Vectors) const
{
    d2Vectors = m_d2Vectors;
}

double GeAugPolyline3d::approxTol() const
{
    return m_approxTol;
}

GeAugPolyline3d& GeAugPolyline3d::setApproxTol(double approxTol)
{
    m_approxTol = approxTol;
    return *this;
}