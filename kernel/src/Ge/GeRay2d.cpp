#include "GeRay2d.h"
#include "GeScale2d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeCircArc2d.h"
#include "GePointOnCurve2d.h"
#include "GeInterval.h"
#include "GeBoundBlock2d.h"
#include "GeImpl.h"



GeRay2d::GeRay2d()
{
	GE_IMP_MEMORY_ENTITY(GeRay2d);
}
GeRay2d::GeRay2d(const GeRay2d& line)
{
	GE_IMP_MEMORY_ENTITY(GeRay2d);

	this->set(GE_IMP_RAY2D(line.m_pImpl)->origin, GE_IMP_RAY2D(line.m_pImpl)->vector);
}
GeRay2d::GeRay2d(const GePoint2d& point, const GeVector2d& vect)
{
	GE_IMP_MEMORY_ENTITY(GeRay2d);

	this->set(point, vect);
}
GeRay2d::GeRay2d(const GePoint2d& point1, const GePoint2d& point2)
{
	GE_IMP_MEMORY_ENTITY(GeRay2d);

	this->set(point1, point2);
}
GeRay2d& GeRay2d::set(const GePoint2d& point, const GeVector2d& vect)
{
	GE_IMP_RAY2D(this->m_pImpl)->origin = GePoint2d(point);
	GE_IMP_RAY2D(this->m_pImpl)->vector = GeVector2d(vect);
	return *this;
}
GeRay2d& GeRay2d::set(const GePoint2d& point1, const GePoint2d& point2)
{
	GE_IMP_RAY2D(this->m_pImpl)->origin = GePoint2d(point1);
	GE_IMP_RAY2D(this->m_pImpl)->vector = GeVector2d(point2 - point1);
	return *this;
}
GeRay2d& GeRay2d::operator =(const GeRay2d& line)
{
	this->set(GE_IMP_RAY2D(line.m_pImpl)->origin, GE_IMP_RAY2D(line.m_pImpl)->vector);
	return *this;
}



bool GeRay2d::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeRay2d::type() const {
	return Ge::EntityId::kRay2d;
}
GeRay2d* GeRay2d::copy() const {
	GeRay2d* pLine = new GeRay2d();
	pLine->set(GE_IMP_RAY2D(this->m_pImpl)->origin, GE_IMP_RAY2D(this->m_pImpl)->vector);
	return pLine;
}
bool GeRay2d::operator == (const GeRay2d& entity) const {
	return this->isEqualTo(entity);
}
bool GeRay2d::operator != (const GeRay2d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeRay2d::isEqualTo(const GeRay2d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeRay2d::isEqualTo(const GeRay2d& entity, const GeTol& tol) const {
	if (this->pointOnLine().isEqualTo(entity.pointOnLine(), tol) == false) {
		return false;
	}
	if (this->direction().isEqualTo(entity.direction(), tol) == false) {
		return false;
	}
	return true;
}
GeRay2d& GeRay2d::transformBy(const GeMatrix2d& xfm) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(p1, p2);
	return *this;
}
GeRay2d& GeRay2d::translateBy(const GeVector2d& translateVec) {
	this->set(this->pointOnLine() + translateVec, this->direction() * this->length());
	return *this;
}
GeRay2d& GeRay2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeRay2d& GeRay2d::rotateBy(double angle, const GePoint2d& wrtPoint) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.rotateBy(angle, wrtPoint);
	p2.rotateBy(angle, wrtPoint);
	this->set(p1, p2);
	return *this;
}
GeRay2d& GeRay2d::mirror(const GeLine2d& line) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.mirror(line);
	p2.mirror(line);
	this->set(p1, p2);
	return *this;
}
GeRay2d& GeRay2d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeRay2d& GeRay2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {
	GePoint2d p1 = this->pointOnLine();
	GePoint2d p2 = p1 + this->direction();
	p1.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	p2.scaleBy(GeScale2d(scaleFactor, scaleFactor), wrtPoint);
	this->set(p1, p2);
	return *this;
}
bool GeRay2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeRay2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	GeVector2d v1 = pnt - this->pointOnLine();
	GeVector2d v2 = this->direction();
	v1.normalize();
	v2.normalize();
	if (v1.isEqualTo(v2, tol) == false)
	{
		return false;
	}
	return true;
}



double GeRay2d::length() const {
	return GE_IMP_RAY2D(this->m_pImpl)->vector.length();
}
double GeRay2d::length(double fromParam, double toParam)const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeRay2d::length(double fromParam, double toParam, double tol)const {
	return (toParam - fromParam) * this->length();
}
double GeRay2d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeRay2d::paramAtLength(double datumParam, double length, double tol) const {
	double param = 0.0;

	param = datumParam + length / this->length();

	return param;
}
double GeRay2d::area() const {
	return this->area(GeContext::gTol);
}
double GeRay2d::area(const GeTol& tol) const {
	return 0.0;
}
double GeRay2d::distanceTo(const GePoint2d& point) const
{
	return this->distanceTo(point, GeContext::gTol);
}
double GeRay2d::distanceTo(const GePoint2d& point, const GeTol& tol) const
{
	GePoint2d closest = this->closestPointTo(point, tol);
	return closest.distanceTo(point);
}
double GeRay2d::distanceTo(const GeLine2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay2d::distanceTo(const GeLine2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeRay2d::distanceTo(const GeLineSeg2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay2d::distanceTo(const GeLineSeg2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeRay2d::distanceTo(const GeRay2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay2d::distanceTo(const GeRay2d& line, const GeTol& tol) const {
	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(line, closest, tol);
	return closest.distanceTo(itself);
}
double GeRay2d::distanceTo(const GeCircArc2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeRay2d::distanceTo(const GeCircArc2d& arc, const GeTol& tol) const {

	GePoint2d closest;
	GePoint2d itself = this->closestPointTo(arc, closest, tol);
	return closest.distanceTo(itself);
}
GePoint2d GeRay2d::closestPointTo(const GePoint2d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeRay2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
	GePoint2d point = GeRay2d::vertical(pnt, *this, tol);
	if (this->isOn(point) == true) {
		return point;
	}
	return this->pointOnLine();
}
GePoint2d GeRay2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeRay2d::closestPointTo(const GeLine2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve2d.closestPointTo(this->pointOnLine(), tol));

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
GePoint2d GeRay2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeRay2d::closestPointTo(const GeLineSeg2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve2d.closestPointTo(this->pointOnLine(), tol));

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
GePoint2d GeRay2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeRay2d::closestPointTo(const GeRay2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve2d.closestPointTo(this->pointOnLine(), tol));

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
GePoint2d GeRay2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeRay2d::closestPointTo(const GeCircArc2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
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
		pointItselfs.append(this->pointOnLine());
		pointOthers.append(curve2d.closestPointTo(this->pointOnLine(), tol));

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
GECURVE2D_METHODS_GETCLOSESTPOINTTO(GeRay2d);
double GeRay2d::paramOf(const GePoint2d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeRay2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {

	double param = 0.0;

	do
	{
		GeLine2d line(this->pointOnLine(), this->direction());
		if (line.isOn(pnt, tol) == false) {
			break;
		}

		GeVector2d direction = pnt - this->pointOnLine();
		direction.normalize();
		if (direction.isEqualTo(this->direction(), tol) == true) {
			param = (pnt.distanceTo(this->pointOnLine())) / this->length();
		}
		else {
			param = 0 - (pnt.distanceTo(this->pointOnLine())) / this->length();
		}
	} while (false);

	return param;
}
void GeRay2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend);
}
void GeRay2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, offsetCurveList, Ge::OffsetCrvExtType::kExtend, GeContext::gTol);
}
void GeRay2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

	GeVector2d axis = this->direction();
	axis.rotateBy(PI / 2.0);
	axis.normalize();

	GePoint2d origin = GE_IMP_LINE2D(this->m_pImpl)->origin;
	origin.translation(axis * distance);

	GeRay2d* line = new GeRay2d();
	line->set(origin, GE_IMP_LINE2D(this->m_pImpl)->vector);

	offsetCurveList.append(line);
}
bool GeRay2d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeRay2d::isClosed(const GeTol& tol) const {
	return false;
}
void GeRay2d::getSplitCurves(double param, GeCurve2d* piece1, GeCurve2d* piece2) const {
	GePointOnCurve2d pointOnCurve(*this, param);
	GePoint2d point = pointOnCurve.point();
	if (this->isOn(point) == false) {
		return;
	}
	piece1 = new GeLineSeg2d(this->pointOnLine(), point);
	piece2 = new GeRay2d(point, this->direction());
}
bool GeRay2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	return false;
}
GeBoundBlock2d GeRay2d::boundBlock() const {
	GeInterval range;
	range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	return this->boundBlock(range);
}
GeBoundBlock2d GeRay2d::boundBlock(const GeInterval& range) const {

	GeBoundBlock2d boundBlock;

	GePoint2d basePoint;
	basePoint = this->pointOnLine() + (this->direction() * this->length()) * range.lowerBound();

	GePoint2d endPoint;
	endPoint = this->pointOnLine() + (this->direction() * this->length()) * range.upperBound();

	boundBlock.set(basePoint, endPoint);

	return boundBlock;
}
GeBoundBlock2d GeRay2d::orthoBoundBlock() const {
	GeInterval range;
	range.set(this->paramOf(this->pointOnLine()), this->paramOf(this->pointOnLine() + this->direction() * this->length()));
	return this->orthoBoundBlock(range);
}
GeBoundBlock2d GeRay2d::orthoBoundBlock(const GeInterval& range) const {

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
bool GeRay2d::hasStartPoint(GePoint2d& startPoint) const {
	GePointOnCurve2d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(0.0);
	startPoint = pointOnCurve.point();
	return true;
}
bool GeRay2d::hasEndPoint(GePoint2d& endPoint) const {
	GePointOnCurve2d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(1.0);
	endPoint = pointOnCurve.point();
	return false;
}



bool GeRay2d::intersectWith(const GeLine2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeRay2d::intersectWith(const GeLine2d& line, GePoint2d& intPnt, const GeTol& tol) const {
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
bool GeRay2d::intersectWith(const GeRay2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeRay2d::intersectWith(const GeRay2d& line, GePoint2d& intPnt, const GeTol& tol) const {

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
bool GeRay2d::intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt) const {
	return this->intersectWith(line, intPnt, GeContext::gTol);
}
bool GeRay2d::intersectWith(const GeLineSeg2d& line, GePoint2d& intPnt, const GeTol& tol) const {
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