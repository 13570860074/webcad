#include "GeScale3d.h"
#include "GeMatrix3d.h"
#include <cmath>



const GeScale3d GeScale3d::kIdentity = GeScale3d(1, 1, 1);

GeScale3d::GeScale3d() {
    this->sx = 1.0;
    this->sy = 1.0;
    this->sz = 1.0;
}
GeScale3d::GeScale3d(const GeScale3d& src) {
    this->sx = src.sx;
    this->sy = src.sy;
    this->sz = src.sz;
}
GeScale3d::GeScale3d(double factor) {
    this->sx = factor;
    this->sy = factor;
    this->sz = factor;
}
GeScale3d::GeScale3d(double xFact, double yFact, double zFact) {
    this->sx = xFact;
    this->sy = yFact;
    this->sz = zFact;
}

GeScale3d GeScale3d::operator *  (const GeScale3d& sclVec) const {
    GeScale3d scale(*this);
    scale.sx *= sclVec.sx;
    scale.sy *= sclVec.sy;
    scale.sz *= sclVec.sz;
    return scale;
}
GeScale3d& GeScale3d::operator *= (const GeScale3d& scl) {
    this->sx = this->sx * scl.sx;
    this->sy = this->sy * scl.sy;
    this->sz = this->sz * scl.sz;
    return *this;
}
GeScale3d& GeScale3d::preMultBy(const GeScale3d& leftSide) {
    this->setToProduct(*this, leftSide);
    return *this;
}
GeScale3d& GeScale3d::postMultBy(const GeScale3d& rightSide) {
    this->setToProduct(*this, rightSide);
    return *this;
}
GeScale3d& GeScale3d::setToProduct(const GeScale3d& sclVec1, const GeScale3d& sclVec2) {
    this->sx = sclVec1.sx * sclVec2.sx;
    this->sy = sclVec1.sy * sclVec2.sy;
    this->sz = sclVec1.sz * sclVec2.sz;
    return *this;
}
GeScale3d GeScale3d::operator *  (double s) const {
    GeScale3d scale;
    scale.sx = this->sx * s;
    scale.sy = this->sy * s;
    scale.sz = this->sz * s;
    return scale;
}
GeScale3d& GeScale3d::operator *= (double s) {
    this->sx = this->sx * s;
    this->sy = this->sy * s;
    this->sz = this->sz * s;
    return *this;
}
GeScale3d& GeScale3d::setToProduct(const GeScale3d& sclVec, double s) {
    this->sx = sclVec.sx * s;
    this->sy = sclVec.sy * s;
    this->sz = sclVec.sz * s;
    return *this;
}
GeScale3d GeScale3d::inverse() const {
    GeScale3d scl(*this);
    scl.invert();
    return scl;
}
GeScale3d& GeScale3d::invert() {
    this->sx = 0 - this->sx;
    this->sy = 0 - this->sy;
    this->sz = 0 - this->sz;
    return *this;
}

bool GeScale3d::isProportional() const {
    return this->isProportional(GeContext::gTol);
}
bool GeScale3d::isProportional(const GeTol& tol) const {
    if (std::fabs(this->sx - this->sy) > tol.equalPoint())
    {
        return false;
    }
    if (std::fabs(this->sx - this->sz) > tol.equalPoint())
    {
        return false;
    }
    if (std::fabs(this->sy - this->sz) > tol.equalPoint())
    {
        return false;
    }
    return true;
}

bool GeScale3d::isEqualTo(const GeScale3d& scaleVec) const {
    return this->isEqualTo(scaleVec, GeContext::gTol);
}
bool GeScale3d::isEqualTo(const GeScale3d& scaleVec, const GeTol& tol) const {
    if (std::fabs(this->sx - scaleVec.sx) > tol.equalPoint()) {
        return false;
    }
    if (std::fabs(this->sy - scaleVec.sy) > tol.equalPoint()) {
        return false;
    }
    if (std::fabs(this->sz - scaleVec.sz) > tol.equalPoint()) {
        return false;
    }
    return true;
}

GeScale3d& GeScale3d::set(double sc0, double sc1, double sc2) {
    this->sx = sc0;
    this->sy = sc1;
    this->sz = sc2;
    return *this;
}
GeScale3d::operator GeMatrix3d () const {
    GeMatrix3d mat;
    this->getMatrix(mat);
    return mat;
}
void GeScale3d::getMatrix(GeMatrix3d& mat) const {
    mat.setToIdentity();
    mat.setToScaling(*this);
}
GeScale3d& GeScale3d::extractScale(const GeMatrix3d& mat) {
    GeScale3d scl = mat.scale();
    this->sx = scl.sx;
    this->sy = scl.sy;
    this->sz = scl.sz;
    return *this;
}
GeScale3d& GeScale3d::removeScale(GeMatrix3d& mat) {
    mat.entry[0][0] = 1;
    mat.entry[1][1] = 1;
    mat.entry[2][2] = 1;
    return *this;
}
