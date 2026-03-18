#ifndef GEVEC2D_H
#define GEVEC2D_H

#include "GeGbl.h"
#include "GeTol.h"
#include "EmObject.h"



class GeMatrix2d;

class GeVector2d INHERIT_EM_OBJECT
{
public:
    GeVector2d();
    GeVector2d(const GeVector2d& vec);
    GeVector2d(double x, double y);

    // The additive identity, x-axis, and y-axis.
    //
    GE_STATIC_EXPORT static const GeVector2d kIdentity;
    GE_STATIC_EXPORT static const GeVector2d kXAxis;
    GE_STATIC_EXPORT static const GeVector2d kYAxis;

    // Matrix multiplication.
    //
    friend GeVector2d operator *  (const GeMatrix2d& mat, const GeVector2d& vec);
    GeVector2d& setToProduct(const GeMatrix2d& mat, const GeVector2d& vec) {
        *this = mat * vec;
        return *this;
    }
    GeVector2d& transformBy(const GeMatrix2d& leftSide) {
        return *this = leftSide * *this;
    }
    GeVector2d& rotateBy(double angle);
    GeVector2d& mirror(const GeVector2d& line);

    double crossProduct(const GeVector2d& vect) const
    {
        return x * vect.y - y * vect.x;
    }

    // Scale multiplication.
    //
    GeVector2d operator *  (double scl) const;
    friend GeVector2d operator * (double scl, const GeVector2d& vec) {
        GeVector2d v;
        v.x = vec.x * scl;
        v.y = vec.y * scl;
        return v;
    }
    GeVector2d& operator *= (double scl);
    GeVector2d& setToProduct(const GeVector2d& vec, double scl);
    GeVector2d operator /  (double scl) const;
    GeVector2d& operator /= (double scl);

    // Addition and subtraction of vectors.
    //
    GeVector2d operator +  (const GeVector2d& vec) const;
    GeVector2d& operator += (const GeVector2d& vec);
    GeVector2d operator -  (const GeVector2d& vec) const;
    GeVector2d& operator -= (const GeVector2d& vec);
    GeVector2d& setToSum(const GeVector2d& vec1, const GeVector2d& vec2);
    GeVector2d operator -  () const;
    GeVector2d& negate();

    // Perpendicular vector
    //
    GeVector2d perpVector() const;

    // Angle argument.
    //
    double angle() const;
    double angleTo(const GeVector2d& vec) const;

    double angleToCCW(const GeVector2d& vect) const;

    // Vector length operations.
    //
    GeVector2d normal() const;
    GeVector2d normal(const GeTol& tol) const;
    GeVector2d& normalize();
    GeVector2d& normalize(const GeTol& tol);
    // Possible errors:  k0This.  Returns object unchanged on error. 
    double length() const;
    double lengthSqrd() const;
    bool isUnitLength() const;
    bool isUnitLength(const GeTol& tol) const;
    bool isZeroLength() const;
    bool isZeroLength(const GeTol& tol) const;

    // Direction tests.
    //
    bool isParallelTo(const GeVector2d& vec) const;
    bool isParallelTo(const GeVector2d& vec,const GeTol& tol) const;

    // Possible errors:  k0This, k0Arg1. 
    // Returns kFalse on error.
    bool isCodirectionalTo(const GeVector2d& vec) const;
    bool isCodirectionalTo(const GeVector2d& vec, const GeTol& tol = GeContext::gTol) const;
    // Possible errors:  k0This, k0Arg1. 
    // Returns kFalse on error.
    bool isPerpendicularTo(const GeVector2d& vec) const;
    bool isPerpendicularTo(const GeVector2d& vec, const GeTol& tol = GeContext::gTol) const;
    // Possible errors:  k0This, k0Arg1. 
    // Returns kFalse on error.

    // Dot product.
    //
    double dotProduct(const GeVector2d& vec) const;

    // Tests for equivalence using the Euclidean norm.
    //
    bool operator == (const GeVector2d& vec) const;
    bool operator != (const GeVector2d& vec) const;
    bool isEqualTo(const GeVector2d& vec) const;
    bool isEqualTo(const GeVector2d& vec, const GeTol& tol) const;

    // For convenient access to the data.
    //
    double operator [] (unsigned int i) const;
    double& operator [] (unsigned int i);
    GeVector2d& set(double x, double y);

    // Convert to/from matrix form.
    //
    operator GeMatrix2d() const;

    // Co-ordinates.
    //
    double x, y;
};

// Creates the identity translation vector.
//
inline
GeVector2d::GeVector2d() : x(0.0), y(0.0)
{
}

inline
GeVector2d::GeVector2d(const GeVector2d& src) : x(src.x), y(src.y)
{
}

// Creates a vector intialized to ( xx, yy ).
//
inline
GeVector2d::GeVector2d(double xx, double yy) : x(xx), y(yy)
{
}

inline bool
GeVector2d::operator == (const GeVector2d& v) const
{
    return this->isEqualTo(v);
}

// This operator is the logical negation of the `==' operator.
//
inline bool
GeVector2d::operator != (const GeVector2d& v) const
{
    return !this->isEqualTo(v);
}

// This operator returns a vector that is the scalar product of
// `s' and this vector.
//
inline GeVector2d
GeVector2d::operator * (double s) const
{
    return GeVector2d(x * s, y * s);
}

// This is equivalent to the statement `v = v * s'.
//
inline GeVector2d&
GeVector2d::operator *= (double s)
{
    x *= s;
    y *= s;
    return *this;
}

inline GeVector2d&
GeVector2d::setToProduct(const GeVector2d& v, double s)
{
    x = s * v.x;
    y = s * v.y;
    return *this;
}

// Returns a vector such that each of the coordinates of this vector
// have been divided by val.
//
inline GeVector2d
GeVector2d::operator / (double val) const
{
    return GeVector2d(x / val, y / val);
}

// This is equivalent to the statement `v = v / val;'
// Each coordinate of this vector is divided by val.
//
inline GeVector2d&
GeVector2d::operator /= (double val)
{
    x /= val;
    y /= val;
    return *this;
}

// Returns a vector that is formed from adding the components of
// this vector with `v'.
//
inline GeVector2d
GeVector2d::operator + (const GeVector2d& v) const
{
    return GeVector2d(x + v.x, y + v.y);
}

// This is equivalent to the statement `thisVec = thisVec + v;'
//
inline GeVector2d&
GeVector2d::operator += (const GeVector2d& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

// Using this operator is equivalent to using `thisVec + (-v);'
//
inline GeVector2d
GeVector2d::operator - (const GeVector2d& v) const
{
    return GeVector2d(x - v.x, y - v.y);
}

// This is equivalent to the statement `thisVec = thisVec - v;'
//
inline GeVector2d&
GeVector2d::operator -= (const GeVector2d& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

inline GeVector2d&
GeVector2d::setToSum(const GeVector2d& v1, const GeVector2d& v2)
{
    x = v1.x + v2.x;
    y = v1.y + v2.y;
    return *this;
}

// Returns a vector that is formed by negating each of the components
// of this vector.
//
inline GeVector2d
GeVector2d::operator - () const
{
    return GeVector2d(-x, -y);
}

// `v.negate()' is equivalent to the statement `v = -v;'
//
inline GeVector2d&
GeVector2d::negate()
{
    x = -x;
    y = -y;
    return *this;
}

// Returns a vector orthogonal to this vector.
//
inline GeVector2d
GeVector2d::perpVector() const
{
    return GeVector2d(-y, x);
}

// Returns the square of the Euclidean length of this vector.
//
inline double
GeVector2d::lengthSqrd() const
{
    return x * x + y * y;
}

// Returns the dot product of this vector and `v'.
//
inline double
GeVector2d::dotProduct(const GeVector2d& v) const
{
    return x * v.x + y * v.y;
}

// Sets the vector to ( xx, yy ).
//
inline GeVector2d&
GeVector2d::set(double xx, double yy)
{
    x = xx;
    y = yy;
    return *this;
}

// Indexes the vector as if it were an array.  `x' is index `0',
// `y' is index `1'.
//
inline double
GeVector2d::operator [] (unsigned int i) const
{
    return *(&x + i);
}

inline double&
GeVector2d::operator [] (unsigned int i)
{
    return *(&x + i);
}




#endif

