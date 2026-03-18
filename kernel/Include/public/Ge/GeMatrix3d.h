
#ifndef MATRIX_3D_H
#define MATRIX_3D_H

#include "GeGbl.h"
#include "EmObject.h"
#include "GeMatrix2d.h"
#include "GePoint3d.h"

class GeLine3d;
class GeVector3d;
class GePlane;
class GeTol;
class GeScale3d;



/** \details
    This class represents 3D transformation matrices that define affine 
    ( translation, rotation, and/or scaling ) transformations.
    
    \remarks
    GeMatrix3d may be viewed as an array[4][4] of doubles.
    
    An GeMatrix3d, M, can be expressed as a 4  4 matrix*, in the form
    
              a00   a01   a02   t0
              a10   a11   a12   t1
              a20   a21   a22   t2
                0     0     0    1
                
    The linear part of M is the matrix
                
              a00   a01   a02
              a10   a11   a12
              a20   a21   a22
                              
    The translational part of M is the column
    
              t0
              t1
              t2
              
    The origin of the coordinate system of M is (t0, t1, t2).

    Corresponding C++ library: TD_Ge

    <group Ge_Classes> 
*/
class GeMatrix3d INHERIT_EM_OBJECT
{
public:
    GeMatrix3d();
    GeMatrix3d(const GeMatrix3d& src);
    GeMatrix3d& operator =(const GeMatrix3d& src);

    GE_STATIC_EXPORT static const GeMatrix3d kIdentity; // Multiplicative identity matrix.

    void set(const GeMatrix3d& mat);
    double get(int row, int col) const;

    // Reset matrix.
        //

        /** \details
        Sets this matrix to the identity matrix, and returns a reference to this matrix.
    */
    GeMatrix3d& setToIdentity();
  
    // Multiplication.

    /** \details
        Returns the product (this matrix) * matrix. 
    */
    GeMatrix3d operator *(const GeMatrix3d& matrix) const;

    /** \details
        Sets this matrix to the product (this matrix) * matrix, and returns
        a reference to this matrix. 
    */
    GeMatrix3d& operator *=(const GeMatrix3d& matrix);
  
    /** \details
        Sets this matrix to the product leftSide * (this matrix), and returns
        a reference to this matrix. 
    
        \param leftSide [in]  Any 2D matrix 
    */
    GeMatrix3d& preMultBy(const GeMatrix3d& leftSide);

    /** \details
        Sets this matrix to the product (this matrix) * rightSide, and returns
        a reference to this matrix. 
    
        \param rightSide [in]  Any 2D matrix 
    */
    GeMatrix3d& postMultBy(const GeMatrix3d& rightSide);

    /** \details
        Sets this matrix to the product matrix1 * matrix2, and returns
        a reference to this matrix. 
    
        \param matrix1 [in]  Any 2D matrix 
        \param matrix2 [in]  Any 2D matrix 
    */
    GeMatrix3d& setToProduct(const GeMatrix3d& matrix1, const GeMatrix3d& matrix2);

    // Multiplicative inverse.
    //

        /** \details
        Sets this matrix to its inverse, and returns
        a reference to this matrix. 
    */
    GeMatrix3d& invert();
    GeMatrix3d inverse() const;
    GeMatrix3d inverse(const GeTol& tol) const;

    /** \details
        Returns true if and only if this matrix has an inverse,
        and the inverse of this matrix.
    
        \param inverseMatrix [out]  Receives the inverse matrix.
        \param tol [in]  Geometric tolerance. 
    */
    bool inverse(GeMatrix3d& inverseMatrix, double tol) const;

    /** \details
        Returns true if and only if this matrix is singular.
    
        \remarks
        * A matrix is singular if and only if its determinant == 0.
        * A singular matrix cannot be inverted.
    */
    bool isSingular() const;
    bool isSingular(const GeTol& tol) const;

    GeMatrix3d& transposeIt();
    GeMatrix3d transpose() const;

    bool isPerspective() const;
    bool isPerspective(const GeTol& tol) const;

    /** \details
        Equality operator. 
    */
    bool operator ==(const GeMatrix3d& matrix) const;

    /** \details
        Inequality operator. 
    */
    bool operator !=(const GeMatrix3d& matrix) const;

    /** \details
        Returns true if and only if matrix is identical to this one,
        within the specified tolerance.

        \param matrix [in]  Matrix to be compared.
        \param tol [in]  Geomentric tolerance.
    */
    bool isEqualTo(const GeMatrix3d& matrix) const;
    bool isEqualTo(const GeMatrix3d& matrix, const GeTol& tol) const;

    /** \details
        Returns the determinant of this matrix.
    */
    double det() const;

    /** \details
        Sets the translation part of the matrix to the specified vector.
    
        \param vect [in]  Translation vector.
    */
    GeMatrix3d& setTranslation(const GeVector3d& vect);
    
    /** \details
        Sets this matrix to the matrix which maps
        the WCS to the coordinate system 
        defined by origin, X-axis, Y-axis, and Z-axis, and returns a reference
        to this matrix.
    
        \param origin [in]  Origin of the coordinate system.
        \param xAxis [in]  X-axis of the coordinate system.
        \param yAxis [in]  Y-axis of the coordinate system.
        \param zAxis [in]  Z-axis of the coordinate system.
    */
    GeMatrix3d& setCoordSystem(const GePoint3d& origin, const GeVector3d& xAxis,const GeVector3d& yAxis, const GeVector3d& zAxis);

    /** \details
        Returns the origin, X-axis, Y-axis, and Z-axis of the coordinate system 
        to which this matrix maps the WCS.
    
        \param origin [out]  Receives the origin of the coordinate system.
        \param xAxis [out]  Receives the X-axis of the coordinate system.
        \param yAxis [out]  Receives the Y-axis of the coordinate system.
        \param zAxis [out]  Receives the Z-axis of the coordinate system.
    */
    void getCoordSystem(GePoint3d& origin, GeVector3d& xAxis, GeVector3d& yAxis, GeVector3d& zAxis) const;
  
    /** \details
        Returns the origin of the coordinate system 
        to which this matrix maps the WCS.
    */
    GePoint3d getCsOrigin() const;

    /** \details
        Returns the X-axis of the coordinate system 
        to which this matrix maps the WCS.
    */
    GeVector3d getCsXAxis() const;

    /** \details
        Returns the Y-axis of the coordinate system 
        to which this matrix maps the WCS.
    */
    GeVector3d getCsYAxis() const;

    /** \details
        Returns the Z-axis of the coordinate system 
        to which this matrix maps the WCS.
    */
    GeVector3d getCsZAxis() const;
 
    /** \details
        Sets this matrix to the matrix which translates
        by vect, and returns a reference
        to this matrix.
    
        \param vect [in]  Translation vector.
    */
    GeMatrix3d& setToTranslation(const GeVector3d& vect);

    /** \details
        Sets this matrix to the matrix which rotates
        by angle about an axis passing through center, and returns a reference
        to this matrix.
    
        \param angle [in]  Rotation angle.
        \param axis [in]  Rotation axis.
        \param center [in]  Center of rotation.
    */
    GeMatrix3d& setToRotation(double angle, const GeVector3d& axis);
    GeMatrix3d& setToRotation(double angle, const GeVector3d& axis, const GePoint3d& center);

    /** \details
        Sets this matrix to the matrix which scales
        by scale about center, and returns a reference
        to this matrix.
    
        \param scale [in]  Scale factor.
        \param center [in]  Center of scaling.
    */
    GeMatrix3d& setToScaling(double scale);
    GeMatrix3d& setToScaling(double scale, const GePoint3d& center);
  
    /** \details
        Sets this matrix to the matrix which scales
        by scale about center, and returns a reference
        to this matrix.
    
        \param scale [in]  Scale factor.
        \param center [in]  Center of scaling.
    */ 
    GeMatrix3d& setToScaling(const GeScale3d& scale);
    GeMatrix3d& setToScaling(const GeScale3d& scale, const GePoint3d& center);

    /** \details
        Sets this matrix to the matrix which mirrors
        about the specified object, and returns a reference
        to this matrix.
    
        \param mirrorPlane [in]  Mirror plane.
    */
    GeMatrix3d& setToMirroring(const GePlane& mirrorPlane);

    /** \details
        Sets this matrix to the matrix which mirrors
        about the specified object, and returns a reference
        to this matrix.

        \param mirrorPoint [in]  Mirror point.
    */
    GeMatrix3d& setToMirroring(const GePoint3d& mirrorPoint);

    /** \details
        Sets this matrix to the matrix which mirrors
        about the specified object, and returns a reference
        to this matrix.
    
        \param mirrorLine [in]  Mirror line entity.
    */
    GeMatrix3d& setToMirroring(const GeLine3d& mirrorLine);
    
    /** \details
        Sets this matrix to the matrix which maps
        the coordinate system defined by fromOrigin, fromXAxis, fromYAxis, and fromZAxis,
        to the coordinate system 
        defined by toOrigin, toXAxis, toYAxis, and toZAxis, and returns a reference
        to this matrix.
    
        \param fromOrigin [in]  Origin of the initial coordinate system.
        \param fromXAxis [in]  X-axis of the initial coordinate system.
        \param fromYAxis [in]  Y-axis of the initial coordinate system.
        \param fromZAxis [in]  Z-axis of the initial coordinate system.
        \param toOrigin [in]  Origin of the initial coordinate system.
        \param toXAxis [in]  X-axis of the initial coordinate system.
        \param toYAxis [in]  Y-axis of the initial coordinate system.
        \param toZAxis [in]  Z-axis of the initial coordinate system.
    */
    GeMatrix3d& setToAlignCoordSys(
        const GePoint3d& fromOrigin,
        const GeVector3d& fromXAxis,
        const GeVector3d& fromYAxis,
        const GeVector3d& fromZAxis,
        const GePoint3d& toOrigin,
        const GeVector3d& toXAxis,
        const GeVector3d& toYAxis,
        const GeVector3d& toZAxis);

    /** \details
        Sets this matrix to the matrix which maps the WCS to
        the plane coordinate system, and returns a reference to this matrix.
    
        \param normal [in]  The normal to the plane.    
    
        \remarks
        If normal is specified, the origin of the plane is (0,0,0).    
    */
    /** 世界坐标系转平面坐标系(三维转二维) */
    GeMatrix3d& setToWorldToPlane(const GeVector3d& normal);

    /** \details
        Sets this matrix to the matrix which maps the WCS to
        the plane coordinate system, and returns a reference to this matrix.
    
        \param plane [in]  The plane.
    */
    /** 世界坐标系转平面坐标系(三维转二维) */
    GeMatrix3d& setToWorldToPlane(const GePlane& plane);
    
    /** \details
        Sets this matrix to the matrix which maps
        the plane coordinate system to the WCS, and returns a reference to this matrix.
    
        \param normal [in]  The normal to the plane.    
    
        \remarks
        If normal is specified, the origin of the plane is (0,0,0).    
    */
    /** 平面坐标系转世界坐标系(二维转三维) */
    GeMatrix3d& setToPlaneToWorld(const GeVector3d& normal);
  
    /** \details
        Sets this matrix to the matrix which maps
        the plane coordinate system to the WCS, and returns a reference to this matrix.
    
        \param plane [in]  The plane. 
    */
    /** 平面坐标系转世界坐标系(二维转三维) */
    GeMatrix3d& setToPlaneToWorld(const GePlane& plane);

    /** \details
        Returns the matrix of the translation by vect.
    
        \param vect [in]  Translation vector.
    */
    static GeMatrix3d translation(const GeVector3d& vect);
 
    /** \details
        Returns the translation vector of this matrix.
    */
    GeVector3d translation() const;
  
    /** \details
        Returns the matrix which rotates
        by angle about an axis passing through center, and returns a reference
        to this matrix.
    
        \param angle [in]  Rotation angle.
        \param axis [in]  Rotation axis.
        \param center [in]  Center of rotation.
    */
    static GeMatrix3d rotation(double angle, const GeVector3d& axis);
    static GeMatrix3d rotation(double angle, const GeVector3d& axis,const GePoint3d& center);

    /** \details
        Returns the matrix which scales
        by scale about center, and returns a reference
        to this matrix.
    
        \param scale [in]  Scale factor.
        \param center [in]  Center of scaling.
    */
    static GeMatrix3d scaling(double scale);
    static GeMatrix3d scaling(double scale, const GePoint3d& center);

    /** \details
        Returns the matrix which scales
        by scale about center, and returns a reference
        to this matrix.
    
        \param scale [in]  Scale factor.
        \param center [in]  Center of scaling.
    */
    static GeMatrix3d scaling(const GeScale3d& scale);
    static GeMatrix3d scaling(const GeScale3d& scale, const GePoint3d& center);

    /** \details
        Sets this matrix to the matrix which mirrors
        about the specified object, and returns a reference
        to this matrix.
    
        \param mirrorPlane [in]  Mirror plane.
    */
    static GeMatrix3d mirroring(const GePlane& mirrorPlane);

    /** \details
        Sets this matrix to the matrix which mirrors
        about the specified object, and returns a reference
        to this matrix.
    
        \param mirrorPoint [in]  Mirror point.
    */  
    static GeMatrix3d mirroring(const GePoint3d& mirrorPoint);

    /** \details
        Sets this matrix to the matrix which mirrors
        about the specified object, and returns a reference
        to this matrix.

        \param mirrorLine [in]  Mirror line entity.
    */
    static GeMatrix3d mirroring(const GeLine3d& mirrorLine);

    /** \details
        Returns the matrix which maps
        the coordinate system defined by fromOrigin, fromXAxis, fromYAxis, and fromZAxis,
        to the coordinate system 
        defined by toOrigin, toXAxis, toYAxis, and toZAxis.
    
        \param fromOrigin [in]  Origin of the initial coordinate system.
        \param fromXAxis [in]  X-axis of the initial coordinate system.
        \param fromYAxis [in]  Y-axis of the initial coordinate system.
        \param fromZAxis [in]  Z-axis of the initial coordinate system.
        \param toOrigin [in]  Origin of the initial coordinate system.
        \param toXAxis [in]  X-axis of the initial coordinate system.
        \param toYAxis [in]  Y-axis of the initial coordinate system.
        \param toZAxis [in]  Z-axis of the initial coordinate system.
    */
    static GeMatrix3d alignCoordSys(
        const GePoint3d& fromOrigin,
        const GeVector3d& fromXAxis,
        const GeVector3d& fromYAxis,
        const GeVector3d& fromZAxis,
        const GePoint3d& toOrigin,
        const GeVector3d& toXAxis,
        const GeVector3d& toYAxis,
        const GeVector3d& toZAxis);

    /** \details
        Returns the matrix which maps the WCS to
        the plane coordinate system.
    
        \param normal [in]  The normal to the plane.    
    
        \remarks
        If normal is specified, the origin of the plane is (0,0,0).   
    */
    static GeMatrix3d worldToPlane(const GeVector3d& normal);
  
    /** \details
        Returns the matrix which maps the WCS to
        the plane coordinate system.
    
        \param plane [in]  The plane.   
    */
    static GeMatrix3d worldToPlane(const GePlane&);
    
    /** \details
        Returns the matrix which maps
        the plane coordinate system to the WCS.
    
        \param normal [in]  The normal to the plane.    
    
        \remarks
        If normal is specified, the origin of the plane is (0,0,0)    
    */
    static GeMatrix3d planeToWorld(const GeVector3d& normal);

    /** \details
        Returns the matrix which maps
        the plane coordinate system to the WCS.
      
        \param plane [in]  The plane.   
    */
    static GeMatrix3d planeToWorld(const GePlane&);

    /** \details
        Returns the scale factor of this matrix.
    
        \remarks
        The scale factor is the square root of the longest column vector
        of the linear part of this matrix.
    */
    GeScale3d scale() const;

    /** \details
        Returns the largest absolute value of the linear part of this matrix.
    */
    double norm() const;
  
    // For convenient access to the data.
    //
  
        /** \details
        Returns or references entry[row] as matrix[row].

        \param row [in]  Row.
    */
    const double* operator [](int row) const;
    double* operator [](int row);

    /** \details
        Returns or references entry[row][column] as matrix(row,column).

        \param row [in]  Row.
        \param column [in]  Column.
    */
    double operator ()(int row, int column) const;
    double& operator ()(int row, int column);

    double entry[4][4]; 
};

// these operations really decrease rendering performance in non-inline case

inline const double* GeMatrix3d::operator [](int row) const 
{ 
  return entry[row]; 
}

inline double* GeMatrix3d::operator [](int row) 
{ 
  return entry[row]; 
}

inline double GeMatrix3d::operator ()(int row, int column) const 
{ 
  return entry[row][column]; 
}

inline double& GeMatrix3d::operator ()(int row, int column) 
{ 
  return entry[row][column]; 
}



#endif
