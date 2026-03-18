#ifndef GELINESEG2D_H
#define GELINESEG2D_H

#include "GeLinearEnt2d.h"
#include "GeEntity2dMethods.h"


class GeLineSeg2d : public GeLinearEnt2d
{
public:
    GeLineSeg2d();
    GeLineSeg2d(const GeLineSeg2d& line);
    GeLineSeg2d(const GePoint2d& pnt1, const GePoint2d& pnt2);
    GeLineSeg2d(const GePoint2d& pnt, const GeVector2d& vec);
    GEENTITY2D_METHODS(GeLineSeg2d);
    GECURVE2D_METHODS(GeLineSeg2d);
    GELINEARENT2D_METHODS(GeLineSeg2d);

    // Set methods.
    //
    GeLineSeg2d& set(const GePoint2d& pnt, const GeVector2d& vec);
    GeLineSeg2d& set(const GePoint2d& pnt1, const GePoint2d& pnt2);


    // Bisector.
    //
    void getBisector(GeLine2d& line) const;

    // Barycentric combination of end points.
    //
    GePoint2d baryComb(double blendCoeff) const;

    // Definition of linear segment
    //
    GePoint2d startPoint() const;
    GePoint2d midPoint() const;
    GePoint2d endPoint() const;

    // Assignment operator.
    //
    GeLineSeg2d& operator =  (const GeLineSeg2d& line);

    // Interval.
    //
    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const override;
};

#endif

