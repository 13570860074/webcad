#include "GePointOnCurve2d.h"
#include "GeScale2d.h"
#include "GeLine2d.h"
#include "GeLineSeg2d.h"
#include "GeRay2d.h"
#include "GeCircArc2d.h"
#include "GeEllipArc2d.h"
#include "GeImpl.h"
#include <cmath>


GePointOnCurve2d::GePointOnCurve2d() {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve2d);
}
GePointOnCurve2d::GePointOnCurve2d(const GeCurve2d& crv) {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve2d);

	GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve = &crv;
}
GePointOnCurve2d::GePointOnCurve2d(const GeCurve2d& crv, double param) {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve2d);

	GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve = &crv;
	GE_IMP_POINTONCURVE2D(this->m_pImpl)->param = param;
}
GePointOnCurve2d::GePointOnCurve2d(const GePointOnCurve2d& src) {
	GE_IMP_MEMORY_ENTITY(GePointOnCurve2d);

	GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve = src.curve();
	GE_IMP_POINTONCURVE2D(this->m_pImpl)->param = src.parameter();
}


GePointOnCurve2d& GePointOnCurve2d::operator = (const GePointOnCurve2d& src) {
	GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve = src.curve();
	GE_IMP_POINTONCURVE2D(this->m_pImpl)->param = src.parameter();
	return *this;
}

const GeCurve2d* GePointOnCurve2d::curve() const {
	return GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve;
}
double GePointOnCurve2d::parameter() const {
	return GE_IMP_POINTONCURVE2D(this->m_pImpl)->param;
}

GePoint2d GePointOnCurve2d::point() const {
	if (this->curve() == nullptr) {
		return GePoint2d::kOrigin;
	}
	return this->point(GE_IMP_POINTONCURVE2D(this->m_pImpl)->param);
}
GePoint2d GePointOnCurve2d::point(double param) const {
	if (this->curve() == nullptr) {
		return GePoint2d::kOrigin;
	}
	return this->point(*GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve, param);
}
GePoint2d GePointOnCurve2d::point(const GeCurve2d& crv, double param) const {
	GePoint2d point;

	if (crv.type() == Ge::EntityId::kLineSeg2d) {
		GeLineSeg2d line = (GeLineSeg2d&)crv;
		point.set(line.startPoint().x, line.startPoint().y);
		point.translation(line.direction() * line.length() * param);
	}
	else if (crv.type() == Ge::EntityId::kLine2d) {
		GeLine2d line = (GeLine2d&)crv;
		point.set(line.pointOnLine().x, line.pointOnLine().y);
		point.translation(line.direction() * line.length() * param);
	}
	else if (crv.type() == Ge::EntityId::kRay2d) {
		GeRay2d line = (GeRay2d&)crv;
		point.set(line.pointOnLine().x, line.pointOnLine().y);
		point.translation(line.direction() * line.length() * param);
	}
	else if (crv.type() == Ge::EntityId::kCircArc2d) {
		GeCircArc2d arc = (GeCircArc2d&)crv;
		point = arc.center() + (arc.refVec() * arc.radius());
		if (arc.isClockWise() == false) {
			point.rotateBy(param, arc.center());
		}
		else {
			point.rotateBy(0 - param, arc.center());
		}
	}
	else if (crv.type() == Ge::EntityId::kEllipArc2d) {
		GeEllipArc2d ellipArc = (GeEllipArc2d&)crv;
		GeVector2d majorAxis = ellipArc.majorAxis();
		GeVector2d minorAxis = ellipArc.minorAxis();
		double majorRadius = majorAxis.length();
		double minorRadius = minorAxis.length();
		if (majorRadius < GeContext::gTol.equalPoint() || minorRadius < GeContext::gTol.equalPoint()) {
			return ellipArc.center();
		}

		GeVector2d majorDir = majorAxis.normal();
		GeVector2d minorDir = minorAxis.normal();
		point = ellipArc.center()
			+ majorDir * (majorRadius * std::cos(param))
			+ minorDir * (minorRadius * std::sin(param));
	}

	return point;
}

GePointOnCurve2d& GePointOnCurve2d::setCurve(const GeCurve2d& crv) {
	GE_IMP_POINTONCURVE2D(this->m_pImpl)->pCurve = &crv;
	return *this;
}
GePointOnCurve2d& GePointOnCurve2d::setParameter(double param) {
	GE_IMP_POINTONCURVE2D(this->m_pImpl)->param = param;
	return *this;
}


bool GePointOnCurve2d::isKindOf(Ge::EntityId entType) const {
	return entType == Ge::EntityId::kEntity2d
		|| entType == Ge::EntityId::kPointEnt2d
		|| entType == this->type();
}
Ge::EntityId GePointOnCurve2d::type() const {
	return Ge::EntityId::kPointOnCurve2d;
}
GePointOnCurve2d* GePointOnCurve2d::copy() const {
	GePointOnCurve2d* cur = new GePointOnCurve2d(*this);
	return cur;
}
bool GePointOnCurve2d::operator == (const GePointOnCurve2d& entity) const {
	return this->isEqualTo(entity);
}
bool GePointOnCurve2d::operator != (const GePointOnCurve2d& entity) const {
	return !this->isEqualTo(entity);
}
bool GePointOnCurve2d::isEqualTo(const GePointOnCurve2d& entity) const {
	return this->isEqualTo(entity, GeContext::gTol);
}
bool GePointOnCurve2d::isEqualTo(const GePointOnCurve2d& entity, const GeTol& tol) const {
	if (std::fabs(this->parameter() - entity.parameter()) > tol.equalPoint()) {
		return false;
	}
	if (this->curve() != entity.curve()) {
		return false;
	}
	return true;
}
GePointOnCurve2d& GePointOnCurve2d::transformBy(const GeMatrix2d& xfm) {
	return *this;
}
GePointOnCurve2d& GePointOnCurve2d::translateBy(const GeVector2d& translateVec) {
	return *this;
}
GePointOnCurve2d& GePointOnCurve2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GePointOnCurve2d& GePointOnCurve2d::rotateBy(double angle, const GePoint2d& wrtPoint) {
	return *this;
}
GePointOnCurve2d& GePointOnCurve2d::mirror(const GeLine2d& line) {
	return *this;
}
GePointOnCurve2d& GePointOnCurve2d::scaleBy(double scaleFactor) {
	return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GePointOnCurve2d& GePointOnCurve2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint) {
	return *this;
}
bool GePointOnCurve2d::isOn(const GePoint2d& pnt) const {
	return this->isOn(pnt, GeContext::gTol);
}
bool GePointOnCurve2d::isOn(const GePoint2d& pnt, const GeTol& tol) const {
	if (this->curve() == nullptr) {
		return false;
	}
	GePointOnCurve2d cur(*this);
	return cur.point().isEqualTo(pnt, tol);
}


GePoint2d GePointOnCurve2d::point2d() const
{
	return this->point();
}
GePointOnCurve2d::operator GePoint2d () const {
	return this->point2d();
}
