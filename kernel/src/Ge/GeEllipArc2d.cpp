#include "GeEllipArc2d.h"
#include "GeScale2d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeRay2d.h"
#include "GeEllipArc2d.h"
#include "GeCircArc2d.h"
#include "GePointOnCurve2d.h"
#include "GeInterval.h"
#include "GeBoundBlock2d.h"
#include "GeCircArc2d.h"
#include "GeImpl.h"
#include <cmath>

namespace {
double ellip2d_normalize_param(double param)
{
    param = std::fmod(param, 2.0 * PI);
    if (param < 0.0) {
        param += 2.0 * PI;
    }
    return param;
}

void ellip2d_append_unique_point(GePoint2dArray& points, const GePoint2d& point, const GeTol& tol)
{
    for (int i = 0; i < points.length(); i++) {
        if (points[i].isEqualTo(point, tol) == true) {
            return;
        }
    }
    points.append(point);
}

double ellip2d_param_sweep(const GeEllipArc2d& arc, const GeTol& tol)
{
    if (arc.isClosed(tol) == true) {
        return 2.0 * PI;
    }

    double start = ellip2d_normalize_param(arc.startAng());
    double end = ellip2d_normalize_param(arc.endAng());
    if (end < start) {
        end += 2.0 * PI;
    }
    return end - start;
}

GePoint2d ellip2d_eval_point(const GeEllipArc2d& arc, double param)
{
    param = ellip2d_normalize_param(param);
    GePoint2d point = arc.center();
    point += arc.majorAxis() * std::cos(param);
    point += arc.minorAxis() * std::sin(param);
    return point;
}

double ellip2d_circle_residual(const GeEllipArc2d& ellipse, const GeCircArc2d& circle, double param)
{
    GePoint2d point = ellip2d_eval_point(ellipse, param);
    return point.distanceTo(circle.center()) - circle.radius();
}

double ellip2d_circle_abs_residual(const GeEllipArc2d& ellipse, const GeCircArc2d& circle, double param)
{
    return std::fabs(ellip2d_circle_residual(ellipse, circle, param));
}

double ellip2d_bisect_circle_param(const GeEllipArc2d& ellipse, const GeCircArc2d& circle, double fromParam, double toParam, const GeTol& tol)
{
    double leftParam = fromParam;
    double rightParam = toParam;
    double leftValue = ellip2d_circle_residual(ellipse, circle, leftParam);
    double midParam = (leftParam + rightParam) * 0.5;

    for (int i = 0; i < 64; i++) {
        midParam = (leftParam + rightParam) * 0.5;
        double midValue = ellip2d_circle_residual(ellipse, circle, midParam);
        if (std::fabs(midValue) <= tol.equalPoint()) {
            break;
        }

        if (leftValue * midValue <= 0.0) {
            rightParam = midParam;
        }
        else {
            leftParam = midParam;
            leftValue = midValue;
        }
    }

    return ellip2d_normalize_param(midParam);
}

double ellip2d_refine_circle_min_param(const GeEllipArc2d& ellipse, const GeCircArc2d& circle, double fromParam, double toParam)
{
    double leftParam = fromParam;
    double rightParam = toParam;
    const double goldenRatio = 0.6180339887498949;
    double x1 = rightParam - (rightParam - leftParam) * goldenRatio;
    double x2 = leftParam + (rightParam - leftParam) * goldenRatio;
    double f1 = ellip2d_circle_abs_residual(ellipse, circle, x1);
    double f2 = ellip2d_circle_abs_residual(ellipse, circle, x2);

    for (int i = 0; i < 64; i++) {
        if (f1 <= f2) {
            rightParam = x2;
            x2 = x1;
            f2 = f1;
            x1 = rightParam - (rightParam - leftParam) * goldenRatio;
            f1 = ellip2d_circle_abs_residual(ellipse, circle, x1);
        }
        else {
            leftParam = x1;
            x1 = x2;
            f1 = f2;
            x2 = leftParam + (rightParam - leftParam) * goldenRatio;
            f2 = ellip2d_circle_abs_residual(ellipse, circle, x2);
        }
    }

    return ellip2d_normalize_param((leftParam + rightParam) * 0.5);
}

double ellip2d_point_dist2(const GeEllipArc2d& ellipse, const GePoint2d& point, double param)
{
    GePoint2d curvePoint = ellip2d_eval_point(ellipse, param);
    double dx = curvePoint.x - point.x;
    double dy = curvePoint.y - point.y;
    return dx * dx + dy * dy;
}

double ellip2d_refine_point_min_param(const GeEllipArc2d& ellipse, const GePoint2d& point, double fromParam, double toParam)
{
    double leftParam = fromParam;
    double rightParam = toParam;
    const double goldenRatio = 0.6180339887498949;
    double x1 = rightParam - (rightParam - leftParam) * goldenRatio;
    double x2 = leftParam + (rightParam - leftParam) * goldenRatio;
    double f1 = ellip2d_point_dist2(ellipse, point, x1);
    double f2 = ellip2d_point_dist2(ellipse, point, x2);

    for (int i = 0; i < 64; i++) {
        if (f1 <= f2) {
            rightParam = x2;
            x2 = x1;
            f2 = f1;
            x1 = rightParam - (rightParam - leftParam) * goldenRatio;
            f1 = ellip2d_point_dist2(ellipse, point, x1);
        }
        else {
            leftParam = x1;
            x1 = x2;
            f1 = f2;
            x2 = leftParam + (rightParam - leftParam) * goldenRatio;
            f2 = ellip2d_point_dist2(ellipse, point, x2);
        }
    }

    return ellip2d_normalize_param((leftParam + rightParam) * 0.5);
}
}


GeEllipArc2d::GeEllipArc2d() {
    GE_IMP_MEMORY_ENTITY(GeEllipArc2d);
}
GeEllipArc2d::GeEllipArc2d(const GeEllipArc2d& ell) {
    GE_IMP_MEMORY_ENTITY(GeEllipArc2d);

    this->set(ell.center(), ell.majorAxis(), ell.minorAxis(), ell.majorRadius(), ell.minorRadius(), ell.startAng(), ell.endAng());
}
GeEllipArc2d::GeEllipArc2d(const GeCircArc2d& arc) {
    GE_IMP_MEMORY_ENTITY(GeEllipArc2d);

    this->set(arc);
}
GeEllipArc2d::GeEllipArc2d(const GePoint2d& cent, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double majorRadius, double minorRadius) {
    GE_IMP_MEMORY_ENTITY(GeEllipArc2d);

    this->set(cent, majorAxis, minorAxis, majorRadius, minorRadius);
}
GeEllipArc2d::GeEllipArc2d(const GePoint2d& cent, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle) {
    GE_IMP_MEMORY_ENTITY(GeEllipArc2d);

    this->set(cent, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);
}

GePoint2dArray GeEllipArc2d::toLineSegment(const GePoint2d& center, const GeVector2d& majorAxis, const GeVector2d& minorAxis, int numSegments) {

    GePoint2dArray points = GeCircArc2d::toLineSegment(center, majorAxis.length(), numSegments);

    GeMatrix2d mat;
    mat.setToScaling(GeScale2d(1, minorAxis.length() / majorAxis.length()), center);
    GeMatrix2d tempMat;
    tempMat.setToRotation(GeVector2d::kXAxis.angleToCCW(majorAxis), center);
    mat *= tempMat;

    for (int i = 0; i < points.length(); i++) {
        GePoint2d position = points[i];
        position.transformBy(mat);
        points[i].set(position.x, position.y);
    }

    return points;
}
GePoint2dArray GeEllipArc2d::toLineSegment(const GePoint2d& center, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double _startAng, double _endAng, int numSegments) {
   
    GePoint2dArray points = GeCircArc2d::toLineSegment(center, majorAxis.length(), _startAng, _endAng, numSegments);

    GeMatrix2d mat;
    mat.setToScaling(GeScale2d(1, minorAxis.length() / majorAxis.length()), center);
    GeMatrix2d tempMat;
    tempMat.setToRotation(GeVector2d::kXAxis.angleToCCW(majorAxis), center);
    mat *= tempMat;

    for (int i = 0; i < points.length(); i++) {
        GePoint2d position = points[i];
        position.transformBy(mat);
        points[i].set(position.x, position.y);
    }

    return points;
}

bool GeEllipArc2d::intersectWith(const GeLinearEnt2d& line, int& intn, GePoint2d& p1, GePoint2d& p2) const {
    return this->intersectWith(line, intn, p1, p2, GeContext::gTol);
}
bool GeEllipArc2d::intersectWith(const GeLinearEnt2d& line, int& intn, GePoint2d& p1, GePoint2d& p2, const GeTol& tol) const {
    GePoint2dArray points = this->intersectWith(line, tol);
    intn = points.length();
    if (intn == 0) {
        return false;
    }
    if (intn >= 1) {
        p1 = points[0];
    }
    if (intn >= 2) {
        p2 = points[1];
    }
    return true;
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeLinearEnt2d& line) const {
    return this->intersectWith(line, GeContext::gTol);
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeLinearEnt2d& line, const GeTol& tol)const {
    GePoint2dArray points;

    double majorRadius = this->majorRadius();
    double minorRadius = this->minorRadius();
    if (majorRadius <= tol.equalPoint() || minorRadius <= tol.equalPoint()) {
        return points;
    }

    GeVector2d majorDir = this->majorAxis();
    GeVector2d minorDir = this->minorAxis();
    if (majorDir.isZeroLength(tol) == true || minorDir.isZeroLength(tol) == true) {
        return points;
    }
    majorDir.normalize();
    minorDir.normalize();

    GeVector2d offset = line.pointOnLine() - this->center();
    GeVector2d direction = line.direction();
    double x0 = offset.dotProduct(majorDir);
    double y0 = offset.dotProduct(minorDir);
    double dx = direction.dotProduct(majorDir);
    double dy = direction.dotProduct(minorDir);

    double quadA = (dx * dx) / (majorRadius * majorRadius) + (dy * dy) / (minorRadius * minorRadius);
    double quadB = 2.0 * ((x0 * dx) / (majorRadius * majorRadius) + (y0 * dy) / (minorRadius * minorRadius));
    double quadC = (x0 * x0) / (majorRadius * majorRadius) + (y0 * y0) / (minorRadius * minorRadius) - 1.0;
    if (std::fabs(quadA) <= tol.equalVector()) {
        return points;
    }

    double discriminant = quadB * quadB - 4.0 * quadA * quadC;
    if (discriminant < -tol.equalPoint()) {
        return points;
    }

    auto tryAppendPoint = [&](double lineParam) {
        GePoint2d point = line.pointOnLine() + direction * lineParam;
        if (line.isOn(point, tol) == false) {
            return;
        }
        if (this->isOn(point, tol) == false) {
            return;
        }
        ellip2d_append_unique_point(points, point, tol);
    };

    if (std::fabs(discriminant) <= tol.equalPoint()) {
        tryAppendPoint(-quadB / (2.0 * quadA));
        return points;
    }

    double root = std::sqrt(discriminant);
    tryAppendPoint((-quadB + root) / (2.0 * quadA));
    tryAppendPoint((-quadB - root) / (2.0 * quadA));
    return points;
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeCircArc2d& arc)const {
    return this->intersectWith(arc, GeContext::gTol);
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeCircArc2d& arc, const GeTol& tol)const {
    GePoint2dArray points;

    if (this->isCircular(tol) == true && this->majorRadius() > tol.equalPoint()) {
        GeCircArc2d ellipseAsCircle;
        ellipseAsCircle.set(this->center(), this->majorRadius(), this->startAng(), this->endAng(), this->majorAxis().normal(), false);
        GePoint2dArray circlePoints = ellipseAsCircle.intersectWith(arc, tol);
        for (int i = 0; i < circlePoints.length(); i++) {
            if (this->isOn(circlePoints[i], tol) == true && arc.isOn(circlePoints[i], tol) == true) {
                ellip2d_append_unique_point(points, circlePoints[i], tol);
            }
        }
        return points;
    }

    double sweep = ellip2d_param_sweep(*this, tol);
    if (sweep <= tol.equalPoint()) {
        sweep = 2.0 * PI;
    }

    int numSegments = 2048;
    if (sweep < PI * 2.0) {
        numSegments = static_cast<int>(2048.0 * sweep / (PI * 2.0));
        if (numSegments < 256) {
            numSegments = 256;
        }
    }

    double startParam = this->startAng();
    double prevParam = startParam;
    double prevValue = ellip2d_circle_residual(*this, arc, prevParam);
    if (std::fabs(prevValue) <= tol.equalPoint()) {
        GePoint2d point = ellip2d_eval_point(*this, prevParam);
        if (this->isOn(point, tol) == true && arc.isOn(point, tol) == true) {
            ellip2d_append_unique_point(points, point, tol);
        }
    }

    for (int i = 1; i <= numSegments; i++) {
        double currentParam = startParam + sweep * i / numSegments;
        double currentValue = ellip2d_circle_residual(*this, arc, currentParam);

        if (std::fabs(currentValue) <= tol.equalPoint()) {
            GePoint2d point = ellip2d_eval_point(*this, currentParam);
            if (this->isOn(point, tol) == true && arc.isOn(point, tol) == true) {
                ellip2d_append_unique_point(points, point, tol);
            }
        }

        if (prevValue * currentValue < 0.0) {
            double param = ellip2d_bisect_circle_param(*this, arc, prevParam, currentParam, tol);
            GePoint2d point = ellip2d_eval_point(*this, param);
            if (this->isOn(point, tol) == true && arc.isOn(point, tol) == true) {
                ellip2d_append_unique_point(points, point, tol);
            }
        }

        if (i < numSegments) {
            double nextParam = startParam + sweep * (i + 1) / numSegments;
            double nextValue = ellip2d_circle_residual(*this, arc, nextParam);
            if (std::fabs(currentValue) <= std::fabs(prevValue) && std::fabs(currentValue) <= std::fabs(nextValue)) {
                double param = ellip2d_refine_circle_min_param(*this, arc, prevParam, nextParam);
                if (ellip2d_circle_abs_residual(*this, arc, param) <= tol.equalPoint()) {
                    GePoint2d point = ellip2d_eval_point(*this, param);
                    if (this->isOn(point, tol) == true && arc.isOn(point, tol) == true) {
                        ellip2d_append_unique_point(points, point, tol);
                    }
                }
            }
        }

        prevParam = currentParam;
        prevValue = currentValue;
    }

    return points;
}
bool GeEllipArc2d::isCircular() const {
    return this->isCircular(GeContext::gTol);
}
bool GeEllipArc2d::isCircular(const GeTol& tol) const {
    if (std::fabs(this->majorRadius() - this->minorRadius()) < tol.equalVector()) {
        return true;
    }
    return false;
}
bool GeEllipArc2d::isInside(const GePoint2d& pnt) const {
    return this->isInside(pnt, GeContext::gTol);
}
bool GeEllipArc2d::isInside(const GePoint2d& pnt, const GeTol& tol) const {
    double res = (pow((pnt.x - this->center().x), 2.0) / pow(this->majorRadius(), 2.0)) + (pow((pnt.y - this->center().y), 2.0) / pow(this->minorRadius(), 2.0));
    if (res < 1.0 + tol.equalPoint()) {
        return true;
    }
    return false;
}
GePoint2d GeEllipArc2d::center() const {
    return GE_IMP_ELLIPARC2D(this->m_pImpl)->center;
}
double GeEllipArc2d::minorRadius() const {
    return this->majorRadius() * GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio;
}
double GeEllipArc2d::majorRadius() const {
    return GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis.length();
}
GeVector2d GeEllipArc2d::minorAxis() const {
    GeVector2d minorAxis = GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis;
    minorAxis.rotateBy(PI / 2.0);
    minorAxis *= GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio;
    return minorAxis;
}
GeVector2d GeEllipArc2d::majorAxis() const {
    return GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis;
}
double GeEllipArc2d::startAng() const {
    return GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle;
}
double GeEllipArc2d::endAng() const {
    return GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle;
}
GePoint2d GeEllipArc2d::startPoint() const {
    GePointOnCurve2d cur(*this, this->startAng());
    return cur.point();
}
GePoint2d GeEllipArc2d::endPoint() const {
    GePointOnCurve2d cur(*this, this->endAng());
    return cur.point();
}
bool GeEllipArc2d::isClockWise() const {
    return false;
}

GeEllipArc2d& GeEllipArc2d::setCenter(const GePoint2d& cent) {
    GE_IMP_ELLIPARC2D(this->m_pImpl)->center = cent;
    return *this;
}
GeEllipArc2d& GeEllipArc2d::setMinorRadius(double rad) {
    if (rad < this->majorRadius() + GeContext::gTol.equalVector()) {
        GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio = rad / this->majorRadius();
    }
    return *this;
}
GeEllipArc2d& GeEllipArc2d::setMajorRadius(double rad) {

    if (rad > this->minorRadius() - GeContext::gTol.equalVector()) {
        GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis = this->majorAxis().normal() * rad;
        GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio = this->minorRadius() / this->majorRadius();
    }
    return *this;
}
GeEllipArc2d& GeEllipArc2d::setAxes(const GeVector2d& majorAxis, const GeVector2d& minorAxis) {
    if (majorAxis.length() - minorAxis.length() > 0 - GeContext::gTol.equalVector()) {
        GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis = majorAxis;
        this->setMinorRadius(minorAxis.length());
    }
    return *this;
}
GeEllipArc2d& GeEllipArc2d::setAngles(double startAngle, double endAngle) {
    double sweep = endAngle - startAngle;
    double absSweep = std::fabs(sweep);
    double start = std::fmod(startAngle, 2.0 * PI);
    if (start < 0.0) {
        start += 2.0 * PI;
    }
    double end = std::fmod(endAngle, 2.0 * PI);
    if (end < 0.0) {
        end += 2.0 * PI;
    }
    if (absSweep >= 2.0 * PI - GeContext::gTol.equalPoint())
    {
        end = start + (sweep >= 0.0 ? 2.0 * PI : -2.0 * PI);
    }
    GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle = start;
    GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle = end;
    return *this;
}

GeEllipArc2d& GeEllipArc2d::set(const GePoint2d& cent, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double majorRadius, double minorRadius) {
    this->set(cent, majorAxis, minorAxis, majorRadius, minorRadius, 0, PI * 2);
    return *this;
}
GeEllipArc2d& GeEllipArc2d::set(const GePoint2d& cent, const GeVector2d& majorAxis, const GeVector2d& minorAxis, double majorRadius, double minorRadius, double startAngle, double endAngle) {
    if (majorRadius > minorRadius - GeContext::gTol.equalVector()) {
        GE_IMP_ELLIPARC2D(this->m_pImpl)->center = cent;
        GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis = majorAxis.normal() * majorRadius;
        GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio = minorRadius / majorRadius;
        this->setAngles(startAngle, endAngle);
    }
    return *this;
}
GeEllipArc2d& GeEllipArc2d::set(const GeCircArc2d& arc) {
    GE_IMP_ELLIPARC2D(this->m_pImpl)->center = arc.center();
    GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis = GeVector2d::kXAxis;
    GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio = 1;
    GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle = arc.startAng();
    GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle = arc.endAng();
    return *this;
}

GeEllipArc2d& GeEllipArc2d::operator = (const GeEllipArc2d& arc) {
    this->set(arc.center(), arc.majorAxis(), arc.minorAxis(), arc.majorRadius(), arc.minorRadius(), arc.startAng(), arc.endAng());
    return *this;
}




bool GeEllipArc2d::isKindOf(Ge::EntityId entType) const {
    if (entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == this->type()) {
        return true;
    }
    return false;
}
Ge::EntityId GeEllipArc2d::type() const {
    return Ge::EntityId::kEllipArc2d;
}
GeEllipArc2d* GeEllipArc2d::copy() const {
    GeEllipArc2d* arc = new GeEllipArc2d();
    GE_IMP_ELLIPARC2D(arc->m_pImpl)->center = GE_IMP_ELLIPARC2D(this->m_pImpl)->center;
    GE_IMP_ELLIPARC2D(arc->m_pImpl)->majorAxis = GE_IMP_ELLIPARC2D(this->m_pImpl)->majorAxis;
    GE_IMP_ELLIPARC2D(arc->m_pImpl)->ratio = GE_IMP_ELLIPARC2D(this->m_pImpl)->ratio;
    GE_IMP_ELLIPARC2D(arc->m_pImpl)->startAngle = GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle;
    GE_IMP_ELLIPARC2D(arc->m_pImpl)->endAngle = GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle;
    return arc;
}
bool GeEllipArc2d::operator == (const GeEllipArc2d& entity) const {
    return this->isEqualTo(entity);
}
bool GeEllipArc2d::operator != (const GeEllipArc2d& entity) const {
    return !(*this == entity);
}
bool GeEllipArc2d::isEqualTo(const GeEllipArc2d& entity) const {
    return this->isEqualTo(entity, GeContext::gTol);
}
bool GeEllipArc2d::isEqualTo(const GeEllipArc2d& entity, const GeTol& tol) const {
    auto angleDiff = [](double a, double b) {
        double diff = std::fmod(a - b, 2.0 * PI);
        if (diff < 0.0) {
            diff += 2.0 * PI;
        }
        if (diff > PI) {
            diff = 2.0 * PI - diff;
        }
        return diff;
    };

    if (this->center().isEqualTo(entity.center(), tol) == false) {
        return false;
    }
    if (this->majorAxis().isEqualTo(entity.majorAxis(), tol) == false) {
        return false;
    }
    if (std::fabs(this->majorRadius() - entity.majorRadius()) > tol.equalPoint()) {
        return false;
    }
    if (std::fabs(this->minorRadius() - entity.minorRadius()) > tol.equalPoint()) {
        return false;
    }

    bool thisClosed = this->isClosed(tol);
    bool entityClosed = entity.isClosed(tol);
    if (thisClosed != entityClosed) {
        return false;
    }
    if (thisClosed == false) {
        if (angleDiff(this->startAng(), entity.startAng()) > tol.equalPoint()) {
            return false;
        }
        if (angleDiff(this->endAng(), entity.endAng()) > tol.equalPoint()) {
            return false;
        }
    }

    return true;
}
GeEllipArc2d& GeEllipArc2d::transformBy(const GeMatrix2d& xfm) {

    GePoint2d center = this->center();
    GeVector2d majorAxis = this->majorAxis();
    GeVector2d minorAxis = this->minorAxis();
    GePoint2d startPoint = this->startPoint();
    GePoint2d endPoint = this->endPoint();
    center.transformBy(xfm);
    majorAxis.transformBy(xfm);
    minorAxis.transformBy(xfm);
    startPoint.transformBy(xfm);
    endPoint.transformBy(xfm);

    GeVector2d majorDir = majorAxis;
    GeVector2d minorDir = minorAxis;
    majorDir.normalize();
    minorDir.normalize();

    GeVector2d startVec = startPoint - center;
    GeVector2d endVec = endPoint - center;
    double startAng = atan2(startVec.dotProduct(minorDir), startVec.dotProduct(majorDir));
    double endAng = atan2(endVec.dotProduct(minorDir), endVec.dotProduct(majorDir));
    this->set(center, majorAxis, minorAxis, majorAxis.length(), minorAxis.length(), startAng, endAng);
    return *this;
}
GeEllipArc2d& GeEllipArc2d::translateBy(const GeVector2d& translateVec) {
    this->setCenter(this->center().translation(translateVec));
    return *this;
}
GeEllipArc2d& GeEllipArc2d::rotateBy(double angle) {
    return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeEllipArc2d& GeEllipArc2d::rotateBy(double angle, const GePoint2d& wrtPoint) {

    GeMatrix2d mat;
    mat.setToRotation(angle, wrtPoint);
    this->transformBy(mat);
    return *this;
}
GeEllipArc2d& GeEllipArc2d::mirror(const GeLine2d& line) {

    GeMatrix2d mat;
    mat.setToMirroring(line);
    this->transformBy(mat);
    return *this;
}
GeEllipArc2d& GeEllipArc2d::scaleBy(double scaleFactor) {
    return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeEllipArc2d& GeEllipArc2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {

    GeMatrix2d mat;
    mat.setToScaling(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
    this->transformBy(mat);
    return *this;
}
bool GeEllipArc2d::isOn(const GePoint2d& pnt) const {
    return this->isOn(pnt, GeContext::gTol);
}

/*
    1.将点反方向旋转到X轴
    2.将点在Y轴做缩放匹配圆弧
    3.判断点是否在其所在圆弧上
*/
bool GeEllipArc2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {

    if (this->majorRadius() <= tol.equalPoint() || this->minorRadius() <= tol.equalPoint()) {
        return false;
    }

    // 获得X轴到长轴的夹角
    double angle = GeVector2d::kXAxis.angleToCCW(this->majorAxis());

    // 点旋转
    GePoint2d position = pnt;
    position.rotateBy(0 - angle, this->center());

    // 做Y轴缩放
    GeMatrix2d mat;
    mat.setToScaling(GeScale2d(1, this->majorRadius() / this->minorRadius()), this->center());
    position.transformBy(mat);

    // 判断点是否在圆弧上
    GeCircArc2d circArc;
    circArc.set(this->center(), this->majorRadius(), this->startAng(), this->endAng());
    return circArc.isOn(position, tol);
}




void GeEllipArc2d::getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const {
	piece1 = NULL;
	piece2 = NULL;

    GePointOnCurve2d pointOnCurve(*this, param);
    GePoint2d point = pointOnCurve.point();
    if (this->isOn(point) == false) {
        return;
    }

    double angle = this->paramOf(point);

    piece1 = new GeEllipArc2d(this->center(), this->majorAxis(), this->minorAxis(),this->majorRadius(),this->minorRadius(), this->startAng(), angle);
    piece2 = new GeEllipArc2d(this->center(), this->majorAxis(), this->minorAxis(), this->majorRadius(), this->minorRadius(), angle, this->endAng());
}
bool GeEllipArc2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
    GeEllipArc2d* arc = new GeEllipArc2d(*this);
    explodedCurves.append(arc);
    newExplodedCurve.append(1);
    return true;
}
GeBoundBlock2d GeEllipArc2d::boundBlock() const {
    GeInterval range;
    double start = this->startAng();
    double end = this->endAng();
    if (this->isClosed(GeContext::gTol) == true) {
        end = start + 2.0 * PI;
    }
    else if (end < start) {
        end += 2.0 * PI;
    }
    range.set(start, end);
    return this->boundBlock(range);
}
GeBoundBlock2d GeEllipArc2d::boundBlock(const GeInterval& range) const {
   
    GeBoundBlock2d boundBlock;

    // 获得x轴到长轴的角度
    double angle = GeVector2d::kXAxis.angleToCCW(this->majorAxis());

    // 复制一个对象后旋转回0度
    GeEllipArc2d ellipArc = *this;
    ellipArc.rotateBy(0 - angle, this->center());

    boundBlock = ellipArc.orthoBoundBlock(range);
    boundBlock.rotateBy(angle, this->center());

    return boundBlock;
}
GeBoundBlock2d GeEllipArc2d::orthoBoundBlock() const {
    GeInterval range;
    double start = this->startAng();
    double end = this->endAng();
    if (this->isClosed(GeContext::gTol) == true) {
        end = start + 2.0 * PI;
    }
    else if (end < start) {
        end += 2.0 * PI;
    }
    range.set(start, end);
    return this->orthoBoundBlock(range);
}
GeBoundBlock2d GeEllipArc2d::orthoBoundBlock(const GeInterval& range) const {

    GeBoundBlock2d boundBlock;

    GePointOnCurve2d pointOnCurve;
    pointOnCurve.setCurve(*this);

    //获得起点
    pointOnCurve.setParameter(range.lowerBound());
    GePoint2d startPoint = pointOnCurve.point();

    //获得终点
    pointOnCurve.setParameter(range.upperBound());
    GePoint2d endPoint = pointOnCurve.point();

    GePoint2dArray points;
    points.append(startPoint);
    points.append(endPoint);

    //获得象限点
    GePoint2d point = this->center() + this->majorAxis();
    if (this->isOn(point) == true) {
        points.append(point);
    }
    point = this->center() + this->minorAxis();
    if (this->isOn(point) == true) {
        points.append(point);
    }
    point = this->center() - this->majorAxis();
    if (this->isOn(point) == true) {
        points.append(point);
    }
    point = this->center() - this->minorAxis();
    if (this->isOn(point) == true) {
        points.append(point);
    }

    GePoint2d minPoint, maxPoint;
    for (int i = 0; i < points.length(); i++) {

        if (i == 0) {
            minPoint = points[i];
            maxPoint = points[i];
            continue;
        }

        if (points[i].x < minPoint.x) {
            minPoint.x = points[i].x;
        }
        if (points[i].x > maxPoint.x) {
            maxPoint.x = points[i].x;
        }
        if (points[i].y < minPoint.y) {
            minPoint.y = points[i].y;
        }
        if (points[i].y > maxPoint.y) {
            maxPoint.y = points[i].y;
        }
    }

    boundBlock.set(minPoint, maxPoint);

    return boundBlock;
}
bool GeEllipArc2d::hasStartPoint(GePoint2d& startPoint) const {
    startPoint = this->startPoint();
    return true;
}
bool GeEllipArc2d::hasEndPoint(GePoint2d& endPoint) const {
    endPoint = this->endPoint();
    return true;
}
double GeEllipArc2d::length() const {
    return this->length(this->startAng(), this->endAng(), GeContext::gTol.equalPoint());
}
double GeEllipArc2d::length(double fromParam, double toParam)const {
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeEllipArc2d::length(double fromParam, double toParam, double tol)const {
    fromParam = ellip2d_normalize_param(fromParam);
    toParam = ellip2d_normalize_param(toParam);
    if (toParam < fromParam) {
        toParam += PI * 2.0;
    }

    GePoint2dArray points = GeEllipArc2d::toLineSegment(this->center(), this->majorAxis(), this->minorAxis(), fromParam, toParam, 256);
    double len = 0.0;
    for (int i = 1; i < points.length(); i++) {
        len += points[i - 1].distanceTo(points[i]);
    }
    return len;
}
double GeEllipArc2d::area() const {
    return this->area(GeContext::gTol);
}
double GeEllipArc2d::area(const GeTol& tol) const {
    if (this->isClosed() == false) {
        return 0.0;
    }
    return PI * this->majorRadius() * this->minorRadius();
}
double GeEllipArc2d::paramAtLength(double datumParam, double length) const {
    return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint()); 
}
double GeEllipArc2d::paramAtLength(double datumParam, double length, double tol) const {
    if (std::fabs(length) <= tol) {
        return ellip2d_normalize_param(datumParam);
    }

    double total = 0.0;
    double currentParam = datumParam;
    const int numSegments = 256;
    const double step = (PI * 2.0) / numSegments;
    const double direction = length >= 0.0 ? 1.0 : -1.0;
    const double targetLen = std::fabs(length);
    GePoint2d prevPoint = this->evalPoint(currentParam);

    for (int i = 1; i <= numSegments; i++) {
        double nextParam = datumParam + direction * step * i;
        GePoint2d nextPoint = this->evalPoint(nextParam);
        double segLen = prevPoint.distanceTo(nextPoint);

        if (total + segLen >= targetLen) {
            double remain = targetLen - total;
            double ratio = 0.0;
            if (segLen > tol) {
                ratio = remain / segLen;
            }
            if (ratio < 0.0) {
                ratio = 0.0;
            }
            if (ratio > 1.0) {
                ratio = 1.0;
            }

            double hitParam = currentParam + (nextParam - currentParam) * ratio;
            return ellip2d_normalize_param(hitParam);
        }

        total += segLen;
        prevPoint = nextPoint;
        currentParam = nextParam;
    }

    return ellip2d_normalize_param(currentParam);
}
double GeEllipArc2d::distanceTo(const GePoint2d& point) const
{
    return this->distanceTo(point, GeContext::gTol);
}
double GeEllipArc2d::distanceTo(const GePoint2d& point, const GeTol& tol) const
{
    GePoint2d closest = this->closestPointTo(point, tol);
    return closest.distanceTo(point);
}
double GeEllipArc2d::distanceTo(const GeLine2d& entity) const {
    return this->distanceTo(entity, GeContext::gTol);
}
double GeEllipArc2d::distanceTo(const GeLine2d& line, const GeTol& tol) const {
    GePoint2d closest;
    GePoint2d itself = this->closestPointTo(line, closest, tol);
    return closest.distanceTo(itself);
}
double GeEllipArc2d::distanceTo(const GeLineSeg2d& entity) const {
    return this->distanceTo(entity, GeContext::gTol);
}
double GeEllipArc2d::distanceTo(const GeLineSeg2d& line, const GeTol& tol) const {
    GePoint2d closest;
    GePoint2d itself = this->closestPointTo(line, closest, tol);
    return closest.distanceTo(itself);
}
double GeEllipArc2d::distanceTo(const GeRay2d& entity) const {
    return this->distanceTo(entity, GeContext::gTol);
}
double GeEllipArc2d::distanceTo(const GeRay2d& line, const GeTol& tol) const {
    GePoint2d closest;
    GePoint2d itself = this->closestPointTo(line, closest, tol);
    return closest.distanceTo(itself);
}
double GeEllipArc2d::distanceTo(const GeCircArc2d& entity) const {
    return this->distanceTo(entity, GeContext::gTol);
}
double GeEllipArc2d::distanceTo(const GeCircArc2d& arc, const GeTol& tol) const {

    GePoint2d closest;
    GePoint2d itself = this->closestPointTo(arc, closest, tol);
    return closest.distanceTo(itself);
}
GePoint2d GeEllipArc2d::closestPointTo(const GePoint2d& pnt) const {
    return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
    GePoint2d closest;

    double sweep = ellip2d_param_sweep(*this, tol);
    if (sweep <= tol.equalPoint()) {
        closest = this->startPoint();
        if (pnt.distanceTo(this->endPoint()) < pnt.distanceTo(this->startPoint())) {
            closest = this->endPoint();
        }
        return closest;
    }

    const int numSegments = 256;
    const double startParam = this->startAng();
    int bestIndex = 0;
    double bestDist2 = 0.0;
    for (int i = 0; i <= numSegments; i++) {
        double param = startParam + sweep * i / numSegments;
        double dist2 = ellip2d_point_dist2(*this, pnt, param);
        if (i == 0 || dist2 < bestDist2) {
            bestDist2 = dist2;
            bestIndex = i;
        }
    }

    int leftIndex = bestIndex > 0 ? bestIndex - 1 : bestIndex;
    int rightIndex = bestIndex < numSegments ? bestIndex + 1 : bestIndex;
    double leftParam = startParam + sweep * leftIndex / numSegments;
    double rightParam = startParam + sweep * rightIndex / numSegments;
    if (rightParam < leftParam) {
        rightParam = leftParam;
    }

    double bestParam = leftParam;
    if (rightParam > leftParam + tol.equalPoint()) {
        bestParam = ellip2d_refine_point_min_param(*this, pnt, leftParam, rightParam);
    }

    closest = ellip2d_eval_point(*this, bestParam);

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    GePoint2dArray intersects = this->intersectWith(curve2d, tol);
    if (intersects.length() > 0) {
        pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
        closest.set(intersects[0].x, intersects[0].y);
        return closest;
    }

    GePoint2dArray points = GeEllipArc2d::toLineSegment(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
    if (points.length() == 0) {
        GePoint2d startPoint = this->startPoint();
        GePoint2d endPoint = this->endPoint();
        GePoint2d startOther = curve2d.closestPointTo(startPoint, tol);
        GePoint2d endOther = curve2d.closestPointTo(endPoint, tol);
        closest = startPoint;
        pntOnOtherCrv = startOther;
        if (endPoint.distanceTo(endOther) < startPoint.distanceTo(startOther)) {
            closest = endPoint;
            pntOnOtherCrv = endOther;
        }
        return closest;
    }

    double minDist = 0.0;
    for (int i = 0; i < points.length(); i++) {
        GePoint2d other = curve2d.closestPointTo(points[i], tol);
        double dist = points[i].distanceTo(other);
        if (i == 0 || dist < minDist) {
            minDist = dist;
            closest = points[i];
            pntOnOtherCrv = other;
        }
    }

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    GePoint2dArray intersects = this->intersectWith(curve2d, tol);
    if (intersects.length() > 0) {
        pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
        closest.set(intersects[0].x, intersects[0].y);
        return closest;
    }

    GePoint2dArray points = GeEllipArc2d::toLineSegment(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
    if (points.length() == 0) {
        GePoint2d startPoint = this->startPoint();
        GePoint2d endPoint = this->endPoint();
        GePoint2d startOther = curve2d.closestPointTo(startPoint, tol);
        GePoint2d endOther = curve2d.closestPointTo(endPoint, tol);
        closest = startPoint;
        pntOnOtherCrv = startOther;
        if (endPoint.distanceTo(endOther) < startPoint.distanceTo(startOther)) {
            closest = endPoint;
            pntOnOtherCrv = endOther;
        }
        return closest;
    }

    double minDist = 0.0;
    for (int i = 0; i < points.length(); i++) {
        GePoint2d other = curve2d.closestPointTo(points[i], tol);
        double dist = points[i].distanceTo(other);
        if (i == 0 || dist < minDist) {
            minDist = dist;
            closest = points[i];
            pntOnOtherCrv = other;
        }
    }

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    GePoint2dArray intersects = this->intersectWith(curve2d, tol);
    if (intersects.length() > 0) {
        pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
        closest.set(intersects[0].x, intersects[0].y);
        return closest;
    }

    GePoint2dArray points = GeEllipArc2d::toLineSegment(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
    if (points.length() == 0) {
        GePoint2d startPoint = this->startPoint();
        GePoint2d endPoint = this->endPoint();
        GePoint2d startOther = curve2d.closestPointTo(startPoint, tol);
        GePoint2d endOther = curve2d.closestPointTo(endPoint, tol);
        closest = startPoint;
        pntOnOtherCrv = startOther;
        if (endPoint.distanceTo(endOther) < startPoint.distanceTo(startOther)) {
            closest = endPoint;
            pntOnOtherCrv = endOther;
        }
        return closest;
    }

    double minDist = 0.0;
    for (int i = 0; i < points.length(); i++) {
        GePoint2d other = curve2d.closestPointTo(points[i], tol);
        double dist = points[i].distanceTo(other);
        if (i == 0 || dist < minDist) {
            minDist = dist;
            closest = points[i];
            pntOnOtherCrv = other;
        }
    }

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    GePoint2dArray intersects = this->intersectWith(curve2d, tol);
    if (intersects.length() > 0) {
        pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
        closest.set(intersects[0].x, intersects[0].y);
        return closest;
    }

    GePoint2dArray points = GeEllipArc2d::toLineSegment(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 256);
    if (points.length() == 0) {
        GePoint2d startPoint = this->startPoint();
        GePoint2d endPoint = this->endPoint();
        GePoint2d startOther = curve2d.closestPointTo(startPoint, tol);
        GePoint2d endOther = curve2d.closestPointTo(endPoint, tol);
        closest = startPoint;
        pntOnOtherCrv = startOther;
        if (endPoint.distanceTo(endOther) < startPoint.distanceTo(startOther)) {
            closest = endPoint;
            pntOnOtherCrv = endOther;
        }
        return closest;
    }

    double minDist = 0.0;
    for (int i = 0; i < points.length(); i++) {
        GePoint2d other = curve2d.closestPointTo(points[i], tol);
        double dist = points[i].distanceTo(other);
        if (i == 0 || dist < minDist) {
            minDist = dist;
            closest = points[i];
            pntOnOtherCrv = other;
        }
    }

    return closest;
}
GECURVE2D_METHODS_GETCLOSESTPOINTTO(GeEllipArc2d);
double GeEllipArc2d::paramOf(const GePoint2d& pnt) const {
    return this->paramOf(pnt, GeContext::gTol);
}
double GeEllipArc2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {

    if (this->isOn(pnt, tol) == false) {
        return 0.0;
    }

    double majorRadius = this->majorRadius();
    double minorRadius = this->minorRadius();
    if (majorRadius < tol.equalPoint() || minorRadius < tol.equalPoint()) {
        return 0.0;
    }

    GeVector2d majorDir = this->majorAxis();
    GeVector2d minorDir = this->minorAxis();
    majorDir.normalize();
    minorDir.normalize();

    GeVector2d vec = pnt - this->center();
    double x = vec.dotProduct(majorDir) / majorRadius;
    double y = vec.dotProduct(minorDir) / minorRadius;
    return ellip2d_normalize_param(std::atan2(y, x));
}
void GeEllipArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {
    return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeEllipArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
    return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend, GeContext::gTol);
}
void GeEllipArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

    if (this->minorRadius() + distance <= tol.equalPoint() ||
        this->majorRadius() + distance <= tol.equalPoint()) {
        return;
    }

    double majorRadius = this->majorRadius();
    double minorRadius = this->minorRadius();

    GeEllipArc2d* circArc = new GeEllipArc2d();
    GE_IMP_ELLIPARC2D(circArc->m_pImpl)->center = this->center();
	GE_IMP_ELLIPARC2D(circArc->m_pImpl)->majorAxis = this->majorAxis().normal() * (majorRadius + distance);
    GE_IMP_ELLIPARC2D(circArc->m_pImpl)->ratio = (minorRadius + distance) / GE_IMP_ELLIPARC2D(circArc->m_pImpl)->majorAxis.length();
    GE_IMP_ELLIPARC2D(circArc->m_pImpl)->startAngle = GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle;
    GE_IMP_ELLIPARC2D(circArc->m_pImpl)->endAngle = GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle;

    offsetCurveList.append(circArc);
}
bool GeEllipArc2d::isClosed() const {
    return this->isClosed(GeContext::gTol);
}
bool GeEllipArc2d::isClosed(const GeTol& tol) const {
    double absSweep = std::fabs(this->endAng() - this->startAng());
    if (absSweep < 2.0 * PI - tol.equalPoint()) {
        return false;
    }

    double turns = absSweep / (2.0 * PI);
    double nearestTurns = std::floor(turns + 0.5);
    if (nearestTurns < 1.0) {
        nearestTurns = 1.0;
    }

    if (std::fabs(absSweep - nearestTurns * 2.0 * PI) <= tol.equalPoint()) {
        return true;
    }
    return false;
}
