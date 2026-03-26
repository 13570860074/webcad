#ifndef DBSPLINE_H
#define DBSPLINE_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"
#include "GeDoubleArray.h"
#include "GePoint3dArray.h"


class DbSpline : public DbCurve
{
public:
    DbSpline();
    DbSpline(const GePoint3dArray& fitPoints, int order, double fitTolerance);
    DbSpline(int degree, bool rational, bool closed, bool periodic,
        const GePoint3dArray& controlPoints, const GeDoubleArray& knots,
        const GeDoubleArray& weights, double controlPtTol, double knotTol);
    virtual ~DbSpline();
    DB_ENTITY_DECLARE_MEMBERS(DbSpline);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS

    bool isRational() const;
    int degree() const;

    int numControlPoints() const;
    Acad::ErrorStatus getControlPointAt(int index, GePoint3d& point) const;
    Acad::ErrorStatus setControlPointAt(int index, const GePoint3d& point);

    int numFitPoints() const;
    Acad::ErrorStatus getFitPointAt(int index, GePoint3d& point) const;
    Acad::ErrorStatus setFitPointAt(int index, const GePoint3d& point);
    Acad::ErrorStatus insertFitPointAt(int index, const GePoint3d& point);
    Acad::ErrorStatus removeFitPointAt(int index);

    double fitTolerance() const;
    Acad::ErrorStatus setFitTol(double fitTolerance);

    Acad::ErrorStatus getFitTangents(GeVector3d& startTangent, GeVector3d& endTangent) const;
    Acad::ErrorStatus setFitTangents(const GeVector3d& startTangent, const GeVector3d& endTangent);

    bool hasFitData() const;

    Acad::ErrorStatus getNurbsData(
        int& degree, bool& rational, bool& closed, bool& periodic,
        GePoint3dArray& controlPoints, GeDoubleArray& knots,
        GeDoubleArray& weights, double& controlPtTol, double& knotTol) const;

    Acad::ErrorStatus setNurbsData(
        int degree, bool rational, bool closed, bool periodic,
        const GePoint3dArray& controlPoints, const GeDoubleArray& knots,
        const GeDoubleArray& weights, double controlPtTol, double knotTol);

    double weightAt(int index) const;
    Acad::ErrorStatus setWeightAt(int index, double weight);

    Adesk::Boolean isNull() const;
};



#endif
