#ifndef GESCL3D_H
#define GESCL3D_H


#include "acadstrc.h"
#include "GeGbl.h"
#include "EmObject.h"


class GeMatrix3d;
class GeScale3d INHERIT_EM_OBJECT
{
public:
    GeScale3d();
    GeScale3d(const GeScale3d& src);
    GeScale3d(double factor);
    GeScale3d(double xFact, double yFact, double zFact);

    // The identity scaling operation.
    //
    static const GeScale3d kIdentity;

    // Multiplication.
    //
    GeScale3d operator *  (const GeScale3d& sclVec) const;
    GeScale3d& operator *= (const GeScale3d& scl);
    GeScale3d& preMultBy(const GeScale3d& leftSide);
    GeScale3d& postMultBy(const GeScale3d& rightSide);
    GeScale3d& setToProduct(const GeScale3d& sclVec1, const GeScale3d& sclVec2);
    GeScale3d operator *  (double s) const;
    GeScale3d& operator *= (double s);
    GeScale3d& setToProduct(const GeScale3d& sclVec, double s);
    friend GeScale3d operator * (double, const GeScale3d& scl);

    // Multiplicative inverse.
    //
    GeScale3d inverse() const;
    GeScale3d& invert();

    bool isProportional() const;
    bool isProportional(const GeTol& tol) const;

    // Tests for equivalence using the infinity norm.
    //
    bool operator == (const GeScale3d& sclVec) const;
    bool operator != (const GeScale3d& sclVec) const;
    bool isEqualTo(const GeScale3d& scaleVec) const;
    bool isEqualTo(const GeScale3d& scaleVec, const GeTol& tol) const;

    // For convenient access to the data.
    //
    double operator [] (unsigned int i) const;
    double& operator [] (unsigned int i);
    GeScale3d& set(double sc0, double sc1, double sc2);

    // Conversion to/from matrix form.
    //
    operator GeMatrix3d () const;
    void getMatrix(GeMatrix3d& mat) const;
    GeScale3d& extractScale(const GeMatrix3d& mat);
    GeScale3d& removeScale(GeMatrix3d& mat);

    // The scale components in x, y and z.
    //
    double sx, sy, sz;
};

inline bool
GeScale3d::operator == (const GeScale3d& s) const
{
    return this->isEqualTo(s);
}

// This operator is the logical negation of the `==' operator.
//
inline bool
GeScale3d::operator != (const GeScale3d& s) const
{
    return !(this->isEqualTo(s));
}

// Indexes the scale vector as if it were an array.  `sx' is index `0',
// `sy' is index `1' and `sz' is index `2'.
//
inline double
GeScale3d::operator [] (unsigned int i) const
{
    return *(&sx + i);
}

inline double&
GeScale3d::operator [] (unsigned int i)
{
    return *(&sx + i);
}







#endif 

