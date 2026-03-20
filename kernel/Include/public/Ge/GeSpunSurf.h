#ifndef GESPUNSURF_H
#define GESPUNSURF_H

#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeCurve3d;

class GeSpunSurf : public GeSurface
{
public:
    GeSpunSurf();
    GeSpunSurf(const GeCurve3d& pProfile, const GePoint3d& pBase, const GeVector3d& pAxis);
    GeSpunSurf(const GeSpunSurf& spunSurf);
    ~GeSpunSurf();

    GEENTITY3D_METHODS(GeSpunSurf);

    GeSpunSurf& operator = (const GeSpunSurf& spunSurface);

    const GeCurve3d* getProfile() const;
    GePoint3d getBasePoint() const;
    GeVector3d getAxis() const;
    GeVector3d getRef() const;

    GeSpunSurf& setAngles(double startAngle, double endAngle);
    GeSpunSurf& setRef(const GeVector3d& pRef);
    void set(const GeCurve3d& pProfile, const GePoint3d& pBase, const GeVector3d& pAxis);

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    void clearOwnedProfile();
    GeVector3d axisUnit() const;

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