#include "GeEntity2d.h"
#include "GeLine2d.h"
#include "GeRay2d.h"
#include "GeCircArc2d.h"
#include "GeLineSeg2d.h"
#include "GeMath.h"
#include "GeImpl.h"
#include <iostream>


GeEntity2d::GeEntity2d()
{
    this->m_pImpl = NULL;
}
GeEntity2d::GeEntity2d(const GeEntity2d& entity)
{
    this->m_pImpl = NULL;
    *this = entity;
}
GeEntity2d::GeEntity2d(GeImpEntity2d *pImpl)
{
    this->m_pImpl = NULL;
    this->connectTo(pImpl);
}
GeEntity2d::~GeEntity2d() {
    if (this->m_pImpl != NULL) {
        delete this->m_pImpl;
        this->m_pImpl = NULL;
    }
}
Ge::EntityId GeEntity2d::type()const
{
    return Ge::EntityId::kEntity2d;
}
bool GeEntity2d::isKindOf(Ge::EntityId entType) const {
    return entType == Ge::EntityId::kEntity2d || entType == this->type();
}

GeEntity2d* GeEntity2d::copy()const {
    return NULL;
}

GeEntity2d& GeEntity2d::operator =(const GeEntity2d& entity) {
    return *this;
}
bool GeEntity2d::operator ==(const GeEntity2d& entity) const {
    return this == &entity;
}
bool GeEntity2d::operator !=(const GeEntity2d& entity) const {
    return !(*this == entity);
}
bool GeEntity2d::isEqualTo(const GeEntity2d& entity) const {
    return this->isEqualTo(entity, GeContext::gTol);
}
bool GeEntity2d::isEqualTo(const GeEntity2d& other, const GeTol& tol) const {
    return this == &other;
}
GeEntity2d& GeEntity2d::transformBy(const GeMatrix2d& xfm) {
    return *this;
}
GeEntity2d& GeEntity2d::translateBy(const GeVector2d& translateVec) {
    return *this;
}
GeEntity2d& GeEntity2d::rotateBy(double angle) {
    return this->rotateBy(angle, GePoint2d::kOrigin);
}
GeEntity2d& GeEntity2d::rotateBy(double angle, const GePoint2d& basePoint) {
    return *this;
}
GeEntity2d& GeEntity2d::mirror(const GeLine2d& line) {
    return *this;
}
GeEntity2d& GeEntity2d::scaleBy(double scaleFactor) {
    return this->scaleBy(scaleFactor, GePoint2d::kOrigin);
}
GeEntity2d& GeEntity2d::scaleBy(double scaleFactor, const GePoint2d& basePoint) {
    return *this;
}
bool GeEntity2d::isOn(const GePoint2d& pnt) const {
    return this->isOn(pnt, GeContext::gTol);
}
bool GeEntity2d::isOn(const GePoint2d& point, const GeTol& tol) const {
    return false;
}
void GeEntity2d::connectTo(GeImpEntity2d* pImpl) {
    if (this->m_pImpl == pImpl) {
        return;
    }
    if (this->m_pImpl != NULL) {
        delete this->m_pImpl;
    }
    this->m_pImpl = pImpl;
}
