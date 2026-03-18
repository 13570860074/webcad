#include "GeBoundBlock3d.h"
#include "GeLine3d.h"
#include "GeRay3d.h"
#include "GeLineSeg3d.h"
#include "GeBoundedPlane.h"
#include "GePoint3dArray.h"
#include "GeImpl.h"

namespace {
void appendCorners(const GePoint3d& basePoint, const GeVector3d& dir1, const GeVector3d& dir2, const GeVector3d& dir3, GePoint3dArray& points)
{
	points.append(basePoint);
	points.append(basePoint + dir1);
	points.append(basePoint + dir2);
	points.append(basePoint + dir3);
	points.append(basePoint + dir1 + dir2);
	points.append(basePoint + dir1 + dir3);
	points.append(basePoint + dir2 + dir3);
	points.append(basePoint + dir1 + dir2 + dir3);
}

double tripleProduct(const GeVector3d& vec1, const GeVector3d& vec2, const GeVector3d& vec3)
{
	return vec1.dotProduct(vec2.crossProduct(vec3));
}

bool containsInParallelepiped(const GePoint3d& basePoint, const GeVector3d& dir1, const GeVector3d& dir2, const GeVector3d& dir3, const GePoint3d& point, const GeTol& tol)
{
	double determinant = tripleProduct(dir1, dir2, dir3);
	double determinantTol = tol.equalVector() * tol.equalVector() * tol.equalVector();
	if (fabs(determinant) <= determinantTol)
	{
		return false;
	}

	GeVector3d offset = point - basePoint;
	double u = tripleProduct(offset, dir2, dir3) / determinant;
	double v = tripleProduct(dir1, offset, dir3) / determinant;
	double w = tripleProduct(dir1, dir2, offset) / determinant;

	return u >= -tol.equalPoint() &&
		u <= 1.0 + tol.equalPoint() &&
		v >= -tol.equalPoint() &&
		v <= 1.0 + tol.equalPoint() &&
		w >= -tol.equalPoint() &&
		w <= 1.0 + tol.equalPoint();
}

bool containsInFace(const GePoint3d& origin, const GeVector3d& side1, const GeVector3d& side2, const GePoint3d& point, const GeTol& tol)
{
	GeVector3d normal = side1.crossProduct(side2);
	double normalLength = normal.length();
	if (normalLength <= tol.equalVector())
	{
		return false;
	}

	GeVector3d offset = point - origin;
	if (fabs(offset.dotProduct(normal)) > tol.equalPoint() * normalLength)
	{
		return false;
	}

	double denominator = normal.lengthSqrd();
	double u = tripleProduct(offset, side2, normal) / denominator;
	double v = tripleProduct(side1, offset, normal) / denominator;
	return u >= -tol.equalPoint() &&
		u <= 1.0 + tol.equalPoint() &&
		v >= -tol.equalPoint() &&
		v <= 1.0 + tol.equalPoint();
}

bool segmentIntersectsFace(const GePoint3d& startPoint, const GePoint3d& endPoint, const GePoint3d& origin, const GeVector3d& side1, const GeVector3d& side2, const GeTol& tol)
{
	if (containsInFace(origin, side1, side2, startPoint, tol) == true ||
		containsInFace(origin, side1, side2, endPoint, tol) == true)
	{
		return true;
	}

	GeVector3d normal = side1.crossProduct(side2);
	double normalLength = normal.length();
	if (normalLength <= tol.equalVector())
	{
		return false;
	}

	GeVector3d direction = endPoint - startPoint;
	GeVector3d startOffset = startPoint - origin;
	double startDistance = startOffset.dotProduct(normal);
	double endDistance = (endPoint - origin).dotProduct(normal);
	double denominator = normal.dotProduct(direction);

	if (fabs(denominator) <= tol.equalVector())
	{
		if (fabs(startDistance) > tol.equalPoint() * normalLength || fabs(endDistance) > tol.equalPoint() * normalLength)
		{
			return false;
		}

		GePoint3d facePoints[4] = {
			origin,
			origin + side1,
			origin + side1 + side2,
			origin + side2
		};
		int faceEdges[4][2] = {
			{0, 1},
			{1, 2},
			{2, 3},
			{3, 0}
		};

		GeLineSeg3d segment(startPoint, endPoint);
		for (int i = 0; i < 4; i++)
		{
			GeLineSeg3d faceEdge(facePoints[faceEdges[i][0]], facePoints[faceEdges[i][1]]);
			GePoint3d intersect;
			if (segment.intersectWith(faceEdge, intersect, tol) == true)
			{
				return true;
			}
		}
		return false;
	}

	double lineParam = -startDistance / denominator;
	if (lineParam < -tol.equalPoint() || lineParam > 1.0 + tol.equalPoint())
	{
		return false;
	}

	GePoint3d intersect = startPoint + direction * lineParam;
	return containsInFace(origin, side1, side2, intersect, tol);
}

bool hasEdgeFaceIntersection(const GePoint3d& edgeBase, const GeVector3d& edgeDir1, const GeVector3d& edgeDir2, const GeVector3d& edgeDir3,
	const GePoint3d& faceBase, const GeVector3d& faceDir1, const GeVector3d& faceDir2, const GeVector3d& faceDir3, const GeTol& tol)
{
	GePoint3dArray edgePoints;
	appendCorners(edgeBase, edgeDir1, edgeDir2, edgeDir3, edgePoints);

	int edgePairs[12][2] = {
		{0, 1}, {0, 2}, {0, 3},
		{1, 4}, {1, 5},
		{2, 4}, {2, 6},
		{3, 5}, {3, 6},
		{4, 7}, {5, 7}, {6, 7}
	};

	GePoint3d faceOrigins[6] = {
		faceBase,
		faceBase + faceDir3,
		faceBase,
		faceBase + faceDir2,
		faceBase,
		faceBase + faceDir1
	};
	GeVector3d faceSides1[6] = {
		faceDir1,
		faceDir1,
		faceDir1,
		faceDir1,
		faceDir2,
		faceDir2
	};
	GeVector3d faceSides2[6] = {
		faceDir2,
		faceDir2,
		faceDir3,
		faceDir3,
		faceDir3,
		faceDir3
	};

	for (int i = 0; i < 12; i++)
	{
		GePoint3d startPoint = edgePoints[edgePairs[i][0]];
		GePoint3d endPoint = edgePoints[edgePairs[i][1]];
		for (int u = 0; u < 6; u++)
		{
			if (segmentIntersectsFace(startPoint, endPoint, faceOrigins[u], faceSides1[u], faceSides2[u], tol) == true)
			{
				return true;
			}
		}
	}

	return false;
}
}



GeBoundBlock3d::GeBoundBlock3d() {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock3d);
}
GeBoundBlock3d::GeBoundBlock3d(const GePoint3d& point1, const GePoint3d& point2) {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock3d);

	this->set(point1, point2);
}
GeBoundBlock3d::GeBoundBlock3d(const GePoint3d& base, const GeVector3d& dir1, const GeVector3d& dir2, const GeVector3d& dir3) {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock3d);

	this->set(base, dir1, dir2, dir3);
}
GeBoundBlock3d::GeBoundBlock3d(const GeBoundBlock3d& source) {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock3d);

	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox = GE_IMP_BOUNDBLOCK3D(source.m_pImpl)->isBox;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint = GE_IMP_BOUNDBLOCK3D(source.m_pImpl)->basePoint;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 = GE_IMP_BOUNDBLOCK3D(source.m_pImpl)->dir1;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 = GE_IMP_BOUNDBLOCK3D(source.m_pImpl)->dir2;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3 = GE_IMP_BOUNDBLOCK3D(source.m_pImpl)->dir3;
}
void GeBoundBlock3d::getMinMaxPoints(GePoint3d& p1, GePoint3d& p2) const {
	GePoint3dArray points;
	appendCorners(GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint,
		GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1,
		GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2,
		GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3,
		points);

	for (int i = 0; i < points.length(); i++) {
		if (i == 0) {
			p1 = points[i];
			p2 = points[i];
			continue;
		}

		if (points[i].x < p1.x) {
			p1.x = points[i].x;
		}
		if (points[i].y < p1.y) {
			p1.y = points[i].y;
		}
		if (points[i].z < p1.z) {
			p1.z = points[i].z;
		}
		if (points[i].x > p2.x) {
			p2.x = points[i].x;
		}
		if (points[i].y > p2.y) {
			p2.y = points[i].y;
		}
		if (points[i].z > p2.z) {
			p2.z = points[i].z;
		}
	}

}
void GeBoundBlock3d::get(GePoint3d& base, GeVector3d& dir1, GeVector3d& dir2, GeVector3d& dir3) const {
	base.set(GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.x, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.y, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.z);
	dir1.set(GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1.x, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1.y, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1.z);
	dir2.set(GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2.x, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2.y, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2.z);
	dir3.set(GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3.x, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3.y, GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3.z);
}
GeBoundBlock3d& GeBoundBlock3d::set(const GePoint3d& p1, const GePoint3d& p2) {
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox = true;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint = p1;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 = GeVector3d(p2.x - p1.x, 0, 0);
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 = GeVector3d(0, p2.y - p1.y, 0);
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3 = GeVector3d(0, 0, p2.z - p1.z);
	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::set(const GePoint3d& base, const GeVector3d& dir1, const GeVector3d& dir2, const GeVector3d& dir3) {
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox = false;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint = base;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 = dir1;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 = dir2;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3 = dir3;
	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::extend(const GePoint3d& point) {

	GeLine3d line1;
	GePoint3d p1 = point;
	GePoint3d p2 = point + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	line1.set(p1, p2);

	GeLine3d line2;
	GePoint3d p3 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GePoint3d p4 = p3 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	line2.set(p3, p4);

	GePoint3d intersect;
	if (line1.intersectWith(line2, intersect) == false) {
		return *this;
	}

	GeVector3d dir1 = intersect - point;
	GeVector3d dir2 = p4 - intersect;

	GePoint3d p5 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 +
		GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	GePoint3d p6 = point + dir1 + dir2;

	GeVector3d dir3 = p5 - p6;

	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 = dir1;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 = dir2;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3 = dir3;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.set(point.x, point.y, point.z);

	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::swell(double distance) {
	GeVector3d dir1 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GeVector3d dir2 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	GeVector3d dir3 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	dir1.normalize();
	dir2.normalize();
	dir3.normalize();
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.translation(dir1 * (0 - distance));
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.translation(dir2 * (0 - distance));
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.translation(dir3 * (0 - distance));
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 += dir1 * (distance * 2);
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 += dir2 * (distance * 2);
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3 += dir3 * (distance * 2);
	return *this;
}
bool GeBoundBlock3d::contains(const GePoint3d& point) const {
	GePoint3d basePoint;
	GeVector3d dir1, dir2, dir3;
	this->get(basePoint, dir1, dir2, dir3);
	return containsInParallelepiped(basePoint, dir1, dir2, dir3, point, GeContext::gTol);
}
bool GeBoundBlock3d::isDisjoint(const GeBoundBlock3d& block) const {

	bool isValue = true;

	GePoint3d basePoint;
	GeVector3d dir1, dir2, dir3;
	block.get(basePoint, dir1, dir2, dir3);

	GePoint3dArray pointOthers;
	appendCorners(basePoint, dir1, dir2, dir3, pointOthers);

	for (int i = 0; i < pointOthers.length(); i++) {
		if (this->contains(pointOthers[i]) == true) {
			isValue = false;
			break;
		}
	}

	if (isValue == true) {
		this->get(basePoint, dir1, dir2, dir3);
		GePoint3dArray pointItselfs;
		appendCorners(basePoint, dir1, dir2, dir3, pointItselfs);
		for (int i = 0; i < pointItselfs.length(); i++) {
			if (block.contains(pointItselfs[i]) == true) {
				isValue = false;
				break;
			}
		}
	}

	if (isValue == true) {
		GePoint3d selfBasePoint;
		GeVector3d selfDir1, selfDir2, selfDir3;
		this->get(selfBasePoint, selfDir1, selfDir2, selfDir3);
		if (hasEdgeFaceIntersection(selfBasePoint, selfDir1, selfDir2, selfDir3, basePoint, dir1, dir2, dir3, GeContext::gTol) == true ||
			hasEdgeFaceIntersection(basePoint, dir1, dir2, dir3, selfBasePoint, selfDir1, selfDir2, selfDir3, GeContext::gTol) == true)
		{
			isValue = false;
		}
	}

	return isValue;
}
GeBoundBlock3d& GeBoundBlock3d::operator =(const GeBoundBlock3d& block) {
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox = GE_IMP_BOUNDBLOCK3D(block.m_pImpl)->isBox;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint = GE_IMP_BOUNDBLOCK3D(block.m_pImpl)->basePoint;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 = GE_IMP_BOUNDBLOCK3D(block.m_pImpl)->dir1;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 = GE_IMP_BOUNDBLOCK3D(block.m_pImpl)->dir2;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3 = GE_IMP_BOUNDBLOCK3D(block.m_pImpl)->dir3;
	return *this;
}
bool GeBoundBlock3d::isBox() const {
	return GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox;
}
GeBoundBlock3d& GeBoundBlock3d::setToBox(bool toBox) {
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox = toBox;
	if (this->isBox() == true) {
		GePoint3d p1, p2;
		this->getMinMaxPoints(p1, p2);
		this->set(p1, p2);
	}
	return *this;
}





bool GeBoundBlock3d::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeBoundBlock3d::type() const {
	return Ge::EntityId::kBoundBlock3d;
}
GeBoundBlock3d* GeBoundBlock3d::copy() const {
	GeBoundBlock3d* boundBlock3d = new GeBoundBlock3d();
	GE_IMP_BOUNDBLOCK3D(boundBlock3d->m_pImpl)->basePoint = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint;
	GE_IMP_BOUNDBLOCK3D(boundBlock3d->m_pImpl)->dir1 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GE_IMP_BOUNDBLOCK3D(boundBlock3d->m_pImpl)->dir2 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	GE_IMP_BOUNDBLOCK3D(boundBlock3d->m_pImpl)->dir3 = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	GE_IMP_BOUNDBLOCK3D(boundBlock3d->m_pImpl)->isBox = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->isBox;
	return boundBlock3d;
}
bool GeBoundBlock3d::operator == (const GeBoundBlock3d& entity) const {
	return this->isEqualTo(entity);
}
bool GeBoundBlock3d::operator != (const GeBoundBlock3d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeBoundBlock3d::isEqualTo(const GeBoundBlock3d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeBoundBlock3d::isEqualTo(const GeBoundBlock3d& entity, const GeTol& tol) const {
	if (GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.isEqualTo(GE_IMP_BOUNDBLOCK3D(entity.m_pImpl)->basePoint, tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1.isEqualTo(GE_IMP_BOUNDBLOCK3D(entity.m_pImpl)->dir1, tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2.isEqualTo(GE_IMP_BOUNDBLOCK3D(entity.m_pImpl)->dir2, tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3.isEqualTo(GE_IMP_BOUNDBLOCK3D(entity.m_pImpl)->dir3, tol) == false) {
		return false;
	}
	return true;
}
GeBoundBlock3d& GeBoundBlock3d::transformBy(const GeMatrix3d& xfm) {
	GePoint3d basePoint = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint;
	GePoint3d p1 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GePoint3d p2 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	GePoint3d p3 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	basePoint.transformBy(xfm);
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	p3.transformBy(xfm);
	this->set(basePoint, p1 - basePoint, p2 - p1, p3 - p2);
	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::translateBy(const GeVector3d& translateVec) {
	GePoint3d basePoint = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint;
	basePoint += translateVec;
	GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint.set(basePoint.x, basePoint.y, basePoint.z);
	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeBoundBlock3d& GeBoundBlock3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {

	GePoint3d basePoint = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint;
	GePoint3d p1 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GePoint3d p2 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	GePoint3d p3 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	basePoint.rotateBy(angle, vec, wrtPoint);
	p1.rotateBy(angle, vec, wrtPoint);
	p2.rotateBy(angle, vec, wrtPoint);
	p3.rotateBy(angle, vec, wrtPoint);
	this->set(basePoint, p1 - basePoint, p2 - p1, p3 - p2);
	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::mirror(const GePlane& plane) {

	GePoint3d basePoint = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint;
	GePoint3d p1 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GePoint3d p2 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	GePoint3d p3 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	basePoint.mirror(plane);
	p1.mirror(plane);
	p2.mirror(plane);
	p3.mirror(plane);
	this->set(basePoint, p1 - basePoint, p2 - p1, p3 - p2);
	return *this;
}
GeBoundBlock3d& GeBoundBlock3d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeBoundBlock3d& GeBoundBlock3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {
	GePoint3d basePoint = GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->basePoint;
	GePoint3d p1 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1;
	GePoint3d p2 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2;
	GePoint3d p3 = basePoint + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir2 + GE_IMP_BOUNDBLOCK3D(this->m_pImpl)->dir3;
	basePoint.scaleBy(scaleFactor, wrtPoint);
	p1.scaleBy(scaleFactor, wrtPoint);
	p2.scaleBy(scaleFactor, wrtPoint);
	p3.scaleBy(scaleFactor, wrtPoint);
	this->set(basePoint, p1 - basePoint, p2 - p1, p3 - p2);
	return *this;
}
bool GeBoundBlock3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeBoundBlock3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	return this->contains(pnt);
}