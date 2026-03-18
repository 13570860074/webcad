#ifndef DBARC_H
#define DBARC_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbArc : public DbCurve
{
public:
    DbArc();
    DbArc(const GePoint3d& center, double radius,double startAngle, double endAngle);
    DbArc(const GePoint3d& center, const GeVector3d& normal, double radius, double startAngle, double endAngle);
    virtual ~DbArc();
    DB_ENTITY_DECLARE_MEMBERS(DbArc);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS;

    bool subOpen(Db::OpenMode mode);

    GePoint3d           center() const;
    Acad::ErrorStatus   setCenter(const GePoint3d&);

    double              radius() const;
    Acad::ErrorStatus   setRadius(double);

    double              startAngle() const;
    Acad::ErrorStatus   setStartAngle(double);

    double              endAngle() const;
    Acad::ErrorStatus   setEndAngle(double);

    double    totalAngle() const;

    double    length() const;

    double              thickness() const;
    Acad::ErrorStatus   setThickness(double);

    GeVector3d        normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

};

#endif
