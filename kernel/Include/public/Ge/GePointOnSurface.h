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

#ifndef GEPOINTONSURFACE_H
#define GEPOINTONSURFACE_H

#include "GePointEnt3d.h"
#include "GeSurface.h"
#include "GeEntity3dMethods.h"

class GePointEnt3dImpl;
class GePointOnSurface : public GePointEnt3d
{
public:
    GePointOnSurface();
    GePointOnSurface(const GeSurface& surf);
    GePointOnSurface(const GeSurface& surf, const GePoint2d& param);
    GePointOnSurface(const GePointOnSurface& src);
    GEENTITY3D_METHODS(GePointOnSurface);
    GEPOINTENT3D_METHODS(GePointOnSurface);

    // Assignment operator.
    //
    GePointOnSurface& operator = (const GePointOnSurface& src);

    // Query functions.
    //
    const GeSurface* surface() const;
    GePoint2d parameter() const;

    // Functions to evaluate a point.
    //
    GePoint3d point() const;
    GePoint3d point(const GePoint2d& param) const;
    GePoint3d point(const GeSurface& surf, const GePoint2d& param) const;

    // Set functions.
    //
    GePointOnSurface& setSurface(const GeSurface& surf);
    GePointOnSurface& setParameter(const GePoint2d& param);
};


#endif
