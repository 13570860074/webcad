#ifndef GENURBCURVE2D_H
#define GENURBCURVE2D_H

#include "GeSplineEnt2d.h"
#include "GeDoubleArray.h"
#include "GeVector2dArray.h"

class GePolyline2d;
class GeEllipArc2d;
class GeLineSeg2d;

class GeNurbCurve2d : public GeSplineEnt2d
{
public:
    GeNurbCurve2d();
    GeNurbCurve2d(const GeNurbCurve2d& source);
    GeNurbCurve2d(int degree, const GeKnotVector& knots, const GePoint2dArray& controlPoints, bool isPeriodic = false);
    GeNurbCurve2d(int degree, const GeKnotVector& knots, const GePoint2dArray& controlPoints, const GeDoubleArray& weights, bool isPeriodic = false);
    GeNurbCurve2d(int degree, const GePolyline2d& fitPolyline, bool isPeriodic = false);
    GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeVector2d& startTangent, const GeVector2d& endTangent, bool startTangentDefined = true, bool endTangentDefined = true, const GeTol& fitTolerance = GeContext::gTol);
    GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeTol& fitTolerance = GeContext::gTol);
    GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeVector2dArray& fitTangents, const GeTol& fitTolerance = GeContext::gTol, bool isPeriodic = false);
    GeNurbCurve2d(const GePoint2dArray& fitPoints, const GeVector2d& startTangent, const GeVector2d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam, const GeTol& fitTol = GeContext::gTol);
    GeNurbCurve2d(const GeEllipArc2d& ellipse);
    GeNurbCurve2d(const GeLineSeg2d& linSeg);

    GEENTITY2D_METHODS(GeNurbCurve2d);

    int numFitPoints() const;
    bool getFitKnotParameterization(Ge::GeKnotParameterization& knotParam) const;
    bool getFitPointAt(int fitPointIndex, GePoint2d& point) const;
    bool getFitTolerance(GeTol& fitTolerance) const;
    bool getFitTangents(GeVector2d& startTangent, GeVector2d& endTangent) const;
    bool getFitData(GePoint2dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector2d& startTangent, GeVector2d& endTangent) const;
    bool getFitData(GePoint2dArray& fitPoints, GeTol& fitTolerance, bool& tangentsExist, GeVector2d& startTangent, GeVector2d& endTangent, Ge::GeKnotParameterization& knotParam) const;

    void getDefinitionData(int& degree, bool& rational, bool& periodic, GeKnotVector& knots, GePoint2dArray& controlPoints, GeDoubleArray& weights) const;

    int numWeights() const;
    double weightAt(int weightIndex) const;
    GeNurbCurve2d& setWeightAt(int weightIndex, double weight);
    bool evalMode() const;

    bool getParamsOfC1Discontinuity(GeDoubleArray& params, const GeTol& tol = GeContext::gTol) const;
    bool getParamsOfG1Discontinuity(GeDoubleArray& params, const GeTol& tol = GeContext::gTol) const;

    GeNurbCurve2d& operator = (const GeNurbCurve2d& spline);

private:
    void ensureDefaultData();
    void rebuildKnotsIfNeeded();
    void setFitMeta(const GePoint2dArray& fitPoints, const GeTol& fitTolerance, bool tangentsExist, const GeVector2d& startTangent, const GeVector2d& endTangent, bool startTangentDefined, bool endTangentDefined, Ge::GeKnotParameterization knotParam);

private:
    GeDoubleArray m_weights;
    bool m_isPeriodic;
    bool m_evalMode;
    GePoint2dArray m_fitPoints;
    GeTol m_fitTolerance;
    bool m_fitTangentsExist;
    GeVector2d m_startTangent;
    GeVector2d m_endTangent;
    bool m_startTangentDefined;
    bool m_endTangentDefined;
    Ge::GeKnotParameterization m_knotParam;
};

#endif