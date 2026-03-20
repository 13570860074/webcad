#ifndef GEREVOLVEDSURFACE_H
#define GEREVOLVEDSURFACE_H

#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeCurve3d;

class GeRevolvedSurface : public GeSurface
{
public:
    GeRevolvedSurface();
    GeRevolvedSurface(
        const GeCurve3d& pProfile,
        const GePoint3d& pBase,
        const GeVector3d& pAxis,
        const GeVector3d& pRef,
        double startAngle = 0.0,
        double endAngle = PI * 2.0);
    GeRevolvedSurface(const GeRevolvedSurface& revolvedSurf);
    ~GeRevolvedSurface();

    GEENTITY3D_METHODS(GeRevolvedSurface);

    GeRevolvedSurface& operator = (const GeRevolvedSurface& revolvedSurf);

    const GeCurve3d& getProfile() const;
    GePoint3d getBasePoint() const;
    GeVector3d getAxis() const;
    GeVector3d getRef() const;
    void getAngles(double& startAngle, double& endAngle) const;

    GeRevolvedSurface& setAngles(double startAngle, double endAngle);
    GeRevolvedSurface& setRef(const GeVector3d& pRef);
    void set(const GeCurve3d& pProfile, const GePoint3d& pBase, const GeVector3d& pAxis, const GeVector3d& pRef);

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    void clearOwnedProfile();
    GeVector3d axisUnit() const;
    GeVector3d refUnit() const;
    GeVector3d orthoRefUnit() const;

private:
    GeCurve3d* m_pProfile;
    bool m_isOwner;
    GePoint3d m_base;
    GeVector3d m_axis;
    GeVector3d m_ref;
    double m_startAngle;
    double m_endAngle;
};

#endif