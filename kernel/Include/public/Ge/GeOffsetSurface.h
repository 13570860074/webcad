#ifndef GEOFFSETSURFACE_H
#define GEOFFSETSURFACE_H

#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeOffsetSurface : public GeSurface
{
public:
    GeOffsetSurface();
    GeOffsetSurface(GeSurface* baseSurface, double offsetDistance, bool makeCopy = true);
    GeOffsetSurface(const GeOffsetSurface& source);
    ~GeOffsetSurface();

    GEENTITY3D_METHODS(GeOffsetSurface);

    bool isPlane() const;
    bool isBoundedPlane() const;
    bool isSphere() const;
    bool isCylinder() const;
    bool isCone() const;
    bool isEllipCylinder() const;
    bool isEllipCone() const;
    bool isTorus() const;

    bool getSurface(GeSurface*& simpleSurface) const;
    void getConstructionSurface(GeSurface*& baseSurface) const;
    double offsetDist() const;

    GeOffsetSurface& set(GeSurface* baseSurface, double offsetDistance, bool makeCopy = true);
    GeOffsetSurface& operator = (const GeOffsetSurface& surface);

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;

private:
    void clearOwnedSurface();

private:
    GeSurface* m_pBaseSurface;
    bool m_isOwner;
    double m_offsetDistance;
};

#endif