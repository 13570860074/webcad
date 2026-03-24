#ifndef GEELLIPCONE_H
#define GEELLIPCONE_H

#include "GeEntity3dMethods.h"
#include "GeInterval.h"
#include "GeMatrix3d.h"
#include "GePointOnSurface.h"
#include "GeSurface.h"

class GeLinearEnt3d;

class GeEllipCone : public GeSurface
{
public:
    GeEllipCone();
    GeEllipCone(double cosineAngle, double sineAngle, const GePoint3d& origin, double minorRadius, double majorRadius, const GeVector3d& axisOfSymmetry);
    GeEllipCone(
        double cosineAngle,
        double sineAngle,
        const GePoint3d& baseOrigin,
        double minorRadius,
        double majorRadius,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& majorAxis,
        const GeInterval& height,
        double startAng,
        double endAng);
    GeEllipCone(const GeEllipCone& cone);

    GEENTITY3D_METHODS(GeEllipCone);

    double radiusRatio() const;
    double minorRadius() const;
    double majorRadius() const;
    GePoint3d baseCenter() const;
    void getAngles(double& startAng, double& endAng) const;
    double halfAngle() const;
    void getHalfAngle(double& cosineAngle, double& sineAngle) const;
    void getHeight(GeInterval& height) const;
    double heightAt(double u) const;
    double getUParamScale() const;
    GeVector3d axisOfSymmetry() const;
    GeVector3d majorAxis() const;
    GeVector3d minorAxis() const;
    GePoint3d apex() const;
    bool isClosed(const GeTol& tol = GeContext::gTol) const;
    bool isOuterNormal() const;
    bool isNormalReversed() const override;
    bool isLeftHanded() const override;
    GeSurface& reverseNormal() override;
    void setUParamScale(double uScale = 0.0);

    GeEllipCone& setMinorRadius(double minorRadius);
    GeEllipCone& setMajorRadius(double majorRadius);
    GeEllipCone& setAngles(double startAng, double endAng);
    GeEllipCone& setHeight(const GeInterval& height);
    double getPoleParam() const;
    bool project(const GePoint3d& pnt, GePoint3d& projPnt) const;
    bool project(const GePoint3d& pnt, GePoint3d& projPnt, const GeTol& tol) const;
    GeEllipCone& set(
        double cosineAngle,
        double sineAngle,
        const GePoint3d& origin,
        double minorRadius,
        double majorRadius,
        const GeVector3d& axisOfSymmetry);
    GeEllipCone& set(
        double cosineAngle,
        double sineAngle,
        const GePoint3d& baseOrigin,
        double minorRadius,
        double majorRadius,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& majorAxis,
        const GeInterval& height,
        double startAng,
        double endAng);

    GeEllipCone& operator = (const GeEllipCone& cone);

    bool intersectWith(
        const GeLinearEnt3d& linEnt,
        int& numInt,
        GePoint3d& p1,
        GePoint3d& p2,
        const GeTol& tol = GeContext::gTol) const;

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d closestPointTo(const GePoint3d& pnt) const override;
    GePoint3d closestPointTo(const GePoint3d& pnt, const GeTol& tol) const override;
    void getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result) const override;
    void getClosestPointTo(const GePoint3d& pnt, GePointOnSurface& result, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    GeVector3d axisUnit() const;
    GeVector3d majorAxisUnit() const;
    GeVector3d minorAxisUnit() const;
    double signedTanHalfAngle() const;
    double majorRadiusAt(double u) const;
    double minorRadiusAt(double u) const;

private:
    double m_cosHalf;
    double m_sinHalf;
    double m_minorRadius;
    double m_majorRadius;
    GePoint3d m_baseCenter;
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