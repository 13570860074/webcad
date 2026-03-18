#include "GeBoundBlock2d.h"
#include "GeRay2d.h"
#include "GeLineSeg2d.h"
#include "GeLine2d.h"
#include "GeImpl.h"



GeBoundBlock2d::GeBoundBlock2d() {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock2d);
}
GeBoundBlock2d::GeBoundBlock2d(const GePoint2d& point1, const GePoint2d& point2) {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock2d);

	this->set(point1, point2);
}
GeBoundBlock2d::GeBoundBlock2d(const GePoint2d& base, const GeVector2d& dir1, const GeVector2d& dir2) {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock2d);

	this->set(base, dir1, dir2);
}
GeBoundBlock2d::GeBoundBlock2d(const GeBoundBlock2d& source) {
	GE_IMP_MEMORY_ENTITY(GeBoundBlock2d);

	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox = GE_IMP_BOUNDBLOCK2D(source.m_pImpl)->isBox;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint = GE_IMP_BOUNDBLOCK2D(source.m_pImpl)->basePoint;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 = GE_IMP_BOUNDBLOCK2D(source.m_pImpl)->dir1;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2 = GE_IMP_BOUNDBLOCK2D(source.m_pImpl)->dir2;
}
void GeBoundBlock2d::getMinMaxPoints(GePoint2d& p1, GePoint2d& p2) const {
	GePoint2d point;
	GePoint2dArray points;

	point.set(GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.x, GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.y);
	points.append(point);

	point = point + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	points.append(point);

	point = point + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	points.append(point);

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
		if (points[i].x > p2.x) {
			p2.x = points[i].x;
		}
		if (points[i].y > p2.y) {
			p2.y = points[i].y;
		}
	}
}
void GeBoundBlock2d::get(GePoint2d& base, GeVector2d& side1, GeVector2d& side2) const {
	base.set(GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.x, GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.y);
	side1.set(GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1.x, GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1.y);
	side2.set(GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2.x, GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2.y);
}

GeBoundBlock2d& GeBoundBlock2d::set(const GePoint2d& p1, const GePoint2d& p2) {
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox = true;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint = p1;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 = GeVector2d(p2.x - p1.x, 0);
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2 = GeVector2d(0, p2.y - p1.y);
	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::set(const GePoint2d& base, const GeVector2d& side1, const GeVector2d& side2) {
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox = false;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint = base;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 = side1;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2 = side2;
	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::extend(const GePoint2d& point) {

	GeLine2d line1;
	GePoint2d p1 = point;
	GePoint2d p2 = point + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	line1.set(p1, p2);

	GeLine2d line2;
	GePoint2d p3 = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GePoint2d p4 = p3 + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	line2.set(p3, p4);

	GePoint2d intersect;
	if (line1.intersectWith(line2, intersect) == false) {
		return *this;
	}

	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 = intersect - point;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2 = p4 - intersect;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.set(point.x, point.y);

	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::swell(double distance) {
	GeVector2d dir1 = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GeVector2d dir2 = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	dir1.normalize();
	dir2.normalize();
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.translation((0 - distance) * dir1);
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.translation((0 - distance) * dir2);
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 += dir1 * (distance * 2);
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2 += dir2 * (distance * 2);
	return *this;
}
bool GeBoundBlock2d::contains(const GePoint2d& point) const {

	GePoint2d basePoint;
	GeVector2d dir1, dir2;
	this->get(basePoint, dir1, dir2);

	GePoint2d p1, p2;
	this->getMinMaxPoints(p1, p2);
	if (p1.isEqualTo(p2) == true) {
		return false;
	}

	GePoint2dArray points;
	points.append(basePoint);
	points.append(points[points.length() - 1] + dir1);
	points.append(points[points.length() - 1] + dir2);
	points.append(points[points.length() - 1] - dir1);
	points.append(points[points.length() - 1] - dir2);

	int numInter = 0;
	GeRay2d ray(point, GeVector2d::kXAxis);
	for (int i = 1; i < points.length(); i++) {
		GeLineSeg2d line(points[i - 1], points[i]);

		GePoint2d intersect;
		if (ray.intersectWith(line, intersect) == true) {
			numInter++;
		}
	}
	if (numInter > 0 && numInter % 2 == 1) {
		return true;
	}
	return false;
}
bool GeBoundBlock2d::isDisjoint(const GeBoundBlock2d& block) const {

	bool isValue = true;

	GePoint2d basePoint;
	GeVector2d dir1, dir2;
	block.get(basePoint, dir1, dir2);

	GePoint2dArray pointOthers;
	pointOthers.append(basePoint);
	pointOthers.append(pointOthers[pointOthers.length() - 1] + dir1);
	pointOthers.append(pointOthers[pointOthers.length() - 1] + dir2);
	pointOthers.append(pointOthers[pointOthers.length() - 1] - dir1);

	for (int i = 0; i < pointOthers.length(); i++) {
		if (this->contains(pointOthers[i]) == true) {
			isValue = false;
			break;
		}
	}

	return false;
}
GeBoundBlock2d& GeBoundBlock2d::operator =(const GeBoundBlock2d& block) {
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox = GE_IMP_BOUNDBLOCK2D(block.m_pImpl)->isBox;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint = GE_IMP_BOUNDBLOCK2D(block.m_pImpl)->basePoint;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 = GE_IMP_BOUNDBLOCK2D(block.m_pImpl)->dir1;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2 = GE_IMP_BOUNDBLOCK2D(block.m_pImpl)->dir2;
	return *this;
}
bool GeBoundBlock2d::isBox() const {
	return GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox;
}
GeBoundBlock2d& GeBoundBlock2d::setToBox(bool toBox) {
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox = toBox;
	if (this->isBox() == true) {
		GePoint2d p1, p2;
		this->getMinMaxPoints(p1, p2);
		this->set(p1, p2);
	}
	return *this;
}






bool GeBoundBlock2d::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GeBoundBlock2d::type() const {
	return Ge::EntityId::kBoundBlock2d;
}
GeBoundBlock2d* GeBoundBlock2d::copy() const {
	GeBoundBlock2d* boundBlock3d = new GeBoundBlock2d();
	GE_IMP_BOUNDBLOCK2D(boundBlock3d->m_pImpl)->basePoint = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint;
	GE_IMP_BOUNDBLOCK2D(boundBlock3d->m_pImpl)->dir1 = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GE_IMP_BOUNDBLOCK2D(boundBlock3d->m_pImpl)->dir2 = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	GE_IMP_BOUNDBLOCK2D(boundBlock3d->m_pImpl)->isBox = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->isBox;
	return boundBlock3d;
}
bool GeBoundBlock2d::operator == (const GeBoundBlock2d& entity) const {
	return this->isEqualTo(entity);
}
bool GeBoundBlock2d::operator != (const GeBoundBlock2d& entity) const {
	return !this->isEqualTo(entity);
}
bool GeBoundBlock2d::isEqualTo(const GeBoundBlock2d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GeBoundBlock2d::isEqualTo(const GeBoundBlock2d& entity, const GeTol& tol) const {
	if (GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.isEqualTo(GE_IMP_BOUNDBLOCK2D(entity.m_pImpl)->basePoint, tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1.isEqualTo(GE_IMP_BOUNDBLOCK2D(entity.m_pImpl)->dir1, tol) == false) {
		return false;
	}
	if (GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2.isEqualTo(GE_IMP_BOUNDBLOCK2D(entity.m_pImpl)->dir2, tol) == false) {
		return false;
	}
	return true;
}
GeBoundBlock2d& GeBoundBlock2d::transformBy(const GeMatrix2d& xfm) {
	GePoint2d basePoint = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint;
	GePoint2d p1 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GePoint2d p2 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	basePoint.transformBy(xfm);
	p1.transformBy(xfm);
	p2.transformBy(xfm);
	this->set(basePoint, p1 - basePoint, p2 - p1);
	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::translateBy(const GeVector2d& translateVec) {
	GePoint2d basePoint = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint;
	basePoint += translateVec;
	GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint.set(basePoint.x, basePoint.y);
	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeBoundBlock2d& GeBoundBlock2d::rotateBy(double angle, const GePoint2d& wrtPoint) {

	GePoint2d basePoint = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint;
	GePoint2d p1 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GePoint2d p2 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	basePoint.rotateBy(angle, wrtPoint);
	p1.rotateBy(angle, wrtPoint);
	p2.rotateBy(angle, wrtPoint);
	this->set(basePoint, p1 - basePoint, p2 - p1);
	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::mirror(const GeLine2d& line) {

	GePoint2d basePoint = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint;
	GePoint2d p1 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GePoint2d p2 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	basePoint.mirror(line);
	p1.mirror(line);
	p2.mirror(line);
	this->set(basePoint, p1 - basePoint, p2 - p1);
	return *this;
}
GeBoundBlock2d& GeBoundBlock2d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeBoundBlock2d& GeBoundBlock2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {
	GePoint2d basePoint = GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->basePoint;
	GePoint2d p1 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1;
	GePoint2d p2 = basePoint + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir1 + GE_IMP_BOUNDBLOCK2D(this->m_pImpl)->dir2;
	basePoint.scaleBy(scaleFactor, wrtPoint);
	p1.scaleBy(scaleFactor, wrtPoint);
	p2.scaleBy(scaleFactor, wrtPoint);
	this->set(basePoint, p1 - basePoint, p2 - p1);
	return *this;
}
bool GeBoundBlock2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GeBoundBlock2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	return this->contains(pnt);
}