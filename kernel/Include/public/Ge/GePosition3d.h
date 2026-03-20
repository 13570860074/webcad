#ifndef GEPOSITION3D_H
#define GEPOSITION3D_H

#include "GePointEnt3d.h"
#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"

class GePosition3d : public GePointEnt3d
{
public:
    GePosition3d();
    GePosition3d(const GePoint3d& point);
    GePosition3d(double x, double y, double z);
    GePosition3d(const GePosition3d& source);

    GEENTITY3D_METHODS(GePosition3d);
    GEPOINTENT3D_METHODS(GePosition3d);

    GePosition3d& set(const GePoint3d& point);
    GePosition3d& set(double x, double y, double z);
    GePosition3d& operator = (const GePosition3d& pos);

private:
    GePoint3d m_point;
};

#endif