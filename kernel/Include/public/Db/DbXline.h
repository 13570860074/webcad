#ifndef DBXLINE_H
#define DBXLINE_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbXline : public DbCurve
{
public:
    DbXline();
    virtual ~DbXline();
    DB_ENTITY_DECLARE_MEMBERS(DbXline);
    DB_CURVE_DECLARE_OVERRIDE_METHOD;
    DBCURVE_METHODS;

    Acad::ErrorStatus getOffsetCurvesGivenPlaneNormal(const GeVector3d& normal, double offsetDist, DbVoidPtrArray& offsetCurves) const override;

    GePoint3d  basePoint() const;
    void         setBasePoint(const GePoint3d& pt);

    GeVector3d unitDir() const;
    void         setUnitDir(const GeVector3d& vec);

    GePoint3d               secondPoint() const;
    Acad::ErrorStatus		setSecondPoint(GePoint3d const& pt);

};

#endif
