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
// This file contains the class GePointEnt3d - An abstract base
// class to represent point entities.

#ifndef GEPOINTENT3D_H
#define GEPOINTENT3D_H

#include "GeEntity3d.h"

class GePointEnt3dImpl;
class GePointEnt3d : public GeEntity3d
{
public:

    // Return point coordinates.
    //
    virtual GePoint3d point3d() const;

    // Conversion operator to convert to GePoint2d.
    //
    virtual operator GePoint3d () const;
    
    // Assignment operator.
    //
    GePointEnt3d& operator =  (const GePointEnt3d& pnt);

em_protected:
    // Private constructors so that no object of this class can be instantiated.
    GePointEnt3d();
    GePointEnt3d(const GePointEnt3d&);
};


#endif
