#ifndef GERAY3D_H
#define GERAY3D_H 

#include "GeLinearEnt3d.h"



class GeRay3d : public GeLinearEnt3d
{
public:
    GeRay3d();
    GeRay3d(const GeRay3d& line);
    GeRay3d(const GePoint3d& pnt, const GeVector3d& vec);
    GeRay3d(const GePoint3d& pnt1, const GePoint3d& pnt2);
    GEENTITY3D_METHODS(GeRay3d);
    GECURVE3D_METHODS(GeRay3d);
    GELINEARENT3D_METHODS(GeRay3d);

    // Set methods.
    //
    GeRay3d& set(const GePoint3d& pnt, const GeVector3d& vec);
    GeRay3d& set(const GePoint3d& pnt1, const GePoint3d& pnt2);

    // Assignment operator.
    //
    GeRay3d& operator =  (const GeRay3d& line);

    // Interval.
    //
    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const override;
};

#endif
