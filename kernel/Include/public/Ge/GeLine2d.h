#ifndef GELINE2D_H
#define GELINE2D_H

#include "GeLinearEnt2d.h"
#include "GeEntity2dMethods.h"


class GeLine2d : public GeLinearEnt2d
{
public:
    GeLine2d();
    GeLine2d(const GeLine2d& line);
    GeLine2d(const GePoint2d& pnt, const GeVector2d& vec);
    GeLine2d(const GePoint2d& pnt1, const GePoint2d& pnt2);
    GEENTITY2D_METHODS(GeLine2d);
    GECURVE2D_METHODS(GeLine2d);
    GELINEARENT2D_METHODS(GeLine2d);

    // The x-axis and y-axis lines.
    //
    static const GeLine2d kXAxis;
    static const GeLine2d kYAxis;

    // Set methods.
    //
    GeLine2d& set(const GePoint2d& pnt, const GeVector2d& vec);
    GeLine2d& set(const GePoint2d& pnt1, const GePoint2d& pnt2);

    // Assignment operator.
    //
    GeLine2d& operator = (const GeLine2d& line);

    // Interval.
    //
    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const override;
};



#endif


