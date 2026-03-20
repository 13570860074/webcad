#ifndef GESURFACECURVE2DTO3D_H
#define GESURFACECURVE2DTO3D_H

#include "GeCurve2d.h"
#include "GeCurve3d.h"
#include "GeEntity3dMethods.h"
#include "GeExtents3d.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeSurfaceCurve2dTo3d : public GeCurve3d
{
public:
    enum OwnershipFlag
    {
        kCurveCopy = 0x01,
        kCurveOwn = 0x02,
        kCurveReference = 0x03,
        kSurfaceCopy = 0x01 << 2,
        kSurfaceOwn = 0x02 << 2,
        kSurfaceReference = 0x03 << 2,
        kCopy = kCurveCopy | kSurfaceCopy,
        kOwn = kCurveOwn | kSurfaceOwn,
        kReference = kCurveReference | kSurfaceReference,
        kCurveMask = 0x03,
        kSurfaceMask = 0x03 << 2
    };

public:
    GeSurfaceCurve2dTo3d();
    GeSurfaceCurve2dTo3d(const GeSurfaceCurve2dTo3d& source);
    GeSurfaceCurve2dTo3d(const GeCurve2d& uvc, const GeSurface& surf);
    GeSurfaceCurve2dTo3d(const GeCurve2d* pUvCurve, const GeSurface* pSurface, OwnershipFlag ownership);
    ~GeSurfaceCurve2dTo3d();

    GEENTITY3D_METHODS(GeSurfaceCurve2dTo3d);

    GeSurfaceCurve2dTo3d& operator = (const GeSurfaceCurve2dTo3d& src);

    const GeCurve2d* curve() const;
    const GeSurface* surface() const;
    void getGeomExtents(GeExtents3d& extents) const;

    bool set(const GeCurve2d* pUvCurve, const GeSurface* pSurface, OwnershipFlag ownership);

    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint3d& startPoint, GePoint3d& endPoint) const override;
    GePoint3d evalPoint(double param) const override;
    double paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    bool isOn(const GePoint3d& pnt, double& param, const GeTol& tol) const override;

private:
    void clearOwnedData();

private:
    GeCurve2d* m_pUvCurve;
    GeSurface* m_pSurface;
    OwnershipFlag m_ownership;
};

inline GeSurfaceCurve2dTo3d::OwnershipFlag operator|(
    GeSurfaceCurve2dTo3d::OwnershipFlag a,
    GeSurfaceCurve2dTo3d::OwnershipFlag b)
{
    return static_cast<GeSurfaceCurve2dTo3d::OwnershipFlag>(static_cast<int>(a) | static_cast<int>(b));
}

#endif