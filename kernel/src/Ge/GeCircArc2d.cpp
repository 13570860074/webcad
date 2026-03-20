#include "GeCircArc2d.h"
#include "GeScale2d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeRay2d.h"
#include "GeCircArc2d.h"
#include "GePointOnCurve2d.h"
#include "GeInterval.h"
#include "GeBoundBlock2d.h"
#include "GeImpl.h"
#include <cmath>

namespace {
double normalize_angle_2pi(double angle)
{
	angle = std::fmod(angle, 2.0 * PI);
	if (angle < 0.0) {
		angle += 2.0 * PI;
	}
	return angle;
}

double periodic_angle_diff(double angle1, double angle2)
{
	double diff = normalize_angle_2pi(angle1) - normalize_angle_2pi(angle2);
	diff = std::fmod(diff + PI, 2.0 * PI);
	if (diff < 0.0) {
		diff += 2.0 * PI;
	}
	return diff - PI;
}

bool angle_in_ccw_sweep(double startAng, double endAng, double angle, double tol)
{
	startAng = normalize_angle_2pi(startAng);
	endAng = normalize_angle_2pi(endAng);
	angle = normalize_angle_2pi(angle);

	if (endAng < startAng) {
		endAng += 2.0 * PI;
	}
	if (angle < startAng) {
		angle += 2.0 * PI;
	}

	return angle >= startAng - tol && angle <= endAng + tol;
}
}



GeCircArc2d::GeCircArc2d()
{
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);
}
GeCircArc2d::GeCircArc2d(const GeCircArc2d& source)
{
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(source.center(), source.radius(), source.startAng(), source.endAng(), source.refVec(), source.isClockWise());
}
GeCircArc2d::GeCircArc2d(const GePoint2d& center, double radius)
{
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(center, radius);
}
GeCircArc2d::GeCircArc2d(const GePoint2d& cent, double radius, double startAngle, double endAngle) {
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(cent, radius, startAngle, endAngle);
}
GeCircArc2d::GeCircArc2d(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec) {
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(cent, radius, startAngle, endAngle, refVec);
}
GeCircArc2d::GeCircArc2d(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec, bool isClockWise) {
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(cent, radius, startAngle, endAngle, refVec, isClockWise);
}
GeCircArc2d::GeCircArc2d(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint)
{
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(startPoint, secondPoint, endPoint);
}
GeCircArc2d::GeCircArc2d(const GePoint2d& startPoint, const GePoint2d& endPoint, double bulge, bool bulgeFlag)
{
	GE_IMP_MEMORY_ENTITY(GeCircArc2d);

	this->set(startPoint, endPoint, bulge, bulgeFlag);
}

GePoint2dArray GeCircArc2d::toLineSegment(const GePoint2d& center, double _radius, int numSegments) {

	// 获得每段划过的角度
	double angleSegment = PI * 2.0 / numSegments;

	// 计算顶点
	GePoint2dArray vertexs;
	for (int i = 0; i < numSegments + 1; i++) {
		double angle = angleSegment * i;

		GePoint2d position(_radius, 0);
		double x = position.x * cos(angle) - position.y * sin(angle);
		double y = position.x * sin(angle) + position.y * cos(angle);
		position.set(x, y);

		position += center.asVector();
		vertexs.append(position);
	}

	return vertexs;

}
GePoint2dArray GeCircArc2d::toLineSegment(const GePoint2d& center, double _radius, double _startAng, double _endAng, int numSegments) {

	if (_startAng > _endAng) {
		_endAng = PI * 2 + _endAng;
	}

	// 获得要分的段数
	numSegments = int((_endAng - _startAng) / (PI * 2 / numSegments)) + 1;
	if(numSegments < 15){
		numSegments = 15;
	}

	// 获得每段划过的角度
	double angleSegment = (_endAng - _startAng) / numSegments;

	// 计算顶点
	GePoint2dArray vertexs;
	for (int i = 0; i < numSegments + 1; i++) {

		GePoint2d position;

		double angle = _startAng + angleSegment * (i);
		position.set(_radius, 0);
		double x = position.x * cos(angle) - position.y * sin(angle);
		double y = position.x * sin(angle) + position.y * cos(angle);
		position.set(x, y);

		position += center.asVector();
		vertexs.append(position);
	}

	return vertexs;
}
bool GeCircArc2d::pointToCenter(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint, GePoint2d& _center) {
	return GeCircArc2d::pointToCenter(startPoint, secondPoint, endPoint, _center, 0.00001);
}
bool GeCircArc2d::pointToCenter(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint, GePoint2d& _center, double tol) {
	double a = startPoint.x - secondPoint.x;
	double b = startPoint.y - secondPoint.y;
	double c = startPoint.x - endPoint.x;
	double d = startPoint.y - endPoint.y;
	double e = ((startPoint.x * startPoint.x - secondPoint.x * secondPoint.x) + (startPoint.y * startPoint.y - secondPoint.y * secondPoint.y)) / 2.0;
	double f = ((startPoint.x * startPoint.x - endPoint.x * endPoint.x) + (startPoint.y * startPoint.y - endPoint.y * endPoint.y)) / 2.0;
	double v = b * c - a * d;
	if (std::fabs(v) < tol) {
		return false;
	}
	_center.x = -(d * e - b * f) / v;
	_center.y = -(a * f - c * e) / v;
	return true;
}
bool GeCircArc2d::isClockWise(const GePoint2d& startPoint, const GePoint2d& secondPoint, const GePoint2d& endPoint)
{
	//double var = (secondPoint.x - startPoint.x) * (endPoint.y - secondPoint.y) - (secondPoint.y - startPoint.y) * (endPoint.x - secondPoint.x);
	double var = (secondPoint.x - startPoint.x) * (endPoint.y - startPoint.y) - (secondPoint.y - startPoint.y) * (endPoint.x - startPoint.x);
	if (var > 0)
	{
		return false;
	}
	return true;
}

Adesk::Boolean GeCircArc2d::intersectWith(const GeLinearEnt2d& line, int& intn, GePoint2d& p1, GePoint2d& p2, const GeTol& tol) const {
	GePoint2dArray pts = this->intersectWith(line, tol);
	intn = pts.length();
	if (intn == 0) {
		return false;
	}
	if (intn == 1) {
		p1 = pts[0];
	}
	else if (intn == 2) {
		p1 = pts[0];
		p2 = pts[1];
	}
	return true;
}
Adesk::Boolean GeCircArc2d::intersectWith(const GeCircArc2d& arc, int& intn, GePoint2d& p1, GePoint2d& p2, const GeTol& tol) const {
	GePoint2dArray pts = this->intersectWith(arc, tol);
	if (pts.length() == 0) {
		return false;
	}
	intn = pts.length();
	if (pts.length() == 1) {
		p1 = pts[0];
	}
	else if (pts.length() == 2) {
		p1 = pts[0];
		p2 = pts[1];
	}
	return true;
}
GePoint2dArray GeCircArc2d::intersectWith(const GeLinearEnt2d& line)const {
	return this->intersectWith(line, GeContext::gTol);
}
GePoint2dArray GeCircArc2d::intersectWith(const GeLinearEnt2d& line, const GeTol& tol)const {
	GePoint2dArray inters;

	do
	{
		//求圆心到直线的垂点
		GePoint2d vertical = GeLinearEnt2d::vertical(this->center(), line, tol);

		//获得垂点到圆心的距离
		double dist = this->center().distanceTo(vertical);
		if (std::fabs(dist - this->radius()) <= tol.equalPoint())
		{
			if (line.isOn(vertical, tol) == true && this->isOn(vertical, tol) == true) {
				inters.append(vertical);
			}
			break;
		}
		if (dist > this->radius() + tol.equalPoint())
		{
			break;
		}

		// 获得对边的长度
		double e = sqrt(this->radius() * this->radius() - dist * dist);

		GePoint2d point;
		GeVector2d direction = line.direction();

		point = vertical;
		point += (direction * e);
		if (line.isOn(point, tol) == true && this->isOn(point, tol)) {
			inters.append(point);
		}

		point = vertical;
		point += (direction * (0.0 - e));
		if (line.isOn(point, tol) == true && this->isOn(point, tol)) {
			bool isAppend = true;
			if (inters.length() > 0) {
				if (point.isEqualTo(inters[0], tol) == true) {
					isAppend = false;
				}
			}
			if (isAppend == true) {
				inters.append(point);
			}
		}

	} while (false);

	return inters;
}
GePoint2dArray GeCircArc2d::intersectWith(const GeCircArc2d& arc)const {
	return this->intersectWith(arc, GeContext::gTol);
}
GePoint2dArray GeCircArc2d::intersectWith(const GeCircArc2d& arc, const GeTol& tol)const {
	GePoint2dArray intersects;

	do
	{
		double dist = this->center().distanceTo(arc.center());
		double radiusThis = this->radius();
		double radiusOther = arc.radius();

		if (dist > radiusThis + radiusOther + tol.equalPoint()) {
			break;
		}
		if (dist < std::fabs(radiusThis - radiusOther) - tol.equalPoint()) {
			break;
		}
		if (dist < tol.equalPoint()) {
			break;
		}

		double a = (radiusThis * radiusThis - radiusOther * radiusOther + dist * dist) / (2.0 * dist);
		double h2 = radiusThis * radiusThis - a * a;
		if (h2 < -tol.equalPoint()) {
			break;
		}
		double h = std::sqrt(h2 < 0.0 ? 0.0 : h2);

		//获得圆的两个交点
		GePoint2dArray points;
		GeVector2d xAxis = arc.center() - this->center();
		xAxis.normalize();
		GeVector2d yAxis = xAxis;
		yAxis.rotateBy(PI / 2.0);
		GePoint2d basePoint = this->center() + xAxis * a;
		points.append(basePoint + yAxis * h);
		points.append(basePoint - yAxis * h);

		for (int i = 0; i < points.length(); i++) {
			if (this->isOn(points[i], tol) == true && arc.isOn(points[i], tol) == true) {
				bool isDuplicate = false;
				for (int k = 0; k < intersects.length(); ++k)
				{
					if (intersects[k].isEqualTo(points[i], tol))
					{
						isDuplicate = true;
						break;
					}
				}
				if (isDuplicate == false)
				{
					intersects.append(points[i]);
				}
			}
		}

	} while (false);

	return intersects;
}
bool GeCircArc2d::tangent(const GePoint2d& pnt, GeLine2d& line) const {
	return this->tangent(pnt, line, GeContext::gTol);
}
bool GeCircArc2d::tangent(const GePoint2d& pnt, GeLine2d& line, const GeTol& tol) const {
	bool isValue = false;

	do
	{

		if (pnt.isEqualTo(this->center(), tol) == true)
		{
			break;
		}

		line.set(pnt, this->center());
		line.rotateBy(PI / 2.0);

		isValue = true;

	} while (false);

	return isValue;
}
bool GeCircArc2d::isInside(const GePoint2d& pnt) const {
	return this->isInside(pnt, GeContext::gTol);
}
bool GeCircArc2d::isInside(const GePoint2d& pnt, const GeTol& tol) const {
	double dist = this->center().distanceTo(pnt);
	if (dist > this->radius() + tol.equalPoint()) {
		return false;
	}
	return true;
}


GePoint2d GeCircArc2d::center() const
{
	return GE_IMP_CIRCARC2D(this->m_pImpl)->center;
}
double GeCircArc2d::radius() const
{
	return GE_IMP_CIRCARC2D(this->m_pImpl)->radius;
}
GePoint2d GeCircArc2d::startPoint() const
{
	GePoint2d startPoint;
	this->hasStartPoint(startPoint);
	return startPoint;
}
GePoint2d GeCircArc2d::endPoint() const
{
	GePoint2d endPoint;
	this->hasEndPoint(endPoint);
	return endPoint;
}
double GeCircArc2d::startAng() const
{
	return GE_IMP_CIRCARC2D(this->m_pImpl)->startAngle;
}
double GeCircArc2d::endAng() const
{
	return GE_IMP_CIRCARC2D(this->m_pImpl)->endAngle;
}
GeVector2d GeCircArc2d::refVec() const {
	return GE_IMP_CIRCARC2D(this->m_pImpl)->refVec;
}
bool GeCircArc2d::isClockWise() const {
	return GE_IMP_CIRCARC2D(this->m_pImpl)->isClockWise;
}


GeCircArc2d& GeCircArc2d::setCenter(const GePoint2d& center)
{
	GE_IMP_CIRCARC2D(this->m_pImpl)->center.set(center.x, center.y);
	return *this;
}
GeCircArc2d& GeCircArc2d::setRadius(double radius)
{
	GE_IMP_CIRCARC2D(this->m_pImpl)->radius = radius;
	return *this;
}
GeCircArc2d& GeCircArc2d::setAngles(double startAng, double endAng)
{
	double sweep = endAng - startAng;
	double absSweep = std::fabs(sweep);
	double start = std::fmod(startAng, 2.0 * PI);
	if (start < 0.0) {
		start += 2.0 * PI;
	}
	double end = std::fmod(endAng, 2.0 * PI);
	if (end < 0.0) {
		end += 2.0 * PI;
	}
	if (absSweep >= 2.0 * PI - GeContext::gTol.equalPoint())
	{
		end = start + (sweep >= 0.0 ? 2.0 * PI : -2.0 * PI);
	}

	GE_IMP_CIRCARC2D(this->m_pImpl)->startAngle = start;
	GE_IMP_CIRCARC2D(this->m_pImpl)->endAngle = end;
	return *this;
}

GeCircArc2d& GeCircArc2d::set(const GePoint2d& cent, double radius) {
	this->set(cent, radius, 0.0, PI * 2, GeVector2d::kXAxis, false);
	return *this;
}
GeCircArc2d& GeCircArc2d::set(const GePoint2d& cent, double radius, double ang1, double ang2) {
	this->set(cent, radius, ang1, ang2, GeVector2d::kXAxis, false);
	return *this;
}
GeCircArc2d& GeCircArc2d::set(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec) {
	this->set(cent, radius, startAngle, endAngle, refVec, false);
	return *this;
}
GeCircArc2d& GeCircArc2d::set(const GePoint2d& cent, double radius, double startAngle, double endAngle, const GeVector2d& refVec, bool isClockWise) {
	this->setCenter(cent);
	this->setRadius(radius);
	this->setAngles(startAngle, endAngle);
	GE_IMP_CIRCARC2D(this->m_pImpl)->refVec.set(refVec.x, refVec.y);
	GE_IMP_CIRCARC2D(this->m_pImpl)->refVec.normalize();
	GE_IMP_CIRCARC2D(this->m_pImpl)->isClockWise = isClockWise;
	return *this;
}
GeCircArc2d& GeCircArc2d::set(const GePoint2d& startPoint, const GePoint2d& pnt, const GePoint2d& endPoint) {

	this->setRadius(0.0);

	GePoint2d center;
	if (GeCircArc2d::pointToCenter(startPoint, pnt, endPoint, center) == false) {
		return *this;
	}

	double radius = sqrt((center.x - startPoint.x) * (center.x - startPoint.x) + (center.y - startPoint.y) * (center.y - startPoint.y));
	if (radius < 0.000002) {
		return *this;
	}

	GE_IMP_CIRCARC2D(this->m_pImpl)->center = center;
	GE_IMP_CIRCARC2D(this->m_pImpl)->refVec = (startPoint - center).normal();
	GE_IMP_CIRCARC2D(this->m_pImpl)->startAngle = 0.0;
	GE_IMP_CIRCARC2D(this->m_pImpl)->endAngle = GE_IMP_CIRCARC2D(this->m_pImpl)->refVec.angleToCCW(GeVector2d(endPoint - center).normal());
	GE_IMP_CIRCARC2D(this->m_pImpl)->isClockWise = GeCircArc2d::isClockWise(startPoint, pnt, endPoint);

	return *this;
}
GeCircArc2d& GeCircArc2d::set(const GePoint2d& startPoint, const GePoint2d& endPoint, double bulge) {
	return this->set(startPoint, endPoint, bulge, true);
}
GeCircArc2d& GeCircArc2d::set(const GePoint2d& startPoint, const GePoint2d& endPoint, double bulge, bool bulgeFlag) {

	GePoint2d midPoint = startPoint + (endPoint - startPoint) / 2;
	double dist = startPoint.distanceTo(midPoint);
	double angle = atan(bulge) * 4;
	double adjacent = dist / tan(angle / 2.0);

	GeVector2d refVec = startPoint - midPoint;
	refVec.normalize();
	refVec.rotateBy(0 - PI / 2.0);

	GePoint2d center = midPoint;
	center += (refVec * adjacent);

	double radius = center.distanceTo(startPoint);

	GE_IMP_CIRCARC2D(this->m_pImpl)->center = center;
	GE_IMP_CIRCARC2D(this->m_pImpl)->refVec = (startPoint - center).normal();
	GE_IMP_CIRCARC2D(this->m_pImpl)->startAngle = 0.0;
	GE_IMP_CIRCARC2D(this->m_pImpl)->endAngle = GE_IMP_CIRCARC2D(this->m_pImpl)->refVec.angleToCCW(GeVector2d(endPoint - center).normal());
	if (bulge > 0) {
		GE_IMP_CIRCARC2D(this->m_pImpl)->isClockWise = false;
	}
	else {
		GE_IMP_CIRCARC2D(this->m_pImpl)->isClockWise = true;
	}

	return *this;
}

GeCircArc2d& GeCircArc2d::operator = (const GeCircArc2d& arc) {
	this->set(arc.center(), arc.radius(), arc.startAng(), arc.endAng(), arc.refVec(), arc.isClockWise());
	return *this;
}


bool GeCircArc2d::isKindOf(Ge::EntityId entType) const {
	if (entType == Ge::kEntity2d || entType == Ge::kCurve2d || entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeCircArc2d::type() const {
	return Ge::EntityId::kCircArc2d;
}
GeCircArc2d* GeCircArc2d::copy() const {
	GeCircArc2d* arc = new GeCircArc2d();
	arc->set(
		GE_IMP_CIRCARC2D(this->m_pImpl)->center,
		GE_IMP_CIRCARC2D(this->m_pImpl)->radius,
		GE_IMP_CIRCARC2D(this->m_pImpl)->startAngle,
		GE_IMP_CIRCARC2D(this->m_pImpl)->endAngle,
		GE_IMP_CIRCARC2D(this->m_pImpl)->refVec,
		GE_IMP_CIRCARC2D(this->m_pImpl)->isClockWise);
	return arc;
}
bool GeCircArc2d::operator == (const GeCircArc2d& entity) const {
	return this->isEqualTo(entity);
}
bool GeCircArc2d::operator != (const GeCircArc2d& entity) const {
	return !(*this == entity);
}
bool GeCircArc2d::isEqualTo(const GeCircArc2d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeCircArc2d::isEqualTo(const GeCircArc2d& entity, const GeTol& tol) const {
	if (this->center().isEqualTo(entity.center(), tol) == false) {
		return false;
	}
	if (std::fabs(this->radius() - entity.radius()) > tol.equalPoint()) {
		return false;
	}
	bool thisClosed = this->isClosed(tol);
	bool entityClosed = entity.isClosed(tol);
	if (thisClosed != entityClosed) {
		return false;
	}
	if (thisClosed == false) {
		if (std::fabs(periodic_angle_diff(this->startAng(), entity.startAng())) > tol.equalPoint()) {
			return false;
		}
		if (std::fabs(periodic_angle_diff(this->endAng(), entity.endAng())) > tol.equalPoint()) {
			return false;
		}
	}
	if (this->refVec().isEqualTo(entity.refVec(), tol) == false) {
		return false;
	}
	if (this->isClockWise() != entity.isClockWise()) {
		return false;
	}
	return true;
}
GeCircArc2d& GeCircArc2d::transformBy(const GeMatrix2d& xfm) {

	GePoint2d center = this->center();
	GePoint2d startPoint = this->startPoint();
	GePoint2d endPoint = this->endPoint();
	center.transformBy(xfm);
	startPoint.transformBy(xfm);
	endPoint.transformBy(xfm);

	this->setCenter(center);
	this->setRadius(this->center().distanceTo(startPoint));

	GeVector2d vector = startPoint - this->center();
	vector.normalize();
	double startAngle = this->refVec().angleToCCW(vector);

	vector = endPoint - this->center();
	vector.normalize();
	double endAngle = this->refVec().angleToCCW(vector);

	this->setAngles(startAngle, endAngle);

	return *this;
}
GeCircArc2d& GeCircArc2d::translateBy(const GeVector2d& translateVec) {
	this->setCenter(this->center().translation(translateVec));
	return *this;
}
GeCircArc2d& GeCircArc2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeCircArc2d& GeCircArc2d::rotateBy(double angle, const GePoint2d& wrtPoint) {
	GeMatrix2d mat;
	mat.setToRotation(angle, wrtPoint);
	this->transformBy(mat);
	return *this;
}
GeCircArc2d& GeCircArc2d::mirror(const GeLine2d& line) {

	this->setCenter(this->center().mirror(line));

	GePoint2d startPoint = this->startPoint();
	startPoint.mirror(line);
	GeVector2d vector = startPoint - this->center();
	vector.normalize();
	double startAngle = this->refVec().angleToCCW(vector);

	GePoint2d endPoint = this->endPoint();
	endPoint.mirror(line);
	vector = endPoint - this->center();
	vector.normalize();
	double endAngle = this->refVec().angleToCCW(vector);

	this->setAngles(startAngle, endAngle);

	return *this;
}
GeCircArc2d& GeCircArc2d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeCircArc2d& GeCircArc2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {
	GePoint2d center = this->center();
	center.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	this->setCenter(center);
	this->setRadius(this->radius() * scaleFactor);
	return *this;
}
bool GeCircArc2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeCircArc2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	if (std::fabs(this->center().distanceTo(pnt) - this->radius()) > tol.equalPoint()) {
		return false;
	}

	if (this->isClosed(tol) == true) {
		return true;
	}

	double start = this->startAng();
	double end = this->endAng();
	if (std::fabs(end - start) <= tol.equalPoint()) {
		return false;
	}

	GeVector2d vector = pnt - this->center();
	double angle = this->refVec().angleToCCW(vector);
	if (this->isClockWise() == true) {
		return angle_in_ccw_sweep(-start, -end, -angle, tol.equalPoint());
	}
	return angle_in_ccw_sweep(start, end, angle, tol.equalPoint());
}
void GeCircArc2d::getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const {
	piece1 = NULL;
	piece2 = NULL;

	GePointOnCurve2d pointOnCurve(*this, param);
	GePoint2d point = pointOnCurve.point();
	if (this->isOn(point) == false) {
		return;
	}

	GeVector2d vector = point - this->center();
	double angle = this->refVec().angleToCCW(vector);

	piece1 = new GeCircArc2d(this->center(), this->radius(), this->startAng(), angle, this->refVec(), this->isClockWise());
	piece2 = new GeCircArc2d(this->center(), this->radius(), angle, this->endAng(), this->refVec(), this->isClockWise());
}
bool GeCircArc2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	GeCircArc2d* arc = new GeCircArc2d(*this);
	explodedCurves.append(arc);
	newExplodedCurve.append(1);
	return true;
}
GeBoundBlock2d GeCircArc2d::boundBlock() const {
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
GeBoundBlock2d GeCircArc2d::boundBlock(const GeInterval& range) const {

	GeBoundBlock2d boundBlock;
	if (std::fabs(range.upperBound() - range.lowerBound() - PI * 2.0) < range.tolerance()) {

		GePoint2d basePoint(this->center());
		basePoint.x -= this->radius();
		basePoint.y -= this->radius();
		boundBlock.set(basePoint, GeVector2d(this->radius() * 2, 0.0), GeVector2d(0.0, this->radius() * 2));
	}
	else {
		GePointOnCurve2d pointOnCurve;
		pointOnCurve.setCurve(*this);

		//获得起点
		pointOnCurve.setParameter(range.lowerBound());
		GePoint2d startPoint = pointOnCurve.point();

		//获得终点
		pointOnCurve.setParameter(range.upperBound());
		GePoint2d endPoint = pointOnCurve.point();

		//获得起点到终点的长度
		double dist = startPoint.distanceTo(endPoint);

		//获得圆心到线的垂点
		GePoint2d vertical = GeLine2d::vertical(this->center(), GeLine2d(startPoint, endPoint));

		//获得基点
		GePoint2d basePoint = vertical;
		basePoint.translation((dist / 2.0) * (endPoint - startPoint).negate());

		//获得方向1
		GeVector2d v1 = (vertical - basePoint).negate() * dist;

		//获得方向2
		GeVector2d v2 = (vertical - this->center()).negate() * (this->radius() - this->center().distanceTo(vertical));

		boundBlock.set(basePoint, v1, v2);
	}

	return boundBlock;
}
GeBoundBlock2d GeCircArc2d::orthoBoundBlock() const {
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
GeBoundBlock2d GeCircArc2d::orthoBoundBlock(const GeInterval& range) const {

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
	GePoint2d point = GePoint2d(this->center().x + this->radius(), this->center().y);
	if (this->isOn(point) == true) {
		points.append(point);
	}
	point = GePoint2d(this->center().x, this->center().y + this->radius());
	if (this->isOn(point) == true) {
		points.append(point);
	}
	point = GePoint2d(this->center().x - this->radius(), this->center().y);
	if (this->isOn(point) == true) {
		points.append(point);
	}
	point = GePoint2d(this->center().x, this->center().y - this->radius());
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
bool GeCircArc2d::hasStartPoint(GePoint2d& startPoint) const {
	startPoint = GePoint2d(this->center().x, this->center().y);
	startPoint += (this->refVec() * this->radius());
	startPoint.rotateBy(this->startAng(), this->center());
	return true;
}
bool GeCircArc2d::hasEndPoint(GePoint2d& endPoint) const {
	endPoint = GePoint2d(this->center().x, this->center().y);
	endPoint += (this->refVec() * this->radius());
	endPoint.rotateBy(this->endAng(), this->center());
	return true;
}



double GeCircArc2d::length() const {
	return this->length(this->startAng(), this->endAng());
}
double GeCircArc2d::length(double fromParam, double toParam)const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeCircArc2d::length(double fromParam, double toParam, double tol)const {
	fromParam = normalize_angle_2pi(fromParam);
	toParam = normalize_angle_2pi(toParam);
	if (this->isClockWise() == false) {
		if (toParam < fromParam) {
			toParam += 2 * PI;
		}
		return this->radius() * (toParam - fromParam);
	}
	if (fromParam < toParam) {
		fromParam += 2 * PI;
	}
	return this->radius() * (fromParam - toParam);
}
double GeCircArc2d::area() const {
	return this->area(GeContext::gTol);
}
double GeCircArc2d::area(const GeTol& tol) const {
	if (this->isClosed() == false) {
		return 0.0;
	}
	return PI * this->radius() * this->radius();
}
double GeCircArc2d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeCircArc2d::paramAtLength(double datumParam, double length, double tol) const {
	if (std::fabs(this->radius()) <= tol) {
		return datumParam;
	}

	double delta = length / this->radius();
	double param = datumParam;
	if (this->isClockWise() == true) {
		param -= delta;
	}
	else {
		param += delta;
	}
	return normalize_angle_2pi(param);
}
double GeCircArc2d::distanceTo(const GePoint2d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeCircArc2d::distanceTo(const GePoint2d& point, const GeTol& tol) const
{
	GePoint2d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeCircArc2d::distanceTo(const GeLine2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc2d::distanceTo(const GeLine2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeCircArc2d::distanceTo(const GeLineSeg2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc2d::distanceTo(const GeLineSeg2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeCircArc2d::distanceTo(const GeRay2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc2d::distanceTo(const GeRay2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeCircArc2d::distanceTo(const GeCircArc2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCircArc2d::distanceTo(const GeCircArc2d& arc, const GeTol& tol) const {

	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint2d GeCircArc2d::closestPointTo(const GePoint2d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeCircArc2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
	GePoint2d closest;

	do
	{
		GePoint2dArray intersects;
		if (pnt.isEqualTo(this->center(), tol) == false) {
			intersects = this->intersectWith(GeRay2d(this->center(), pnt), tol);
		}
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
GePoint2d GeCircArc2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeCircArc2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
GePoint2d GeCircArc2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeCircArc2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
GePoint2d GeCircArc2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeCircArc2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
GePoint2d GeCircArc2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeCircArc2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
GECURVE2D_METHODS_GETCLOSESTPOINTTO(GeCircArc2d);
double GeCircArc2d::paramOf(const GePoint2d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCircArc2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {

	GeCircArc2d arc(this->center(), this->radius());
	if (arc.isOn(pnt, tol) == false) {
		return 0.0;
	}

	GeVector2d vector = pnt - this->center();
	return normalize_angle_2pi(this->refVec().angleToCCW(vector));
}
void GeCircArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeCircArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend, GeContext::gTol);
}
void GeCircArc2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

	double newRadius = this->radius() + distance;
	if (newRadius <= tol.equalPoint()) {
		return;
	}

	GeCircArc2d* circArc = new GeCircArc2d();
	circArc->set(this->center(), newRadius, this->startAng(), this->endAng(), this->refVec(), this->isClockWise());

	offsetCurveList.append(circArc);
}
bool GeCircArc2d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeCircArc2d::isClosed(const GeTol& tol) const {
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