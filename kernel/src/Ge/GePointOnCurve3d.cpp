#include "GePointOnCurve3d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeRay2d.h"
#include "GeLine3d.h"
#include "GeCircArc2d.h"
#include "GeLineSeg3d.h"
#include "GeRay3d.h"
#include "GeCircArc3d.h"
#include "GeImpl.h"


GePointOnCurve3d::GePointOnCurve3d() {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve3d);
}
GePointOnCurve3d::GePointOnCurve3d(const GeCurve3d& crv) {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve3d);

	GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve = &crv;
}
GePointOnCurve3d::GePointOnCurve3d(const GeCurve3d& crv, double param) {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve3d);

	GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve = &crv;
	GE_IMP_POINTONCURVE3D(this->m_pImpl)->param = param;
}
GePointOnCurve3d::GePointOnCurve3d(const GePointOnCurve3d& src) {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve3d);

	GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve = src.curve();
	GE_IMP_POINTONCURVE3D(this->m_pImpl)->param = src.parameter();
}


GePointOnCurve3d& GePointOnCurve3d::operator = (const GePointOnCurve3d& src) {
	GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve = src.curve();
	GE_IMP_POINTONCURVE3D(this->m_pImpl)->param = src.parameter();
	return *this;
}

const GeCurve3d* GePointOnCurve3d::curve() const {
	return GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve;
}
double GePointOnCurve3d::parameter() const {
	return GE_IMP_POINTONCURVE3D(this->m_pImpl)->param;
}

GePoint3d GePointOnCurve3d::point() {
	return this->point(GE_IMP_POINTONCURVE3D(this->m_pImpl)->param);
}
GePoint3d GePointOnCurve3d::point(double param) {
	return this->point(*GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve, param);
}
GePoint3d GePointOnCurve3d::point(const GeCurve3d& crv, double param) {
	GePoint3d point;

	if (crv.isKindOf(Ge::EntityId::kLine3d) == true) {
		GeLine3d line = (GeLine3d&)crv;
		point = line.pointOnLine();
		point += line.direction() * param;
	}
	else if (crv.isKindOf(Ge::EntityId::kLinearEnt3d) == true) {
		GeLineSeg3d line = (GeLineSeg3d&)crv;
		point = line.pointOnLine();
		point += line.direction() * param;
	}
	else if (crv.isKindOf(Ge::EntityId::kRay3d) == true) {
		GeRay3d line = (GeRay3d&)crv;
		point = line.pointOnLine();
		point += line.direction() * param;
	}
	else if (crv.isKindOf(Ge::EntityId::kCircArc3d) == true) {

		GeCircArc3d circArc = (GeCircArc3d&)crv;

		GeVector3d refVec = circArc.refVec();
		point = circArc.center() + refVec * circArc.radius();
		point.rotateBy(param, circArc.normal(), circArc.center());
	}

	return point;
}

GePointOnCurve3d& GePointOnCurve3d::setCurve(const GeCurve3d& crv) {
	GE_IMP_POINTONCURVE3D(this->m_pImpl)->pCurve = &crv;
	return *this;
}
GePointOnCurve3d& GePointOnCurve3d::setParameter(double param) {
	GE_IMP_POINTONCURVE3D(this->m_pImpl)->param = param;
	return *this;
}


bool GePointOnCurve3d::isKindOf(Ge::EntityId entType) const {
	if (entType == this->type()) {
		return true;
	}
	return false;
}
Ge::EntityId GePointOnCurve3d::type() const {
	return Ge::EntityId::kPointOnCurve2d;
}
GePointOnCurve3d* GePointOnCurve3d::copy() const {
	GePointOnCurve3d* cur = new GePointOnCurve3d(*this);
	return cur;
}
bool GePointOnCurve3d::operator == (const GePointOnCurve3d& entity) const {
	return this->isEqualTo(entity);
}
bool GePointOnCurve3d::operator != (const GePointOnCurve3d& entity) const {
	return !this->isEqualTo(entity);
}
bool GePointOnCurve3d::isEqualTo(const GePointOnCurve3d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GePointOnCurve3d::isEqualTo(const GePointOnCurve3d& entity, const GeTol& tol) const {
	if (abs(this->parameter() - entity.parameter()) > tol.equalPoint()) {
		return false;
	}
	if (this->curve() != entity.curve()) {
		return false;
	}
	return true;
}
GePointOnCurve3d& GePointOnCurve3d::transformBy(const GeMatrix3d& xfm) {
	return *this;
}
GePointOnCurve3d& GePointOnCurve3d::translateBy(const GeVector3d& translateVec) {
	return *this;
}
GePointOnCurve3d& GePointOnCurve3d::rotateBy(double angle, const GeVector3d& vec) {
	return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GePointOnCurve3d& GePointOnCurve3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& wrtPoint) {
	return *this;
}
GePointOnCurve3d& GePointOnCurve3d::mirror(const GePlane& line) {
	return *this;
}
GePointOnCurve3d& GePointOnCurve3d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GePointOnCurve3d& GePointOnCurve3d::scaleBy(double scaleFactor, const GePoint3d& wrtPoint) {
	return *this;
}
bool GePointOnCurve3d::isOn(const GePoint3d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GePointOnCurve3d::isOn(const GePoint3d& pnt, const GeTol& tol) const {
	return false;
}


GePoint3d GePointOnCurve3d::point3d() const
{
	GePointOnCurve3d cur(*this);
	return cur.point();
}
GePointOnCurve3d::operator GePoint3d () const {
	return this->point3d();
}
