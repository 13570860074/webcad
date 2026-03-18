#include "GeMatrix2d.h"
#include "GeScale2d.h"
#include "GeLine2d.h"


const GeMatrix2d GeMatrix2d::kIdentity = GeMatrix2d();


GeMatrix2d::GeMatrix2d()
{
	this->entry[0][0] = 1;
	this->entry[0][1] = 0;
	this->entry[0][2] = 0;

	this->entry[1][0] = 0;
	this->entry[1][1] = 1;
	this->entry[1][2] = 0;

	this->entry[2][0] = 0;
	this->entry[2][1] = 0;
	this->entry[2][2] = 1;
}
GeMatrix2d::GeMatrix2d(const GeMatrix2d& src)
{
	this->set(src);
}
GeMatrix2d& GeMatrix2d::operator =(const GeMatrix2d& src)
{
	this->set(src);
	return *this;
}

void GeMatrix2d::set(const GeMatrix2d& mat)
{
	for (int i = 0; i < 3; i++)
	{
		for (int u = 0; u < 3; u++)
		{
			this->entry[i][u] = mat.entry[i][u];
		}
	}
}
GeMatrix2d& GeMatrix2d::setToIdentity()
{
	this->entry[0][0] = 1;
	this->entry[0][1] = 0;
	this->entry[0][2] = 0;

	this->entry[1][0] = 0;
	this->entry[1][1] = 1;
	this->entry[1][2] = 0;

	this->entry[2][0] = 0;
	this->entry[2][1] = 0;
	this->entry[2][2] = 1;
	return *this;
}
bool GeMatrix2d::isEqualTo(const GeMatrix2d& mat) const {
	return this->isEqualTo(mat, GeContext::gTol);
}
bool GeMatrix2d::isEqualTo(const GeMatrix2d& mat, const GeTol& tol) const
{
	bool IsEqual = true;
	for (int i = 0; i < 3; i++)
	{
		for (int u = 0; u < 3; u++)
		{
			if (abs(this->entry[i][u] - mat.entry[i][u]) > GeContext::gTol.equalPoint())
			{
				IsEqual = false;
				break;
			}
		}
	}
	return IsEqual;
}

GeMatrix2d GeMatrix2d::operator*(const GeMatrix2d& mat) const
{
	GeMatrix2d Matrix = GeMatrix2d(*this);
	return Matrix.postMultBy(mat);
}
GeMatrix2d& GeMatrix2d::operator *= (const GeMatrix2d& mat)
{
	GeMatrix2d Matrix = GeMatrix2d(*this);
	Matrix.postMultBy(mat);
	this->set(Matrix);
	return *this;
}

GeMatrix2d& GeMatrix2d::preMultBy(const GeMatrix2d& leftSide)
{
	GeMatrix2d Matrix;
	Matrix.entry[0][0] = this->entry[0][0] * leftSide.entry[0][0] + this->entry[1][0] * leftSide.entry[0][1];
	Matrix.entry[0][1] = this->entry[0][1] * leftSide.entry[0][0] + this->entry[1][1] * leftSide.entry[0][1];
	Matrix.entry[0][2] = this->entry[0][2] * leftSide.entry[0][0] + this->entry[1][2] * leftSide.entry[0][1] + leftSide.entry[0][2];

	Matrix.entry[1][0] = this->entry[0][0] * leftSide.entry[1][0] + this->entry[1][0] * leftSide.entry[1][1];
	Matrix.entry[1][1] = this->entry[0][1] * leftSide.entry[1][0] + this->entry[1][1] * leftSide.entry[1][1];
	Matrix.entry[1][2] = this->entry[0][2] * leftSide.entry[1][0] + this->entry[1][2] * leftSide.entry[1][1] + leftSide.entry[1][2];

	Matrix.entry[2][0] = this->entry[0][0] * leftSide.entry[2][0] + this->entry[1][0] * leftSide.entry[2][1];
	Matrix.entry[2][1] = this->entry[0][1] * leftSide.entry[2][0] + this->entry[1][1] * leftSide.entry[2][1];
	Matrix.entry[2][2] = this->entry[0][2] * leftSide.entry[2][0] + this->entry[1][2] * leftSide.entry[2][1] + leftSide.entry[2][2];

	this->set(Matrix);
	return *this;
}
GeMatrix2d& GeMatrix2d::postMultBy(const GeMatrix2d& rightSide)
{
	GeMatrix2d Matrix;
	for (int i = 0; i < 3; i++) {
		for (int u = 0; u < 3; u++) {
			Matrix.entry[i][u] =
				this->entry[i][0] * rightSide.entry[0][u] +
				this->entry[i][1] * rightSide.entry[1][u] +
				this->entry[i][2] * rightSide.entry[2][u];
		}
	}
	this->set(Matrix);
	return *this;
}
GeMatrix2d& GeMatrix2d::setToProduct(const GeMatrix2d& mat1, const GeMatrix2d& mat2)
{
	this->setToIdentity();
	*this = mat1 * mat2;
	return *this;
}

GeMatrix2d& GeMatrix2d::invert()
{
	double aNewMat[2][2];
	aNewMat[0][0] = this->entry[1][1];
	aNewMat[0][1] = -this->entry[0][1];
	aNewMat[1][0] = -this->entry[1][0];
	aNewMat[1][1] = this->entry[0][0];
	double aDet = aNewMat[0][0] * aNewMat[1][1] - aNewMat[0][1] * aNewMat[1][0];
	double val = aDet;
	if (val < 0)
	{
		val = -val;
	}
	aDet = 1.0 / aDet;
	this->entry[0][0] = aNewMat[0][0] * aDet;
	this->entry[1][0] = aNewMat[1][0] * aDet;
	this->entry[0][1] = aNewMat[0][1] * aDet;
	this->entry[1][1] = aNewMat[1][1] * aDet;

	double x = this->entry[0][0] * this->entry[0][2] + this->entry[0][1] * this->entry[1][2];
	double y = this->entry[1][0] * this->entry[0][2] + this->entry[1][1] * this->entry[1][2];
	this->entry[0][2] = 0 - x;
	this->entry[1][2] = 0 - y;
	return *this;
}
GeMatrix2d GeMatrix2d::inverse() const
{
	GeMatrix2d aNewMat = *this;
	aNewMat.invert();
	return aNewMat;
}
bool GeMatrix2d::isSingular() const {
	return this->isSingular(GeContext::gTol);
}
bool GeMatrix2d::isSingular(const GeTol& tol) const
{
	double aDet = this->entry[0][0] * this->entry[1][1] - this->entry[1][0] * this->entry[0][1];
	if (aDet < 0)
	{
		aDet = -aDet;
	}
	return aDet <= (2.2250738585072014e-308 + tol.equalPoint());
}

GeMatrix2d& GeMatrix2d::transposeIt() {
	GeMatrix2d mat(*this);

	this->entry[0][0] = mat.entry[0][0];
	this->entry[1][0] = mat.entry[0][1];
	this->entry[2][0] = mat.entry[0][2];

	this->entry[0][1] = mat.entry[1][0];
	this->entry[1][1] = mat.entry[1][1];
	this->entry[2][1] = mat.entry[1][2];

	this->entry[0][2] = mat.entry[2][0];
	this->entry[1][2] = mat.entry[2][1];
	this->entry[2][2] = mat.entry[2][2];

	return *this;
}
GeMatrix2d GeMatrix2d::transpose() const {
	GeMatrix2d mat(*this);
	mat.transposeIt();
	return mat;
}

GeScale2d GeMatrix2d::scale() const
{
	return GeScale2d(this->entry[0][0], this->entry[1][1]);
}

double GeMatrix2d::det() const
{
	return this->entry[0][0] * this->entry[1][1] - this->entry[1][0] * this->entry[0][1];
}

GeMatrix2d& GeMatrix2d::setTranslation(const GeVector2d& vec)
{
	this->entry[0][2] += vec.x;
	this->entry[1][2] += vec.y;
	return *this;
}
GeVector2d GeMatrix2d::translation() const
{
	return GeVector2d(this->entry[0][2], this->entry[1][2]);
}

GeMatrix2d& GeMatrix2d::setCoordSystem(const GePoint2d& origin, const GeVector2d& e0, const GeVector2d& e1)
{
	this->setToIdentity();
	this->entry[0][0] = e0.x;
	this->entry[1][0] = e0.y;
	this->entry[0][1] = e1.x;
	this->entry[1][1] = e1.y;
	this->setTranslation(origin.asVector());
	return *this;
}
void GeMatrix2d::getCoordSystem(GePoint2d& origin, GeVector2d& e0, GeVector2d& e1) const
{
	e0.x = this->entry[0][0];
	e0.y = this->entry[1][0];
	e1.x = this->entry[0][1];
	e1.y = this->entry[1][1];
	origin.x = this->entry[0][2];
	origin.y = this->entry[1][2];
}

GeMatrix2d& GeMatrix2d::setToTranslation(const GeVector2d& vec)
{
	this->setToIdentity();
	this->entry[0][2] = vec.x;
	this->entry[1][2] = vec.y;
	return *this;
}
GeMatrix2d& GeMatrix2d::setToRotation(double angle) {
	return this->setToRotation(angle, GePoint2d::kOrigin);
}
GeMatrix2d& GeMatrix2d::setToRotation(double angle, const GePoint2d& center)
{
	this->setToIdentity();
	this->entry[0][0] = cos(angle);
	this->entry[0][1] = 0 - sin(angle);
	this->entry[1][0] = sin(angle);
	this->entry[1][1] = cos(angle);
	this->entry[0][2] = (-center.x) * this->entry[0][0] + (-center.y) * this->entry[0][1] + center.x;
	this->entry[1][2] = (-center.x) * this->entry[1][0] + (-center.y) * this->entry[1][1] + center.y;
	return *this;
}
GeMatrix2d& GeMatrix2d::setToScaling(const GeScale2d& scaleAll) {
	this->setToScaling(scaleAll, GePoint2d::kOrigin);
	return *this;
}
GeMatrix2d& GeMatrix2d::setToScaling(const GeScale2d& scaleAll, const GePoint2d& center)
{
	this->setToIdentity();
	this->entry[0][0] = scaleAll.sx;
	this->entry[1][1] = scaleAll.sy;
	this->entry[0][1] = 0.0;
	this->entry[1][0] = 0.0;
	this->entry[0][2] = center.x - center.x * scaleAll.sx;
	this->entry[1][2] = center.y - center.y * scaleAll.sy;
	return *this;
}
GeMatrix2d& GeMatrix2d::setToScaling(double scaleAll) {
	return this->setToScaling(scaleAll, GePoint2d::kOrigin);
}
GeMatrix2d& GeMatrix2d::setToScaling(double scaleAll, const GePoint2d& center)
{
	this->setToIdentity();
	this->entry[0][0] = scaleAll;
	this->entry[1][1] = scaleAll;
	this->entry[0][1] = 0.0;
	this->entry[1][0] = 0.0;
	this->entry[0][2] = center.x - center.x * scaleAll;
	this->entry[1][2] = center.y - center.y * scaleAll;
	return *this;
}
GeMatrix2d& GeMatrix2d::setToMirroring(const GePoint2d& pnt)
{
	this->setToIdentity();
	this->entry[0][0] = -1;
	this->entry[1][1] = -1;
	this->entry[0][2] = pnt.x * 2;
	this->entry[1][2] = pnt.y * 2;
	return *this;
}
GeMatrix2d& GeMatrix2d::setToMirroring(const GeLine2d& line)
{
	const GeVector2d& V = line.direction();
	const GePoint2d& P = line.pointOnLine();
	double VX = V.x;
	double VY = V.y;
	double X0 = P.x;
	double Y0 = P.y;
	this->entry[0][0] = 0 - (1.0 - 2.0 * VX * VX);
	this->entry[1][0] = 0 - (-2.0 * VX * VY);
	this->entry[0][1] = 0 - (-2.0 * VX * VY);
	this->entry[1][1] = 0 - (1.0 - 2.0 * VY * VY);

	this->entry[0][2] = -2.0 * ((VX * VX - 1.0) * X0 + (VX * VY * Y0));
	this->entry[1][2] = -2.0 * ((VX * VY * X0) + (VY * VY - 1.0) * Y0);
	return *this;
}
GeMatrix2d& GeMatrix2d::setToAlignCoordSys(const GePoint2d& fromOrigin,
	const GeVector2d& fromE0,
	const GeVector2d& fromE1,
	const GePoint2d& toOrigin,
	const GeVector2d& toE0,
	const GeVector2d& toE1)
{
	this->setCoordSystem(fromOrigin, fromE0, fromE1);
	this->invert();

	GeMatrix2d mat2;
	mat2.setCoordSystem(toOrigin, toE0, toE1);

	this->preMultBy(mat2);
	return *this;
}

// Functions that make a 2d transformation matrix using various approaches
GeMatrix2d GeMatrix2d::translation(const GeVector2d& vec)
{
	GeMatrix2d Matrix;
	Matrix.setToTranslation(vec);
	return Matrix;
}
GeMatrix2d GeMatrix2d::rotation(double angle) {
	return GeMatrix2d::rotation(angle, GePoint2d::kOrigin);
}
GeMatrix2d GeMatrix2d::rotation(double angle, const GePoint2d& center)
{
	GeMatrix2d Matrix;
	Matrix.setToRotation(angle, center);
	return Matrix;
}
GeMatrix2d scaling(const GeScale2d& scale, const GePoint2d& center = GePoint2d::kOrigin)
{
	GeMatrix2d Matrix;
	Matrix.setToScaling(scale, center);
	return Matrix;
}
GeMatrix2d GeMatrix2d::scaling(double scaleAll) {
	return GeMatrix2d::scaling(scaleAll, GePoint2d::kOrigin);
}
GeMatrix2d GeMatrix2d::scaling(double scaleAll, const GePoint2d& center)
{
	GeMatrix2d Matrix;
	Matrix.setToScaling(scaleAll, center);
	return Matrix;
}
GeMatrix2d GeMatrix2d::scaling(const GeScale2d& scaleAll) {
	return GeMatrix2d::scaling(scaleAll, GePoint2d::kOrigin);
}
GeMatrix2d GeMatrix2d::scaling(const GeScale2d& scaleAll, const GePoint2d& center) {
	GeMatrix2d Matrix;
	Matrix.setToScaling(scaleAll, center);
	return Matrix;
}
GeMatrix2d GeMatrix2d::mirroring(const GePoint2d& pnt)
{
	GeMatrix2d Matrix;
	Matrix.setToMirroring(pnt);
	return Matrix;
}
GeMatrix2d GeMatrix2d::mirroring(const GeLine2d& line)
{
	GeMatrix2d Matrix;
	Matrix.setToMirroring(line);
	return Matrix;
}
GeMatrix2d GeMatrix2d::alignCoordSys(const GePoint2d& fromOrigin,
	const GeVector2d& fromE0,
	const GeVector2d& fromE1,
	const GePoint2d& toOrigin,
	const GeVector2d& toE0,
	const GeVector2d& toE1)
{
	GeMatrix2d Matrix;
	Matrix.setToAlignCoordSys(fromOrigin, fromE0, fromE1, toOrigin, toE0, toE1);
	return Matrix;
}
