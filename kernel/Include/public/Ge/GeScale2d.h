#ifndef GESCL2D_H
#define GESCL2D_H


class GeMatrix2d;
class GeScale3d;
#include "GeGbl.h"
#include "EmObject.h"


/** \details
    This class represents scaling transformations (scale vectors) in 2D space.

    \remarks
    GeScale2d may be viewed as an array[2] of doubles.

    Corresponding C++ library: TD_Ge

    <group Ge_Classes>
*/
class GeScale2d INHERIT_EM_OBJECT
{
public:
    /** \param factor [in]  Uniform scale factor.
        \param xFactor [in]  The X scale factor.
        \param yFactor [in]  The Y scale factor.
        \param source [in]  Object to be cloned.
    */
    GeScale2d();
    GeScale2d(double factor);
    GeScale2d(const GeScale2d& scale);
    GeScale2d(double xFactor, double yFactor);

    static const GeScale2d kIdentity; // Multplicitive identity vector.

    /** \param scaleVec [in]  Any 2D scale vector.
        \param factor [in]  Uniform scale factor.
    */
    GeScale2d operator * (const GeScale2d& scaleVec) const;

    friend GeScale2d operator * (double factor,const GeScale2d& scaleVec);

    /** \param scaleVec [in]  Any 2D scale vector.
        \param factor [in]  Uniform scale factor.

        \remarks
        Multiplication of scale vectors is defined as follows:

                scl * [xs ys]          = [scl*xs scl*ys]
                [xs1 ys1] * [xs2 ys2]  = [xs1*xs2 ys1*ys2]
    */
    GeScale2d& operator *= (const GeScale2d& scaleVec);

    /** \details
        Sets this scale vector to the product leftSide * (this scale vector), and returns
        a reference to this scale vector.

        \param leftSide [in]  Any 2D scale vector.

        \remarks
        Scale muliplications is commutative.
    */
    GeScale2d& preMultBy(const GeScale2d& leftSide);

    /** \details
        Sets this scale vector to the product (this scale vector) * (rightSide), and returns
        a reference to this scale vector.

        \param rightSide [in]  Any 2D scale vector.

        \remarks
        Scale muliplications is commutative.
    */
    GeScale2d& postMultBy(const GeScale2d& rightSide);


    /** \details
        Sets this scale vector to the product factor * scaleVec, and returns
        a reference to this scale vector.

        \param factor [in]  Uniform scale factor.
        \param scaleVec [in]  Any 2D scale vector.

        \remarks
        Multiplication of scale vectors is defined as follows:

                scl * [xs1 ys1]        = [scl*xs1 scl*ys1]
                [xs1 ys1 ] * [xs2 ys2] = [xs1*xs2 ys1*ys2]
    */
    GeScale2d& setToProduct(const GeScale2d& scaleVec, double factor);

    /** \details
        Sets this scale vector to the product scaleVec1 * scaleVec2 and returns
        a reference to this scale vector.

        \param scaleVec1 [in]  Any 2D scale vector.
        \param scaleVec2 [in]  Any 2D scale vector.

        \remarks
        Multiplication of scale vectors is defined as follows:

                scl * [xs1 ys1]        = [scl*xs1 scl*ys1]
                [xs1 ys1 ] * [xs2 ys2] = [xs1*xs2 ys1*ys2]
    */
    GeScale2d& setToProduct(const GeScale2d& scaleVec1,const GeScale2d& scaleVec2);

    /** \param factor [in]  Uniform scale factor.
        \remarks
        Multiplication of scale vectors is defined as follows:

                scl * [xs1 ys1]        = [scl*xs1 scl*ys1]
                [xs1 ys1 ] * [xs2 ys2] = [xs1*xs2 ys1*ys2]
    */
    GeScale2d operator * (double factor) const;


    /** \param factor [in]  Uniform scale factor.
    */
    GeScale2d& operator *= (double factor);



    /** \details
        Returns the inverse of this scale vector.
    */
    GeScale2d inverse() const;

    /** \details
        Sets this scale vector to its inverse, and returns
        a reference to this scale vector.
    */
    GeScale2d& invert();

    /** \details
        Returns true if and only if the scaling matrix corresponding to this scale vector isUniScaledOrtho().

        \param tol [in]  Geometric tolerance.
    */
    bool isProportional() const;
    bool isProportional(const GeTol& tol) const;

    bool operator == (const GeScale2d& scaleVec) const
    {
        return this->isEqualTo(scaleVec);
    }
    bool operator != (const GeScale2d& scaleVec) const
    {
        return !this->isEqualTo(scaleVec);
    }

    /** \details
        Returns true if and only if scaleVec is identical to this one,
        within the specified tolerance.

        \param scaleVec [in]  Any 2D scale vector.
        \param tol [in]  Geometric tolerance.
    */
    bool isEqualTo(const GeScale2d& scaleVec) const;
    bool isEqualTo(const GeScale2d& scaleVec,const GeTol& tol) const;

    /** \details
        Returns or references the ith component of the scale vector.

        \param i [in]  Index of component.

        \remarks
        * 0 returns or references the X scale factor.
        * 1 returns or references the Y scale factor.
    */
    double& operator [] (unsigned int i)
    {
        return *(&sx + i);
    }
    double operator [] (unsigned int i) const
    {
        return *(&sx + i);
    }

    /** \details
        Sets this scale vector to the specified X and Y factors,
        and returns a reference to this vector.

        \param xFactor [in]  The X scale factor.
        \param yFactor [in]  The Y scale factor.
    */
    GeScale2d& set(double xFactor,double yFactor);


    /** \details
        Returns the transformation matrix equivalent to this scale vector.

        \param xfm [out]  Receives the 2D transformation matrix.

    */
    void getMatrix(GeMatrix2d& xfm) const;

    /** \details
        Returns the scale vector corresponding to the
        lengths of the column vectors of the transformation matrix.

        \param xfm [out]  Receives the 2D transformation matrix.

        \remarks
        xfm must be scaled ortho; i.e., xfm.isScaledOrtho() == true.
    */
    GeScale2d& extractScale(const GeMatrix2d& xfm);

    /** \details
        Returns the scale vector corresponding to the
        lengths of the column vectors of the transformation matrix,
        and sets the scale factor of the matrix to 1 .

        \param xfm [out]  Receives the 2D transformation matrix.

        \remarks
        xfm must be scaled ortho; i.e., xfm.isScaledOrtho() == true.
    */
    GeScale2d& removeScale(GeMatrix2d& xfm);

    /** \details
        Returns the equivalent 2D tranformation matrix.
        or a 3D scale vector [sx sy 1].
    */
    operator GeMatrix2d () const;
    operator GeScale3d () const;

    double sx; // X scale *factor*.
    double sy; // Y scale *factor*.
};



#endif

