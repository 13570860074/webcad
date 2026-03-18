#include "GeEntity3d.h"
#include "GeLine2d.h"
#include "GeRay2d.h"
#include "GeCircArc2d.h"
#include "GeLineSeg2d.h"
#include "GeMath.h"
#include "GeImpl.h"



GeEntity3d::GeEntity3d()
{

}
GeEntity3d::GeEntity3d(const GeEntity3d& entity)
{
    *this = entity;
}
GeEntity3d::GeEntity3d(GeImpEntity3d *pImpl)
{
    if (pImpl != NULL)
    {
        this->m_pImpl = pImpl;
    }
}
GeEntity3d::~GeEntity3d() {
    if (this->m_pImpl != NULL) {
        delete this->m_pImpl;
        this->m_pImpl = NULL;
    }
}
Ge::EntityId GeEntity3d::type()const
{
    return Ge::EntityId::kEntity3d;
}
bool GeEntity3d::isKindOf(Ge::EntityId entType) const {
    return false;
}

GeEntity3d* GeEntity3d::copy()const {
    return NULL;
}

GeEntity3d& GeEntity3d::operator =(const GeEntity3d& entity) {
    return *this;
}
bool GeEntity3d::operator ==(const GeEntity3d& entity) const {
    return false;
}
bool GeEntity3d::operator !=(const GeEntity3d& entity) const {
    return false;
}
bool GeEntity3d::isEqualTo(const GeEntity3d& entity) const {
    return this->isEqualTo(entity, GeContext::gTol);
}
bool GeEntity3d::isEqualTo(const GeEntity3d& other, const GeTol& tol) const {
    return false;
}
GeEntity3d& GeEntity3d::transformBy(const GeMatrix3d& xfm) {
    return *this;
}
GeEntity3d& GeEntity3d::translateBy(const GeVector3d& translateVec) {
    return *this;
}
GeEntity3d& GeEntity3d::rotateBy(double angle, const GeVector3d& vec) {
    return this->rotateBy(angle, vec, GePoint3d::kOrigin);
}
GeEntity3d& GeEntity3d::rotateBy(double angle, const GeVector3d& vec, const GePoint3d& basePoint) {
    return *this;
}
GeEntity3d& GeEntity3d::mirror(const GePlane& plane) {
    return *this;
}
GeEntity3d& GeEntity3d::scaleBy(double scaleFactor) {
    return this->scaleBy(scaleFactor, GePoint3d::kOrigin);
}
GeEntity3d& GeEntity3d::scaleBy(double scaleFactor, const GePoint3d& basePoint) {
    return *this;
}
bool GeEntity3d::isOn(const GePoint3d& pnt) const {
    return this->isOn(pnt, GeContext::gTol);
}
bool GeEntity3d::isOn(const GePoint3d& point, const GeTol& tol) const {
    return false;
}
