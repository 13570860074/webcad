#ifndef DBLINE_H
#define DBLINE_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbLine : public DbCurve
{
public:
    DbLine();
    DbLine(const GePoint3d& start, const GePoint3d& end);
    virtual ~DbLine();
    DB_ENTITY_DECLARE_MEMBERS(DbLine);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS


    Acad::ErrorStatus getOffsetCurvesGivenPlaneNormal(
        const GeVector3d& normal,
        double offsetDist,
        DbVoidPtrArray& offsetCurves) const;

    GePoint3d startPoint() const;
    Acad::ErrorStatus   setStartPoint(const GePoint3d&);

    GePoint3d endPoint() const;
    Acad::ErrorStatus setEndPoint(const GePoint3d&);

    double thickness() const;
    Acad::ErrorStatus setThickness(double);
};


#endif
