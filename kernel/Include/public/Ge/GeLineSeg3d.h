#ifndef GELINESEG3D_H
#define GELINESEG3D_H

#include "GeLinearEnt3d.h"
#include "GeEntity3dMethods.h"

class GeLineSeg3d : public GeLinearEnt3d
{
public:
    GeLineSeg3d();
    GeLineSeg3d(const GeLineSeg3d& line);
    GeLineSeg3d(const GePoint3d& pnt1, const GePoint3d& pnt2);
    GeLineSeg3d(const GePoint3d& pnt, const GeVector3d& vec);
    GEENTITY3D_METHODS(GeLineSeg3d);
    GECURVE3D_METHODS(GeLineSeg3d);
    GELINEARENT3D_METHODS(GeLineSeg3d);

    // Set methods.
    //
    GeLineSeg3d& set(const GePoint3d& pnt, const GeVector3d& vec);
    GeLineSeg3d& set(const GePoint3d& pnt1, const GePoint3d& pnt2);

    // Bisector.
    //
    void getBisector(GeLine3d& line) const;

    // Barycentric combination of end points.
    //
    GePoint3d baryComb(double blendCoeff) const;

    // Definition of linear segment
    //
    GePoint3d startPoint() const;
    GePoint3d midPoint() const;
    GePoint3d endPoint() const;

    // Assignment operator.
    //
    GeLineSeg3d& operator =  (const GeLineSeg3d& line);
};

#endif

