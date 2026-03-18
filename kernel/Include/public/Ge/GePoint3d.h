#ifndef GEPNT3D_H
#define GEPNT3D_H /*!DOM*/

#include "GePoint2d.h"
#include "GeVector3d.h"
#include "GeScale3d.h"
#include "EmObject.h"

class GeMatrix3d;
class GeLinearEnt3d;
class GePlane;
class GePlanarEnt;
class GePoint2d;



/** \details
    This class represents points (locations) in 3D space. 

    \remarks
    GePoint3d may be viewed as an array[3] of doubles.

    Corresponding C++ library: TD_Ge

    <group Ge_Classes> 

    \sa
    <link ge_BasicTypes.html, Working with Basic Geometry Types>
*/
class GePoint3d INHERIT_EM_OBJECT
{
public:
    /** \param xx [in]  The X-coordinate of this point.
        \param yy [in]  The Y-coordinate of this point.
        \param zz [in]  The Z-coordinate of this point.
        \param plane [in]  Any plane.
        \param pnt2d [in]  Any 2D point.
        \param source [in]  Object to be cloned.
    
        \remarks
        plane and pnt2d define a point as follows:
            
                p + uAxis * pnt2d.x + vAxis * pnt2d.y,
            
        where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
    
        With no arguments, creates a point at the origin.
    */
    GePoint3d() : x(0.0), y(0.0), z(0.0) {}
    GePoint3d(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
    GePoint3d(const GePoint3d& point) {
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
    }
    //GePoint3d(const GePlanarEnt& plane, const GePoint2d& pnt2d);

    GE_STATIC_EXPORT static const GePoint3d kOrigin; // Origin (0,0,0).

    friend GePoint3d operator *(const GeMatrix3d& matrix, const GePoint3d& point);
    friend GePoint3d operator *(double scale, const GePoint3d& point) {
        return GePoint3d(point.x * scale, point.y * scale, point.z * scale);
    }

    /** \details
        Sets this point to the product of xfm * point, and returns a reference to this point.
    
        \param xfm [in]  A 3D matrix. 
        \param point [in]  Any 3D point. 
    */
    GePoint3d& setToProduct(const GeMatrix3d& xfm, const GePoint3d& point);

    /** \details
        Sets this point to the result of the matrix multiplication xfm  this point*.
        Returns a reference to this point.

        \param xfm [in]  A 3D matrix.
    */
    GePoint3d& transformBy(const GeMatrix3d& xfm);
    GePoint3d& transformBy(const GeMatrix3d& xfm, Ge::ErrorCondition& status);

    //移动
    GePoint3d& translation(const GeVector3d& vec)
    {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;
        return *this;
    }

    /** \details
        Rotates this point about the specified basePoint and axis of rotation by the specified angle. 

        \param angle [in]  Rotation angle.
        \param basePoint [in]  Basepoint.
        \param vect [in]  Axis of rotation.
    */
    GePoint3d& rotateBy(double angle, const GeVector3d& vect);
    GePoint3d& rotateBy(double angle, const GeVector3d& vect, const GePoint3d& basePoint);

    /** \details
        Mirrors this point about the specified plane.

        \param line [in]  Mirror Line.
    */
    GePoint3d& mirror(const GePlane& plane);

    /** \details
        Scales this point by the scale factor about the basepoint.

        \param scaleFactor [in]  Scale Factor. The scale factor must be greater than zero.
        \param basePoint [in]  Basepoint.
    */
    GePoint3d& scaleBy(double scaleFactor);
    GePoint3d& scaleBy(double scaleFactor, const GePoint3d& basePoint);
    GePoint3d& scaleBy(const GeScale3d& scaleFactor);
    GePoint3d& scaleBy(const GeScale3d& scaleFactor, const GePoint3d& basePoint);

    /** \details
        Projects this point onto the specified plane.
    
        \param plane [in]  Any plane.
    
        Remarks,
        If no plane is specified, this point is projected into the XY plane.
    */
    GePoint2d convert2d (const GePlanarEnt& plane) const;
   
    GePoint2d convert2d() const{
        return GePoint2d(x, y);
    }
    GePoint3d operator *(double scale) const{
        return GePoint3d (x * scale, y * scale, z * scale);
    }
    GePoint3d& operator *=(double scale){
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }
    GePoint3d operator /(double scale) const{
        return GePoint3d (x / scale, y / scale, z / scale);
    }
    GePoint3d& operator /=(double scale){
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }
    GePoint3d operator +(const GeVector3d& vect) const;
    GePoint3d& operator +=(const GeVector3d& vect);
    GePoint3d operator -(const GeVector3d& vect) const;
    GePoint3d& operator -=(const GeVector3d& vect);
      
    /** \details
        Sets this point to point + vect, and returns a reference to this point.
    
        \param point [in]  Any 3D point.
        \param vect [in]  Any 3D vector.
    */
    GePoint3d& setToSum(const GePoint3d& point, const GeVector3d& vect);

    GeVector3d operator -(const GePoint3d& point) const;
    
    /** \details
        Returns this point as a vector.
    */
    const GeVector3d& asVector() const;

    /** \details
        Returns the distance from this point to the specified point.

        \param point [in]  Any 3D point.
    */
    double distanceTo(const GePoint3d& point) const{
        double Var = (point.x - this->x) * (point.x - this->x) + (point.y - this->y) * (point.y - this->y) + (point.z - this->z) * (point.z - this->z);
        return sqrt(Var);
    }

    /** \details
        Projects this point onto the specified plane along the specified vector.

        \param plane [in]  Plane on which this point is to be projected
        \param vect [in]  Vector defining the projection direction.
    */
    GePoint3d project(const GePlane& plane, const GeVector3d& vect) const;
    
    /** \details
        Projects this point onto the specified plane along the normal to the plane.

        \param plane [in]  Plane on which this point is to be projected
    */
    GePoint3d orthoProject(const GePlane& plane) const;

    bool operator ==(const GePoint3d& point) const{
        return isEqualTo (point);
    }
    
    bool operator !=(const GePoint3d& point) const{
        return !isEqualTo (point);
    }
    
    /** \details
        Returns true if and only if the distance from this point to point is within the 
        specified tolerance.

        \param point [in]  Any 3D point.
        \param tol [in]  Geometric tolerance.
    */
    bool isEqualTo(const GePoint3d& point) const;
    bool isEqualTo(const GePoint3d& point,const GeTol& tol) const;

    /** \param i [in]  Index of coordinate.
    
        \details
        Returns or references the ith coordinate of this point.
        \remarks
        * 0 returns or references the X-coordinate.
        * 1 returns or references the Y-coordinate.
        * 2 returns or references the Z-coordinate.
    */
    double operator [](unsigned int i) const{
        return * (&x+i);
    }
      
    double& operator [](unsigned int i)
    {
        return * (&x+i);
    }
  
    /** \details
        Sets the parameters for this point according to the arguments
    
        \param xx [in]  The X-coordinate of this point.
        \param yy [in]  The Y-coordinate of this point.
        \param zz [in]  The Z-coordinate of this point.
    
        \remarks
        Returns a reference to this point.
        \remarks
        plane and pnt2d define a point as follows:
            
                p + uAxis * point.x + vAxis * point.y,
            
        where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
    */
    GePoint3d& set(double xx, double yy, double zz){
        x = xx;
        y = yy;
        z = zz;
        return *this;
    }

    GePoint3d& set(const GePoint2d& xy) {
        return this->set(xy, 0.0);
    }
    GePoint3d& set(const GePoint2d& xy, double zz) {
        x = xy.x;
        y = xy.y;
        z = zz;
        return *this;
    }

    /** \details
        Sets the parameters for this point according to the arguments
    
        \param plane [in]  Any plane.
        \param point [in]  Any 2D point.
    
        \remarks
        Returns a reference to this point.
        \remarks
        plane and pnt2d define a point as follows:
            
                p + uAxis * point.x + vAxis * point.y,
            
        where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
    */  
    GePoint3d& set(const GePlanarEnt& plane, const GePoint2d& point);

    double x; // X-coordinate
    double y; // Y-coordinate
    double z; // Z-coordinate

    GePoint3d& swapWithPoint(GePoint3d& point);
};


#include "GeVector3d.h"

inline GePoint3d GePoint3d::operator +(const GeVector3d& vect) const {
  return GePoint3d(x + vect.x, y + vect.y, z + vect.z);
}

inline GePoint3d& GePoint3d::operator +=(const GeVector3d& vect) {
    x += vect.x;
    y += vect.y;
    z += vect.z;
    return *this;
}

inline GePoint3d GePoint3d::operator -(const GeVector3d& vect) const {
    return GePoint3d(x - vect.x, y - vect.y, z - vect.z);
}

inline GePoint3d& GePoint3d::operator -=(const GeVector3d& vect) {
    x -= vect.x;
    y -= vect.y;
    z -= vect.z;
    return *this;
}

inline GeVector3d GePoint3d::operator -(const GePoint3d& point) const {
    return GeVector3d(x - point.x, y - point.y, z - point.z);
}

inline const GeVector3d& GePoint3d::asVector() const {
    return *reinterpret_cast<const GeVector3d*>(this);
}



#endif

