#ifndef GEVEC3D_H
#define GEVEC3D_H


#include "GeGbl.h"
#include "EmObject.h"
#include "GeVector2d.h" // for convert2d

class GeMatrix3d;
class GePlane;
class GePlanarEnt;
class GePoint3d;



/** \details
    This class represents vectors in 3D space. 

    \remarks
    GeVector3d may be viewed as an array[3] of doubles.
    
    Corresponding C++ library: TD_Ge

    <group Ge_Classes> 

    \sa
    <link ge_BasicTypes.html, Working with Basic Geometry Types>
*/
class GeVector3d INHERIT_EM_OBJECT
{
public:
    /** \param source [in]  Object to be cloned.
        \param xx [in]  X-coordinate.
        \param yy [in]  Y-coordinate.
        \param zz [in]  Z-coordinate.
        \param vect [in]  Any 2D vector.
        \param plane [in]  Any plane.
    
        \remarks
        When called with no arguments, constructs a zero-length vector.

        When called with plane and vect, constructs 
        the 3D vector correspoponding to the 2D vector
        in the coordinates of the plane:
    
            uAxis * vect.x + vAxis * vect.y
    
        where uAxis and vAxis are returned by
    
            plane.get(origin, uAxis, vAxis)
            
        The 3D vector will be parallel to the 2D vector.
    */
    GeVector3d () : x(0.0), y(0.0), z(0.0) {}
    GeVector3d(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {};
    GeVector3d(const GePlanarEnt& plane, const GeVector2d& vector2d);

    GE_STATIC_EXPORT static const GeVector3d kIdentity; // Additive identity vector.
    GE_STATIC_EXPORT static const GeVector3d kXAxis; // X-Axis vector.
    GE_STATIC_EXPORT static const GeVector3d kYAxis; // Y-Axis vector.
    GE_STATIC_EXPORT static const GeVector3d kZAxis; // Z-Axis vector.


    friend GeVector3d operator * (const GeMatrix3d& xfm, const GeVector3d& vect);

    /** \details
        Sets this vector to the product matrix * vect and returns
        a reference to this vector. 
    
        \param matrix [in]  Any 3D matrix. 
        \param vect [in]  Any 3D vector.
    */
    GeVector3d& setToProduct (const GeMatrix3d& matrix, const GeVector3d& vect);
 
    /** \details
        Sets this vector to the product scale * vect and returns
        a reference to this vector. 
     
        \param vect [in]  Any 3D vector.
        \param scale [in]  Scale factor.
    */
    GeVector3d& setToProduct(const GeVector3d& vect, double scale);
      

    /** \details
        Applies the 3D transformation matrix to this vector.

        \param xfm [in]  3D transformation matrix.
    */
    GeVector3d& transformBy (const GeMatrix3d& xfm);
    
    /** \details
        Rotates this vector the specified angle
        about the specified axis,
        and returns a reference to this vector.

        \param angle [in]  Rotation angle.
        \param axis [in]  Axis of rotation.
    */
    GeVector3d& rotateBy (double angle, const GeVector3d& axis);
    
    /** \details
        Mirrors the entity about the plane passing through the
        origin with the specified normal, and returns
        a reference to the entity.

        \param normalToPlane [in]  Normal to Plane.
    */
    GeVector3d& mirror (const GeVector3d& normalToPlane);

    /** \details
        Returns the 2D vector, in the coordinate system
        of the plane, corresponding to the 3D vector.
    
        \remarks
        The 3D vector must be parallel to the plane.
    
        If no plane is specified, the XY plane is used.
    */
    GeVector2d convert2d (const GePlanarEnt& plane) const;

    /** \details
        Returns the 2D vector, in the coordinate system
        of the plane, corresponding to the 3D vector.
    
        \remarks
        The 3D vector must be parallel to the plane.
    
        If no plane is specified, the XY plane is used.
    */
    GeVector2d convert2d () const { 
        return GeVector2d(x, y); 
    } 
  
    GeVector3d operator * (double scale) const{ 
        return GeVector3d (x * scale, y * scale, z * scale);
    }
    GeVector3d& operator *= (double scale) {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }
    GeVector3d operator / (double scale) const {
        return GeVector3d (x/scale, y/scale, z/scale);
    }
    GeVector3d& operator /= (double scale) {
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }
    GeVector3d operator + (const GeVector3d& vect) const {
        return GeVector3d(x + vect.x, y + vect.y, z + vect.z);
    }
    GeVector3d operator += (const GeVector3d& vect){ 
        return GeVector3d(x += vect.x, y += vect.y, z += vect.z);
    }    
    GeVector3d operator - (const GeVector3d& vect) const{ 
        return GeVector3d(x - vect.x, y - vect.y, z - vect.z);
    }    
    GeVector3d operator -= (const GeVector3d& vect){ 
        return GeVector3d(x -= vect.x, y -= vect.y, z -= vect.z);
    }    

    /** \details
        Returns this vector as a point.
    */
    const GePoint3d& asPoint() const;

    /** \details
        Sets this vector to vector1 + vector1, and returns a reference to this vector.
    
        \param vector1 [in]  Any 3D vector.
        \param vector2 [in]  Any 3D vector.
    */
    GeVector3d& setToSum(const GeVector3d& vector1, const GeVector3d& vector2);

    GeVector3d operator - () const {
        return GeVector3d (-x, -y, -z); 
    }
  
    /** \details
        Negates this vector (-x, -y, -z), and returns a reference to this vector.
    */
    GeVector3d& negate() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }
  

    /** \details
        Returns a vector perpendicular to this one.
    
        \remarks
        The orthogonal vector is determined by function GeContext::gOrthoVector()
    */
    GeVector3d perpVector (const GeVector3d& OrthoVector) const;


    double angle(const GeVector3d& vect) const;

    /** \details
        Returns the angle to the specified vector.
    
        \param vect [in]  Any 3D vector.

        \remarks
        If refVector is not specified:
        * Returns a value in the range [0.0 .. OdaPI].
        * This function is commutative.
    */
    double angleTo (const GeVector3d& vect) const;

    /** \details
        Returns the angle from this vector to the specified vector in the counterclockwise direction.

        \param vect [in]  Any 2D vector.

        \remarks
        Returns a value in the range [0.0 .. Oda2PI].
    */
	double angleToCCW(const GeVector3d& vect) const;
    double angleToCCW(const GeVector3d& vect, const GeVector3d& normal) const;
  
    /** \details
        Returns the angle of this vector projected onto
        the specified plane
    
        \param plane [in]  Any 3D plane.
    
        \remarks
        This vector is projected orthogonally onto the 
        plane through its origin, and is measured with
        respect to axis1 as returned by
    
        plane.getCoordSystem(origin, axis1, axis2)
    */
    double angleOnPlane(const GePlanarEnt& plane) const;

    /** \details
        Returns the unit vector codirectional with this vector.
    
        \param tol [in]  Geometric tolerance.
        
        \remarks
        If the length() <= tol, this vector is returned.
    */
    GeVector3d normal() const;
    GeVector3d normal (const GeTol& tol) const;

    /** \details
        Sets this vector to the unit vector codirectional with this vector,
        and returns a reference to this vector
    
        \param tol [in]  Geometric tolerance.

        \remarks
        If this.length() <= tol, this vector is unchanged.    
    */
    GeVector3d& normalize();
    GeVector3d& normalize (const GeTol& tol);

    /** \details
        Returns the length of this vector.
    */
    double length () const;

    /** \details
        Sets the length of this vector.
    */
    void setLength(double length);

    /** \details
     Returns the square of the length of this vector.
    */
    double lengthSqrd () const { 
        return x * x + y * y + z * z;
    }


    /** \details
        Returns true if and only if the length of this vector is 1.0 within the specified tolerance.

        \param tol [in]  Geometric tolerance.
    */
    bool isUnitLength() const;
    bool isUnitLength (const GeTol& tol) const;

    /** \details
        Returns true if and only if the length of this vector is 0.0 within the specified tolerance.

        \param tol [in]  Geometric tolerance.
    */
    bool isZeroLength() const;
    bool isZeroLength (const GeTol& tol) const;

    /** \details
        Returns true if and only if the specified vector is parallel to this vector within the specified tolerance.

        \param vect [in]  Any 3D vector.
        \param tol [in]  Geometric tolerance.   
    */
    bool isParallelTo(const GeVector3d& vect) const;
    bool isParallelTo (const GeVector3d& vect,const GeTol& tol) const;

    /** \details
        Returns true if and only if the specified vector is codirectional to this vector within the specified tolerance.

        \param vect [in]  Any 3D vector.
        \param tol [in]  Geometric tolerance. 
    */
    bool isCodirectionalTo(const GeVector3d& vect) const;
    bool isCodirectionalTo (const GeVector3d& vect,const GeTol& tol) const;

    /** \details
        Returns true if and only if the specified vector is perpendicular to this vector within the specified tolerance.

        \param vect [in]  Any 3D vector.
        \param tol [in]  Geometric tolerance.   
    */
    bool isPerpendicularTo(const GeVector3d& vect) const;
    bool isPerpendicularTo (const GeVector3d& vect,const GeTol& tol) const;
    
    /** \details
        Returns the dot product of this vector and the specified vector.
    
        \param vect [in]  Any 3D vector.
    */
    double dotProduct (const GeVector3d& vect) const{
        return x * vect.x + y * vect.y + z * vect.z;
    }    
    
    
    /** \details
        Returns the cross product of this vector and the specified vector.
    
        \param vect [in]  Any 3D vector.
    */
    GeVector3d crossProduct (const GeVector3d& vect) const;

    
    GeMatrix3d rotateTo(const GeVector3d& vector) const;
    GeMatrix3d rotateTo(const GeVector3d& vector, const GeVector3d& axis) const;

    GeVector3d project(const GeVector3d& planeNormal, const GeVector3d& projectDirection) const;
    GeVector3d project(const GeVector3d& planeNormal, const GeVector3d& projectDirection, const GeTol& tol) const;
  
    GeVector3d orthoProject (const GeVector3d& planeNormal) const;
    GeVector3d orthoProject(const GeVector3d& planeNormal, const GeTol& tol) const;


    bool operator == (const GeVector3d& vect) const{
        return this->isEqualTo(vect);
    }
    bool operator != (const GeVector3d& vect) const{
        return !this->isEqualTo(vect);
    }
    
    /** \details
        Returns true if and only if vect is identical to this vector,
        within the specified tolerance.

        \param vect [in]  Any 3D vector.
        \param tol [in]  Geometric tolerance.
    */
    bool isEqualTo(const GeVector3d& vect) const;
    bool isEqualTo(const GeVector3d& vect, const GeTol& tol) const;

    /** \param i [in]  Index of coordinate.

        \details
        Returns or references the ith coordinate of this vector.

        * 0 returns or references the X-coordinate.
        * 1 returns or references the Y-coordinate.
        * 2 returns or references the Z-coordinate.
    */
    double operator [] (unsigned int i) const {
        return *(&x + i);
    }
    double& operator [] (unsigned int i) {
        return *(&x + i);
    }

    /** \details
        Sets this vector to the specified arguments, 
        and returns a reference to this vector.

        \param xx [in]  X-coordinate.
        \param yy [in]  Y-coordinate.
        \param zz [in]  Z-coordinate.
    */
    GeVector3d& set(double xx, double yy, double zz) {
        x = xx;
        y = yy;
        z = zz;
        return *this;
    } 

    /** \details
        Sets this vector to the specified arguments, 
        and returns a reference to this vector.

        \param vect [in]  Any 2D vector.
        \param plane [in]  Any plane.
    
        \remarks
        When called with plane and vector, constructs 
        the 3D vector correspoponding to the 2D vector
        in the coordinates of the plane:
    
            uAxis * vect.x + vAxis * vect.y
    
        where uAxis and vAxis are returned by
    
            plane.get(origin, uAxis, vAxis)
            
        The 3D vector will be parallel to the 2D vector.
    */  
    GeVector3d& set(const GePlanarEnt& plane, const GeVector2d& vect);

    /** \details
        Returns the equivalent 3D tranformation matrix.
    */
    operator GeMatrix3d () const;
  
    /*
        static GeVector3d givePerp (
        const GeVector2d& vector1, 
        const GeVector2d& vector2);
    */

    double x; //  X-coordinate.
    double y; //  Y-coordinate.
    double z; //  Z-coordinate.
};


#endif

