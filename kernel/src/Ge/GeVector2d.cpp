#include "GeVector2d.h"
#include "GeMatrix2d.h"


const GeVector2d GeVector2d::kIdentity = GeVector2d(0, 0); // Additive identity vector.
const GeVector2d GeVector2d::kXAxis = GeVector2d(1, 0);    // X-Axis vector.
const GeVector2d GeVector2d::kYAxis = GeVector2d(0, 1);    // Y-Axis vector.

GeVector2d operator* (const GeMatrix2d& xfm, const GeVector2d& vect) {
	GeVector2d vec;
	double aXresult = xfm.entry[0][0] * vect.x + xfm.entry[0][1] * vect.y;
	double anYresult = xfm.entry[1][0] * vect.x + xfm.entry[1][1] * vect.y;
	vec.x = aXresult;
	vec.y = anYresult;
	vec.x += xfm.entry[0][2];
	vec.y += xfm.entry[1][2];
	return vec;
}


GeVector2d& GeVector2d::rotateBy(double angle)
{
	GeMatrix2d mat;
	mat.setToRotation(angle);
	this->transformBy(mat);
	return *this;
}
GeVector2d& GeVector2d::mirror(const GeVector2d& line)
{
	GeMatrix2d mat;
	mat.setToMirroring(GePoint2d(line.x, line.y));
	this->transformBy(mat);
	return *this;
}
double GeVector2d::angle() const
{
	return this->angleTo(GeVector2d::kXAxis);
}
double GeVector2d::angleTo(const GeVector2d& vec) const
{
	double v1 = sqrt((vec.x - this->x) * (vec.x - this->x) + (vec.y - this->y) * (vec.y - this->y));
	double v2 = sqrt(this->x * this->x + this->y * this->y);
	double v3 = sqrt(vec.x * vec.x + vec.y * vec.y);
	if (abs(v1 - (v2 * v3)) < 0.000001)
	{
		return 0.0;
	}
	return acos((v2 * v2 + v3 * v3 - v1 * v1) / (2 * v2 * v3));
}
double GeVector2d::angleToCCW(const GeVector2d& vect) const {
	double angle = angleTo(vect);

	GeVector2d verThis(this->x, this->y);
	verThis.rotateBy(angle);
	if (verThis.isEqualTo(vect) == true) {
		return angle;
	}
	return PI * 2.0 - angle;
}
GeVector2d GeVector2d::normal() const {
	return this->normal(GeContext::gTol);
}
GeVector2d GeVector2d::normal(const GeTol& tol) const
{
	GeVector2d vector(this->x, this->y);
	vector.normalize(tol);
	return vector;
}
GeVector2d& GeVector2d::normalize() {
	return this->normalize(GeContext::gTol);
}
GeVector2d& GeVector2d::normalize(const GeTol& tol)
{
	double Max = this->x * this->x + this->y * this->y;
	Max = sqrt(Max);
	if (Max < GeContext::gTol.equalVector())
	{
		return *this;
	}
	this->x = this->x / Max;
	this->y = this->y / Max;
	return *this;
}
double GeVector2d::length() const
{
	double Var = this->x * this->x + this->y * this->y;
	return sqrt(Var);
}
bool GeVector2d::isZeroLength() const {
	return this->isZeroLength(GeContext::gTol);
}
bool GeVector2d::isZeroLength(const GeTol& tol) const
{
	if (this->length() < GeContext::gTol.equalVector())
	{
		return true;
	}
	return false;
}
bool GeVector2d::isParallelTo(const GeVector2d& vect) const {
	return this->isParallelTo(vect, GeContext::gTol);
}
bool GeVector2d::isParallelTo(const GeVector2d& vec,const GeTol& tol) const
{
	double Angle = this->angleTo(vec);
	if (abs(Angle) < tol.equalVector() || abs(Angle - PI) < tol.equalVector()) {
		return true;
	}
	return false;
}
bool GeVector2d::isCodirectionalTo(const GeVector2d& vect) const {
	return this->isCodirectionalTo(vect, GeContext::gTol);
}
bool GeVector2d::isCodirectionalTo(const GeVector2d& vec,const GeTol& tol) const
{
	GeVector2d v1(this->x, this->y);
	v1.normalize(tol);
	GeVector2d v2(vec.x, vec.y);
	v2.normalize(tol);
	return v1.isEqualTo(v2, tol);
}
bool GeVector2d::isPerpendicularTo(const GeVector2d& vect) const {
	return this->isPerpendicularTo(vect, GeContext::gTol);
}
bool GeVector2d::isPerpendicularTo(const GeVector2d& vec,const GeTol& tol) const
{
	double Angle = this->angleTo(vec);
	if (abs(Angle - PI / 2) < tol.equalVector() || abs(Angle - PI / 2 * 3) < tol.equalVector())
	{
		return true;
	}
	return false;
}
bool GeVector2d::isEqualTo(const GeVector2d& vect) const {
	return this->isEqualTo(vect, GeContext::gTol);
}
bool GeVector2d::isEqualTo(const GeVector2d& vec, const GeTol& tol) const
{
	if (abs(vec.x - this->x) > tol.equalVector()) {
		return false;
	}
	if (abs(vec.y - this->y) > tol.equalVector()) {
		return false;
	}
	return true;
}
bool GeVector2d::isUnitLength() const {
	return this->isUnitLength(GeContext::gTol);
}
bool GeVector2d::isUnitLength(const GeTol& tol) const
{
	if (this->length() < 1.0 + tol.equalVector())
	{
		return true;
	}
	return false;
}