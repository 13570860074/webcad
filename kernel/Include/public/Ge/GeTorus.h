#ifndef GETORUS_H
#define GETORUS_H

#include "GeEntity3dMethods.h"
#include "GeMatrix3d.h"
#include "GeSurface.h"

class GeLinearEnt3d;

class GeTorus : public GeSurface
{
public:
    GeTorus();
    GeTorus(double majorRadius, double minorRadius, const GePoint3d& center, const GeVector3d& axisOfSymmetry);
    GeTorus(
        double majorRadius,
        double minorRadius,
        const GePoint3d& center,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& refAxis,
        double startAngleU,
        double endAngleU,
        double startAngleV,
        double endAngleV);
    GeTorus(const GeTorus& source);

    GEENTITY3D_METHODS(GeTorus);

    double majorRadius() const;
    double minorRadius() const;
    void getAnglesInU(double& startAngleU, double& endAngleU) const;
    void getAnglesInV(double& startAngleV, double& endAngleV) const;
    GePoint3d center() const;
    GeVector3d axisOfSymmetry() const;
    GeVector3d refAxis() const;
    bool isOuterNormal() const;

    GeTorus& setMajorRadius(double radius);
    GeTorus& setMinorRadius(double radius);
    GeTorus& setAnglesInU(double startAngleU, double endAngleU);
    GeTorus& setAnglesInV(double startAngleV, double endAngleV);
    GeTorus& set(double majorRadius, double minorRadius, const GePoint3d& center, const GeVector3d& axisOfSymmetry);
    GeTorus& set(
        double majorRadius,
        double minorRadius,
        const GePoint3d& center,
        const GeVector3d& axisOfSymmetry,
        const GeVector3d& refAxis,
        double startAngleU,
        double endAngleU,
        double startAngleV,
        double endAngleV);

    GeTorus& operator = (const GeTorus& torus);

    bool intersectWith(
        const GeLinearEnt3d& linEnt,
        int& numInt,
        GePoint3d& p1,
        GePoint3d& p2,
        GePoint3d& p3,
        GePoint3d& p4,
        const GeTol& tol = GeContext::gTol) const;

    bool isLemon() const;
    bool isApple() const;
    bool isVortex() const;
    bool isDoughnut() const;
    bool isDegenerate() const;
    bool isHollow() const;
    bool isReverseV() const;
    void setReverseV(bool isReverseV);

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    GeVector3d axisUnit() const;
    GeVector3d refUnit() const;
    GeVector3d orthoRefUnit() const;

private:
    double m_majorRadius;
    double m_minorRadius;
    GePoint3d m_center;
    GeVector3d m_axis;
    GeVector3d m_ref;
    double m_startU;
    double m_endU;
    double m_startV;
    double m_endV;
    bool m_outerNormal;
    bool m_reverseV;
};

#endif