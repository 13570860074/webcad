#include "GeSphere.h"
#include "GeCircArc3d.h"
#include "GeInterval.h"
#include "GeLinearEnt3d.h"
#include "GeMatrix3d.h"
#include "GePlane.h"
#include "GePointOnSurface.h"
#include "GeImpl.h"
#include <cmath>

static double sphere_normalize_angle(double angle)
{
    while (angle > PI)
    {
        angle -= PI * 2.0;
    }
    while (angle <= -PI)
    {
        angle += PI * 2.0;
    }
    return angle;
}

static double sphere_clamp_value(double value, double minValue, double maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}

static bool sphere_is_uni_scaled_ortho(const GeMatrix3d& xfm, double& scaleFactor, const GeTol& tol = GeContext::gTol)
{
    if (xfm.isPerspective())
    {
        return false;
    }

    GeVector3d axisX = GeVector3d::kXAxis;
    GeVector3d axisY = GeVector3d::kYAxis;
    GeVector3d axisZ = GeVector3d::kZAxis;
    axisX.transformBy(xfm);
    axisY.transformBy(xfm);
    axisZ.transformBy(xfm);

    double lengthX = axisX.length();
    double lengthY = axisY.length();
    double lengthZ = axisZ.length();
    if (lengthX <= tol.equalVector() || lengthY <= tol.equalVector() || lengthZ <= tol.equalVector())
    {
        return false;
    }

    if (std::fabs(lengthX - lengthY) > tol.equalVector() || std::fabs(lengthX - lengthZ) > tol.equalVector())
    {
        return false;
    }

    GeVector3d unitX = axisX.normal();
    GeVector3d unitY = axisY.normal();
    GeVector3d unitZ = axisZ.normal();
    if (std::fabs(unitX.dotProduct(unitY)) > tol.equalVector()
        || std::fabs(unitX.dotProduct(unitZ)) > tol.equalVector()
        || std::fabs(unitY.dotProduct(unitZ)) > tol.equalVector())
    {
        return false;
    }

    scaleFactor = (lengthX + lengthY + lengthZ) / 3.0;
    return true;
}

static void sphere_get_basis(const GeSphere& sphere, GeVector3d& axisU, GeVector3d& axisV, GeVector3d& axisW)
{
    axisW = sphere.northAxis().normal();
    if (axisW.isZeroLength())
    {
        axisW = GeVector3d::kYAxis;
    }

    axisU = sphere.refAxis().normal();
    if (axisU.isZeroLength())
    {
        axisU = GeVector3d::kXAxis;
    }

    axisV = axisW.crossProduct(axisU);
    if (axisV.isZeroLength())
    {
        GeVector3d fallback = std::fabs(axisW.x) < 0.9 ? GeVector3d::kXAxis : GeVector3d::kZAxis;
        axisU = fallback;
        axisV = axisW.crossProduct(axisU);
    }

    axisU.normalize();
    axisV.normalize();

    if (sphere.isReverseV())
    {
        axisV.negate();
    }
}

static GePoint2d sphere_param_from_vector(const GeSphere& sphere, const GeVector3d& direction)
{
    GeVector3d axisU;
    GeVector3d axisV;
    GeVector3d axisW;
    sphere_get_basis(sphere, axisU, axisV, axisW);

    GeVector3d normalDir = direction.normal();
    double x = normalDir.dotProduct(axisU);
    double y = normalDir.dotProduct(axisV);
    double z = normalDir.dotProduct(axisW);
    double xy = sqrt(x * x + y * y);
    double u = atan2(z, xy);
    double v = atan2(y, x);
    return GePoint2d(u, sphere_normalize_angle(v));
}

static bool sphere_u_in_range(double angle, double start, double end, const GeTol& tol)
{
    double sweep = std::fabs(end - start);
    if (sweep >= PI - tol.equalPoint())
    {
        return true;
    }
    double minValue = start < end ? start : end;
    double maxValue = start < end ? end : start;
    return angle >= minValue - tol.equalPoint() && angle <= maxValue + tol.equalPoint();
}

static bool sphere_v_in_range(double angle, double start, double end, const GeTol& tol)
{
    double sweep = std::fabs(end - start);
    if (sweep >= PI * 2.0 - tol.equalPoint())
    {
        return true;
    }

    double testAngle = angle;
    while (testAngle < start - tol.equalPoint())
    {
        testAngle += PI * 2.0;
    }
    while (testAngle > end + tol.equalPoint())
    {
        testAngle -= PI * 2.0;
    }
    return testAngle >= start - tol.equalPoint() && testAngle <= end + tol.equalPoint();
}

static GePoint2d sphere_clamp_param(const GeSphere& sphere, const GePoint2d& param, const GeTol& tol)
{
    double startU;
    double endU;
    double startV;
    double endV;
    sphere.getAnglesInU(startU, endU);
    sphere.getAnglesInV(startV, endV);

    double u = param.x;
    double minU = startU < endU ? startU : endU;
    double maxU = startU < endU ? endU : startU;
    u = sphere_clamp_value(u, minU, maxU);

    double v = param.y;
    if (sphere_v_in_range(v, startV, endV, tol) == false)
    {
        double angle = sphere_normalize_angle(v);
        double begin = startV;
        double finish = endV;
        while (angle < begin)
        {
            angle += PI * 2.0;
        }
        while (angle > finish)
        {
            angle -= PI * 2.0;
        }
        double distToStart = std::fabs(angle - begin);
        double distToEnd = std::fabs(angle - finish);
        v = distToStart <= distToEnd ? begin : finish;
    }

    return GePoint2d(u, sphere_normalize_angle(v));
}

static double sphere_closest_periodic_value(double value, double center)
{
    while (value - center > PI)
    {
        value -= PI * 2.0;
    }
    while (value - center <= -PI)
    {
        value += PI * 2.0;
    }
    return value;
}

static double sphere_move_into_valid_interval(double value, double start, double end)
{
    return sphere_closest_periodic_value(value, (start + end) * 0.5);
}

static void sphere_make_interval_valid(double& start, double& end, double maxSpan)
{
    if (end < start)
    {
        double temp = start;
        start = end;
        end = temp;
    }

    double span = end - start;
    if (span > maxSpan)
    {
        end = start + maxSpan;
    }
}

static GeVector3d sphere_rotated_ref_axis(const GeSphere& sphere, double v)
{
    GeVector3d ref = sphere.refAxis();
    ref.rotateBy(sphere.isReverseV() ? -v : v, sphere.northAxis());
    return ref;
}

static double sphere_longitude_from_point(const GeSphere& sphere, const GePoint3d& point, const GeTol& tol)
{
    GeVector3d normal = sphere.northAxis().normal();
    GeVector3d toPoint = point - sphere.center();
    double height = toPoint.dotProduct(normal);
    GeVector3d projected = toPoint - normal * height;
    if (projected.length() <= tol.equalVector())
    {
        return 0.0;
    }

    double v = sphere.refAxis().normal().angleToCCW(projected.normal(), normal);
    if (v > PI)
    {
        v -= PI * 2.0;
    }
    if (sphere.isReverseV())
    {
        v = -v;
    }
    return v;
}

GeSphere::GeSphere()
{
    GE_IMP_MEMORY_ENTITY(GeSphere);
}
GeSphere::GeSphere(double radius, const GePoint3d &center)
{
    GE_IMP_MEMORY_ENTITY(GeSphere);

    this->set(radius, center);
}
GeSphere::GeSphere(double radius, const GePoint3d &center,
                   const GeVector3d &northAxis, const GeVector3d &refAxis,
                   double startAngleU, double endAngleU,
                   double startAngleV, double endAngleV)
{
    GE_IMP_MEMORY_ENTITY(GeSphere);

    this->set(radius, center, northAxis, refAxis, startAngleU, endAngleU, startAngleV, endAngleV);
}
GeSphere::GeSphere(const GeSphere &sphere)
{
    GE_IMP_MEMORY_ENTITY(GeSphere);

    GE_IMP_SPHERE(this->m_pImpl)->radius = GE_IMP_SPHERE(sphere.m_pImpl)->radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = GE_IMP_SPHERE(sphere.m_pImpl)->center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = GE_IMP_SPHERE(sphere.m_pImpl)->northAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = GE_IMP_SPHERE(sphere.m_pImpl)->refAxis;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal = GE_IMP_SPHERE(sphere.m_pImpl)->isOuterNormal;
    GE_IMP_SPHERE(this->m_pImpl)->reverseV = GE_IMP_SPHERE(sphere.m_pImpl)->reverseV;
}

bool GeSphere::isKindOf(Ge::EntityId entType) const
{
    return entType == Ge::EntityId::kEntity3d
        || entType == Ge::EntityId::kSurface
        || entType == this->type();
}

Ge::EntityId GeSphere::type() const
{
    return Ge::EntityId::kSphere;
}

GeSphere* GeSphere::copy() const
{
    GeSphere* sphere = new GeSphere();
    GE_IMP_SPHERE(sphere->m_pImpl)->radius = GE_IMP_SPHERE(this->m_pImpl)->radius;
    GE_IMP_SPHERE(sphere->m_pImpl)->center = GE_IMP_SPHERE(this->m_pImpl)->center;
    GE_IMP_SPHERE(sphere->m_pImpl)->northAxis = GE_IMP_SPHERE(this->m_pImpl)->northAxis;
    GE_IMP_SPHERE(sphere->m_pImpl)->refAxis = GE_IMP_SPHERE(this->m_pImpl)->refAxis;
    GE_IMP_SPHERE(sphere->m_pImpl)->startAngleU = GE_IMP_SPHERE(this->m_pImpl)->startAngleU;
    GE_IMP_SPHERE(sphere->m_pImpl)->endAngleU = GE_IMP_SPHERE(this->m_pImpl)->endAngleU;
    GE_IMP_SPHERE(sphere->m_pImpl)->startAngleV = GE_IMP_SPHERE(this->m_pImpl)->startAngleV;
    GE_IMP_SPHERE(sphere->m_pImpl)->endAngleV = GE_IMP_SPHERE(this->m_pImpl)->endAngleV;
    GE_IMP_SPHERE(sphere->m_pImpl)->isOuterNormal = GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal;
    GE_IMP_SPHERE(sphere->m_pImpl)->reverseV = GE_IMP_SPHERE(this->m_pImpl)->reverseV;
    return sphere;
}

GeSphere& GeSphere::transformBy(const GeMatrix3d& xfm)
{
    double scaleFactor = 1.0;
    if (sphere_is_uni_scaled_ortho(xfm, scaleFactor) == false)
    {
        return *this;
    }

    GE_IMP_SPHERE(this->m_pImpl)->center.transformBy(xfm);
    GE_IMP_SPHERE(this->m_pImpl)->radius *= scaleFactor;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis.transformBy(xfm);
    GE_IMP_SPHERE(this->m_pImpl)->refAxis.transformBy(xfm);
    GE_IMP_SPHERE(this->m_pImpl)->northAxis.normalize();
    GE_IMP_SPHERE(this->m_pImpl)->refAxis.normalize();

    if (xfm.det() < 0.0)
    {
        GE_IMP_SPHERE(this->m_pImpl)->reverseV = !GE_IMP_SPHERE(this->m_pImpl)->reverseV;
    }

    return *this;
}

GeSphere& GeSphere::translateBy(const GeVector3d& translateVec)
{
    GE_IMP_SPHERE(this->m_pImpl)->center += translateVec;
    return *this;
}

GeSphere& GeSphere::rotateBy(double angle, const GeVector3d& vec)
{
    return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}

GeSphere& GeSphere::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint)
{
    GeMatrix3d xfm;
    xfm.setToRotation(angle, vec, wrtPoint);
    return this->transformBy(xfm);
}

GeSphere& GeSphere::mirror(const GePlane& plane)
{
    GeMatrix3d xfm;
    xfm.setToMirroring(plane);
    return this->transformBy(xfm);
}

GeSphere& GeSphere::scaleBy(double scaleFactor)
{
    return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}

GeSphere& GeSphere::scaleBy(double scaleFactor, const GePoint3d& wrtPoint)
{
    GeMatrix3d xfm;
    xfm.setToScaling(scaleFactor, wrtPoint);
    return this->transformBy(xfm);
}

double GeSphere::radius() const
{
    return std::fabs(GE_IMP_SPHERE(this->m_pImpl)->radius);
}
GePoint3d GeSphere::center() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->center;
}
void GeSphere::getAnglesInU(double &start, double &end) const
{
    start = GE_IMP_SPHERE(this->m_pImpl)->startAngleU;
    end = GE_IMP_SPHERE(this->m_pImpl)->endAngleU;
}
void GeSphere::getAnglesInV(double &start, double &end) const
{
    start = GE_IMP_SPHERE(this->m_pImpl)->startAngleV;
    end = GE_IMP_SPHERE(this->m_pImpl)->endAngleV;
}
GeVector3d GeSphere::northAxis() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->northAxis;
}
GeVector3d GeSphere::refAxis() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->refAxis;
}
GePoint3d GeSphere::northPole() const
{
    GePoint3d pos = this->center();
    pos += this->northAxis().normal() * this->radius();
    return pos;
}
GePoint3d GeSphere::southPole() const
{
    GePoint3d pos = this->center();
    pos -= this->northAxis().normal() * this->radius();
    return pos;
}
Adesk::Boolean GeSphere::isOuterNormal() const
{
    return (this->isNormalReversed() ? -GE_IMP_SPHERE(this->m_pImpl)->radius : GE_IMP_SPHERE(this->m_pImpl)->radius) > 0.0;
}
Adesk::Boolean GeSphere::isClosed(const GeTol &tol) const
{
    double vSweep = std::fabs(GE_IMP_SPHERE(this->m_pImpl)->endAngleV - GE_IMP_SPHERE(this->m_pImpl)->startAngleV);
    if (vSweep >= PI * 2.0 - tol.equalPoint())
    {
        return true;
    }
    return false;
}

Adesk::Boolean GeSphere::isReverseV() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->reverseV;
}

GeSphere &GeSphere::setRadius(double _radius)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = _radius;
    return *this;
}
GeSphere &GeSphere::setAnglesInU(double start, double end)
{
    sphere_make_interval_valid(start, end, PI);
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = start;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = end;
    return *this;
}
GeSphere &GeSphere::setAnglesInV(double start, double end)
{
    sphere_make_interval_valid(start, end, PI * 2.0);
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = start;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = end;
    return *this;
}

GeSphere &GeSphere::setReverseV(Adesk::Boolean isReverseV)
{
    GE_IMP_SPHERE(this->m_pImpl)->reverseV = isReverseV;
    return *this;
}

GeSphere &GeSphere::set(double radius, const GePoint3d &center)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = GeVector3d::kYAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = GeVector3d::kXAxis;
    GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal = false;
    GE_IMP_SPHERE(this->m_pImpl)->reverseV = false;
    this->setAnglesInU(-PI * 0.5, PI * 0.5);
    this->setAnglesInV(-PI, PI);
    return *this;
}
GeSphere &GeSphere::set(double radius,
                        const GePoint3d &center,
                        const GeVector3d &northAxis,
                        const GeVector3d &refAxis,
                        double startAngleU,
                        double endAngleU,
                        double startAngleV,
                        double endAngleV)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = northAxis.normal();
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = refAxis.normal();
    GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal = false;
    GE_IMP_SPHERE(this->m_pImpl)->reverseV = false;
    this->setAnglesInU(startAngleU, endAngleU);
    this->setAnglesInV(startAngleV, endAngleV);
    return *this;
}

GeSphere &GeSphere::operator=(const GeSphere &sphere)
{
    GE_IMP_SPHERE(this->m_pImpl)->radius = GE_IMP_SPHERE(sphere.m_pImpl)->radius;
    GE_IMP_SPHERE(this->m_pImpl)->center = GE_IMP_SPHERE(sphere.m_pImpl)->center;
    GE_IMP_SPHERE(this->m_pImpl)->northAxis = GE_IMP_SPHERE(sphere.m_pImpl)->northAxis;
    GE_IMP_SPHERE(this->m_pImpl)->refAxis = GE_IMP_SPHERE(sphere.m_pImpl)->refAxis;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleU = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleU;
    GE_IMP_SPHERE(this->m_pImpl)->startAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->startAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->endAngleV = GE_IMP_SPHERE(sphere.m_pImpl)->endAngleV;
    GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal = GE_IMP_SPHERE(sphere.m_pImpl)->isOuterNormal;
    GE_IMP_SPHERE(this->m_pImpl)->reverseV = GE_IMP_SPHERE(sphere.m_pImpl)->reverseV;
    return *this;
}

Adesk::Boolean GeSphere::intersectWith(const GeLinearEnt3d &line, int &intn,
                                       GePoint3d &p1, GePoint3d &p2,
                                       const GeTol &tol) const
{
    intn = 0;

    double sphereRadius = this->radius();
    if (sphereRadius < tol.equalPoint())
    {
        return Adesk::kFalse;
    }

    GePoint3d origin = line.pointOnLine();
    GeVector3d dir = line.direction();
    if (dir.length() < tol.equalVector())
    {
        return Adesk::kFalse;
    }

    GeVector3d offset = origin - this->center();
    double b = 2.0 * dir.dotProduct(offset);
    double c = offset.dotProduct(offset) - sphereRadius * sphereRadius;
    double discriminant = b * b - 4.0 * c;
    if (discriminant < -tol.equalPoint())
    {
        return Adesk::kFalse;
    }

    if (std::fabs(discriminant) <= tol.equalPoint())
    {
        double t = -b / 2.0;
        GePoint3d hit = origin + dir * t;
        if (line.isOn(hit, tol) == false)
        {
            return Adesk::kFalse;
        }
        p1 = hit;
        p2 = hit;
        intn = 1;
        return Adesk::kTrue;
    }

    double sqrtDiscriminant = sqrt(discriminant);
    double t1 = (-b - sqrtDiscriminant) / 2.0;
    double t2 = (-b + sqrtDiscriminant) / 2.0;
    GePoint3d hit1 = origin + dir * t1;
    GePoint3d hit2 = origin + dir * t2;
    bool valid1 = line.isOn(hit1, tol);
    bool valid2 = line.isOn(hit2, tol);

    if (valid1 == false && valid2 == false)
    {
        return Adesk::kFalse;
    }
    if (valid1 == true && valid2 == true)
    {
        p1 = hit1;
        p2 = hit2;
        if (p1.isEqualTo(p2, tol))
        {
            intn = 1;
            p2 = p1;
        }
        else
        {
            intn = 2;
        }
        return Adesk::kTrue;
    }

    GePoint3d hit = valid1 ? hit1 : hit2;
    p1 = hit;
    p2 = hit;
    intn = 1;
    return Adesk::kTrue;
}

GePoint2d GeSphere::paramOf(const GePoint3d& pnt) const
{
    return this->paramOf(pnt, GeContext::gTol);
}

GePoint2d GeSphere::paramOf(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d vec = pnt - this->center();
    if (vec.length() < tol.equalPoint())
    {
        double startV;
        double endV;
        this->getAnglesInV(startV, endV);
        return GePoint2d(0.0, sphere_move_into_valid_interval(0.0, startV, endV));
    }

    GePoint2d param = sphere_param_from_vector(*this, vec);
    double startV;
    double endV;
    this->getAnglesInV(startV, endV);
    param.y = sphere_move_into_valid_interval(param.y, startV, endV);
    return param;
}

bool GeSphere::isOn(const GePoint3d& pnt) const
{
    return this->isOn(pnt, GeContext::gTol);
}

bool GeSphere::isOn(const GePoint3d& pnt, const GeTol& tol) const
{
    GePoint2d param;
    return this->isOn(pnt, param, tol);
}

bool GeSphere::isOn(const GePoint3d& pnt, GePoint2d& paramPoint) const
{
    return this->isOn(pnt, paramPoint, GeContext::gTol);
}

bool GeSphere::isOn(const GePoint3d& pnt, GePoint2d& paramPoint, const GeTol& tol) const
{
    if (std::fabs(this->center().distanceTo(pnt) - this->radius()) > tol.equalPoint())
    {
        return false;
    }

    paramPoint = this->paramOf(pnt, tol);
    double startU;
    double endU;
    double startV;
    double endV;
    this->getAnglesInU(startU, endU);
    this->getAnglesInV(startV, endV);
    return sphere_u_in_range(paramPoint.x, startU, endU, tol)
        && sphere_v_in_range(paramPoint.y, startV, endV, tol);
}

GePoint3d GeSphere::closestPointTo(const GePoint3d& pnt) const
{
    return this->closestPointTo(pnt, GeContext::gTol);
}

GePoint3d GeSphere::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const
{
    GeVector3d vec = pnt - this->center();
    if (vec.length() < tol.equalPoint())
    {
        double startU;
        double endU;
        double startV;
        double endV;
        this->getAnglesInU(startU, endU);
        this->getAnglesInV(startV, endV);
		double minU = startU < endU ? startU : endU;
		double maxU = startU < endU ? endU : startU;
		double u = sphere_clamp_value(0.0, minU, maxU);
		double v = sphere_move_into_valid_interval(0.0, startV, endV);
		return this->evalPoint(GePoint2d(u, v));
    }

    double startU;
    double endU;
    double startV;
    double endV;
    this->getAnglesInU(startU, endU);
    this->getAnglesInV(startV, endV);

    double v = sphere_longitude_from_point(*this, pnt, tol);
    v = sphere_move_into_valid_interval(v, startV, endV);

    double u = startU;
    GeInterval intervalU(startU, endU);

    if (sphere_v_in_range(v, startV, endV, tol) == false)
    {
        GeVector3d ref1 = sphere_rotated_ref_axis(*this, startV);
        GeCircArc3d uCircle1(this->center(), -this->northAxis().crossProduct(ref1), ref1, this->radius(), startU, endU);
        double u1 = startU;
        GePoint3d pnt1 = static_cast<const GeCurve3d&>(uCircle1).closestPointTo(pnt, u1, &intervalU, tol);

        GeVector3d ref2 = sphere_rotated_ref_axis(*this, endV);
        GeCircArc3d uCircle2(this->center(), -this->northAxis().crossProduct(ref2), ref2, this->radius(), startU, endU);
        double u2 = startU;
        GePoint3d pnt2 = static_cast<const GeCurve3d&>(uCircle2).closestPointTo(pnt, u2, &intervalU, tol);

        if (pnt.distanceTo(pnt1) <= pnt.distanceTo(pnt2))
        {
            v = startV;
            u = u1;
        }
        else
        {
            v = endV;
            u = u2;
        }
    }
    else
    {
        GeVector3d ref = sphere_rotated_ref_axis(*this, v);
        GeCircArc3d uCircle(this->center(), -this->northAxis().crossProduct(ref), ref, this->radius(), startU, endU);
        static_cast<const GeCurve3d&>(uCircle).closestPointTo(pnt, u, &intervalU, tol);
    }

    GePoint2d param(u, v);
    GePoint2d clamped = sphere_clamp_param(*this, param, tol);
    return this->evalPoint(clamped);
}

void GeSphere::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const
{
    return this->getClosestPointTo(pnt, result, GeContext::gTol);
}

void GeSphere::getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const
{
    GePoint3d closest = this->closestPointTo(pnt, tol);
    GePoint2d param = this->paramOf(closest, tol);
    result.setSurface(*this);
    result.setParameter(param);
}

double GeSphere::distanceTo(const GePoint3d& pnt) const
{
    return this->distanceTo(pnt, GeContext::gTol);
}

double GeSphere::distanceTo(const GePoint3d& pnt, const GeTol& tol) const
{
    return this->closestPointTo(pnt, tol).distanceTo(pnt);
}

bool GeSphere::isNormalReversed() const
{
    return GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal;
}

bool GeSphere::isLeftHanded() const
{
    return this->isOuterNormal() != (GE_IMP_SPHERE(this->m_pImpl)->reverseV != 0);
}

GeSurface& GeSphere::reverseNormal()
{
    GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal = !GE_IMP_SPHERE(this->m_pImpl)->isOuterNormal;
    return *this;
}

void GeSphere::getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const
{
    double startU;
    double endU;
    double startV;
    double endV;
    this->getAnglesInU(startU, endU);
    this->getAnglesInV(startV, endV);
    rangeU.set(startU, endU);
    rangeV.set(startV, endV);
}

GePoint3d GeSphere::evalPoint(const GePoint2d& param) const
{
    GeVector3d axisU;
    GeVector3d axisV;
    GeVector3d axisW;
    sphere_get_basis(*this, axisU, axisV, axisW);

    double radiusValue = this->radius();
    double sinU = sin(param.x);
    double cosU = cos(param.x);
    double sinV = sin(param.y);
    double cosV = cos(param.y);
    GeVector3d vec = (axisU * cosV + axisV * sinV) * (radiusValue * cosU)
        + axisW * (radiusValue * sinU);
    return this->center() + vec;
}

void create_flat_sphere(double R,
                        std::vector<double> &positions_spheres,
                        std::vector<double> &normals_spheres,
                        int prec = 36)
{
    // The more small they are, the more precise the Sphere will be.
    //  Must be divisors of 180 and 360.
    const float rings = float(prec) / 2;
    const float sectors = float(prec);
    const float to_rad = static_cast<float>(PI / 180.0);

    float T, P;
    float x[4], y[4], z[4];

    using std::cos;
    using std::sin;

    // Top of the sphere
    for (float t = 0; t < 360.f; t += sectors)
    {

        positions_spheres.push_back(0);
        positions_spheres.push_back(0);
        positions_spheres.push_back(R);

        normals_spheres.push_back(0);
        normals_spheres.push_back(0);
        normals_spheres.push_back(1);

        P = rings * to_rad;
        T = t * to_rad;
        x[1] = sin(P) * cos(T);
        y[1] = sin(P) * sin(T);
        z[1] = cos(P);
        positions_spheres.push_back(R * x[1]);
        positions_spheres.push_back(R * y[1]);
        positions_spheres.push_back(R * z[1]);

        normals_spheres.push_back(x[1]);
        normals_spheres.push_back(y[1]);
        normals_spheres.push_back(z[1]);

        //
        P = rings * to_rad;
        T = (t + sectors) * to_rad;
        x[2] = sin(P) * cos(T);
        y[2] = sin(P) * sin(T);
        z[2] = cos(P);
        positions_spheres.push_back(R * x[2]);
        positions_spheres.push_back(R * y[2]);
        positions_spheres.push_back(R * z[2]);

        normals_spheres.push_back(x[2]);
        normals_spheres.push_back(y[2]);
        normals_spheres.push_back(z[2]);
    }

    // Body of the sphere
    for (float p = rings; p < 180.f - rings; p += rings)
        for (float t = 0; t < 360.f; t += sectors)
        {
            // A
            P = p * to_rad;
            T = t * to_rad;
            x[0] = sin(P) * cos(T);
            y[0] = sin(P) * sin(T);
            z[0] = cos(P);

            positions_spheres.push_back(R * x[0]);
            positions_spheres.push_back(R * y[0]);
            positions_spheres.push_back(R * z[0]);

            normals_spheres.push_back(x[0]);
            normals_spheres.push_back(y[0]);
            normals_spheres.push_back(z[0]);

            // B
            P = (p + rings) * to_rad;
            T = t * to_rad;
            x[1] = sin(P) * cos(T);
            y[1] = sin(P) * sin(T);
            z[1] = cos(P);
            positions_spheres.push_back(R * x[1]);
            positions_spheres.push_back(R * y[1]);
            positions_spheres.push_back(R * z[1]);

            normals_spheres.push_back(x[1]);
            normals_spheres.push_back(y[1]);
            normals_spheres.push_back(z[1]);

            // C
            P = p * to_rad;
            T = (t + sectors) * to_rad;
            x[2] = sin(P) * cos(T);
            y[2] = sin(P) * sin(T);
            z[2] = cos(P);
            positions_spheres.push_back(R * x[2]);
            positions_spheres.push_back(R * y[2]);
            positions_spheres.push_back(R * z[2]);

            normals_spheres.push_back(x[2]);
            normals_spheres.push_back(y[2]);
            normals_spheres.push_back(z[2]);
            // D
            P = (p + rings) * to_rad;
            T = (t + sectors) * to_rad;
            x[3] = sin(P) * cos(T);
            y[3] = sin(P) * sin(T);
            z[3] = cos(P);
            positions_spheres.push_back(R * x[3]);
            positions_spheres.push_back(R * y[3]);
            positions_spheres.push_back(R * z[3]);

            normals_spheres.push_back(x[3]);
            normals_spheres.push_back(y[3]);
            normals_spheres.push_back(z[3]);

            positions_spheres.push_back(R * x[1]);
            positions_spheres.push_back(R * y[1]);
            positions_spheres.push_back(R * z[1]);

            normals_spheres.push_back(x[1]);
            normals_spheres.push_back(y[1]);
            normals_spheres.push_back(z[1]);

            positions_spheres.push_back(R * x[2]);
            positions_spheres.push_back(R * y[2]);
            positions_spheres.push_back(R * z[2]);

            normals_spheres.push_back(x[2]);
            normals_spheres.push_back(y[2]);
            normals_spheres.push_back(z[2]);
        }
    // Bottom of the sphere
    for (float t = 0; t < 360.f; t += sectors)
    {
        positions_spheres.push_back(0);
        positions_spheres.push_back(0);
        positions_spheres.push_back(-R);

        normals_spheres.push_back(0);
        normals_spheres.push_back(0);
        normals_spheres.push_back(-1);

        P = (180 - rings) * to_rad;
        T = t * to_rad;
        x[1] = sin(P) * cos(T);
        y[1] = sin(P) * sin(T);
        z[1] = cos(P);
        positions_spheres.push_back(R * x[1]);
        positions_spheres.push_back(R * y[1]);
        positions_spheres.push_back(R * z[1]);

        normals_spheres.push_back(x[1]);
        normals_spheres.push_back(y[1]);
        normals_spheres.push_back(z[1]);

        P = (180 - rings) * to_rad;
        T = (t + sectors) * to_rad;
        x[2] = sin(P) * cos(T);
        y[2] = sin(P) * sin(T);
        z[2] = cos(P);
        positions_spheres.push_back(R * x[2]);
        positions_spheres.push_back(R * y[2]);
        positions_spheres.push_back(R * z[2]);

        normals_spheres.push_back(x[2]);
        normals_spheres.push_back(y[2]);
        normals_spheres.push_back(z[2]);
    }
}