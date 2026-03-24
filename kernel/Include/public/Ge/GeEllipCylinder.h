#ifndef GEELLIPCYLINDER_H
#define GEELLIPCYLINDER_H

#include "GeEntity3dMethods.h"
#include "GeInterval.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeLinearEnt3d;

class GeEllipCylinder : public GeSurface
{
public:
    GeEllipCylinder();
    GeEllipCylinder(double minorRadius, double majorRadius, const GePoint3d& origin, const GeVector3d& axisOfSymmetry);
    GeEllipCylinder(
        double minorRadius,
        double majorRadius,
        const GePoint3d& origin,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& majorAxis,
        const GeInterval& height,
        double startAng,
        double endAng);
    GeEllipCylinder(const GeEllipCylinder& cylinder);

    GEENTITY3D_METHODS(GeEllipCylinder);

    double radiusRatio() const;
    double minorRadius() const;
    double majorRadius() const;
    GePoint3d origin() const;
    void getAngles(double& startAng, double& endAng) const;
    void getHeight(GeInterval& height) const;
    double heightAt(double u) const;
    double getUParamScale() const;
    GeVector3d axisOfSymmetry() const;
    GeVector3d majorAxis() const;
    GeVector3d minorAxis() const;
    bool isOuterNormal() const;
    bool isNormalReversed() const override;
    bool isLeftHanded() const;
    bool isClosed(const GeTol& tol = GeContext::gTol) const;
    GeSurface& reverseNormal() override;

    void setIsOuterNormal(bool isOuterNormal);
    void setUParamScale(double uScale = 0.0);
    GeEllipCylinder& setMinorRadius(double minorRadius);
    GeEllipCylinder& setMajorRadius(double majorRadius);
    GeEllipCylinder& setAngles(double startAng, double endAng);
    GeEllipCylinder& setHeight(const GeInterval& height);
    GeEllipCylinder& set(double minorRadius, double majorRadius, const GePoint3d& origin, const GeVector3d& axisOfSymmetry);
    GeEllipCylinder& set(
        double minorRadius,
        double majorRadius,
        const GePoint3d& origin,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& majorAxis,
        const GeInterval& height,
        double startAng,
        double endAng);

    bool project(const GePoint3d& pnt, GePoint3d& projPnt) const;
    bool project(const GePoint3d& pnt, GePoint3d& projPnt, const GeTol& tol) const;

    GeEllipCylinder& operator = (const GeEllipCylinder& cylinder);

    bool intersectWith(
        const GeLinearEnt3d& linEnt,
        int& numInt,
        GePoint3d& p1,
        GePoint3d& p2,
        const GeTol& tol = GeContext::gTol) const;

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    GeVector3d axisUnit() const;
    GeVector3d majorAxisUnit() const;
    GeVector3d minorAxisUnit() const;

private:
    double m_minorRadius;
    double m_majorRadius;
    GePoint3d m_origin;
    GeVector3d m_axis;
    GeVector3d m_majorAxis;
    GeInterval m_height;
    double m_startAng;
    double m_endAng;
    double m_uParamScale;
    bool m_hasCustomUParamScale;
    bool m_outerNormal;
};

#endif