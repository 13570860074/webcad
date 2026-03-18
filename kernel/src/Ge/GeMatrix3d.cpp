#include "GeMatrix3d.h"
#include "GeScale3d.h"
#include "GeLine3d.h"
#include "GePlane.h"

const GeMatrix3d GeMatrix3d::kIdentity = GeMatrix3d();

GeMatrix3d::GeMatrix3d()
{
    this->setToIdentity();
}
GeMatrix3d::GeMatrix3d(const GeMatrix3d &src)
{
    for (int i = 0; i < 4; i++)
    {
        for (int u = 0; u < 4; u++)
        {
            this->entry[i][u] = src.entry[i][u];
        }
    }
}

void GeMatrix3d::set(const GeMatrix3d &mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int u = 0; u < 4; u++)
        {
            this->entry[i][u] = mat.entry[i][u];
        }
    }
}
double GeMatrix3d::get(int row, int col) const
{
    return this->entry[row][col];
}

GeMatrix3d &GeMatrix3d::setToIdentity()
{
    this->entry[0][0] = 1;
    this->entry[0][1] = 0;
    this->entry[0][2] = 0;
    this->entry[0][3] = 0;

    this->entry[1][0] = 0;
    this->entry[1][1] = 1;
    this->entry[1][2] = 0;
    this->entry[1][3] = 0;

    this->entry[2][0] = 0;
    this->entry[2][1] = 0;
    this->entry[2][2] = 1;
    this->entry[2][3] = 0;

    this->entry[3][0] = 0;
    this->entry[3][1] = 0;
    this->entry[3][2] = 0;
    this->entry[3][3] = 1;

    return *this;
}

GeMatrix3d GeMatrix3d::operator*(const GeMatrix3d &matrix) const
{
    GeMatrix3d result;
    result.setToProduct(*this, matrix);
    return result;
}
GeMatrix3d &GeMatrix3d::operator*=(const GeMatrix3d &matrix)
{
    return this->setToProduct(*this, matrix);
}

GeMatrix3d &GeMatrix3d::operator=(const GeMatrix3d &src)
{
    this->set(src);
    return *this;
}

GeMatrix3d &GeMatrix3d::preMultBy(const GeMatrix3d &leftSide)
{
    return this->setToProduct(leftSide, *this);
}
GeMatrix3d &GeMatrix3d::postMultBy(const GeMatrix3d &rightSide)
{
    return this->setToProduct(*this, rightSide);
}
GeMatrix3d &GeMatrix3d::setToProduct(const GeMatrix3d &matrix1, const GeMatrix3d &matrix2)
{
    GeMatrix3d result;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.entry[row][col] =
                matrix1.entry[row][0] * matrix2.entry[0][col] +
                matrix1.entry[row][1] * matrix2.entry[1][col] +
                matrix1.entry[row][2] * matrix2.entry[2][col] +
                matrix1.entry[row][3] * matrix2.entry[3][col];
        }
    }
    this->set(result);
    return *this;
}
GeMatrix3d &GeMatrix3d::invert()
{
    double aNewMat[3][3];

    // calcul de  la transposee de la commatrice
    aNewMat[0][0] = this->entry[1][1] * this->entry[2][2] - this->entry[1][2] * this->entry[2][1];
    aNewMat[1][0] = -(this->entry[1][0] * this->entry[2][2] - this->entry[2][0] * this->entry[1][2]);
    aNewMat[2][0] = this->entry[1][0] * this->entry[2][1] - this->entry[2][0] * this->entry[1][1];
    aNewMat[0][1] = -(this->entry[0][1] * this->entry[2][2] - this->entry[2][1] * this->entry[0][2]);
    aNewMat[1][1] = this->entry[0][0] * this->entry[2][2] - this->entry[2][0] * this->entry[0][2];
    aNewMat[2][1] = -(this->entry[0][0] * this->entry[2][1] - this->entry[2][0] * this->entry[0][1]);
    aNewMat[0][2] = this->entry[0][1] * this->entry[1][2] - this->entry[1][1] * this->entry[0][2];
    aNewMat[1][2] = -(this->entry[0][0] * this->entry[1][2] - this->entry[1][0] * this->entry[0][2]);
    aNewMat[2][2] = this->entry[0][0] * this->entry[1][1] - this->entry[0][1] * this->entry[1][0];
    double aDet = this->entry[0][0] * aNewMat[0][0] + this->entry[0][1] * aNewMat[1][0] + this->entry[0][2] * aNewMat[2][0];
    double aVal = aDet;
    if (aVal < 0)
    {
        aVal = -aVal;
    }

    aDet = 1.0e0 / aDet;
    this->setToIdentity();
    this->entry[0][0] = aNewMat[0][0];
    this->entry[1][0] = aNewMat[1][0];
    this->entry[2][0] = aNewMat[2][0];
    this->entry[0][1] = aNewMat[0][1];
    this->entry[1][1] = aNewMat[1][1];
    this->entry[2][1] = aNewMat[2][1];
    this->entry[0][2] = aNewMat[0][2];
    this->entry[1][2] = aNewMat[1][2];
    this->entry[2][2] = aNewMat[2][2];
    this->entry[0][0] *= aDet;
    this->entry[0][1] *= aDet;
    this->entry[0][2] *= aDet;
    this->entry[1][0] *= aDet;
    this->entry[1][1] *= aDet;
    this->entry[1][2] *= aDet;
    this->entry[2][0] *= aDet;
    this->entry[2][1] *= aDet;
    this->entry[2][2] *= aDet;

    double x = this->entry[0][0] * this->entry[0][3] + this->entry[0][1] * this->entry[1][3] + this->entry[0][2] * this->entry[2][3];
    double y = this->entry[1][0] * this->entry[0][3] + this->entry[1][1] * this->entry[1][3] + this->entry[1][2] * this->entry[2][3];
    double z = this->entry[2][0] * this->entry[0][3] + this->entry[2][1] * this->entry[1][3] + this->entry[2][2] * this->entry[2][3];
    this->entry[0][3] = 0 - x;
    this->entry[1][3] = 0 - y;
    this->entry[2][3] = 0 - z;
    return *this;
}
GeMatrix3d GeMatrix3d::inverse() const
{
    return this->inverse(GeContext::gTol);
}
GeMatrix3d GeMatrix3d::inverse(const GeTol &tol) const
{
    GeMatrix3d mat;
    this->inverse(mat, tol.equalPoint());
    return mat;
}
bool GeMatrix3d::inverse(GeMatrix3d &inverseMatrix, double tol) const
{
    bool isError = true;

    double aNewMat[3][3];

    // calcul de  la transposee de la commatrice
    aNewMat[0][0] = this->entry[1][1] * this->entry[2][2] - this->entry[1][2] * this->entry[2][1];
    aNewMat[1][0] = -(this->entry[1][0] * this->entry[2][2] - this->entry[2][0] * this->entry[1][2]);
    aNewMat[2][0] = this->entry[1][0] * this->entry[2][1] - this->entry[2][0] * this->entry[1][1];
    aNewMat[0][1] = -(this->entry[0][1] * this->entry[2][2] - this->entry[2][1] * this->entry[0][2]);
    aNewMat[1][1] = this->entry[0][0] * this->entry[2][2] - this->entry[2][0] * this->entry[0][2];
    aNewMat[2][1] = -(this->entry[0][0] * this->entry[2][1] - this->entry[2][0] * this->entry[0][1]);
    aNewMat[0][2] = this->entry[0][1] * this->entry[1][2] - this->entry[1][1] * this->entry[0][2];
    aNewMat[1][2] = -(this->entry[0][0] * this->entry[1][2] - this->entry[1][0] * this->entry[0][2]);
    aNewMat[2][2] = this->entry[0][0] * this->entry[1][1] - this->entry[0][1] * this->entry[1][0];
    double aDet = this->entry[0][0] * aNewMat[0][0] + this->entry[0][1] * aNewMat[1][0] + this->entry[0][2] * aNewMat[2][0];
    double aVal = aDet;
    if (aVal < 0)
    {
        aVal = -aVal;
    }
    if (aVal <= 2.2250738585072014e-308 - tol)
    {
        isError = false;
    }

    aDet = 1.0e0 / aDet;
    inverseMatrix.setToIdentity();
    inverseMatrix.entry[0][0] = aNewMat[0][0];
    inverseMatrix.entry[1][0] = aNewMat[1][0];
    inverseMatrix.entry[2][0] = aNewMat[2][0];
    inverseMatrix.entry[0][1] = aNewMat[0][1];
    inverseMatrix.entry[1][1] = aNewMat[1][1];
    inverseMatrix.entry[2][1] = aNewMat[2][1];
    inverseMatrix.entry[0][2] = aNewMat[0][2];
    inverseMatrix.entry[1][2] = aNewMat[1][2];
    inverseMatrix.entry[2][2] = aNewMat[2][2];
    inverseMatrix.entry[0][0] *= aDet;
    inverseMatrix.entry[0][1] *= aDet;
    inverseMatrix.entry[0][2] *= aDet;
    inverseMatrix.entry[1][0] *= aDet;
    inverseMatrix.entry[1][1] *= aDet;
    inverseMatrix.entry[1][2] *= aDet;
    inverseMatrix.entry[2][0] *= aDet;
    inverseMatrix.entry[2][1] *= aDet;
    inverseMatrix.entry[2][2] *= aDet;

    double x = this->entry[0][0] * this->entry[0][3] + this->entry[0][1] * this->entry[1][3] + this->entry[0][2] * this->entry[2][3];
    double y = this->entry[1][0] * this->entry[0][3] + this->entry[1][1] * this->entry[1][3] + this->entry[1][2] * this->entry[2][3];
    double z = this->entry[2][0] * this->entry[0][3] + this->entry[2][1] * this->entry[1][3] + this->entry[2][2] * this->entry[2][3];
    inverseMatrix.entry[0][3] = 0 - x;
    inverseMatrix.entry[1][3] = 0 - y;
    inverseMatrix.entry[2][3] = 0 - z;
    return isError;
}
bool GeMatrix3d::isSingular() const
{
    return this->isSingular(GeContext::gTol);
}
bool GeMatrix3d::isSingular(const GeTol &tol) const
{
    return abs(this->det()) <= tol.equalPoint();
}
GeMatrix3d &GeMatrix3d::transposeIt()
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = row + 1; col < 4; col++)
        {
            double temp = this->entry[row][col];
            this->entry[row][col] = this->entry[col][row];
            this->entry[col][row] = temp;
        }
    }
    return *this;
}
GeMatrix3d GeMatrix3d::transpose() const
{
    GeMatrix3d result;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.entry[row][col] = this->entry[col][row];
        }
    }
    return result;
}
bool GeMatrix3d::isPerspective() const
{
    return this->isPerspective(GeContext::gTol);
}
bool GeMatrix3d::isPerspective(const GeTol &tol) const
{
    return abs(this->entry[3][0]) > tol.equalVector() ||
        abs(this->entry[3][1]) > tol.equalVector() ||
        abs(this->entry[3][2]) > tol.equalVector();
}
bool GeMatrix3d::operator==(const GeMatrix3d &matrix) const
{
    return this->isEqualTo(matrix);
}
bool GeMatrix3d::operator!=(const GeMatrix3d &matrix) const
{
    return !this->isEqualTo(matrix);
}
bool GeMatrix3d::isEqualTo(const GeMatrix3d &matrix) const
{
    return this->isEqualTo(matrix, GeContext::gTol);
}
bool GeMatrix3d::isEqualTo(const GeMatrix3d &matrix, const GeTol &tol) const
{
    bool IsEqual = true;
    for (int i = 0; i < 4; i++)
    {
        for (int u = 0; u < 4; u++)
        {
            if (abs(this->entry[i][u] - matrix.entry[i][u]) > tol.equalVector())
            {
                IsEqual = false;
                break;
            }
        }
        if (IsEqual == false)
        {
            break;
        }
    }
    return IsEqual;
}
GeMatrix3d &GeMatrix3d::setTranslation(const GeVector3d &vect)
{
    this->entry[0][3] = vect.x;
    this->entry[1][3] = vect.y;
    this->entry[2][3] = vect.z;
    return *this;
}
GeMatrix3d &GeMatrix3d::setCoordSystem(const GePoint3d &origin, const GeVector3d &xAxis, const GeVector3d &yAxis, const GeVector3d &zAxis)
{
    GeMatrix3d mat = this->setToAlignCoordSys(
        GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
        origin, xAxis, yAxis, zAxis);
    this->set(mat);
    return *this;
}
void GeMatrix3d::getCoordSystem(GePoint3d &origin, GeVector3d &xAxis, GeVector3d &yAxis, GeVector3d &zAxis) const
{
    GeVector3d vec = this->translation();
    origin = GePoint3d(vec.x, vec.y, vec.z);

    GeMatrix3d mat = GeMatrix3d(*this);
    mat.entry[0][3] = 0;
    mat.entry[1][3] = 0;
    mat.entry[2][3] = 0;

    xAxis = GeVector3d(GeVector3d::kXAxis);
    xAxis.transformBy(mat);
    xAxis.normalize();

    yAxis = GeVector3d(GeVector3d::kYAxis);
    yAxis.transformBy(mat);
    yAxis.normalize();

    zAxis = GeVector3d(GeVector3d::kZAxis);
    zAxis.transformBy(mat);
    zAxis.normalize();
}
GePoint3d GeMatrix3d::getCsOrigin() const
{
    GeVector3d vec;
    this->translation(vec);
    return GePoint3d(vec.x, vec.y, vec.z);
}
GeVector3d GeMatrix3d::getCsXAxis() const
{
    GeMatrix3d mat = GeMatrix3d(*this);
    mat.entry[0][3] = 0;
    mat.entry[1][3] = 0;
    mat.entry[2][3] = 0;

    GeVector3d Axis = GeVector3d(GeVector3d::kXAxis);
    Axis.transformBy(mat);
    Axis.normalize();
    return Axis;
}
GeVector3d GeMatrix3d::getCsYAxis() const
{
    GeMatrix3d mat = GeMatrix3d(*this);
    mat.entry[0][3] = 0;
    mat.entry[1][3] = 0;
    mat.entry[2][3] = 0;

    GeVector3d Axis = GeVector3d(GeVector3d::kYAxis);
    Axis.transformBy(mat);
    Axis.normalize();
    return Axis;
}
GeVector3d GeMatrix3d::getCsZAxis() const
{
    GeMatrix3d mat = GeMatrix3d(*this);
    mat.entry[0][3] = 0;
    mat.entry[1][3] = 0;
    mat.entry[2][3] = 0;

    GeVector3d Axis = GeVector3d(GeVector3d::kZAxis);
    Axis.transformBy(mat);
    Axis.normalize();
    return Axis;
}
double GeMatrix3d::det() const
{
    return this->entry[0][0] * (this->entry[1][1] * this->entry[2][2] - this->entry[2][1] * this->entry[1][2]) -
           this->entry[0][1] * (this->entry[1][0] * this->entry[2][2] - this->entry[2][0] * this->entry[1][2]) +
           this->entry[0][2] * (this->entry[1][0] * this->entry[2][1] - this->entry[2][0] * this->entry[1][1]);
}
GeMatrix3d &GeMatrix3d::setToTranslation(const GeVector3d &vect)
{
    this->setToIdentity();
    this->entry[0][3] = vect.x;
    this->entry[1][3] = vect.y;
    this->entry[2][3] = vect.z;
    return *this;
}
GeMatrix3d &GeMatrix3d::setToRotation(double angle, const GeVector3d &axis)
{
    return this->setToRotation(angle, axis, GePoint3d::kOrigin);
}



GeMatrix3d &GeMatrix3d::setToRotation(double angle, const GeVector3d &axis, const GePoint3d &center)
{
    GeMatrix3d Matrix3d;

    GeVector3d TempNormal = axis.normal();
    Matrix3d.entry[0][0] = 0;
    Matrix3d.entry[1][1] = 0;
    Matrix3d.entry[2][2] = 0;
    Matrix3d.entry[0][1] = 0 - TempNormal.z;
    Matrix3d.entry[0][2] = TempNormal.y;
    Matrix3d.entry[1][2] = 0 - TempNormal.x;
    Matrix3d.entry[1][0] = TempNormal.z;
    Matrix3d.entry[2][0] = 0 - TempNormal.y;
    Matrix3d.entry[2][1] = TempNormal.x;

    double Sin = sin(angle);
    Matrix3d.entry[0][0] *= Sin;
    Matrix3d.entry[0][1] *= Sin;
    Matrix3d.entry[0][2] *= Sin;
    Matrix3d.entry[1][0] *= Sin;
    Matrix3d.entry[1][1] *= Sin;
    Matrix3d.entry[1][2] *= Sin;
    Matrix3d.entry[2][0] *= Sin;
    Matrix3d.entry[2][1] *= Sin;
    Matrix3d.entry[2][2] *= Sin;

    GeMatrix3d TempMatrix;
    Matrix3d.entry[0][0] += TempMatrix.entry[0][0];
    Matrix3d.entry[0][1] += TempMatrix.entry[0][1];
    Matrix3d.entry[0][2] += TempMatrix.entry[0][2];
    Matrix3d.entry[0][3] += TempMatrix.entry[0][3];
    Matrix3d.entry[1][0] += TempMatrix.entry[1][0];
    Matrix3d.entry[1][1] += TempMatrix.entry[1][1];
    Matrix3d.entry[1][2] += TempMatrix.entry[1][2];
    Matrix3d.entry[1][3] += TempMatrix.entry[1][3];
    Matrix3d.entry[2][0] += TempMatrix.entry[2][0];
    Matrix3d.entry[2][1] += TempMatrix.entry[2][1];
    Matrix3d.entry[2][2] += TempMatrix.entry[2][2];
    Matrix3d.entry[2][3] += TempMatrix.entry[2][3];
    Matrix3d.entry[3][0] += TempMatrix.entry[3][0];
    Matrix3d.entry[3][1] += TempMatrix.entry[3][1];
    Matrix3d.entry[3][2] += TempMatrix.entry[3][2];

    TempMatrix.entry[0][0] = -TempNormal.z * TempNormal.z - TempNormal.y * TempNormal.y;
    TempMatrix.entry[0][1] = TempNormal.x * TempNormal.y;
    TempMatrix.entry[0][2] = TempNormal.x * TempNormal.z;
    TempMatrix.entry[1][0] = TempNormal.x * TempNormal.y;
    TempMatrix.entry[1][1] = -TempNormal.x * TempNormal.x - TempNormal.z * TempNormal.z;
    TempMatrix.entry[1][2] = TempNormal.y * TempNormal.z;
    TempMatrix.entry[2][0] = TempNormal.x * TempNormal.z;
    TempMatrix.entry[2][1] = TempNormal.y * TempNormal.z;
    TempMatrix.entry[2][2] = -TempNormal.x * TempNormal.x - TempNormal.y * TempNormal.y;

    double Cos = 1.0 - cos(angle);
    TempMatrix.entry[0][0] *= Cos;
    TempMatrix.entry[0][1] *= Cos;
    TempMatrix.entry[0][2] *= Cos;
    TempMatrix.entry[1][0] *= Cos;
    TempMatrix.entry[1][1] *= Cos;
    TempMatrix.entry[1][2] *= Cos;
    TempMatrix.entry[2][0] *= Cos;
    TempMatrix.entry[2][1] *= Cos;
    TempMatrix.entry[2][2] *= Cos;
    Matrix3d.entry[0][0] += TempMatrix.entry[0][0];
    Matrix3d.entry[0][1] += TempMatrix.entry[0][1];
    Matrix3d.entry[0][2] += TempMatrix.entry[0][2];
    Matrix3d.entry[0][3] += TempMatrix.entry[0][3];
    Matrix3d.entry[1][0] += TempMatrix.entry[1][0];
    Matrix3d.entry[1][1] += TempMatrix.entry[1][1];
    Matrix3d.entry[1][2] += TempMatrix.entry[1][2];
    Matrix3d.entry[1][3] += TempMatrix.entry[1][3];
    Matrix3d.entry[2][0] += TempMatrix.entry[2][0];
    Matrix3d.entry[2][1] += TempMatrix.entry[2][1];
    Matrix3d.entry[2][2] += TempMatrix.entry[2][2];
    Matrix3d.entry[2][3] += TempMatrix.entry[2][3];
    Matrix3d.entry[3][0] += TempMatrix.entry[3][0];
    Matrix3d.entry[3][1] += TempMatrix.entry[3][1];
    Matrix3d.entry[3][2] += TempMatrix.entry[3][2];

    Matrix3d.entry[0][3] = (-center.x) * Matrix3d.entry[0][0] + (-center.y) * Matrix3d.entry[0][1] + (-center.z) * Matrix3d.entry[0][2] + center.x;
    Matrix3d.entry[1][3] = (-center.x) * Matrix3d.entry[1][0] + (-center.y) * Matrix3d.entry[1][1] + (-center.z) * Matrix3d.entry[1][2] + center.y;
    Matrix3d.entry[2][3] = (-center.x) * Matrix3d.entry[2][0] + (-center.y) * Matrix3d.entry[2][1] + (-center.z) * Matrix3d.entry[2][2] + center.z;
    this->set(Matrix3d);
    return *this;
}
GeMatrix3d &GeMatrix3d::setToScaling(double scale)
{
    return this->setToScaling(scale, GePoint3d::kOrigin);
}
GeMatrix3d &GeMatrix3d::setToScaling(double scale, const GePoint3d &center)
{
    this->setToIdentity();

    this->entry[0][0] = scale;
    this->entry[1][1] = scale;
    this->entry[2][2] = scale;

    this->entry[0][3] = (1 - scale) * center.x;
    this->entry[1][3] = (1 - scale) * center.y;
    this->entry[2][3] = (1 - scale) * center.z;

    return *this;
}
GeMatrix3d &GeMatrix3d::setToScaling(const GeScale3d &scale)
{
    return this->setToScaling(scale, GePoint3d::kOrigin);
}
GeMatrix3d &GeMatrix3d::setToScaling(const GeScale3d &scale, const GePoint3d &center)
{
    this->setToIdentity();

    this->entry[0][0] = scale.sx;
    this->entry[1][1] = scale.sy;
    this->entry[2][2] = scale.sz;

    this->entry[0][3] = (1 - scale.sx) * center.x;
    this->entry[1][3] = (1 - scale.sy) * center.y;
    this->entry[2][3] = (1 - scale.sz) * center.z;

    return *this;
}
GeMatrix3d &GeMatrix3d::setToMirroring(const GePlane &mirrorPlane)
{
    GeMatrix3d mat;
    const double X = mirrorPlane.normal().x;
    const double Y = mirrorPlane.normal().y;
    const double Z = mirrorPlane.normal().z;
    mat.entry[0][0] = X * X;
    mat.entry[1][1] = Y * Y;
    mat.entry[2][2] = Z * Z;
    mat.entry[0][1] = X * Y;
    mat.entry[0][2] = X * Z;
    mat.entry[1][2] = Y * Z;
    mat.entry[1][0] = mat.entry[0][1];
    mat.entry[2][0] = mat.entry[0][2];
    mat.entry[2][1] = mat.entry[1][2];

    for (int i = 0; i < 3; i++)
    {
        for (int u = 0; u < 3; u++)
        {
            mat.entry[i][u] *= -2.0;
        }
    }

    mat.entry[0][0] = mat.entry[0][0] + 1;
    mat.entry[1][1] = mat.entry[1][1] + 1;
    mat.entry[2][2] = mat.entry[2][2] + 1;

    for (int i = 0; i < 3; i++)
    {
        for (int u = 0; u < 3; u++)
        {
            mat.entry[i][u] *= -1.0;
        }
    }

    GePoint3d loc = mirrorPlane.pointOnPlane();
    loc.transformBy(mat);
    loc.x += mirrorPlane.pointOnPlane().x;
    loc.y += mirrorPlane.pointOnPlane().y;
    loc.z += mirrorPlane.pointOnPlane().z;
    mat.setTranslation(loc.asVector());

    for (int i = 0; i < 3; i++)
    {
        for (int u = 0; u < 3; u++)
        {
            mat.entry[i][u] *= -1.0;
        }
    }

    this->set(mat);

    return *this;
}
GeMatrix3d &GeMatrix3d::setToMirroring(const GePoint3d &mirrorPoint)
{
    this->setToIdentity();
    this->entry[0][0] = -1;
    this->entry[1][1] = -1;
    this->entry[2][2] = -1;
    this->entry[0][3] = mirrorPoint.x * 2;
    this->entry[1][3] = mirrorPoint.y * 2;
    this->entry[2][3] = mirrorPoint.z * 2;
    return *this;
}
GeMatrix3d &GeMatrix3d::setToMirroring(const GeLine3d &mirrorLine)
{
    this->setToIdentity();
    this->setToRotation(PI, mirrorLine.direction(), mirrorLine.pointOnLine());
    return *this;
}

GeMatrix3d &GeMatrix3d::setToAlignCoordSys(
    const GePoint3d &fromOrigin,
    const GeVector3d &fromXAxis,
    const GeVector3d &fromYAxis,
    const GeVector3d &fromZAxis,
    const GePoint3d &toOrigin,
    const GeVector3d &toXAxis,
    const GeVector3d &toYAxis,
    const GeVector3d &toZAxis)
{
    GeMatrix3d mat;

    GeVector3d xAaxisFrom = fromXAxis.normal();
    GeVector3d yAaxisFrom = fromYAxis.normal();
    GeVector3d zAaxisFrom = fromZAxis.normal();
    GeVector3d xAaxisTo = toXAxis.normal();
    GeVector3d yAaxisTo = toYAxis.normal();
    GeVector3d zAaxisTo = toZAxis.normal();

    // 对齐z轴
    {
        // 获得z轴之间的夹角
        double angle = zAaxisFrom.angleTo(zAaxisTo);

        // 叉乘得到垂直法向
        GeVector3d normal = zAaxisFrom.crossProduct(zAaxisTo);

        // 构建Z轴对齐的旋转矩阵
        GeMatrix3d tempMat;
        tempMat.setToRotation(angle, normal);
        GeVector3d vec = zAaxisFrom;
        vec.transformBy(tempMat);
        if (vec.normal().isEqualTo(zAaxisTo.normal()) == false)
        {
            tempMat.setToRotation(0 - angle, normal);
        }

        // 轴变换
        xAaxisFrom.transformBy(tempMat);
        yAaxisFrom.transformBy(tempMat);
        zAaxisFrom.transformBy(tempMat);

        // 矩阵左乘
        mat.preMultBy(tempMat);
    }

    // 对齐x轴
    {
        // 获得x轴的夹角
        double angle = xAaxisFrom.angleTo(xAaxisTo);

        // 构建旋转矩阵
        GeMatrix3d tempMat;
        tempMat.setToRotation(angle, zAaxisTo);
        GeVector3d vec = xAaxisFrom;
        vec.transformBy(tempMat);
        if (vec.normal().isEqualTo(xAaxisTo.normal()) == false)
        {
            tempMat.setToRotation(0 - angle, zAaxisTo);
        }

        // 轴变换
        xAaxisFrom.transformBy(tempMat);
        yAaxisFrom.transformBy(tempMat);
        zAaxisFrom.transformBy(tempMat);

        // 矩阵左乘
        mat.preMultBy(tempMat);
    }

    // 对齐y轴
    {
        if (yAaxisFrom.normal().isEqualTo(yAaxisTo.normal()) == false)
        {
            GePlane plane;
            plane.set(GePoint3d::kOrigin, yAaxisFrom);

            GeMatrix3d tempMat;
            tempMat.setToMirroring(plane);

            // 矩阵左乘
            mat.preMultBy(tempMat);
        }
    }

    GeMatrix3d tempMat;
    tempMat.setToTranslation(toOrigin - fromOrigin);
    mat *= tempMat;

    this->set(mat);

    return *this;


#if 0
    GeMatrix3d mat;

    GeVector3d xAaxisFrom = fromXAxis.normal();
    GeVector3d yAaxisFrom = fromYAxis.normal();
    GeVector3d zAaxisFrom = fromZAxis.normal();
    GeVector3d xAaxisTo = toXAxis.normal();
    GeVector3d yAaxisTo = toYAxis.normal();
    GeVector3d zAaxisTo = toZAxis.normal();

    // 对齐z轴
    {
        // 获得z轴之间的夹角
        double angle = zAaxisFrom.angleTo(zAaxisTo);

        // 叉乘得到垂直法向
        GeVector3d normal = zAaxisFrom.crossProduct(zAaxisTo);

        // 构建旋转矩阵
        GeMatrix3d tempMat;
        tempMat.setToRotation(angle, normal);
        GeVector3d vec = zAaxisFrom;
        vec.transformBy(tempMat);
        if (vec.normal().isEqualTo(zAaxisTo.normal()) == false)
        {
            tempMat.setToRotation(0 - angle, normal);
        }

        // 轴变换
        xAaxisFrom.transformBy(tempMat);
        yAaxisFrom.transformBy(tempMat);
        zAaxisFrom.transformBy(tempMat);

        //mat *= tempMat;
        mat.preMultBy(tempMat);
    }

    // 对齐x轴
    {
        // 获得x轴的夹角
        double angle = xAaxisFrom.angleTo(xAaxisTo);

        // 构建旋转矩阵
        GeMatrix3d tempMat;
        tempMat.setToRotation(angle, zAaxisTo);
        GeVector3d vec = xAaxisFrom;
        vec.transformBy(tempMat);
        if (vec.normal().isEqualTo(xAaxisTo.normal()) == false)
        {
            tempMat.setToRotation(0 - angle, zAaxisTo);
        }

        // 轴变换
        xAaxisFrom.transformBy(tempMat);
        yAaxisFrom.transformBy(tempMat);
        zAaxisFrom.transformBy(tempMat);

        //mat *= tempMat;
        mat.preMultBy(tempMat);
    }

    // 对齐y轴
    {
        if (yAaxisFrom.normal().isEqualTo(yAaxisTo.normal()) == false)
        {
            GePlane plane;
            plane.set(GePoint3d::kOrigin, yAaxisFrom);

            GeMatrix3d tempMat;
            tempMat.setToMirroring(plane);

            //mat *= tempMat;
            mat.preMultBy(tempMat);
        }
    }

    GeMatrix3d tempMat;
    tempMat.setToTranslation(toOrigin - fromOrigin);
    mat *= tempMat;

    this->set(mat);

    return *this;
#endif
}

GeMatrix3d &GeMatrix3d::setToWorldToPlane(const GeVector3d &normal)
{
    GePlane plane(GePoint3d::kOrigin, normal);
    return this->setToWorldToPlane(plane);
}

GeMatrix3d &GeMatrix3d::setToWorldToPlane(const GePlane &plane)
{
    this->setToIdentity();

    GeVector3d normal = plane.normal();
    normal.normalize();
    if (normal.isEqualTo(GeVector3d::kZAxis) == true) {
        GeMatrix3d tempMat;
        tempMat.setToTranslation(plane.pointOnPlane().asVector());
        //*this *= tempMat;
        this->preMultBy(tempMat);
        return *this;
    }
    else if (normal.isEqualTo(GeVector3d(0, 0, -1)) == true) {

        this->setToRotation(PI, GeVector3d::kZAxis);

        GeMatrix3d tempMat;
        tempMat.setToRotation(PI, GeVector3d::kXAxis);
        *this *= tempMat;

        tempMat.setToTranslation(-plane.pointOnPlane().asVector());
        *this *= tempMat;

        return *this;
    }
    
    GeVector3d zAxis = plane.normal();

    GeVector3d xAxis = GeVector3d::kZAxis.crossProduct(zAxis);
    xAxis.normalize();

    GeVector3d yAxis = zAxis.crossProduct(xAxis);

    this->setToAlignCoordSys(
        plane.pointOnPlane(), xAxis, yAxis, zAxis,
        GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis);

    return *this;
}


GeMatrix3d &GeMatrix3d::setToPlaneToWorld(const GeVector3d &normal)
{
    return this->setToPlaneToWorld(GePlane(GePoint3d::kOrigin, normal));
}

GeMatrix3d &GeMatrix3d::setToPlaneToWorld(const GePlane &plane)
{

    this->setToIdentity();

    GeVector3d normal = plane.normal();
    normal.normalize();
    if (normal.isEqualTo(GeVector3d::kZAxis) == true) {

        GeMatrix3d tempMat;
        tempMat.setToTranslation(plane.pointOnPlane().asVector());
        *this *= tempMat;

        return *this;
    }
    else if (normal.isEqualTo(GeVector3d(0, 0, -1)) == true) {

        this->setToRotation(PI, GeVector3d::kZAxis);

        GeMatrix3d tempMat;
        tempMat.setToRotation(PI, GeVector3d::kXAxis);
        *this *= tempMat;

        tempMat.setToTranslation(plane.pointOnPlane().asVector());
        *this *= tempMat;

        return *this;
    }

    GeVector3d zAxis = plane.normal();

    GeVector3d xAxis = GeVector3d::kZAxis.crossProduct(zAxis);
    xAxis.normalize();

    GeVector3d yAxis = zAxis.crossProduct(xAxis);

    this->setToAlignCoordSys(
        GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
        GePoint3d::kOrigin, xAxis, yAxis, zAxis);

    GeMatrix3d tempMat;
    GeVector3d vec = plane.pointOnPlane().asVector();
    tempMat.setToTranslation(vec);
    this->preMultBy(tempMat);

    return *this;
}
GeVector3d GeMatrix3d::translation() const
{
    return GeVector3d(this->entry[0][3], this->entry[1][3], this->entry[2][3]);
}

GeMatrix3d GeMatrix3d::translation(const GeVector3d &vect)
{
    GeMatrix3d mat;
    mat.setToTranslation(vect);
    return mat;
}
GeMatrix3d GeMatrix3d::rotation(double angle, const GeVector3d &axis)
{
    return GeMatrix3d::rotation(angle, axis, GePoint3d::kOrigin);
}
GeMatrix3d GeMatrix3d::rotation(double angle, const GeVector3d &axis, const GePoint3d &center)
{
    GeMatrix3d mat;
    mat.setToRotation(angle, axis, center);
    return mat;
}
GeMatrix3d GeMatrix3d::scaling(double scale)
{
    return GeMatrix3d::scaling(scale, GePoint3d::kOrigin);
}
GeMatrix3d GeMatrix3d::scaling(double scale, const GePoint3d &center)
{
    GeMatrix3d mat;
    mat.setToScaling(scale, center);
    return mat;
}
GeMatrix3d GeMatrix3d::scaling(const GeScale3d &scale)
{
    return GeMatrix3d::scaling(scale, GePoint3d::kOrigin);
}
GeMatrix3d GeMatrix3d::scaling(const GeScale3d &scale, const GePoint3d &center)
{
    GeMatrix3d mat;
    mat.setToScaling(scale, center);
    return mat;
}
GeMatrix3d GeMatrix3d::mirroring(const GePlane &mirrorPlane)
{
    GeMatrix3d mat;
    mat.setToMirroring(mirrorPlane);
    return mat;
}
GeMatrix3d GeMatrix3d::mirroring(const GePoint3d &mirrorPoint)
{
    GeMatrix3d mat;
    mat.setToMirroring(mirrorPoint);
    return mat;
}
GeMatrix3d GeMatrix3d::mirroring(const GeLine3d &mirrorLine)
{
    GeMatrix3d mat;
    mat.setToMirroring(mirrorLine);
    return mat;
}
GeMatrix3d GeMatrix3d::alignCoordSys(
    const GePoint3d &fromOrigin,
    const GeVector3d &fromXAxis,
    const GeVector3d &fromYAxis,
    const GeVector3d &fromZAxis,
    const GePoint3d &toOrigin,
    const GeVector3d &toXAxis,
    const GeVector3d &toYAxis,
    const GeVector3d &toZAxis)
{
    GeMatrix3d mat;
    mat.setToAlignCoordSys(fromOrigin, fromXAxis, fromYAxis, fromZAxis, toOrigin, toXAxis, toYAxis, toZAxis);
    return mat;
}
GeMatrix3d GeMatrix3d::worldToPlane(const GeVector3d &normal)
{
    GeMatrix3d mat;
    mat.setToWorldToPlane(normal);
    return mat;
}
GeMatrix3d GeMatrix3d::worldToPlane(const GePlane &plane)
{
    GeMatrix3d mat;
    mat.setToWorldToPlane(plane);
    return mat;
}
GeMatrix3d GeMatrix3d::planeToWorld(const GeVector3d &normal)
{
    GeMatrix3d mat;
    mat.setToPlaneToWorld(normal);
    return mat;
}

GeMatrix3d GeMatrix3d::planeToWorld(const GePlane &plane)
{
    GeMatrix3d mat;
    mat.setToPlaneToWorld(plane);
    return mat;
}

GeScale3d GeMatrix3d::scale() const
{
    return GeScale3d(this->entry[0][0], this->entry[1][1], this->entry[2][2]);
}

double GeMatrix3d::norm() const
{
    return 0;
}
