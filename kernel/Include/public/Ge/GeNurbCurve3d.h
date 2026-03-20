#ifndef GENURBCURVE3D_H
#define GENURBCURVE3D_H

#include "GeSplineEnt3d.h"
#include "GeDoubleArray.h"
#include "GeVector3dArray.h"

class GeEllipArc3d;
class GeLineSeg3d;
class GePolyline3d;

class GeNurbCurve3d : public GeSplineEnt3d
{
public:
    GeNurbCurve3d();
    GeNurbCurve3d(const GeNurbCurve3d& source);
    GeNurbCurve3d(int degree, const GeKnotVector& knots, const GePoint3dArray& controlPoints, bool isPeriodic = false);
    GeNurbCurve3d(int degree, const GeKnotVector& knots, const GePoint3dArray& controlPoints, const GeDoubleArray& weights, bool isPeriodic = false);
    GeNurbCurve3d(int degree, const GeKnotVector& knots, const GePoint3d* controlPoints, unsigned int numControlPoints, const double* weights, unsigned int numWeights, bool isPeriodic = false);
    GeNurbCurve3d(int degree, const GePolyline3d& fitPolyline, bool isPeriodic = false);
    GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeVector3d& startTangent, const GeVector3d& endTangent, bool startTangentDefined = true, bool endTangentDefined = true, const GeTol& fitTol = GeContext::gTol);
    GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeTol& fitTolerance = GeContext::gTol);
    GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeVector3dArray& fitTangents, const GeTol& fitTolerance = GeContext::gTol, bool isPeriodic = false);
    GeNurbCurve3d(const GePoint3dArray& fitPoints, const GeVector3d& startTangent, const GeVector3d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam, const GeTol& fitTolerance = GeContext::gTol);
    GeNurbCurve3d(const GeEllipArc3d& ellipse, int numSpans = 0);
    GeNurbCurve3d(const GeLineSeg3d& lineSeg);

    GEENTITY3D_METHODS(GeNurbCurve3d);

    int numFitPoints() const;
    bool getFitKnotParameterization(Ge::GeKnotParameterization& knotParam) const;
    bool getFitPointAt(int fitPointIndex, GePoint3d& point) const;
    bool getFitTolerance(GeTol& fitTolerance) const;
    bool getFitTangents(GeVector3d& startTangent, GeVector3d& endTangent) const;
    bool getFitTangents(GeVector3d& startTangent, GeVector3d& endTangent, bool& startTangentDefined, bool& endTangentDefined) const;
    bool getFitData(GePoint3dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector3d& startTangent, GeVector3d& endTangent) const;
    bool getFitData(GePoint3dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector3d& startTangent, GeVector3d& endTangent, Ge::GeKnotParameterization& knotParam) const;

    void getDefinitionData(int& degree, bool& rational, bool& periodic, GeKnotVector& knots, GePoint3dArray& controlPoints, GeDoubleArray& weights) const;

    int numWeights() const;
    double weightAt(int weightIndex) const;
    GeNurbCurve3d& setWeightAt(int weightIndex, double weight);
    bool evalMode() const;
    bool getParamsOfC1Discontinuity(GeDoubleArray& params, const GeTol& tol = GeContext::gTol) const;
    bool getParamsOfG1Discontinuity(GeDoubleArray& params, const GeTol& tol = GeContext::gTol) const;

    GeNurbCurve3d& operator = (const GeNurbCurve3d& spline);

private:
    void ensureDefaultData();
    void rebuildKnotsIfNeeded();
    void setFitMeta(const GePoint3dArray& fitPoints, const GeTol& fitTolerance, bool tangentsExist, const GeVector3d& startTangent, const GeVector3d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam);

private:
    GeDoubleArray m_weights;
    bool m_isPeriodic;
    bool m_evalMode;
    GePoint3dArray m_fitPoints;
    GeTol m_fitTolerance;
    bool m_fitTangentsExist;
    GeVector3d m_startTangent;
    GeVector3d m_endTangent;
    bool m_startTangentDefined;
    bool m_endTangentDefined;
    Ge::GeKnotParameterization m_knotParam;
};

#endif