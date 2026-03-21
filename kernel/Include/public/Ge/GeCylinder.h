#ifndef GECYLINDER_H
#define GECYLINDER_H

#include "GeEntity3dMethods.h"
#include "GeInterval.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeCylinder : public GeSurface
{
public:
    GeCylinder();
    GeCylinder(double radius, const GePoint3d& origin, const GeVector3d& axisOfSymmetry);
    GeCylinder(
        double radius,
        const GePoint3d& origin,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& refAxis,
        const GeInterval& height,
        double startAng,
        double endAng);
    GeCylinder(const GeCylinder& cylinder);

    GEENTITY3D_METHODS(GeCylinder);

    double radius() const;
    GePoint3d origin() const;
    void getAngles(double& startAng, double& endAng) const;
    void getHeight(GeInterval& height) const;
    double heightAt(double u) const;
    GeVector3d axisOfSymmetry() const;
    GeVector3d refAxis() const;
    bool isOuterNormal() const;
    bool isClosed(const GeTol& tol = GeContext::gTol) const;

    GeCylinder& setRadius(double radius);
    GeCylinder& setAngles(double startAng, double endAng);
    GeCylinder& setHeight(const GeInterval& height);
    GeCylinder& set(double radius, const GePoint3d& origin, const GeVector3d& axisOfSym);
    GeCylinder& set(
        double radius,
        const GePoint3d& origin,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& refAxis,
        const GeInterval& height,
        double startAng,
        double endAng);

    GeCylinder& operator = (const GeCylinder& cylinder);

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    GeVector3d axisUnit() const;
    GeVector3d refUnit() const;
    GeVector3d orthoRefUnit() const;

private:
    double m_radius;
    GePoint3d m_origin;
    GeVector3d m_axis;
    GeVector3d m_ref;
    GeInterval m_height;
    double m_startAng;
    double m_endAng;
    bool m_outerNormal;
};

#endif