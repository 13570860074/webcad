#ifndef GEGBL_H
#define GEGBL_H


#include "Ge.h"
#include "GeTol.h"

/** \details
    This structure provides a namespace for tolerance values and functions ubiquitous to the Ge library.

    \sa
    TD_Ge

    <group !!RECORDS_TD_APIRef>
*/
struct GeContext
{
    /** \details
      Provides the global default GeTol tolerance object.
      \remarks
      gTol.equalPoint == 1e-10  gTolequalVector == 1.e-10.
    */
    static GeTol gTol;

    /** \details
      Provides the global 0.0 default GeTol tolerance object.
      \remarks
      gZeroTol.equalPoint == 0.0 and gZeroTol.equalVector == 1.e-10.
    */
    static GeTol gZeroTol;
};



#endif 

