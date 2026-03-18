#include "GePointOnSurface.h"
#include "GeMatrix3d.h"
#include "GePoint2d.h"
#include "GePlane.h"
#include "GeImpl.h"


GePointOnSurface::GePointOnSurface() {
	GE_IMP_MEMORY_ENTITY(GePointOnSurface);
}
GePointOnSurface::GePointOnSurface(const GeSurface& surf) {
	GE_IMP_MEMORY_ENTITY(GePointOnSurface);
}
GePointOnSurface::GePointOnSurface(const GeSurface& surf, const GePoint2d& param) {
	GE_IMP_MEMORY_ENTITY(GePointOnSurface);

	this->setSurface(surf);
	this->setParameter(param);
}
GePointOnSurface::GePointOnSurface(const GePointOnSurface& src) {
	GE_IMP_MEMORY_ENTITY(GePointOnSurface);

	this->setSurface(*src.surface());
	this->setParameter(src.parameter());
}
GePointOnSurface& GePointOnSurface::operator = (const GePointOnSurface& src) {
	this->setSurface(*src.surface());
	this->setParameter(src.parameter());
	return *this;
}
const GeSurface* GePointOnSurface::surface() const {
	return GE_IMP_POINTONSURFACE(this->m_pImpl)->surface;
}
GePoint2d GePointOnSurface::parameter() const {
	return GE_IMP_POINTONSURFACE(this->m_pImpl)->param;
}
GePoint3d GePointOnSurface::point() const {
	GePoint3d pos;

	if (this->surface()->type() == Ge::EntityId::kPlane) {

		GePlane* plane = (GePlane*)this->surface();

		//获得坐标系
		GePoint3d origin;
		GeVector3d xAxis, yAxis;
		plane->getCoordSystem(origin, xAxis, yAxis);
		yAxis = xAxis.crossProduct(plane->normal()).normal();

		//通过坐标系获得矩阵
		GeMatrix3d mat;
		mat.setCoordSystem(origin, xAxis, yAxis, plane->normal());

		//二维点转三维点
		pos = GePoint3d(this->parameter().x, this->parameter().y, 0.0).transformBy(mat);
	}

	return pos;
}
GePoint3d GePointOnSurface::point(const GePoint2d& param) {
	GePoint3d pos;

	if (this->surface()->type() == Ge::EntityId::kPlane) {

		GePlane* plane = (GePlane*)this->surface();

		//获得坐标系
		GePoint3d origin;
		GeVector3d xAxis, yAxis;
		plane->getCoordSystem(origin, xAxis, yAxis);
		yAxis = xAxis.crossProduct(plane->normal()).normal();

		//通过坐标系获得矩阵
		GeMatrix3d mat;
		mat.setCoordSystem(origin, xAxis, yAxis, plane->normal());

		//二维点转三维点
		pos = GePoint3d(param.x, param.y, 0.0).transformBy(mat);
	}

	return pos;
}
GePoint3d GePointOnSurface::point(const GeSurface& surf, const GePoint2d& param) {
	GePoint3d pos;

	if (surf.type() == Ge::EntityId::kPlane) {

		GePlane* plane = (GePlane*&)surf;

		//获得坐标系
		GePoint3d origin;
		GeVector3d xAxis, yAxis;
		plane->getCoordSystem(origin, xAxis, yAxis);
		yAxis = xAxis.crossProduct(plane->normal()).normal();

		//通过坐标系获得矩阵
		GeMatrix3d mat;
		mat.setCoordSystem(origin, xAxis, yAxis, plane->normal());

		//二维点转三维点
		pos = GePoint3d(param.x, param.y, 0.0).transformBy(mat);
	}

	return pos;
}
GePointOnSurface& GePointOnSurface::setSurface(const GeSurface& surf) {
	GE_IMP_POINTONSURFACE(this->m_pImpl)->surface = &surf;
	return *this;
}
GePointOnSurface& GePointOnSurface::setParameter(const GePoint2d& param) {
	GE_IMP_POINTONSURFACE(this->m_pImpl)->param = param;
	return *this;
}




bool GePointOnSurface::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GePointOnSurface::type() const {
	return Ge::EntityId::kPointOnSurface;
}
GePointOnSurface* GePointOnSurface::copy() const {
	GePointOnSurface* pointOnSurface = new GePointOnSurface();
	pointOnSurface->setSurface(*this->surface());
	pointOnSurface->setParameter(this->parameter());
	return pointOnSurface;
}
bool GePointOnSurface::operator == (const GePointOnSurface& entity) const {
	return this->isEqualTo(entity);
}
bool GePointOnSurface::operator != (const GePointOnSurface& entity) const {
	return !this->isEqualTo(entity);
}
bool GePointOnSurface::isEqualTo(const GePointOnSurface& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GePointOnSurface::isEqualTo(const GePointOnSurface& entity, const GeTol& tol) const {
	if (this->surface() != entity.surface()) {
		return false;
	}
	if (this->parameter().isEqualTo(entity.parameter(), tol) == false) {
		return false;
	}
	return true;
}
GePointOnSurface& GePointOnSurface::transformBy(const GeMatrix3d& xfm) {
	return *this;
}
GePointOnSurface& GePointOnSurface::translateBy(const GeVector3d& translateVec) {
	return *this;
}
GePointOnSurface& GePointOnSurface::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GePointOnSurface& GePointOnSurface::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {
	return *this;
}
GePointOnSurface& GePointOnSurface::mirror(const GePlane& plane) {
	return *this;
}
GePointOnSurface& GePointOnSurface::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GePointOnSurface& GePointOnSurface::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {
	return *this;
}
bool GePointOnSurface::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GePointOnSurface::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	return false;
}




GePoint3d GePointOnSurface::point3d() const {
	return this->point();
}
GePointOnSurface::operator GePoint3d () const {
	return this->point();
}