#ifndef GEPOSITION2D_H
#define GEPOSITION2D_H

#include "GePointEnt2d.h"
#include "GeEntity2dMethods.h"
#include "GeMatrix2d.h"

class GePosition2d : public GePointEnt2d
{
public:
    GePosition2d();
    GePosition2d(const GePoint2d& point);
    GePosition2d(double x, double y);
    GePosition2d(const GePosition2d& source);

    GEENTITY2D_METHODS(GePosition2d);
    GEPOINTENT2D_METHODS(GePosition2d);

    GePosition2d& set(const GePoint2d& point);
    GePosition2d& set(double x, double y);
    GePosition2d& operator = (const GePosition2d& pos);

private:
    GePoint2d m_point;
};

#endif