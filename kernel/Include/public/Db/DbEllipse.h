#ifndef DBELLIPSE_H
#define DBELLIPSE_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbEllipse : public DbCurve
{
public:
    DbEllipse();
    DbEllipse(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio);
    DbEllipse(const GePoint3d& center,
        const GeVector3d& unitNormal,
        const GeVector3d& majorAxis,
        double              radiusRatio,
        double              startAngle = 0.0,
        double              endAngle = 6.28318530717958647692);
    virtual ~DbEllipse();
    DB_ENTITY_DECLARE_MEMBERS(DbEllipse);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS

    GePoint3d center() const;
    Acad::ErrorStatus setCenter(const GePoint3d& center);

    GeVector3d normal() const;
    GeVector3d majorAxis() const;
    GeVector3d minorAxis() const;
    double majorRadius() const;
    Acad::ErrorStatus setMajorRadius(double);
    double minorRadius() const;
    Acad::ErrorStatus setMinorRadius(double);

    double radiusRatio() const;
    Acad::ErrorStatus setRadiusRatio(double ratio);

    double startAngle() const;
    Acad::ErrorStatus setStartAngle(double startAngle);

    double endAngle() const;
    Acad::ErrorStatus setEndAngle(double endAngle);

    Acad::ErrorStatus setStartParam(double startParam);
    Acad::ErrorStatus setEndParam(double endParam);

    double paramAtAngle(double angle) const;
    double angleAtParam(double param) const;

    Acad::ErrorStatus get(GePoint3d& center, GeVector3d& unitNormal,
        GeVector3d& majorAxis, double& radiusRatio, double& startAngle,
        double& endAngle) const;

    Acad::ErrorStatus set(const GePoint3d& center, const GeVector3d& unitNormal, const GeVector3d& majorAxis, double radiusRatio);
    Acad::ErrorStatus set(const GePoint3d& center,
        const GeVector3d& unitNormal, const GeVector3d& majorAxis,
        double radiusRatio, double startAngle, double endAngle);

    Adesk::Boolean isNull() const;
};



#endif
