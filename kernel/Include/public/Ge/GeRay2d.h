#ifndef GERAY2D_H
#define GERAY2D_H /*!DOM*/

#include "GeLinearEnt2d.h"



class GeRay2d : public GeLinearEnt2d
{
public:
    GeRay2d();
    GeRay2d(const GeRay2d& line);
    GeRay2d(const GePoint2d& pnt, const GeVector2d& vec);
    GeRay2d(const GePoint2d& pnt1, const GePoint2d& pnt2);
    GEENTITY2D_METHODS(GeRay2d);
    GECURVE2D_METHODS(GeRay2d);
    GELINEARENT2D_METHODS(GeRay2d);

    // Set methods.
    //
    GeRay2d& set(const GePoint2d& pnt, const GeVector2d& vec);
    GeRay2d& set(const GePoint2d& pnt1, const GePoint2d& pnt2);

    // Assignment operator.
    //
    GeRay2d& operator =  (const GeRay2d& line);

    // Interval.
    //
    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const override;
};

#endif
