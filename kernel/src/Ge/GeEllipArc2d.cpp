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

    //std::vector<Point> intersections;

    // 转为一般式
    double A, B, C;
    GeLinearEnt2d::generalFormula(line, A, B, C);

    double a = this->majorRadius();
    double b = this->minorRadius();

    double discriminant, x1, x2, y1, y2;

    // 将直线方程代入椭圆方程  
    double term1 = a * a * B * B;
    double term2 = b * b * A * A;
    double term3 = 2 * a * a * B * C;
    double term4 = a * a * C * C - b * b * B * B;

    discriminant = term3 * term3 - 4 * term1 * term4;

    // 判断是否有交点  
    if (discriminant < 0) {
        // 无交点  
        intn = 0;
        return false;
    }
    else if (abs(discriminant) < 0.00002) {
        // 一个交点  
        intn = 1;
        x1 = -term3 / (2 * term1);
        y1 = -(A / B) * x1 - C / B;
        p1.set(x1, y1);
        return true;
    }
    else {
        // 两个交点  
        intn = 2;
        x1 = (-term3 + sqrt(discriminant)) / (2 * term1);
        x2 = (-term3 - sqrt(discriminant)) / (2 * term1);
        y1 = -(A / B) * x1 - C / B;
        y2 = -(A / B) * x2 - C / B;
        p1.set(x1, y1);
        p2.set(x2, y2);
        return true;
    }
    return true;
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeLinearEnt2d& line) const {
    return this->intersectWith(line, GeContext::gTol);
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeLinearEnt2d& line, const GeTol& tol)const {
    
    GePoint2dArray points;

    int intn;
    GePoint2d p1, p2;
    this->intersectWith(line, intn, p1, p2, tol);
    if (intn == 1) {
        points.append(p1);
    }
    else if (intn == 2) {
        points.append(p2);
    }
    return points;
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeCircArc2d& arc)const {
    return this->intersectWith(arc, GeContext::gTol);
}
GePoint2dArray GeEllipArc2d::intersectWith(const GeCircArc2d& arc, const GeTol& tol)const {
    GePoint2dArray points;
    return points;
}
bool GeEllipArc2d::isCircular() const {
    return this->isCircular(GeContext::gTol);
}
bool GeEllipArc2d::isCircular(const GeTol& tol) const {
    if (abs(this->majorRadius() - this->minorRadius()) < tol.equalVector()) {
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
    GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle = startAngle;
    GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle = endAngle;
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
        GE_IMP_ELLIPARC2D(this->m_pImpl)->startAngle = startAngle;
        GE_IMP_ELLIPARC2D(this->m_pImpl)->endAngle = endAngle;
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
    if (this->center().isEqualTo(entity.center(), tol) == false) {
        return false;
    }
    if (this->majorAxis().isEqualTo(entity.majorAxis(), tol) == false) {
        return false;
    }
    if (abs(this->majorRadius() - entity.majorRadius()) > tol.equalPoint()) {
        return false;
    }
    if (abs(this->minorRadius() - entity.minorRadius()) > tol.equalPoint()) {
        return false;
    }
    if (abs(this->startAng() - entity.startAng()) > tol.equalPoint()) {
        return false;
    }
    if (abs(this->endAng() - entity.endAng()) > tol.equalPoint()) {
        return false;
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

    // 获得X轴到长轴的夹角
    double angle = GeVector2d::kXAxis.angleToCCW(this->majorAxis());

    // 点旋转
    GePoint2d position = pnt;
    position.rotateBy(0 - angle, this->center());

    // 做Y轴缩放
    GeMatrix2d mat;
    mat.setToScaling(GeScale2d(1, this->majorRadius() / this->minorRadius()));
    position.transformBy(mat);

    // 判断点是否在圆弧上
    GeCircArc2d circArc;
    circArc.set(this->center(), this->majorRadius(), this->startAng(), this->endAng());
    return circArc.isOn(position);
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
    range.set(this->paramOf(this->startPoint()), this->paramOf(this->endPoint()));
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
    range.set(this->paramOf(this->startPoint()), this->paramOf(this->endPoint()));
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
    GePoint2dArray points = GeEllipArc2d::toLineSegment(this->center(), this->majorAxis(), this->minorAxis(), this->startAng(), this->endAng(), 200);
    double len = 0.0;
    for (int i = 1; i < points.length(); i++) {
        len += (points[i - 1].distanceTo(points[i]));
    }
    return len;
}
double GeEllipArc2d::length(double fromParam, double toParam)const {
    return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeEllipArc2d::length(double fromParam, double toParam, double tol)const {
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
    double total = 0.0;
    double currentParam = datumParam;
    GePoint2d prevPoint = this->evalPoint(currentParam);

    for (int i = 1; i <= 256; i++) {
        double nextParam = datumParam + (PI * 2.0) / 256.0 * i;
        GePoint2d nextPoint = this->evalPoint(nextParam);
        double segLen = prevPoint.distanceTo(nextPoint);

        if (total + segLen >= length) {
            while (nextParam >= PI * 2.0) {
                nextParam -= PI * 2.0;
            }
            while (nextParam < 0.0) {
                nextParam += PI * 2.0;
            }
            return nextParam;
        }

        total += segLen;
        prevPoint = nextPoint;
        currentParam = nextParam;
    }

    while (currentParam >= PI * 2.0) {
        currentParam -= PI * 2.0;
    }
    while (currentParam < 0.0) {
        currentParam += PI * 2.0;
    }
    return currentParam;
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

    do
    {
        //获得圆弧和垂线是否存在交点
        GePoint2dArray intersects = this->intersectWith(GeLineSeg2d(pnt, this->center()), tol);
        if (intersects.length() > 0) {
            closest.set(intersects[0].x, intersects[0].y);
            break;
        }

        double dist = pnt.distanceTo(this->startPoint());
        closest.set(this->startPoint().x, this->startPoint().y);
        if (pnt.distanceTo(this->endPoint()) < dist) {
            closest.set(this->endPoint().x, this->endPoint().y);
        }

    } while (false);

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    do
    {
        GePoint2dArray intersects = this->intersectWith(curve2d, tol);
        if (intersects.length() > 0) {
            pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
            closest.set(intersects[0].x, intersects[0].y);
            break;
        }


        GePoint2dArray pointItselfs, pointOthers;
        pointItselfs.append(this->startPoint());
        pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
        pointItselfs.append(this->endPoint());
        pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

        //获得圆弧和垂线是否存在交点
        GePoint2d vertical = GeLine2d::vertical(this->center(), curve2d);
        intersects = this->intersectWith(GeLineSeg2d(vertical, this->center()), tol);
        if (intersects.length() > 0) {
            if (this->isOn(intersects[0], tol) == true) {
                pointItselfs.append(intersects[0]);
            }
            if (curve2d.isOn(vertical, tol) == true) {
                pointOthers.append(vertical);
            }
        }

        double minDist = 0.0;
        for (int i = 0; i < pointItselfs.length(); i++) {
            for (int u = 0; u < pointOthers.length(); u++) {
                double dist = pointItselfs[i].distanceTo(pointOthers[u]);
                if (i == 0 && u == 0) {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                    continue;
                }
                if (dist < minDist)
                {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                }
            }
        }

    } while (false);

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    do
    {
        GePoint2dArray intersects = this->intersectWith(curve2d, tol);
        if (intersects.length() > 0) {
            pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
            closest.set(intersects[0].x, intersects[0].y);
            break;
        }

        GePoint2dArray pointItselfs, pointOthers;
        pointItselfs.append(this->closestPointTo(curve2d.startPoint(), tol));
        pointOthers.append(curve2d.startPoint());
        pointItselfs.append(this->closestPointTo(curve2d.endPoint(), tol));
        pointOthers.append(curve2d.endPoint());
        pointItselfs.append(this->startPoint());
        pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
        pointItselfs.append(this->endPoint());
        pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

        //获得圆弧和垂线是否存在交点
        GePoint2d vertical = GeLine2d::vertical(this->center(), curve2d);
        intersects = this->intersectWith(GeLineSeg2d(vertical, this->center()), tol);
        if (intersects.length() > 0) {
            if (this->isOn(intersects[0], tol) == true) {
                pointItselfs.append(intersects[0]);
            }
            if (curve2d.isOn(vertical, tol) == true) {
                pointOthers.append(vertical);
            }
        }

        double minDist = 0.0;
        for (int i = 0; i < pointItselfs.length(); i++) {
            for (int u = 0; u < pointOthers.length(); u++) {
                double dist = pointItselfs[i].distanceTo(pointOthers[u]);
                if (i == 0 && u == 0) {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                    continue;
                }
                if (dist < minDist)
                {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                }
            }
        }

    } while (false);

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    do
    {
        GePoint2dArray intersects = this->intersectWith(curve2d, tol);
        if (intersects.length() > 0) {
            pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
            closest.set(intersects[0].x, intersects[0].y);
            break;
        }

        GePoint2dArray pointItselfs, pointOthers;
        pointItselfs.append(this->closestPointTo(curve2d.pointOnLine(), tol));
        pointOthers.append(curve2d.pointOnLine());
        pointItselfs.append(this->startPoint());
        pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
        pointItselfs.append(this->endPoint());
        pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

        //获得圆弧和垂线是否存在交点
        GePoint2d vertical = GeLine2d::vertical(this->center(), curve2d);
        intersects = this->intersectWith(GeLineSeg2d(vertical, this->center()), tol);
        if (intersects.length() > 0) {
            if (this->isOn(intersects[0], tol) == true) {
                pointItselfs.append(intersects[0]);
            }
            if (curve2d.isOn(vertical, tol) == true) {
                pointOthers.append(vertical);
            }
        }

        double minDist = 0.0;
        for (int i = 0; i < pointItselfs.length(); i++) {
            for (int u = 0; u < pointOthers.length(); u++) {
                double dist = pointItselfs[i].distanceTo(pointOthers[u]);
                if (i == 0 && u == 0) {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                    continue;
                }
                if (dist < minDist)
                {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                }
            }
        }

    } while (false);

    return closest;
}
GePoint2d GeEllipArc2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const {
    return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeEllipArc2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
    GePoint2d closest;

    do
    {
        GePoint2dArray intersects = this->intersectWith(curve2d, tol);
        if (intersects.length() > 0) {
            pntOnOtherCrv.set(intersects[0].x, intersects[0].y);
            closest.set(intersects[0].x, intersects[0].y);
            break;
        }

        GePoint2dArray pointItselfs, pointOthers;
        pointItselfs.append(this->closestPointTo(curve2d.startPoint(), tol));
        pointOthers.append(curve2d.startPoint());
        pointItselfs.append(this->closestPointTo(curve2d.endPoint(), tol));
        pointOthers.append(curve2d.endPoint());
        pointItselfs.append(this->startPoint());
        pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
        pointItselfs.append(this->endPoint());
        pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

        //获得圆弧和垂线是否存在交点
        intersects = this->intersectWith(GeLineSeg2d(this->center(), curve2d.center()), tol);
        for (int i = 0; i < intersects.length(); i++) {
            pointItselfs.append(intersects[i]);
        }
        intersects = curve2d.intersectWith(GeLineSeg2d(this->center(), curve2d.center()), tol);
        for (int i = 0; i < intersects.length(); i++) {
            pointOthers.append(intersects[i]);
        }

        double minDist = 0.0;
        for (int i = 0; i < pointItselfs.length(); i++) {
            for (int u = 0; u < pointOthers.length(); u++) {
                double dist = pointItselfs[i].distanceTo(pointOthers[u]);
                if (i == 0 && u == 0) {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                    continue;
                }
                if (dist < minDist)
                {
                    minDist = dist;
                    closest.set(pointItselfs[i].x, pointItselfs[i].y);
                    pntOnOtherCrv.set(pointOthers[u].x, pointOthers[u].y);
                }
            }
        }

    } while (false);

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
    return atan2(y, x);
}
void GeEllipArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {
    return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeEllipArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
    return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend, GeContext::gTol);
}
void GeEllipArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

    if (abs(this->minorRadius() + distance) < tol.equalPoint() ||
        abs(this->majorRadius() + distance) < tol.equalPoint()) {
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
    if (abs(this->endAng() - this->startAng()) - PI * 2 < tol.equalPoint()) {
        return true;
    }
    return false;
}
