#ifndef GEMATRIX2D_H
#define GEMATRIX2D_H

#include "GeGbl.h"
#include "EmObject.h"
#include "GePoint2d.h"

class GeVector2d;
class GeLine2d;
class GeTol;
class GeScale2d;





class GeVector2d;
class GeLine2d;
class GeTol;

class GeMatrix2d INHERIT_EM_OBJECT
{
public:
    GeMatrix2d();
    GeMatrix2d(const GeMatrix2d& src);

    // The multiplicative identity.
    //
    static const GeMatrix2d kIdentity;


    void set(const GeMatrix2d& mat);

    // Reset matrix.
    //
    GeMatrix2d& setToIdentity();

    // Multiplication.
    //
    GeMatrix2d   operator *   (const GeMatrix2d& mat) const;
    GeMatrix2d& operator *=  (const GeMatrix2d& mat);
    GeMatrix2d& preMultBy(const GeMatrix2d& leftSide);
    GeMatrix2d& postMultBy(const GeMatrix2d& rightSide);
    GeMatrix2d& setToProduct(const GeMatrix2d& mat1, const GeMatrix2d& mat2);

    // Multiplicative inverse.
    //
    GeMatrix2d& invert();
    GeMatrix2d inverse() const;

    // Test if it is a singular matrix. A singular matrix is not invertable.
    //
    bool isSingular() const;
    bool isSingular(const GeTol& tol) const;

    // Matrix transposition.
    //
    GeMatrix2d& transposeIt();
    GeMatrix2d transpose() const;

    // Tests for equivalence using the infinity norm.
    //
    GeMatrix2d& operator =(const GeMatrix2d& src);
    bool operator ==  (const GeMatrix2d& mat) const;
    bool operator !=  (const GeMatrix2d& mat) const;
    bool isEqualTo(const GeMatrix2d& mat) const;
    bool isEqualTo(const GeMatrix2d& mat, const GeTol& tol) const;

    // Test scaling effects of matrix
    //
    GeScale2d scale(void)const;

    // Determinant
    //
    double det() const;

    // Set/retrieve translation.
    //
    GeMatrix2d& setTranslation(const GeVector2d& vec);
    GeVector2d translation() const;

    // Set/get coordinate system
    //
    GeMatrix2d& setCoordSystem(const GePoint2d& origin, const GeVector2d& e0, const GeVector2d& e1);
    void getCoordSystem(GePoint2d& origin, GeVector2d& e0, GeVector2d& e1) const;

    // Set the matrix to be a specified transformation
    //
    GeMatrix2d& setToTranslation(const GeVector2d& vec);
    GeMatrix2d& setToRotation(double angle);
    GeMatrix2d& setToRotation(double angle, const GePoint2d& center);
    GeMatrix2d& setToScaling(double scaleAll);
    GeMatrix2d& setToScaling(double scaleAll, const GePoint2d& center);
    GeMatrix2d& setToScaling(const GeScale2d& scaleAll);
    GeMatrix2d& setToScaling(const GeScale2d& scaleAll, const GePoint2d& center);
    GeMatrix2d& setToMirroring(const GePoint2d& pnt);
    GeMatrix2d& setToMirroring(const GeLine2d& line);
    GeMatrix2d& setToAlignCoordSys(const GePoint2d& fromOrigin,
        const GeVector2d& fromE0,
        const GeVector2d& fromE1,
        const GePoint2d& toOrigin,
        const GeVector2d& toE0,
        const GeVector2d& toE1);

    // Functions that make a 2d transformation matrix using various approaches
    //
    static GeMatrix2d translation(const GeVector2d& vec);
    static GeMatrix2d rotation(double angle);
    static GeMatrix2d rotation(double angle, const GePoint2d& center);
    static GeMatrix2d scaling(double scaleAll);
    static GeMatrix2d scaling(double scaleAll, const GePoint2d& center);
    static GeMatrix2d scaling(const GeScale2d& scaleAll);
    static GeMatrix2d scaling(const GeScale2d& scaleAll, const GePoint2d& center);
    static GeMatrix2d mirroring(const GePoint2d& pnt);
    static GeMatrix2d mirroring(const GeLine2d& line);
    static GeMatrix2d alignCoordSys(const GePoint2d& fromOrigin,
            const GeVector2d& fromE0,
            const GeVector2d& fromE1,
            const GePoint2d& toOrigin,
            const GeVector2d& toE0,
            const GeVector2d& toE1);

    // For convenient access to the data.
    //
    double operator ()  (unsigned int, unsigned int) const;
    double& operator ()  (unsigned int, unsigned int);

    // The components of the matrix.
    //
    double entry[3][3]; // [row][column]
};

inline bool
GeMatrix2d::operator == (const GeMatrix2d& otherMatrix) const
{
    return this->isEqualTo(otherMatrix);
}

// This operator is the logical negation of the `==' operator.
//
inline bool
GeMatrix2d::operator != (const GeMatrix2d& otherMatrix) const
{
    return !this->isEqualTo(otherMatrix);
}

// Return a reference to the element in position [row][column]
// of the `entry' array.
//
inline double
GeMatrix2d::operator () (unsigned int row, unsigned int column) const
{
    return entry[row][column];
}

inline double&
GeMatrix2d::operator () (unsigned int row, unsigned int column)
{
    return entry[row][column];
}












#endif
