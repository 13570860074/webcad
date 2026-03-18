#ifndef GETOL_H
#define GETOL_H

#include <cmath>
#include "GeExport.h"


/** \details
  This class represents tolerances for vectors or points coincidence.  
  For evaluating points and vectors GeTol stores the equalPoint and equalVector values which are used as follows:
  1. Two points, p1 and p2, are equal if (p1 - p2).length() <= equalPoint.
  2. Two vectors, v1 and v2, are equal if (v1 - v2).length() <= equalVector.
  3. Two vectors, v1 and v2, are parallel if v1/v1.length() - v2/v2.length()).length() < equalVector OR (v1/v1.length() + v2/v2.length()).length() < equalVector.
  4. Two vectors, v1 and v2, are perpendicular if abs((v1.dotProduct(v2))/(v1.length()*v2.length())) <= equalVector.
  5. Two lines or rays are parallel (or perpendicular) if their directional vectors are parallel (or perpendicular).
    
    Corresponding C++ library: TD_Ge

    <group Ge_Classes>

    \sa
    <link ge_GeTol.html, Working with Tolerances>
*/
class GeTol
{
public:

  /** \param t [in]  The tolerance for vectors and points evaluation.
    \param t1 [in]  The tolerance for points evaluation.
    \param t2 [in]  The tolerance for vectors evaluation.
  */
  GeTol(double t = 1.e-5) 
    : m_vectorTol(t), m_pointTol(t)
  {
  }
  GeTol(double t1, double t2) 
    : m_vectorTol(t2), m_pointTol(t1)
  {
  }

  /** \details
    Returns the equalPoint tolerance.
  */
  double equalPoint() const 
  { 
    return m_pointTol; 
  }

  /** \details
    Returns the equalVector tolerance.
  */
  double equalVector() const 
  { 
    return m_vectorTol;
  }

  /** \details
    Sets the equalPoint tolerance to a specified value.
    
    \param val [in]  equalPoint value to be set.
  */
  void setEqualPoint(double val)
  { 
    m_pointTol = val; 
  }

  /** \details
    Sets the equalVector tolerance to a specified value.
    
    \param val [in]  equalVector tolerance to be set.
  */
  void setEqualVector(double val) 
  {
    m_vectorTol = val; 
  }

private:
  double m_vectorTol;
  double m_pointTol;
};


#endif

