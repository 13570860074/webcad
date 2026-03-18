#include "GeLine2d.h"
#include "GeScale2d.h"
#include "GeLineSeg2d.h"
#include "GeRay2d.h"
#include "GeCircArc2d.h"
#include "GePointOnCurve2d.h"
#include "GeEntity2dMethods.h"
#include "GeInterval.h"
#include "GeBoundBlock2d.h"
#include "GeImpl.h"

namespace {
void selectClosestPair(const GePoint2dArray& pointItselfs, const GePoint2dArray& pointOthers, GePoint2d& closest, GePoint2d& pntOnOtherCrv)
{
	int pairCount = pointItselfs.length();
	if (pointOthers.length() < pairCount) {
		pairCount = pointOthers.length();
	}
	for (int i = 0; i < pairCount; ++i) {
		double dist = pointItselfs[i].distanceTo(pointOthers[i]);
		if (i == 0 || dist < closest.distanceTo(pntOnOtherCrv)) {
			closest.set(pointItselfs[i].x, pointItselfs[i].y);
			pntOnOtherCrv.set(pointOthers[i].x, pointOthers[i].y);
		}
	}
}
}




const GeLine2d GeLine2d::kXAxis = GeLine2d(GePoint2d(0, 0), GeVector2d(1, 0));
const GeLine2d GeLine2d::kYAxis = GeLine2d(GePoint2d(0, 0), GeVector2d(0, 1));



GeLine2d::GeLine2d()
{
	GE_IMP_MEMORY_ENTITY(GeLine2d);

	this->set(GePoint2d(0, 0), GeVector2d(1, 0));
}
GeLine2d::GeLine2d(const GeLine2d& source)
{
	GE_IMP_MEMORY_ENTITY(GeLine2d);

	this->set(GE_IMP_LINE2D(source.m_pImpl)->origin, GE_IMP_LINE2D(source.m_pImpl)->vector);
}
GeLine2d::GeLine2d(const GePoint2d& point, const GeVector2d& vect)
{
	GE_IMP_MEMORY_ENTITY(GeLine2d);

	this->set(point, vect);
}
GeLine2d::GeLine2d(const GePoint2d& point1, const GePoint2d& point2)
{
	GE_IMP_MEMORY_ENTITY(GeLine2d);

	GE_IMP_LINE2D(this->m_pImpl)->origin = point1;
	GE_IMP_LINE2D(this->m_pImpl)->vector = point2 - point1;
}
GeLine2d& GeLine2d::set(const GePoint2d& point, const GeVector2d& vect)
{
	GE_IMP_LINE2D(this->m_pImpl)->origin = point;
	GE_IMP_LINE2D(this->m_pImpl)->vector = vect;
	return *this;
}
GeLine2d& GeLine2d::set(const GePoint2d& point1, const GePoint2d& point2)
{
	GE_IMP_LINE2D(this->m_pImpl)->origin = point1;
	GE_IMP_LINE2D(this->m_pImpl)->vector = point2 - point1;
	return *this;
}
GeLine2d& GeLine2d::operator = (const GeLine2d& entity) {
	this->set(entity.pointOnLine(), entity.direction());
	return *this;
}

void GeLine2d::getInterval(GeInterval& range) const {
	range.set();
}

void GeLine2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const {
	range.set();
	startPoint = this->pointOnLine();
	endPoint = this->pointOnLine() + this->direction() * this->length();
}



bool GeLine2d::isKindOf(Ge::EntityId entType) const {
	if (entType == Ge::EntityId::kEntity2d || entType == Ge::EntityId::kCurve2d || entType == Ge::EntityId::kLinearEnt2d || entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeLine2d::type() const {
	return Ge::EntityId::kLine2d;
}
GeLine2d* GeLine2d::copy() const {
	GeLine2d* pLine = new GeLine2d();
	pLine->set(GE_IMP_LINE2D(this->m_pImpl)->origin, GE_IMP_LINE2D(this->m_pImpl)->vector);
	return pLine;
}
bool GeLine2d::operator == (const GeLine2d& entity) const {
	return this->isEqualTo(entity);
}
bool GeLine2d::operator != (const GeLine2d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeLine2d::isEqualTo(const GeLine2d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeLine2d::isEqualTo(const GeLine2d& entity, const GeTol& tol) const {
	return this->isColinearTo(entity, tol);
}
GeLine2d& GeLine2d::transformBy(const GeMatrix2d& xfm) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(p1, p2);
	return *this;
}
GeLine2d& GeLine2d::translateBy(const GeVector2d& translateVec) {
	this->set(this->pointOnLine() + translateVec, this->direction() * this->length());
	return *this;
}
GeLine2d& GeLine2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeLine2d& GeLine2d::rotateBy(double angle, const GePoint2d& wrtPoint) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.rotateBy(angle, wrtPoint);
	p2.rotateBy(angle, wrtPoint);
	this->set(p1, p2);
	return *this;
}
GeLine2d& GeLine2d::mirror(const GeLine2d& line) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.mirror(line);
	p2.mirror(line);
	this->set(p1, p2);
	return *this;
}
GeLine2d& GeLine2d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeLine2d& GeLine2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	p2.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	this->set(p1, p2);
	return *this;
}
bool GeLine2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeLine2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	GePoint2d vertical = GeLinearEnt2d::vertical(pnt, *this, tol);
	if (vertical.distanceTo(pnt) > tol.equalPoint())
	{
		return false;
	}
	return true;
}


double GeLine2d::length() const {
	return GE_IMP_LINE2D(this->m_pImpl)->vector.length();
}
double GeLine2d::length(double fromParam, double toParam)const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeLine2d::length(double fromParam, double toParam, double tol)const {
	return (toParam - fromParam) * this->length();
}
double GeLine2d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeLine2d::paramAtLength(double datumParam, double length, double tol) const {
	double param = 0.0;

	param = datumParam + length / this->length();

	return param;
}
double GeLine2d::area() const {
	return this->area(GeContext::gTol);
}
double GeLine2d::area(const GeTol& tol) const {
	return 0.0;
}
double GeLine2d::distanceTo(const GePoint2d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeLine2d::distanceTo(const GePoint2d& point, const GeTol& tol) const
{
	GePoint2d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeLine2d::distanceTo(const GeLine2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine2d::distanceTo(const GeLine2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLine2d::distanceTo(const GeLineSeg2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine2d::distanceTo(const GeLineSeg2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLine2d::distanceTo(const GeRay2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine2d::distanceTo(const GeRay2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLine2d::distanceTo(const GeCircArc2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLine2d::distanceTo(const GeCircArc2d& arc, const GeTol& tol) const {

	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint2d GeLine2d::closestPointTo(const GePoint2d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeLine2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
	GePoint2d point = GeLine2d::vertical(pnt, *this, tol);
	return point;
}
GePoint2d GeLine2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLine2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint2d closest;

	do
	{
		GePoint2d intersect;
		if (this->intersectWith(curve2d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y);
			closest.set(intersect.x, intersect.y);
			break;
		}

		pntOnOtherCrv.set(curve2d.pointOnLine().x, curve2d.pointOnLine().y);
		closest = this->closestPointTo(curve2d.pointOnLine(), tol);

	} while (false);

	return closest;
}
GePoint2d GeLine2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLine2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint2d closest;

	do
	{
		GePoint2d intersect;
		if (this->intersectWith(curve2d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y);
			closest.set(intersect.x, intersect.y);
			break;
		}

		GePoint2dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve2d.startPoint(), tol));
		pointOthers.append(curve2d.startPoint());
		pointItselfs.append(this->closestPointTo(curve2d.endPoint(), tol));
		pointOthers.append(curve2d.endPoint());

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint2d GeLine2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLine2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint2d closest;

	do
	{
		GePoint2d intersect;
		if (this->intersectWith(curve2d, intersect, tol) == true) {
			pntOnOtherCrv.set(intersect.x, intersect.y);
			closest.set(intersect.x, intersect.y);
			break;
		}

		GePoint2dArray pointItselfs, pointOthers;
		pointItselfs.append(this->closestPointTo(curve2d.pointOnLine(), tol));
		pointOthers.append(curve2d.pointOnLine());

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint2d GeLine2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLine2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint2d closest;

	do
	{
		//获得交点
		GePoint2dArray intersects = curve2d.intersectWith(*this, tol);
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

		//获得圆弧和垂线是否存在交点
		GePoint2d vertical = GeLine2d::vertical(curve2d.center(), *this);
		intersects = curve2d.intersectWith(GeLineSeg2d(vertical, curve2d.center()), tol);
		if (intersects.length() > 0) {
			pointItselfs.append(vertical);
			pointOthers.append(intersects[0]);
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
GECURVE2D_METHODS_GETCLOSESTPOINTTO(GeLine2d);
double GeLine2d::paramOf(const GePoint2d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeLine2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {
	if (this->isOn(pnt, tol) == false) {
		return 0.0;
	}

	GeVector2d axis = GE_IMP_LINE2D(this->m_pImpl)->vector;
	double lengthSqrd = axis.lengthSqrd();
	if (lengthSqrd <= tol.equalPoint() * tol.equalPoint()) {
		return 0.0;
	}

	GeVector2d offset = pnt - this->pointOnLine();
	return offset.dotProduct(axis) / lengthSqrd;
}
void GeLine2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeLine2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend, GeContext::gTol);
}
void GeLine2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

	GeVector2d axis = this->direction();
	axis.rotateBy(PI / 2.0);
	axis.normalize();

	GePoint2d origin = GE_IMP_LINE2D(this->m_pImpl)->origin;
	origin.translation(axis * distance);

	GeLine2d* line = new GeLine2d();
	line->set(origin, GE_IMP_LINE2D(this->m_pImpl)->vector);

	offsetCurveList.append(line);
}
bool GeLine2d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeLine2d::isClosed(const GeTol& tol) const {
	return false;
}
void GeLine2d::getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const {
	GePointOnCurve2d pointOnCurve(*this, param);
	GePoint2d point = pointOnCurve.point();
	piece1 = new GeRay2d(point, this->direction());
	piece2 = new GeRay2d(point, this->direction().negate());
}
bool GeLine2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	GeLine2d* line = new GeLine2d(*this);
	explodedCurves.append(line);
	newExplodedCurve.append(1);
	return true;
}
GeBoundBlock2d GeLine2d::boundBlock() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBoundedBelow() == false || range.isBoundedAbove() == false) {
		range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	}
	return this->boundBlock(range);
}
GeBoundBlock2d GeLine2d::boundBlock(const GeInterval& range) const {

	GeBoundBlock2d boundBlock;
	
	GePoint2d basePoint;
	basePoint = this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound();

	GePoint2d endPoint;
	endPoint = this->pointOnLine() + (this->direction() * this->length()) * range.upperBound();

	boundBlock.set(basePoint, endPoint);

	return boundBlock;
}
GeBoundBlock2d GeLine2d::orthoBoundBlock() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBoundedBelow() == false || range.isBoundedAbove() == false) {
		range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	}
	return this->orthoBoundBlock(range);
}
GeBoundBlock2d GeLine2d::orthoBoundBlock(const GeInterval& range) const {

	GeBoundBlock2d boundBlock;

	GePoint2dArray points;
	points.append(this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound());
	points.append(this->pointOnLine() + (this->direction() * this->length()) * range.upperBound());
	
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
bool GeLine2d::hasStartPoint(GePoint2d& startPoint) const {
	GeInterval range;
	GePoint2d endPoint;
	this->getInterval(range, startPoint, endPoint);
	return range.isBoundedBelow();
}
bool GeLine2d::hasEndPoint(GePoint2d& endPoint) const {
	GeInterval range;
	GePoint2d startPoint;
	this->getInterval(range, startPoint, endPoint);
	return range.isBoundedAbove();
}


bool GeLine2d::intersectWith(const GeLine2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLine2d::intersectWith(const GeLine2d& line, GePoint2d& intPnt, const GeTol& tol) const {
	return GeLinearEnt2d::intersectWith(line, intPnt, tol);
}
bool GeLine2d::intersectWith(const GeRay2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLine2d::intersectWith(const GeRay2d& line, GePoint2d& intPnt, const GeTol& tol) const {

	bool isValue = false;

	do
	{
		//获得和直线的延长交点
		GeLine2d line1(this->pointOnLine(), this->direction());
		GeLine2d line2(line.pointOnLine(), line.direction());
		if (line1.intersectWith(line2, intPnt, tol) == false) {
			break;
		}

		//判断延长交点是否在线段上,如果在射线上则表相交
		if (line.isOn(intPnt, tol) == true) {
			isValue = true;
		}

	} while (false);

	return isValue;
}
bool GeLine2d::intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLine2d::intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt, const GeTol& tol) const {
	bool isValue = false;

	do
	{
		//获得和直线的延长交点
		GeLine2d line1(this->pointOnLine(), this->direction());
		GeLine2d line2(line.pointOnLine(), line.direction());
		if (line1.intersectWith(line2, intPnt, tol) == false) {
			break;
		}

		//判断延长交点是否在线段上,如果在射线上则表相交
		if (line.isOn(intPnt, tol) == true) {
			isValue = true;
		}

	} while (false);

	return isValue;
}