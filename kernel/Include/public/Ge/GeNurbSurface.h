#ifndef GENURBSURFACE_H
#define GENURBSURFACE_H

#include "GeDoubleArray.h"
#include "GeEntity3dMethods.h"
#include "GeKnotVector.h"
#include "GePoint3dArray.h"
#include "GeSurface.h"

class GeNurbCurve3d;

class GeNurbSurface : public GeSurface
{
public:
    GeNurbSurface();
    GeNurbSurface(
        int degreeInU,
        int degreeInV,
        int propsInU,
        int propsInV,
        int numControlPointsInU,
        int numControlPointsInV,
        const GePoint3dArray& controlPoints,
        const GeDoubleArray& weights,
        const GeKnotVector& uKnots,
        const GeKnotVector& vKnots,
        const GeTol& tol = GeContext::gTol);
    GeNurbSurface(const GeNurbSurface& source);

    GEENTITY3D_METHODS(GeNurbSurface);

    GeNurbSurface& operator = (const GeNurbSurface& nurb);

    bool isRationalInU() const;
    bool isPeriodicInU(double& period) const;
    bool isRationalInV() const;
    bool isPeriodicInV(double& period) const;
    int singularityInU() const;
    int singularityInV() const;

    int degreeInU() const;
    int numControlPointsInU() const;
    int degreeInV() const;
    int numControlPointsInV() const;

    void getControlPoints(GePoint3dArray& controlPoints) const;
    bool getWeights(GeDoubleArray& weights) const;
    int numKnotsInU() const;
    void getUKnots(GeKnotVector& uKnots) const;
    int numKnotsInV() const;
    void getVKnots(GeKnotVector& vKnots) const;

    void getDefinition(
        int& degreeInU,
        int& degreeInV,
        int& propsInU,
        int& propsInV,
        int& numControlPointsInU,
        int& numControlPointsInV,
        GePoint3dArray& controlPoints,
        GeDoubleArray& weights,
        GeKnotVector& uKnots,
        GeKnotVector& vKnots) const;

    GeNurbSurface& set(
        int degreeInU,
        int degreeInV,
        int propsInU,
        int propsInV,
        int numControlPointsInU,
        int numControlPointsInV,
        const GePoint3dArray& controlPoints,
        const GeDoubleArray& weights,
        const GeKnotVector& uKnots,
        const GeKnotVector& vKnots,
        const GeTol& tol = GeContext::gTol);

    GeNurbSurface& setFitData(
        const GePoint3dArray& fitPoints,
        const GeVector3dArray& arrTangentsInU,
        const GeVector3dArray& arrTangentsInV,
        const GeVector3dArray& arrMixedDerivs,
        const GeKnotVector& uKnots,
        const GeKnotVector& vKnots,
        const GeTol& tol);

    void computeVIsoLine(double V, GeNurbCurve3d& isoline) const;
    void computeUIsoLine(double U, GeNurbCurve3d& isoline) const;

    GePoint2d paramOfPrec(const GePoint3d& point, const GeTol& tol = GeContext::gTol) const;
    int loc(int i, int j) const;

    GePoint2d paramOf(const GePoint3d& pnt, const GeTol& tol) const override;
    GePoint3d evalPoint(const GePoint2d& param) const override;
    void getEnvelope(GeInterval& rangeU, GeInterval& rangeV) const override;

private:
    void ensureDefaults();
    GePoint3d controlPointAt(int i, int j) const;

private:
    int m_degreeU;
    int m_degreeV;
    int m_propsU;
    int m_propsV;
    int m_numCtrlU;
    int m_numCtrlV;
    GePoint3dArray m_controlPoints;
    GeDoubleArray m_weights;
    GeKnotVector m_uKnots;
    GeKnotVector m_vKnots;
    GeTol m_tol;
};

#endif