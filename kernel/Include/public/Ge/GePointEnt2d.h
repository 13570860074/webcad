//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2019 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//
// This file contains the class GePointEnt2d - An abstract base
// class to represent point entities.

#ifndef GEPOINTENT2D_H
#define GEPOINTENT2D_H

#include "GeEntity2d.h"

class GePointEnt2dImpl;
class GePointEnt2d : public GeEntity2d
{
public:

    // Return point coordinates.
    //
    virtual GePoint2d point2d() const;

    // Conversion operator to convert to GePoint2d.
    //
    virtual operator GePoint2d () const;
    
    // Assignment operator.
    //
    GePointEnt2d& operator =  (const GePointEnt2d& pnt);

em_protected:
    // Private constructors so that no object of this class can be instantiated.
    GePointEnt2d();
    GePointEnt2d(const GePointEnt2d&);
};


#endif
