#include "GePoint2d.h"
#include "GeMatrix2d.h"
#include "AcString.h"


const GePoint2d GePoint2d::kOrigin = GePoint2d(0, 0);


GePoint2d operator* (const GeMatrix2d& xfm, const GePoint2d& vect) {
	GePoint2d vec;
	vec.x = xfm.entry[0][0] * vect.x + xfm.entry[0][1] * vect.y + xfm.entry[0][2];
	vec.y = xfm.entry[1][0] * vect.x + xfm.entry[1][1] * vect.y + xfm.entry[1][2];
	return vec;
}

GePoint2d& GePoint2d::setToProduct(const GeMatrix2d& mat, const GePoint2d& pnt)
{
	*this = mat * pnt;
	return *this;
}
GePoint2d& GePoint2d::transformBy(const GeMatrix2d& leftSide)
{
	*this = leftSide * *this;
	return *this;
}
GePoint2d& GePoint2d::rotateBy(double angle) {
	return this->rotateBy(angle, GePoint2d::kOrigin);
}
GePoint2d& GePoint2d::rotateBy(double angle, const GePoint2d& wrtPoint)
{
	GeMatrix2d mat;
	mat.setToRotation(angle, wrtPoint);
	this->transformBy(mat);
	return *this;
}
GePoint2d& GePoint2d::mirror(const GeLine2d& line)
{
	GeMatrix2d mat;
	mat.setToMirroring(line);
	this->transformBy(mat);
	return *this;
}

GePoint2d& GePoint2d::scaleBy(const double scaleFactor) {
	GeMatrix2d mat;
	mat.setToScaling(scaleFactor);
	this->transformBy(mat);
	return *this;
}
GePoint2d& GePoint2d::scaleBy(double scaleFactor, const GePoint2d& wrtPoint)
{
	GeMatrix2d mat;
	mat.setToScaling(scaleFactor, wrtPoint);
	this->transformBy(mat);
	return *this;
}

GePoint2d& GePoint2d::scaleBy(const GeScale2d& scaleFactor) {
	GeMatrix2d mat;
	mat.setToScaling(scaleFactor);
	this->transformBy(mat);
	return *this;
}
GePoint2d& GePoint2d::scaleBy(const GeScale2d& scaleFactor, const GePoint2d& basePoint)
{
	GeMatrix2d mat;
	mat.setToScaling(scaleFactor, basePoint);
	this->transformBy(mat);
	return *this;
}


bool GePoint2d::isEqualTo(const GePoint2d& point) const {
	return this->isEqualTo(point, GeContext::gTol);
}
bool GePoint2d::isEqualTo(const GePoint2d& pnt, const GeTol& tol) const
{
	if (abs(pnt.x - this->x) > tol.equalPoint()) {
		return false;
	}
	if (abs(pnt.y - this->y) > tol.equalPoint()) {
		return false;
	}
	return true;
}