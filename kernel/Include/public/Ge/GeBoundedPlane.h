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
// This file contains the class GeBoundedPlane - A mathematical entity
// used to represent a 4-side bounded plane in 3-space.

#ifndef GEBOUNDEDPLANE_H
#define GEBOUNDEDPLANE_H


#include "GePlane.h"
#include "GeEntity3dMethods.h"


class GePlane;
class GeVector3d;
class GePoint3d;
class GePoint2d;
class GeLineSeg3d;

class GeBoundedPlane : public GePlanarEnt
{
public:
    GeBoundedPlane();
    GeBoundedPlane(const GeBoundedPlane& plane);
    GeBoundedPlane(const GePoint3d& origin, const GeVector3d& uVec, const GeVector3d& vVec);
    GeBoundedPlane(const GePoint3d& p1, const GePoint3d& origin, const GePoint3d& p2);
    GEENTITY3D_METHODS(GeBoundedPlane);
    GESURFACE_METHODS(GeBoundedPlane);
    GEPLANARENT_METHODS(GeBoundedPlane);

    bool isLeftHanded() const;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const;

    // Intersection.
    //
    bool intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& point) const;
    bool intersectWith(const GeLinearEnt3d& linEnt, GePoint3d& point, const GeTol& tol) const;
    bool intersectWith(const GePlane& plane, GeLineSeg3d& results) const;
    bool intersectWith(const GePlane& plane, GeLineSeg3d& results, const GeTol& tol) const;
    bool intersectWith(const GeBoundedPlane& plane, GeLineSeg3d& result) const;
    bool intersectWith(const GeBoundedPlane& plane, GeLineSeg3d& result, const GeTol& tol) const;

    // Set methods.
    //
    GeBoundedPlane& set(const GePoint3d& origin, const GeVector3d& uVec, const GeVector3d& vVec);
    GeBoundedPlane& set(const GePoint3d& p1, const GePoint3d& origin, const GePoint3d& p2);
    // Assignment operator.
    //
    GeBoundedPlane& operator = (const GeBoundedPlane& bplane);
};



#endif
