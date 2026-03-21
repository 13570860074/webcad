#ifndef GECONE_H
#define GECONE_H

#include "GeEntity3dMethods.h"
#include "GeInterval.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeCone : public GeSurface
{
public:
    GeCone();
    GeCone(double cosineAngle, double sineAngle, const GePoint3d& baseOrigin, double baseRadius, const GeVector3d& axisOfSymmetry);
    GeCone(
        double cosineAngle,
        double sineAngle,
        const GePoint3d& baseOrigin,
        double baseRadius,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& refAxis,
        const GeInterval& height,
        double startAng,
        double endAng);
    GeCone(const GeCone& cone);

    GEENTITY3D_METHODS(GeCone);

    double baseRadius() const;
    GePoint3d baseCenter() const;
    void getAngles(double& startAng, double& endAng) const;
    double halfAngle() const;
    void getHalfAngle(double& cosineAngle, double& sineAngle) const;
    void getHalfAngleSigned(double& cosineAngle, double& sineAngle) const;
    void getHeight(GeInterval& height) const;
    double heightAt(double u) const;
    GeVector3d axisOfSymmetry() const;
    GeVector3d refAxis() const;
    GePoint3d apex() const;
    bool isClosed(const GeTol& tol = GeContext::gTol) const;
    bool isOuterNormal() const;

    GeCone& setBaseRadius(double baseRadius);
    GeCone& setBaseCenter(const GePoint3d& baseOrigin);
    GeCone& setAngles(double startAng, double endAng);
    GeCone& setHeight(const GeInterval& height);
    GeCone& set(
        double cosineAngle,
        double sineAngle,
        const GePoint3d& baseOrigin,
        double baseRadius,
        const GeVector3d& axisOfSymmetry);
    GeCone& set(
        double cosineAngle,
        double sineAngle,
        const GePoint3d& baseOrigin,
        double baseRadius,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& refAxis,
        const GeInterval& height,
        double startAng,
        double endAng);

    GeCone& operator = (const GeCone& cone);

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    GeVector3d axisUnit() const;
    GeVector3d refUnit() const;
    GeVector3d orthoRefUnit() const;
    double signedTanHalfAngle() const;
    double radiusAt(double u) const;

private:
    double m_cosHalf;
    double m_sinHalf;
    GePoint3d m_baseCenter;
    double m_baseRadius;
    GeVector3d m_axis;
    GeVector3d m_ref;
    GeInterval m_height;
    double m_startAng;
    double m_endAng;
    bool m_outerNormal;
};

#endif