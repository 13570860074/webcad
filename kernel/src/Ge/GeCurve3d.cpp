#include "GeCurve3d.h"
#include "GePointOnCurve3d.h"
#include "GeBoundBlock3d.h"
#include "GeInterval.h"
#include "GePlane.h"
#include "GeLine3d.h"

namespace {
bool curve3d_build_plane_from_direction(const GePoint3d& point, const GeVector3d& direction, const GeTol& tol, GePlane& plane)
{
	GeVector3d dir = direction;
	if (dir.length() <= tol.equalVector()) {
		plane.set(point, GeVector3d::kZAxis);
		return true;
	}

	GeVector3d refAxis = GeVector3d::kZAxis;
	if (dir.isParallelTo(refAxis, tol) == true) {
		refAxis = GeVector3d::kXAxis;
	}

	GeVector3d normal = dir.crossProduct(refAxis);
	if (normal.length() <= tol.equalVector()) {
		refAxis = GeVector3d::kYAxis;
		normal = dir.crossProduct(refAxis);
	}
	if (normal.length() <= tol.equalVector()) {
		plane.set(point, GeVector3d::kZAxis);
		return true;
	}

	plane.set(point, normal.normal());
	return true;
}
}

GeCurve3d::GeCurve3d()
{

}
GeCurve3d::GeCurve3d(const GeCurve3d& source)
{

}

void GeCurve3d::getInterval(GeInterval& range) const {
	range.set();
}
void GeCurve3d::getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const {
	this->getInterval(range);
	startPoint = GePoint3d::kOrigin;
	endPoint = GePoint3d::kOrigin;
}
GeCurve3d& GeCurve3d::reverseParam() {
	return *this;
}
GeCurve3d& GeCurve3d::setInterval() {
	return *this;
}
bool GeCurve3d::setInterval(const GeInterval& range) {
	return false;
}

double GeCurve3d::distanceTo(const GePoint3d& pnt) const {
	return this->distanceTo(pnt, GeContext::gTol);
}
double GeCurve3d::distanceTo(const GePoint3d& pnt, const GeTol& tol) const {
	return this->closestPointTo(pnt, tol).distanceTo(pnt);
}
double GeCurve3d::distanceTo(const GeCurve3d& entity) const {
	return this->distanceTo(entity, GeContext::gTol);
}
double GeCurve3d::distanceTo(const GeCurve3d& entity, const GeTol& tol) const {
	GePoint3d pntOnOtherCrv;
	return this->closestPointTo(entity, pntOnOtherCrv, tol).distanceTo(pntOnOtherCrv);
}

GePoint3d GeCurve3d::closestPointTo(const GePoint3d& pnt) const {
	return this->closestPointTo(pnt, GeContext::gTol);
}
GePoint3d GeCurve3d::closestPointTo(const GePoint3d& pnt, const GeTol& tol) const {
	return GePoint3d::kOrigin;
}
GePoint3d GeCurve3d::closestPointTo(const GePoint3d& pnt, double& param, const GeInterval* range, const GeTol& tol) const {
	GePoint3d closest = this->closestPointTo(pnt, tol);
	param = this->paramOf(closest, tol);
	if (range == NULL || range->isBounded() == false || range->contains(param) == true) {
		return closest;
	}

	bool hasCandidate = false;
	GePoint3d bestPoint;
	double bestParam = param;
	double bestDist = 0.0;
	if (range->isBoundedBelow() == true) {
		GePoint3d point = this->evalPoint(range->lowerBound());
		double dist = point.distanceTo(pnt);
		hasCandidate = true;
		bestPoint = point;
		bestParam = range->lowerBound();
		bestDist = dist;
	}
	if (range->isBoundedAbove() == true) {
		GePoint3d point = this->evalPoint(range->upperBound());
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
GePoint3d GeCurve3d::closestPointTo(const GeCurve3d& curve2d, GePoint3d& pntOnOtherCrv) const {
	return this->closestPointTo(curve2d, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCurve3d::closestPointTo(const GeCurve3d& curve2d, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	GeInterval thisRange;
	GeInterval otherRange;
	this->getInterval(thisRange);
	curve2d.getInterval(otherRange);
	if (thisRange.isBounded() == false || otherRange.isBounded() == false) {
		pntOnOtherCrv = GePoint3d::kOrigin;
		return GePoint3d::kOrigin;
	}

	GePoint3d bestPoint;
	GePoint3d bestOtherPoint;
	double bestDist = 0.0;
	bool hasCandidate = false;

	GePoint3dArray thisPoints;
	GeDoubleArray thisParams;
	this->getSamplePoints(thisRange.lowerBound(), thisRange.upperBound(), (thisRange.upperBound() - thisRange.lowerBound()) / 32.0, thisPoints, thisParams, false);
	for (int i = 0; i < thisPoints.length(); i++) {
		GePoint3d otherPoint = curve2d.closestPointTo(thisPoints[i], tol);
		double dist = thisPoints[i].distanceTo(otherPoint);
		if (hasCandidate == false || dist < bestDist) {
			hasCandidate = true;
			bestDist = dist;
			bestPoint = thisPoints[i];
			bestOtherPoint = otherPoint;
		}
	}

	GePoint3dArray otherPoints;
	GeDoubleArray otherParams;
	curve2d.getSamplePoints(otherRange.lowerBound(), otherRange.upperBound(), (otherRange.upperBound() - otherRange.lowerBound()) / 32.0, otherPoints, otherParams, false);
	for (int i = 0; i < otherPoints.length(); i++) {
		GePoint3d thisPoint = this->closestPointTo(otherPoints[i], tol);
		double dist = thisPoint.distanceTo(otherPoints[i]);
		if (hasCandidate == false || dist < bestDist) {
			hasCandidate = true;
			bestDist = dist;
			bestPoint = thisPoint;
			bestOtherPoint = otherPoints[i];
		}
	}

	if (hasCandidate == false) {
		pntOnOtherCrv = GePoint3d::kOrigin;
		return GePoint3d::kOrigin;
	}

	pntOnOtherCrv = bestOtherPoint;
	return bestPoint;
}

void GeCurve3d::getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {
	return this->getClosestPointTo(pnt, pntOnCrv, GeContext::gTol);
}
void GeCurve3d::getClosestPointTo(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {
	GePoint3d point = this->closestPointTo(pnt, tol);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(point, tol));
}
void GeCurve3d::getClosestPointTo(const GeCurve3d& curve2d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getClosestPointTo(curve2d, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCurve3d::getClosestPointTo(const GeCurve3d& curve2d, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pointOnOther;
	GePoint3d pointOnThis = this->closestPointTo(curve2d, pointOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pointOnThis, tol));
	pntOnOtherCrv.setCurve(curve2d);
	pntOnOtherCrv.setParameter(curve2d.paramOf(pointOnOther, tol));
}

GePoint3d GeCurve3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection) const {
	return this->projClosestPointTo(pnt, projectDirection, GeContext::gTol);
}
GePoint3d GeCurve3d::projClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, const GeTol& tol) const {
	return this->closestPointTo(pnt, tol);
}
GePoint3d GeCurve3d::projClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv) const {
	return this->projClosestPointTo(curve3d, projectDirection, pntOnOtherCrv, GeContext::gTol);
}
GePoint3d GeCurve3d::projClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePoint3d& pntOnOtherCrv, const GeTol& tol) const {
	return this->closestPointTo(curve3d, pntOnOtherCrv, tol);
}

void GeCurve3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv) const {
	return this->getProjClosestPointTo(pnt, projectDirection, pntOnCrv, GeContext::gTol);
}
void GeCurve3d::getProjClosestPointTo(const GePoint3d& pnt, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {
	GePoint3d point = this->projClosestPointTo(pnt, projectDirection, tol);
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(this->paramOf(point, tol));
}
void GeCurve3d::getProjClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv) const {
	return this->getProjClosestPointTo(curve3d, projectDirection, pntOnThisCrv, pntOnOtherCrv, GeContext::gTol);
}
void GeCurve3d::getProjClosestPointTo(const GeCurve3d& curve3d, const GeVector3d& projectDirection, GePointOnCurve3d& pntOnThisCrv, GePointOnCurve3d& pntOnOtherCrv, const GeTol& tol) const {
	GePoint3d pntOnOther;
	GePoint3d pntOnThis = this->projClosestPointTo(curve3d, projectDirection, pntOnOther, tol);
	pntOnThisCrv.setCurve(*this);
	pntOnThisCrv.setParameter(this->paramOf(pntOnThis, tol));
	pntOnOtherCrv.setCurve(curve3d);
	pntOnOtherCrv.setParameter(curve3d.paramOf(pntOnOther, tol));
}

bool GeCurve3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv) const {
	return this->getNormalPoint(pnt, pntOnCrv, GeContext::gTol);
}
bool GeCurve3d::getNormalPoint(const GePoint3d& pnt, GePointOnCurve3d& pntOnCrv, const GeTol& tol) const {
	GePoint3d closest = this->closestPointTo(pnt, tol);
	double param = 0.0;
	if (this->isOn(closest, param, tol) == false) {
		return false;
	}
	pntOnCrv.setCurve(*this);
	pntOnCrv.setParameter(param);
	return true;
}

GeBoundBlock3d GeCurve3d::boundBlock() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return GeBoundBlock3d();
	}
	return this->boundBlock(range);
}
GeBoundBlock3d GeCurve3d::boundBlock(const GeInterval& range) const {
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeCurve3d::orthoBoundBlock() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return GeBoundBlock3d();
	}
	return this->orthoBoundBlock(range);
}
GeBoundBlock3d GeCurve3d::orthoBoundBlock(const GeInterval& range) const {
	GeBoundBlock3d boundBlock;
	if (range.isBounded() == false) {
		return boundBlock;
	}

	GePoint3dArray pointArray;
	GeDoubleArray paramArray;
	double lower = range.lowerBound();
	double upper = range.upperBound();
	double step = upper - lower;
	if (step < 0.0) {
		step = 0.0 - step;
	}
	step = step / 32.0;
	this->getSamplePoints(lower, upper, step, pointArray, paramArray, false);
	if (pointArray.length() == 0) {
		pointArray.append(this->evalPoint(lower));
		pointArray.append(this->evalPoint(upper));
	}

	GePoint3d minPoint;
	GePoint3d maxPoint;
	for (int i = 0; i < pointArray.length(); i++) {
		if (i == 0) {
			minPoint = pointArray[i];
			maxPoint = pointArray[i];
			continue;
		}
		if (pointArray[i].x < minPoint.x) minPoint.x = pointArray[i].x;
		if (pointArray[i].y < minPoint.y) minPoint.y = pointArray[i].y;
		if (pointArray[i].z < minPoint.z) minPoint.z = pointArray[i].z;
		if (pointArray[i].x > maxPoint.x) maxPoint.x = pointArray[i].x;
		if (pointArray[i].y > maxPoint.y) maxPoint.y = pointArray[i].y;
		if (pointArray[i].z > maxPoint.z) maxPoint.z = pointArray[i].z;
	}
	boundBlock.set(minPoint, maxPoint);
	return boundBlock;
}

GeEntity3d* GeCurve3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection) const {
	return this->project(projectionPlane, projectDirection, GeContext::gTol);
}
GeEntity3d* GeCurve3d::project(const GePlane& projectionPlane, const GeVector3d& projectDirection, const GeTol& tol) const {
	if (projectDirection.length() <= tol.equalVector()) {
		return NULL;
	}

	GeLine3d line;
	if (this->isLinear(line, tol) == true) {
		return line.project(projectionPlane, projectDirection, tol);
	}
	return NULL;
}
GeEntity3d* GeCurve3d::orthoProject(const GePlane& projectionPlane) const {
	return this->orthoProject(projectionPlane, GeContext::gTol);
}
GeEntity3d* GeCurve3d::orthoProject(const GePlane& projectionPlane, const GeTol& tol) const {
	return this->project(projectionPlane, projectionPlane.normal(), tol);
}

bool GeCurve3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	double param;
	return this->isOn(pnt, param, tol);
}
bool GeCurve3d::isOn(const GePoint3d& pnt, double& param) const {
	return this->isOn(pnt, param, GeContext::gTol);
}
bool GeCurve3d::isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const {
	param = this->paramOf(pnt, tol);
	if (this->isOn(param, tol) == false) {
		return false;
	}
	return this->evalPoint(param).isEqualTo(pnt, tol);
}
bool GeCurve3d::isOn(double param) const {
	return this->isOn(param, GeContext::gTol);
}
bool GeCurve3d::isOn(double param, const GeTol& tol) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return true;
	}
	return range.contains(param);
}

double GeCurve3d::paramOf(const GePoint3d& pnt) const {
	return this->paramOf(pnt, GeContext::gTol);
}
double GeCurve3d::paramOf(const GePoint3d& pnt, const GeTol& tol) const {
	return 0.0;
}

void GeCurve3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, Ge::OffsetCrvExtType::kFillet, GeContext::gTol);
}
void GeCurve3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType) const {
	return this->getTrimmedOffset(distance, planeNormal, offsetCurveList, extensionType, GeContext::gTol);
}
void GeCurve3d::getTrimmedOffset(double distance, const GeVector3d& planeNormal, GeVoidPointerArray& offsetCurveList, Ge::OffsetCrvExtType extensionType, const GeTol& tol) const {

}

bool GeCurve3d::isClosed() const {
	return this->isClosed(GeContext::gTol);
}
bool GeCurve3d::isClosed(const GeTol& tol) const {
	GePoint3d startPoint;
	GePoint3d endPoint;
	if (this->hasStartPoint(startPoint) == false || this->hasEndPoint(endPoint) == false) {
		return false;
	}
	return startPoint.isEqualTo(endPoint, tol);
}
bool GeCurve3d::isPlanar(GePlane& plane) const {
	return this->isPlanar(plane, GeContext::gTol);
}
bool GeCurve3d::isPlanar(GePlane& plane, const GeTol& tol) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return false;
	}

	GePoint3dArray pointArray;
	GeDoubleArray paramArray;
	this->getSamplePoints(range.lowerBound(), range.upperBound(), (range.upperBound() - range.lowerBound()) / 32.0, pointArray, paramArray, false);
	if (pointArray.length() == 0) {
		return false;
	}
	if (pointArray.length() == 1) {
		plane.set(pointArray[0], GeVector3d::kZAxis);
		return true;
	}

	GePoint3d point0 = pointArray[0];
	GePoint3d point1 = pointArray[pointArray.length() - 1];
	GeVector3d direction = point1 - point0;
	for (int i = 1; i < pointArray.length(); i++) {
		direction = pointArray[i] - point0;
		if (direction.length() > tol.equalVector()) {
			point1 = pointArray[i];
			break;
		}
	}

	GeVector3d baseDirection = point1 - point0;
	if (baseDirection.length() <= tol.equalVector()) {
		plane.set(point0, GeVector3d::kZAxis);
		return true;
	}

	for (int i = 1; i < pointArray.length(); i++) {
		GeVector3d vec = pointArray[i] - point0;
		GeVector3d normal = baseDirection.crossProduct(vec);
		if (normal.length() > tol.equalVector()) {
			plane.set(point0, normal.normal());
			for (int u = 0; u < pointArray.length(); u++) {
				if (plane.isOn(pointArray[u], tol) == false) {
					return false;
				}
			}
			return true;
		}
	}

	return curve3d_build_plane_from_direction(point0, baseDirection, tol, plane);
}
bool GeCurve3d::isLinear(GeLine3d& line) const {
	return this->isLinear(line, GeContext::gTol);
}
bool GeCurve3d::isLinear(GeLine3d& line, const GeTol& tol) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return false;
	}

	GePoint3dArray pointArray;
	GeDoubleArray paramArray;
	this->getSamplePoints(range.lowerBound(), range.upperBound(), (range.upperBound() - range.lowerBound()) / 32.0, pointArray, paramArray, false);
	if (pointArray.length() < 2) {
		return false;
	}

	GePoint3d point0 = pointArray[0];
	GePoint3d point1 = pointArray[pointArray.length() - 1];
	for (int i = 1; i < pointArray.length(); i++) {
		if (pointArray[i].distanceTo(point0) > tol.equalPoint()) {
			point1 = pointArray[i];
			break;
		}
	}
	if (point1.distanceTo(point0) <= tol.equalPoint()) {
		return false;
	}

	line.set(point0, point1 - point0);
	for (int i = 0; i < pointArray.length(); i++) {
		if (line.isOn(pointArray[i], tol) == false) {
			return false;
		}
	}
	return true;
}
bool GeCurve3d::isCoplanarWith(const GeCurve3d& curve3d, GePlane& plane) const {
	return this->isCoplanarWith(curve3d, plane, GeContext::gTol);
}
bool GeCurve3d::isCoplanarWith(const GeCurve3d& curve3d, GePlane& plane, const GeTol& tol) const {
	GePlane thisPlane;
	if (this->isPlanar(thisPlane, tol) == false) {
		return false;
	}
	GePlane otherPlane;
	if (curve3d.isPlanar(otherPlane, tol) == false) {
		return false;
	}
	if (thisPlane.isCoplanarTo(otherPlane, tol) == false) {
		return false;
	}
	plane = thisPlane;
	return true;
}


double GeCurve3d::length() const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return 0.0;
	}
	return this->length(range.lowerBound(), range.upperBound(), GeContext::gTol.equalPoint());
}
double GeCurve3d::length(double fromParam, double toParam) const {
	return this->length(fromParam, toParam, GeContext::gTol.equalPoint());
}
double GeCurve3d::length(double fromParam, double toParam, double tol) const {
	GePoint3dArray pointArray;
	GeDoubleArray paramArray;
	double step = tol;
	if (step <= 0.0) {
		double span = toParam - fromParam;
		if (span < 0.0) {
			span = 0.0 - span;
		}
		step = span / 64.0;
	}
	this->getSamplePoints(fromParam, toParam, step, pointArray, paramArray, false);
	if (pointArray.length() == 0) {
		return this->evalPoint(fromParam).distanceTo(this->evalPoint(toParam));
	}

	double totalLength = 0.0;
	for (int i = 1; i < pointArray.length(); i++) {
		totalLength += pointArray[i - 1].distanceTo(pointArray[i]);
	}
	return totalLength;
}
double GeCurve3d::paramAtLength(double datumParam, double length) const {
	return this->paramAtLength(datumParam, length, GeContext::gTol.equalPoint());
}
double GeCurve3d::paramAtLength(double datumParam, double length, double tol) const {
	if (length == 0.0) {
		return datumParam;
	}

	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return datumParam;
	}

	double targetParam = length > 0.0 ? range.upperBound() : range.lowerBound();
	double step = tol;
	if (step <= 0.0) {
		double span = targetParam - datumParam;
		if (span < 0.0) {
			span = 0.0 - span;
		}
		step = span / 128.0;
	}

	GePoint3dArray pointArray;
	GeDoubleArray paramArray;
	this->getSamplePoints(datumParam, targetParam, step, pointArray, paramArray, false);
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
			return paramArray[i - 1] + (paramArray[i] - paramArray[i - 1]) * ratio;
		}
		walkedLength += segLength;
	}
	return targetParam;
}
double GeCurve3d::area() const {
	return this->area(GeContext::gTol);
}
double GeCurve3d::area(const GeTol& tol) const {
	if (this->isClosed(tol) == false) {
		return 0.0;
	}

	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return 0.0;
	}

	GePlane plane;
	if (this->isPlanar(plane, tol) == false) {
		return 0.0;
	}

	GePoint3dArray pointArray;
	GeDoubleArray paramArray;
	this->getSamplePoints(range.lowerBound(), range.upperBound(), (range.upperBound() - range.lowerBound()) / 64.0, pointArray, paramArray, false);
	if (pointArray.length() < 3) {
		return 0.0;
	}

	double areaValue = 0.0;
	for (int i = 0; i < pointArray.length(); i++) {
		GePoint2d param1 = plane.paramOf(pointArray[i], tol);
		GePoint2d param2 = plane.paramOf(pointArray[(i + 1) % pointArray.length()], tol);
		areaValue += param1.x * param2.y - param2.x * param1.y;
	}
	if (areaValue < 0.0) {
		areaValue = 0.0 - areaValue;
	}
	return areaValue * 0.5;
}

void GeCurve3d::getSplitCurves(double param, GeCurve3d*& piece1, GeCurve3d*& piece2) const {
	piece1 = NULL;
	piece2 = NULL;
	return;
}

bool GeCurve3d::explode(GeVoidPointerArray& explodedCurves, GeIntArray& newExplodedCurve) const {
	return false;
}

bool GeCurve3d::hasStartPoint(GePoint3d& startPoint) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBoundedBelow() == false) {
		return false;
	}
	startPoint = this->evalPoint(range.lowerBound());
	return true;
}
bool GeCurve3d::hasEndPoint(GePoint3d& endPoint) const {
	GeInterval range;
	this->getInterval(range);
	if (range.isBoundedAbove() == false) {
		return false;
	}
	endPoint = this->evalPoint(range.upperBound());
	return true;
}

GePoint3d GeCurve3d::evalPoint(double param) const {
	return GePoint3d::kOrigin;
}

GeCurve3d& GeCurve3d::operator =  (const GeCurve3d& curve) {
	return *this;
}
void GeCurve3d::getSamplePoints(double fromParam,
	double toParam,
	double approxEps,
	GePoint3dArray& pointArray,
	GeDoubleArray& paramArray) const {
	return this->getSamplePoints(fromParam, toParam, approxEps, pointArray, paramArray, false);
}
void GeCurve3d::getSamplePoints(double fromParam,
	double toParam,
	double approxEps,
	GePoint3dArray& pointArray,
	GeDoubleArray& paramArray,
	bool forceResampling) const {
	double span = toParam - fromParam;
	if (span < 0.0) {
		span = 0.0 - span;
	}
	double step = approxEps;
	if (step <= 0.0) {
		step = span / 16.0;
	}
	if (step <= 0.0) {
		step = 1.0;
	}

	int sampleCount = int(span / step) + 1;
	if (sampleCount < 2 || forceResampling == true) {
		sampleCount = 2;
	}

	for (int i = 0; i < sampleCount; i++) {
		double ratio = 0.0;
		if (sampleCount > 1) {
			ratio = double(i) / double(sampleCount - 1);
		}
		double param = fromParam + (toParam - fromParam) * ratio;
		pointArray.append(this->evalPoint(param));
		paramArray.append(param);
	}
}
void GeCurve3d::getSamplePoints(int numSample, GePoint3dArray& pointArray) const {
	GeDoubleArray paramArray;
	return this->getSamplePoints(numSample, pointArray, paramArray);
}
void GeCurve3d::getSamplePoints(int numSample, GePoint3dArray& pointArray, GeDoubleArray& paramArray) const {
	if (numSample <= 0) {
		return;
	}

	GeInterval range;
	this->getInterval(range);
	if (range.isBounded() == false) {
		return;
	}

	double lower = range.lowerBound();
	double upper = range.upperBound();
	for (int i = 0; i < numSample; i++) {
		double ratio = 0.0;
		if (numSample > 1) {
			ratio = double(i) / double(numSample - 1);
		}
		double param = lower + (upper - lower) * ratio;
		pointArray.append(this->evalPoint(param));
		paramArray.append(param);
	}
}