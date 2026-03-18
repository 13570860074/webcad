#ifndef DBCIRCLE_H
#define DBCIRCLE_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbCircle : public DbCurve
{
public:
    DbCircle();
    DbCircle(const GePoint3d& cntr, const GeVector3d& nrm, double radius);
    virtual ~DbCircle();
    DB_ENTITY_DECLARE_MEMBERS(DbCircle);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS;

    bool subOpen(Db::OpenMode mode);

    GePoint3d center() const;
    Acad::ErrorStatus   setCenter(const GePoint3d&);

    double              radius() const;
    Acad::ErrorStatus   setRadius(double);

    double              thickness() const;
    Acad::ErrorStatus   setThickness(double);

     GeVector3d        normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

    double              circumference() const;
    Acad::ErrorStatus   setCircumference(double);
    double              diameter() const;
    Acad::ErrorStatus   setDiameter(double);
};



#endif
