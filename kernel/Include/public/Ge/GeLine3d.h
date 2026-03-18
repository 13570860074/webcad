#ifndef GELINE3D_H
#define GELINE3D_H

#include "GeLinearEnt3d.h"
#include "GeEntity3dMethods.h"


class GeLine3d : public GeLinearEnt3d
{
public:
    GeLine3d();
    GeLine3d(const GeLine3d& line);
    GeLine3d(const GePoint3d& pnt, const GeVector3d& vec);
    GeLine3d(const GePoint3d& pnt1, const GePoint3d& pnt2);
    GEENTITY3D_METHODS(GeLine3d);
    GECURVE3D_METHODS(GeLine3d);
    GELINEARENT3D_METHODS(GeLine3d);

    // The x-axis and y-axis and z-axis lines.
    //
    GE_STATIC_EXPORT static const GeLine3d kXAxis;
    GE_STATIC_EXPORT static const GeLine3d kYAxis;
    GE_STATIC_EXPORT static const GeLine3d kZAxis;

    // Set methods.
    //
    GeLine3d& set(const GePoint3d& pnt, const GeVector3d& vec);
    GeLine3d& set(const GePoint3d& pnt1, const GePoint3d& pnt2);

    // Assignment operator.
    //
    GeLine3d& operator = (const GeLine3d& line);

    // Interval.
    //
    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const override;
};



#endif


