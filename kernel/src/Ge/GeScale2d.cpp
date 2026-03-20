#include "GeScale2d.h"
#include "GeMatrix2d.h"
#include "GeScale3d.h"
#include <cmath>


const GeScale2d GeScale2d::kIdentity = GeScale2d();


GeScale2d::GeScale2d()
{
    this->sx = 1;
    this->sy = 1;
}
GeScale2d::GeScale2d(const GeScale2d& src)
{
    this->sx = src.sx;
    this->sy = src.sy;
}
GeScale2d::GeScale2d(double factor)
{
    this->sx = factor;
    this->sy = factor;
}
GeScale2d::GeScale2d(double xFactor, double yFactor)
{
    this->sx = xFactor;
    this->sy = yFactor;
}


// Multiplication.
GeScale2d operator * (double factor, const GeScale2d& scaleVec)
{
    GeScale2d Scale;
    Scale.sx = scaleVec.sx * factor;
    Scale.sy = scaleVec.sy * factor;
    return Scale;
}
GeScale2d GeScale2d::operator * (const GeScale2d& sclVec) const
{
    GeScale2d Scale;
    Scale.sx = this->sx * sclVec.sx;
    Scale.sy = this->sy * sclVec.sy;
    return Scale;
}
GeScale2d& GeScale2d::operator *= (const GeScale2d& scl)
{
    this->sx = this->sx * scl.sx;
    this->sy = this->sy * scl.sy;
    return *this;
}
GeScale2d& GeScale2d::preMultBy(const GeScale2d& leftSide)
{
    this->setToProduct(*this, leftSide);
    return *this;
}
GeScale2d& GeScale2d::postMultBy(const GeScale2d& rightSide)
{
    this->setToProduct(*this, rightSide);
    return *this;
}
GeScale2d& GeScale2d::setToProduct(const GeScale2d& sclVec1, const GeScale2d& sclVec2)
{
    this->sx = sclVec1.sx * sclVec2.sx;
    this->sy = sclVec1.sy * sclVec2.sy;
    return *this;
}
GeScale2d GeScale2d::operator * (double s) const
{
    GeScale2d Scale;
    Scale.sx = this->sx * s;
    Scale.sy = this->sy * s;
    return Scale;
}
GeScale2d& GeScale2d::operator *= (double s)
{
    this->sx = this->sx * s;
    this->sy = this->sy * s;
    return *this;
}
GeScale2d& GeScale2d::setToProduct(const GeScale2d& sclVec, double s)
{
    this->sx = sclVec.sx * s;
    this->sy = sclVec.sy * s;
    return *this;
}


// Multiplicative inverse.
GeScale2d GeScale2d::inverse() const
{
    GeScale2d scl(*this);
    scl.invert();
    return scl;
}
GeScale2d& GeScale2d::invert()
{
    this->sx = 0 - this->sx;
    this->sy = 0 - this->sy;
    return *this;
}

bool GeScale2d::isProportional() const {
    return this->isProportional(GeContext::gTol);
}
bool GeScale2d::isProportional(const GeTol& tol) const
{
    if (std::fabs(this->sx - this->sy) > tol.equalPoint())
    {
        return false;
    }
    return true;
}

// Tests for equivalence using the infinity norm.
bool GeScale2d::isEqualTo(const GeScale2d& scaleVec) const
{
    return this->isEqualTo(scaleVec, GeContext::gTol);
}
bool GeScale2d::isEqualTo(const GeScale2d& scaleVec, const GeTol& tol) const
{
    if (std::fabs(this->sx - scaleVec.sx) >= tol.equalPoint())
    {
        return false;
    }
    else if (std::fabs(this->sy - scaleVec.sy) >= tol.equalPoint())
    {
        return false;
    }
    return true;
}

GeScale2d& GeScale2d::set(double sc0, double sc1)
{
    this->sx = sc0;
    this->sy = sc1;
    return *this;
}

// Conversion to/from matrix form.
void GeScale2d::getMatrix(GeMatrix2d& mat) const
{
    mat.setToIdentity();
    mat.setToScaling(*this);
}
GeScale2d& GeScale2d::extractScale(const GeMatrix2d& mat)
{
    GeScale2d scl = mat.scale();
    this->sx = scl.sx;
    this->sy = scl.sy;
    return *this;
}
GeScale2d& GeScale2d::removeScale(GeMatrix2d& mat)
{
    mat.entry[0][0] = 1;
    mat.entry[1][1] = 1;
    return *this;
}
GeScale2d::operator GeMatrix2d () const {
    GeMatrix2d mat;
    this->getMatrix(mat);
    return mat;
}
GeScale2d::operator GeScale3d () const {
    GeScale3d s(this->sx, this->sy, 1);
    return s;
}

