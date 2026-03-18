#include "GeCurve2d.h"
#include "GePointOnCurve2d.h"
#include "GeBoundBlock2d.h"
#include "GeInterval.h"

namespace {
void curve2d_sample_points(const GeCurve2d& curve, double fromParam, double toParam, int sampleCount, GePoint2dArray& pointArray)
{
	if (sampleCount <= 0) {
		return;
	}

	for (int i = 0; i < sampleCount; i++) {
		double ratio = 0.0;
		if (sampleCount > 1) {
			ratio = double(i) / double(sampleCount - 1);
		}
		double param = fromParam + (toParam - fromParam) * ratio;
		pointArray.append(curve.evalPoint(param));
	}
}
}

GeCurve2d::GeCurve2d()
{

}
GeCurve2d::GeCurve2d(const GeCurve2d& source)
{

}

void GeCurve2d::getInterval(GeInterval& range) const {
	range.set();
}
void GeCurve2d::getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const {
	this->getInterval(range);
	startPoint = GePoint2d::kOrigin;
	endPoint = GePoint2d::kOrigin;
}
GeCurve2d& GeCurve2d::reverseParam() {
	return *this;
}
GeCurve2d& GeCurve2d::setInterval() {
	return *this;
}
bool GeCurve2d::setInterval(const GeInterval& range) {
	return false;
}

double GeCurve2d::distanceTo(const GePoint2d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeCurve2d::distanceTo(const GePoint2d& pnt, const GeTol& tol) const {
	return this->closestPointTo(pnt, tol).distanceTo(pnt);
}
double GeCurve2d::distanceTo(const GeCurve2d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCurve2d::distanceTo(const GeCurve2d& entity, const GeTol& tol) const {
	GePoint2d pntOnOtherCrv;
	return this->closestPointTo(entity, pntOnOtherCrv, tol).distanceTo(pntOnOtherCrv);
}
GePoint2d GeCurve2d::closestPointTo(const GePoint2d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint2d GeCurve2d::closestPointTo(const GePoint2d& pnt, const GeTol& tol) const {
	return GePoint2d::kOrigin;
}
GePoint2d GeCurve2d::closestPointTo(const GePoint2d& pnt, double& param, const GeInterval* range, const GeTol& tol) const {
	GePoint2d closest = this->closestPointTo(pnt, tol);
	param = this->paramOf(closest, tol);
	if (range == NULL || range->isBounded() == false || range->contains(param) == true) {
		return closest;
	}

	bool hasCandidate = false;
	GePoint2d bestPoint;
	double bestParam = param;
	double bestDist = 0.0;
	if (range->isBoundedBelow() == true) {
		GePoint2d point = this->evalPoint(range->lowerBound());
		double dist = point.distanceTo(pnt);
		hasCandidate = true;
		bestPoint = point;
		bestParam = range->lowerBound();
		bestDist = dist;
	}
	if (range->isBoundedAbove() == true) {
		GePoint2d point = this->evalPoint(range->upperBound());
		double dist = point.distanceTo(pnt);
		if (hasCandidate == false || dist < bestDist) {
			hasCandidate = true;
			bestPoint = point;
			bestParam = range->upperBound();
			bestDist = dist;
		}
	}
	if (hasCandidate == true) {
		param = bestParam;
		return bestPoint;
	}
	return closest;
}
GePoint2d GeCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint2d GeCurve2d::closestPointTo(const GeCurve2d& curve2d, GePoint2d& pntOnOtherCrv, const GeTol& tol) const {
	GeInterval thisRange;
	GeInterval otherRange;
	this->getInterval(thisRange);
	curve2d.getInterval(otherRange);
	if (thisRange.isBounded() == false || otherRange.isBounded() == false) {
		pntOnOtherCrv = GePoint2d::kOrigin;
		return GePoint2d::kOrigin;
	}

	GePoint2d bestPoint;
	GePoint2d bestOtherPoint;
	double bestDist = 0.0;
	bool hasCandidate = false;

	GePoint2dArray thisPoints;
	curve2d_sample_points(*this, thisRange.lowerBound(), thisRange.upperBound(), 33, thisPoints);
	for (int i = 0; i < thisPoints.length(); i++) {
		GePoint2d otherPoint = curve2d.closestPointTo(thisPoints[i], tol);
		double dist = thisPoints[i].distanceTo(otherPoint);
		if (hasCandidate == false || dist < bestDist) {
			hasCandidate = true;
			bestDist = dist;
			bestPoint = thisPoints[i];
			bestOtherPoint = otherPoint;
		}
	}

	GePoint2dArray otherPoints;
	curve2d_sample_points(curve2d, otherRange.lowerBound(), otherRange.upperBound(), 33, otherPoints);
	for (int i = 0; i < otherPoints.length(); i++) {
		GePoint2d thisPoint = this->closestPointTo(otherPoints[i], tol);
		double dist = thisPoint.distanceTo(otherPoints[i]);
		if (hasCandidate == false || dist < bestDist) {
			hasCandidate = true;
			bestDist = dist;
			bestPoint = thisPoint;
			bestOtherPoint = otherPoints[i];
		}
	}

	if (hasCandidate == false) {
		pntOnOtherCrv = GePoint2d::kOrigin;
		return GePoint2d::kOrigin;
	}

	pntOnOtherCrv = bestOtherPoint;
	return bestPoint;
}
bool GeCurve2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	double param;
	return this->isOn(pnt, param, tol);
}
void GeCurve2d::getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv) const {
	return this->getClosestPointTo(pnt, pntOnCrv, GeContext::gTol);
}
void GeCurve2d::getClosestPointTo(const GePoint2d& pnt, GePointOnCurve2d& pntOnCrv, const GeTol& tol) const {
	GePoint2d point = this->closestPointTo(pnt, tol);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(point, tol));
}
void GeCurve2d::getClosestPointTo(const GeCurve2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv) const {
	return this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCurve2d::getClosestPointTo(const GeCurve2d& curve2d, GePointOnCurve2d& pntOnThisCrv, GePointOnCurve2d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint2d pointOnOther;
	GePoint2d pointOnThis = this->closestPointTo(curve2d, pointOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pointOnThis, tol));
	pntOnOtherCrv.setCurve(curve2d);
	pntOnOtherCrv.setParameter(curve2d.paramOf(pointOnOther, tol));
}
bool GeCurve2d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeCurve2d::isClosed(const GeTol& tol) const {
	GePoint2d startPoint;
	GePoint2d endPoint;
	if (this->hasStartPoint(startPoint) == false || this->hasEndPoint(endPoint) == false) {
		return false;
	}
	return startPoint.isEqualTo(endPoint, tol);
}
double GeCurve2d::paramOf(const GePoint2d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCurve2d::paramOf(const GePoint2d& pnt, const GeTol& tol) const {
	return 0.0;
}
void GeCurve2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList) const {

}
void GeCurve2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {

}
void GeCurve2d::getTrimmedOffset(double distance, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

}
double GeCurve2d::length() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return 0.0;
	}
	return this->length(range.lowerBound(), range.upperBound(), GeContext::gTol.equalPoint());
}
double GeCurve2d::length(double fromParam, double toParam) const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeCurve2d::length(double fromParam, double toParam, double tol) const {
	GePoint2dArray pointArray;
	int sampleCount = 33;
	if (tol > 0.0) {
		double span = toParam - fromParam;
		if (span < 0.0) {
			span = 0.0 - span;
		}
		int dynamicCount = int(span / tol) + 1;
		if (dynamicCount > sampleCount) {
			sampleCount = dynamicCount;
		}
	}
	curve2d_sample_points(*this, fromParam, toParam, sampleCount, pointArray);
	if (pointArray.length() == 0) {
		return 0.0;
	}

	double totalLength = 0.0;
	for (int i = 1; i < pointArray.length(); i++) {
		totalLength += pointArray[i - 1].distanceTo(pointArray[i]);
	}
	return totalLength;
}
double GeCurve2d::area() const {
	return this->area(GeContext::gTol);
}
double GeCurve2d::area(const GeTol& tol) const {
	if (this->isClosed(tol) == false) {
		return 0.0;
	}

	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return 0.0;
	}

	GePoint2dArray pointArray;
	curve2d_sample_points(*this, range.lowerBound(), range.upperBound(), 65, pointArray);
	if (pointArray.length() < 3) {
		return 0.0;
	}

	double areaValue = 0.0;
	for (int i = 0; i < pointArray.length(); i++) {
		const GePoint2d& point1 = pointArray[i];
		const GePoint2d& point2 = pointArray[(i + 1) % pointArray.length()];
		areaValue += point1.x * point2.y - point2.x * point1.y;
	}
	if (areaValue < 0.0) {
		areaValue = 0.0 - areaValue;
	}
	return areaValue * 0.5;
}
void GeCurve2d::getSplitCurves(double param, GeCurve2d*& piece1, GeCurve2d*& piece2) const {
	piece1 = NULL;
	piece2 = NULL;
	return;
}
bool GeCurve2d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	return false;
}
double GeCurve2d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeCurve2d::paramAtLength(double datumParam, double length, double tol) const {
	if (length == 0.0) {
		return datumParam;
	}

	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return datumParam;
	}

	double targetParam = length > 0.0 ? range.upperBound() : range.lowerBound();
	int sampleCount = 65;
	if (tol > 0.0) {
		double span = targetParam - datumParam;
		if (span < 0.0) {
			span = 0.0 - span;
		}
		int dynamicCount = int(span / tol) + 1;
		if (dynamicCount > sampleCount) {
			sampleCount = dynamicCount;
		}
	}

	GePoint2dArray pointArray;
	curve2d_sample_points(*this, datumParam, targetParam, sampleCount, pointArray);
	if (pointArray.length() == 0) {
		return datumParam;
	}

	double remainLength = length;
	if (remainLength < 0.0) {
		remainLength = 0.0 - remainLength;
	}

	double walkedLength = 0.0;
	for (int i = 1; i < pointArray.length(); i++) {
		double segLength = pointArray[i - 1].distanceTo(pointArray[i]);
		if (walkedLength + segLength >= remainLength && segLength > 0.0) {
			double ratio = (remainLength - walkedLength) / segLength;
			double param0 = datumParam + (targetParam - datumParam) * double(i - 1) / double(sampleCount - 1);
			double param1 = datumParam + (targetParam - datumParam) * double(i) / double(sampleCount - 1);
			return param0 + (param1 - param0) * ratio;
		}
		walkedLength += segLength;
	}
	return targetParam;
}
GeBoundBlock2d GeCurve2d::boundBlock() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return GeBoundBlock2d();
	}
	return this->boundBlock(range);
}
GeBoundBlock2d GeCurve2d::boundBlock(const GeInterval& range) const {
	return this->orthoBoundBlock(range);
}
GeBoundBlock2d GeCurve2d::orthoBoundBlock() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return GeBoundBlock2d();
	}
	return this->orthoBoundBlock(range);
}
GeBoundBlock2d GeCurve2d::orthoBoundBlock(const GeInterval& range) const {
	GeBoundBlock2d boundBlock;
	if (range.isBounded() == false) {
		return boundBlock;
	}

	GePoint2dArray pointArray;
	curve2d_sample_points(*this, range.lowerBound(), range.upperBound(), 33, pointArray);
	if (pointArray.length() == 0) {
		return boundBlock;
	}

	GePoint2d minPoint = pointArray[0];
	GePoint2d maxPoint = pointArray[0];
	for (int i = 1; i < pointArray.length(); i++) {
		if (pointArray[i].x < minPoint.x) minPoint.x = pointArray[i].x;
		if (pointArray[i].y < minPoint.y) minPoint.y = pointArray[i].y;
		if (pointArray[i].x > maxPoint.x) maxPoint.x = pointArray[i].x;
		if (pointArray[i].y > maxPoint.y) maxPoint.y = pointArray[i].y;
	}
	boundBlock.set(minPoint, maxPoint);
	return boundBlock;
}
bool GeCurve2d::isOn(const GePoint2d& pnt, double& param) const {
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeCurve2d::isOn(const GePoint2d& pnt, double& param, const GeTol& tol) const {
	param = this->paramOf(pnt, tol);
	if (this->isOn(param, tol) == false) {
		return false;
	}
	return this->evalPoint(param).isEqualTo(pnt, tol);
}
bool GeCurve2d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeCurve2d::isOn(double param, const GeTol& tol) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return true;
	}
	return range.contains(param);
}
bool GeCurve2d::hasStartPoint(GePoint2d& startPoint) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBoundedBelow() == false) {
		return false;
	}
	startPoint = this->evalPoint(range.lowerBound());
	return true;
}
bool GeCurve2d::hasEndPoint(GePoint2d& endPoint) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBoundedAbove() == false) {
		return false;
	}
	endPoint = this->evalPoint(range.upperBound());
	return true;
}
GePoint2d GeCurve2d::evalPoint(double param) const {
	GePointOnCurve2d pointOnCurve;
	pointOnCurve.setCurve(*this);
	pointOnCurve.setParameter(param);
	return pointOnCurve.point();
}
GeCurve2d& GeCurve2d::operator =  (const GeCurve2d& curve) {
	return *this;
}