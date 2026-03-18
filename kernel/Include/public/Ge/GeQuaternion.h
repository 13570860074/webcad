#ifndef GEQUATERNION_H
#define GEQUATERNION_H

#include "EmObject.h"
#include "GeVector3d.h"
#include "GeMatrix3d.h"

//! Represents operation of rotation in 3d space as quaternion
//! and implements operations with rotations basing on
//! quaternion mathematics.
//!
//! In addition, provides methods for conversion to and from other
//! representations of rotation ( matrix, vector and
//! angle, Euler angles)
class GeQuaternion INHERIT_EM_OBJECT
{
public:
    //! Creates an identity quaternion
    GeQuaternion()
        : x(0.0),
        y(0.0),
        z(0.0),
        w(1.0)
    {}

    //! Creates quaternion directly from component values
    GeQuaternion(const double theX, const double theY, const double theZ, const double theW)
        : x(theX),
        y(theY),
        z(theZ),
        w(theW)
    {}

    //! Creates quaternion representing shortest-arc rotation
    //! operator producing vector theVecTo from vector theVecFrom.
    GeQuaternion(const GeVector3d& theVecFrom, const GeVector3d& theVecTo)
    {
        setRotation(theVecFrom, theVecTo);
    }

    //! Creates quaternion representing shortest-arc rotation
    //! operator producing vector theVecTo from vector theVecFrom.
    //! Additional vector theHelpCrossVec defines preferred direction for
    //! rotation and is used when theVecTo and theVecFrom are directed
    //! oppositely.
    GeQuaternion(const GeVector3d& theVecFrom, const GeVector3d& theVecTo, const GeVector3d& theHelpCrossVec)
    {
        setRotation(theVecFrom, theVecTo, theHelpCrossVec);
    }

    //! Creates quaternion representing rotation on angle
    //! theAngle around vector theAxis
    GeQuaternion(const GeVector3d& theAxis, const double theAngle)
    {
        setVectorAndAngle(theAxis, theAngle);
    }

    //! Creates quaternion from rotation matrix 3*3
    //! (which should be orthonormal skew-symmetric matrix)
    GeQuaternion(const GeMatrix3d& theMat)
    {
        setMatrix(theMat);
    }

    //! Simple equal test without precision
    bool isEqual(const GeQuaternion& theOther) const;

    //! Sets quaternion to shortest-arc rotation producing
    //! vector theVecTo from vector theVecFrom.
    //! If vectors theVecFrom and theVecTo are opposite then rotation
    //! axis is computed as theVecFrom ^ (1,0,0) or theVecFrom ^ (0,0,1).
    void setRotation(const GeVector3d& theVecFrom, const GeVector3d& theVecTo);

    //! Sets quaternion to shortest-arc rotation producing
    //! vector theVecTo from vector theVecFrom.
    //! If vectors theVecFrom and theVecTo are opposite then rotation
    //! axis is computed as theVecFrom ^ theHelpCrossVec.
    void setRotation(const GeVector3d& theVecFrom, const GeVector3d& theVecTo, const GeVector3d& theHelpCrossVec);

    //! Create a unit quaternion from Axis+Angle representation
    void setVectorAndAngle(const GeVector3d& theAxis, const double theAngle);

    //! Convert a quaternion to Axis+Angle representation,
    //! preserve the axis direction and angle from -PI to +PI
    GeVector3d getVectorAndAngle() const;
    void getVectorAndAngle(GeVector3d& theAxis, double& theAngle) const;

    //! Create a unit quaternion by rotation matrix
    //! matrix must contain only rotation (not scale or shear)
    //!
    //! For numerical stability we find first the greatest component of quaternion
    //! and than search others from this one
    void setMatrix(const GeMatrix3d& theMat);

    //! Returns rotation operation as 3*3 matrix
    GeMatrix3d getMatrix() const;

    void set(const double theX, const double theY, const double theZ, const double theW);

    void set(const GeQuaternion& theQuaternion);


    //! Make identity quaternion (zero-rotation)
    void setIdent()
    {
        x = y = z = 0.0;
        w = 1.0;
    }

    //! Reverse direction of rotation (conjugate quaternion)
    void reverse()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    //! Return rotation with reversed direction (conjugated quaternion)
    GeQuaternion reversed() const { return GeQuaternion(-x, -y, -z, w); }

    //! Inverts quaternion (both rotation direction and norm)
    void invert()
    {
        double anIn = 1.0 / squareNorm();
        set(-x * anIn, -y * anIn, -z * anIn, w * anIn);
    }

    //! Return inversed quaternion q^-1
    GeQuaternion inverted() const
    {
        double anIn = 1.0 / squareNorm();
        return GeQuaternion(-x * anIn, -y * anIn, -z * anIn, w * anIn);
    }

    //! Returns square norm of quaternion
    double squareNorm() const
    {
        return x * x + y * y + z * z + w * w;
    }

    //! Returns norm of quaternion
    double norm() const { return sqrt(squareNorm()); }

    //! Scale all components by quaternion by theScale; note that
    //! rotation is not changed by this operation (except 0-scaling)
    void scale(const double theScale);

    void operator *= (const double theScale) { scale(theScale); }

    //! Returns scaled quaternion
    GeQuaternion scaled(const double theScale) const
    {
        return GeQuaternion(x * theScale, y * theScale, z * theScale, w * theScale);
    }

    GeQuaternion operator * (const double theScale) const { return scaled(theScale); }

    //! Stabilize quaternion length within 1 - 1/4.
    //! This operation is a lot faster than normalization
    //! and preserve length goes to 0 or infinity
    void stabilizeLength();

    //! Scale quaternion that its norm goes to 1.
    //! The appearing of 0 magnitude or near is a error,
    //! so we can be sure that can divide by magnitude
    void normalize();

    //! Returns quaternion scaled so that its norm goes to 1.
    GeQuaternion normalized() const
    {
        GeQuaternion aNormilizedQ(*this);
        aNormilizedQ.normalize();
        return aNormilizedQ;
    }

    //! Returns quaternion with all components negated.
    //! Note that this operation does not affect neither
    //! rotation operator defined by quaternion nor its norm.
    GeQuaternion negated() const { return GeQuaternion(-x, -y, -z, -w); }

    GeQuaternion operator -() const { return negated(); }

    //! Makes sum of quaternion components; result is "rotations mix"
    GeQuaternion added(const GeQuaternion& theOther) const
    {
        return GeQuaternion(x + theOther.x, y + theOther.y, z + theOther.z, w + theOther.w);
    }

    GeQuaternion operator + (const GeQuaternion& theOther) const { return added(theOther); }

    //! Makes difference of quaternion components; result is "rotations mix"
    GeQuaternion subtracted(const GeQuaternion& theOther) const
    {
        return GeQuaternion(x - theOther.x, y - theOther.y, z - theOther.z, w - theOther.w);
    }

    GeQuaternion operator - (const GeQuaternion& theOther) const { return subtracted(theOther); }

    //! Multiply function - work the same as Matrices multiplying.
    //! @code
    //! qq' = (cross(v,v') + wv' + w'v, ww' - dot(v,v'))
    //! @endcode
    //! Result is rotation combination: q' than q (here q=this, q'=theQ).
    //! Notices that:
    //! @code
    //! qq' != q'q;
    //! qq^-1 = q;
    //! @endcode
    GeQuaternion multiplied(const GeQuaternion& theOther) const;

    GeQuaternion operator * (const GeQuaternion& theOther) const { return multiplied(theOther); }

    //! Adds components of other quaternion; result is "rotations mix"
    void add(const GeQuaternion& theOther);

    void operator += (const GeQuaternion& theOther) { add(theOther); }

    //! Subtracts components of other quaternion; result is "rotations mix"
    void subtract(const GeQuaternion& theOther);

    void operator -= (const GeQuaternion& theOther) { subtract(theOther); }

    //! Adds rotation by multiplication
    void multiply(const GeQuaternion& theOther)
    {
        (*this) = multiplied(theOther);  // have no optimization here
    }

    void operator *= (const GeQuaternion& theOther) { multiply(theOther); }

    //! Computes inner product / scalar product / Dot
    double dot(const GeQuaternion& theOther) const
    {
        return x * theOther.x + y * theOther.y + z * theOther.z + w * theOther.w;
    }

    //! Return rotation angle from -PI to PI
    double getRotationAngle() const;

    //! Rotates vector by quaternion as rotation operator
    GeVector3d multiply(const GeVector3d& theVec) const;

    GeVector3d operator * (const GeVector3d& theVec) const { return multiply(theVec); }

public:

    double x;
    double y;
    double z;
    double w;

};


#endif