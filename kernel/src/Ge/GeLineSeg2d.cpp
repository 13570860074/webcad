#include "GeLineSeg2d.h"
#include "GePoint2d.h"
#include "GeVector2d.h"
#include "GeRay2d.h"
#include "GeLine2d.h"
#include "GeScale2d.h"
#include "GeCircArc2d.h"
#include "GePointOnCurve2d.h"
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



GeLineSeg2d::GeLineSeg2d()
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg2d);

	this->set(GePoint2d::kOrigin, GeVector2d::kXAxis);
}
GeLineSeg2d::GeLineSeg2d(const GeLineSeg2d& source)
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg2d);

	GE_IMP_LINESEG2D(this->m_pImpl)->origin = GE_IMP_LINESEG2D(source.m_pImpl)->origin;
	GE_IMP_LINESEG2D(this->m_pImpl)->vector = GE_IMP_LINESEG2D(source.m_pImpl)->vector;
}
GeLineSeg2d::GeLineSeg2d(const GePoint2d& point1, const GePoint2d& point2)
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg2d);

	GE_IMP_LINESEG2D(this->m_pImpl)->origin = point1;
	GE_IMP_LINESEG2D(this->m_pImpl)->vector = point2 - point1;
}
GeLineSeg2d::GeLineSeg2d(const GePoint2d& point, const GeVector2d& vect)
{
	GE_IMP_MEMORY_ENTITY(GeLineSeg2d);

	GE_IMP_LINESEG2D(this->m_pImpl)->origin = point;
	GE_IMP_LINESEG2D(this->m_pImpl)->vector = vect;
}


GeLineSeg2d& GeLineSeg2d::set(const GePoint2d& point, const GeVector2d& vect)
{
	GE_IMP_LINESEG2D(this->m_pImpl)->origin = point;
	GE_IMP_LINESEG2D(this->m_pImpl)->vector = vect;
	return *this;
}
GeLineSeg2d& GeLineSeg2d::set(const GePoint2d& point1, const GePoint2d& point2)
{
	GE_IMP_LINESEG2D(this->m_pImpl)->origin = point1;
	GE_IMP_LINESEG2D(this->m_pImpl)->vector = point2 - point1;
	return *this;
}


void GeLineSeg2d::getBisector(GeLine2d& line) const
{
	GeVector2d dire = this->direction();
	dire.rotateBy(PI / 2);
	GePoint2d point = this->midPoint();
	line.set(point, dire);
}


GePoint2d GeLineSeg2d::baryComb(double blendCoeff) const
{
	GeVector2d dire = this->direction();
	GePoint2d point = this->startPoint();
	point.x += blendCoeff * dire.x;
	point.y += blendCoeff * dire.y;
	return point;
}


GePoint2d GeLineSeg2d::startPoint() const
{
	return GE_IMP_LINESEG2D(this->m_pImpl)->origin;
}
GePoint2d GeLineSeg2d::midPoint() const
{
	GePoint2d point;
	point.x = GE_IMP_LINESEG2D(this->m_pImpl)->origin.x + GE_IMP_LINESEG2D(this->m_pImpl)->vector.x / 2;
	point.y = GE_IMP_LINESEG2D(this->m_pImpl)->origin.y + GE_IMP_LINESEG2D(this->m_pImpl)->vector.y / 2;
	return point;
}
GePoint2d GeLineSeg2d::endPoint() const
{
	GePoint2d point;
	point.x = GE_IMP_LINESEG2D(this->m_pImpl)->origin.x + GE_IMP_LINESEG2D(this->m_pImpl)->vector.x;
	point.y = GE_IMP_LINESEG2D(this->m_pImpl)->origin.y + GE_IMP_LINESEG2D(this->m_pImpl)->vector.y;
	return point;
}

GeLineSeg2d& GeLineSeg2d::operator =(const GeLineSeg2d& line)
{
	GE_IMP_LINESEG2D(this->m_pImpl)->origin = GE_IMP_LINESEG2D(line.m_pImpl)->origin;
	GE_IMP_LINESEG2D(this->m_pImpl)->vector = GE_IMP_LINESEG2D(line.m_pImpl)->vector;
	return *this;
}

void GeLineSeg2d::getInterval(GeInterval& range) const {
	range.set(0.0, 1.0);
}

void GeLineSeg2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const {
	range.set(0.0, 1.0);
	startPoint = this->startPoint();
	endPoint = this->endPoint();
}


bool GeLineSeg2d::isKindOf(Ge::EntityId entType) const {
	if (entType == Ge::EntityId::kEntity2d || entType == Ge::EntityId::kCurve2d || entType == Ge::EntityId::kLinearEnt2d || entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeLineSeg2d::type() const {
	return Ge::EntityId::kLineSeg2d;
}
GeLineSeg2d* GeLineSeg2d::copy() const {
	GeLineSeg2d* pLine = new GeLineSeg2d();
	pLine->set(GE_IMP_LINESEG2D(this->m_pImpl)->origin, GE_IMP_LINESEG2D(this->m_pImpl)->vector);
	return pLine;
}
bool GeLineSeg2d::operator == (const GeLineSeg2d& entity) const {
	return this->isEqualTo(entity);
}
bool GeLineSeg2d::operator != (const GeLineSeg2d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeLineSeg2d::isEqualTo(const GeLineSeg2d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeLineSeg2d::isEqualTo(const GeLineSeg2d& entity, const GeTol& tol) const {
	if (this->startPoint().isEqualTo(entity.startPoint(), tol) && this->endPoint().isEqualTo(entity.endPoint(), tol)) {
		return true;
	}
	if (this->startPoint().isEqualTo(entity.endPoint(), tol) && this->endPoint().isEqualTo(entity.startPoint(), tol)) {
		return true;
	}
	return false;
}
GeLineSeg2d& GeLineSeg2d::transformBy(const GeMatrix2d& xfm) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction() * this->length();
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(p1, p2);
	return *this;
}
GeLineSeg2d& GeLineSeg2d::translateBy(const GeVector2d& translateVec) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction() * this->length();
	p1 += translateVec;
	p2 += translateVec;
	this->set(p1, p2);
	return *this;
}
GeLineSeg2d& GeLineSeg2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeLineSeg2d& GeLineSeg2d::rotateBy(double angle, const GePoint2d& wrtPoint) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction() * this->length();
	p1.rotateBy(angle, wrtPoint);
	p2.rotateBy(angle, wrtPoint);
	this->set(p1, p2);
	return *this;
}
GeLineSeg2d& GeLineSeg2d::mirror(const GeLine2d& line) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction() * this->length();
	p1.mirror(line);
	p2.mirror(line);
	this->set(p1, p2);
	return *this;
}
GeLineSeg2d& GeLineSeg2d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeLineSeg2d& GeLineSeg2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction() * this->length();
	p1.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	p2.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	this->set(p1, p2);
	return *this;
}
bool GeLineSeg2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeLineSeg2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	GePoint2d start = this->startPoint();
	GeVector2d segment = this->endPoint() - start;
	GePoint2d projected = GeLinearEnt2d::vertical(pnt, *this, tol);
	if (projected.distanceTo(pnt) > tol.equalPoint())
	{
		return false;
	}

	GeVector2d offset = pnt - start;
	double projection = offset.dotProduct(segment);
	if (projection < -tol.equalPoint())
	{
		return false;
	}

	double lengthSqrd = segment.lengthSqrd();
	if (projection > lengthSqrd + tol.equalPoint())
	{
		return false;
	}
	return true;
}



double GeLineSeg2d::length() const {
	return this->startPoint().distanceTo(this->endPoint());
}
double GeLineSeg2d::length(double fromParam, double toParam)const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeLineSeg2d::length(double fromParam, double toParam, double tol)const {
	return (toParam - fromParam) * this->length();
}
double GeLineSeg2d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeLineSeg2d::paramAtLength(double datumParam, double length, double tol) const {
	double param = 0.0;

	param = datumParam + length / this->length();

	return param;
}
double GeLineSeg2d::area() const {
	return this->area(GeContext::gTol);
}
double GeLineSeg2d::area(const GeTol& tol) const {
	return 0.0;
}
double GeLineSeg2d::distanceTo(const GePoint2d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeLineSeg2d::distanceTo(const GePoint2d& point, const GeTol& tol) const
{
	GePoint2d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeLineSeg2d::distanceTo(const GeLine2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg2d::distanceTo(const GeLine2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLineSeg2d::distanceTo(const GeLineSeg2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg2d::distanceTo(const GeLineSeg2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLineSeg2d::distanceTo(const GeRay2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg2d::distanceTo(const GeRay2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeLineSeg2d::distanceTo(const GeCircArc2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeLineSeg2d::distanceTo(const GeCircArc2d& arc, const GeTol& tol) const {

	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint2d GeLineSeg2d::closestPointTo(const GePoint2d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeLineSeg2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
	GePoint2d point = GeLineSeg2d::vertical(pnt, *this, tol);
	if (this->isOn(point, tol) == true) {
		return point;
	}
	if (pnt.distanceTo(this->startPoint()) > pnt.distanceTo(this->endPoint())) {
		return this->endPoint();
	}
	return this->startPoint();
}
GePoint2d GeLineSeg2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLineSeg2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint2d GeLineSeg2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLineSeg2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint2d GeLineSeg2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLineSeg2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

		selectClosestPair(pointItselfs, pointOthers, closest, pntOnOtherCrv);

	} while (false);

	return closest;
}
GePoint2d GeLineSeg2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeLineSeg2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->startPoint());
		pointOthers.append(curve2d.closestPointTo(this->startPoint(), tol));
		pointItselfs.append(this->endPoint());
		pointOthers.append(curve2d.closestPointTo(this->endPoint(), tol));

		//获得圆弧和垂线是否存在交点
		GePoint2d vertical = GeLine2d::vertical(curve2d.center(), *this);
		intersects = curve2d.intersectWith(GeLineSeg2d(vertical, curve2d.center()), tol);
		if (intersects.length() > 0) {
			if (this->isOn(vertical, tol) == true) {
				pointItselfs.append(vertical);
			}
			if (curve2d.isOn(intersects[0], tol) == true) {
				pointOthers.append(intersects[0]);
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
GECURVE2D_METHODS_GETCLOSESTPOINTTO(GeLineSeg2d);
double GeLineSeg2d::paramOf(const GePoint2d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeLineSeg2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {
	if (this->isOn(pnt, tol) == false) {
		return 0.0;
	}

	GeVector2d axis = GE_IMP_LINESEG2D(this->m_pImpl)->vector;
	double lengthSqrd = axis.lengthSqrd();
	if (lengthSqrd <= tol.equalPoint() * tol.equalPoint()) {
		return 0.0;
	}

	GeVector2d offset = pnt - this->startPoint();
	return offset.dotProduct(axis) / lengthSqrd;
}
void GeLineSeg2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeLineSeg2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend, GeContext::gTol);
}
void GeLineSeg2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

	GeVector2d axis = this->direction();
	axis.rotateBy(PI / 2.0);
	axis.normalize();

	GePoint2d p1 = this->startPoint();
	GePoint2d p2 = this->endPoint();
	p1.translation(axis * distance);
	p2.translation(axis * distance);

	GeLineSeg2d* line = new GeLineSeg2d();
	line->set(p1, p2);

	offsetCurveList.append(line);
}
bool GeLineSeg2d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeLineSeg2d::isClosed(const GeTol& tol) const {
	return false;
}
void GeLineSeg2d::getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const {
	piece1 = NULL;
	piece2 = NULL;
	GePointOnCurve2d pointOnCurve(*this, param);
	GePoint2d point = pointOnCurve.point();
	if (this->isOn(point) == false) {
		return;
	}
	piece1 = new GeLineSeg2d(this->startPoint(), point);
	piece2 = new GeLineSeg2d(point, this->endPoint());
}
bool GeLineSeg2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	GeLineSeg2d* line = new GeLineSeg2d(*this);
	explodedCurves.append(line);
	newExplodedCurve.append(1);
	return true;
}
GeBoundBlock2d GeLineSeg2d::boundBlock() const {
	GeInterval range;
	this->getInterval(range);
	return this->boundBlock(range);
}
GeBoundBlock2d GeLineSeg2d::boundBlock(const GeInterval& range) const {

	GeBoundBlock2d boundBlock;

	GePoint2d basePoint;
	basePoint = this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound();

	GePoint2d endPoint;
	endPoint = this->pointOnLine() + (this->direction() * this->length()) * range.upperBound();

	boundBlock.set(basePoint, endPoint);

	return boundBlock;
}
GeBoundBlock2d GeLineSeg2d::orthoBoundBlock() const {
	GeInterval range;
	this->getInterval(range);
	return this->orthoBoundBlock(range);
}
GeBoundBlock2d GeLineSeg2d::orthoBoundBlock(const GeInterval& range) const {

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
bool GeLineSeg2d::hasStartPoint(GePoint2d& startPoint) const {
	GeInterval range;
	GePoint2d endPoint;
	this->getInterval(range, startPoint, endPoint);
	return range.isBoundedBelow();
}
bool GeLineSeg2d::hasEndPoint(GePoint2d& endPoint) const {
	GeInterval range;
	GePoint2d startPoint;
	this->getInterval(range, startPoint, endPoint);
	return range.isBoundedAbove();
}



bool GeLineSeg2d::intersectWith(const GeLine2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLineSeg2d::intersectWith(const GeLine2d& line, GePoint2d& intPnt, const GeTol& tol) const {
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
		if (line.isOn(intPnt, tol) == true &&
			this->isOn(intPnt, tol) == true) {
			isValue = true;
		}

	} while (false);

	return isValue;
}
bool GeLineSeg2d::intersectWith(const GeRay2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLineSeg2d::intersectWith(const GeRay2d& line, GePoint2d& intPnt, const GeTol& tol) const {

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
		if (line.isOn(intPnt, tol) == true &&
			this->isOn(intPnt, tol) == true) {
			isValue = true;
		}

	} while (false);

	return isValue;
}
bool GeLineSeg2d::intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeLineSeg2d::intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt, const GeTol& tol) const {
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
		if (line.isOn(intPnt, tol) == true &&
			this->isOn(intPnt, tol) == true) {
			isValue = true;
		}

	} while (false);

	return isValue;
}