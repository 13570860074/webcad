#ifndef GESURFACECURVE3DTO2D_H
#define GESURFACECURVE3DTO2D_H

#include "GeCurve2d.h"
#include "GeCurve3d.h"
#include "GeEntity2dMethods.h"
#include "GeExtents2d.h"
#include "GeMatrix2d.h"
#include "GeSurface.h"

class GeSurfaceCurve3dTo2d : public GeCurve2d
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
    GeSurfaceCurve3dTo2d();
    GeSurfaceCurve3dTo2d(const GeSurfaceCurve3dTo2d& source);
    GeSurfaceCurve3dTo2d(const GeCurve3d& curve3d, const GeSurface& surf);
    GeSurfaceCurve3dTo2d(const GeCurve3d* pCurve3d, const GeSurface* pSurface, OwnershipFlag ownership);
    ~GeSurfaceCurve3dTo2d();

    GEENTITY2D_METHODS(GeSurfaceCurve3dTo2d);

    GeSurfaceCurve3dTo2d& operator = (const GeSurfaceCurve3dTo2d& src);

    const GeCurve3d* curve() const;
    const GeSurface* surface() const;
    void getGeomExtents(GeExtents2d& extents) const;

    bool set(const GeCurve3d* pCurve3d, const GeSurface* pSurface, OwnershipFlag ownership);

    void getInterval(GeInterval& range) const override;
    void getInterval(GeInterval& range, GePoint2d& startPoint, GePoint2d& endPoint) const override;
    GePoint2d evalPoint(double param) const override;
    double paramOf(const GePoint2d& pnt, const GeTol& tol) const override;
    bool isOn(const GePoint2d& pnt, double& param, const GeTol& tol) const override;

private:
    void clearOwnedData();

private:
    GeCurve3d* m_pCurve;
    GeSurface* m_pSurface;
    OwnershipFlag m_ownership;
    GeMatrix2d m_uvXfm;
};

inline GeSurfaceCurve3dTo2d::OwnershipFlag operator|(
    GeSurfaceCurve3dTo2d::OwnershipFlag a,
    GeSurfaceCurve3dTo2d::OwnershipFlag b)
{
    return static_cast<GeSurfaceCurve3dTo2d::OwnershipFlag>(static_cast<int>(a) | static_cast<int>(b));
}

#endif