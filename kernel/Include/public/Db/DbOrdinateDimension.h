#ifndef DBORDINATEDIMENSION_H
#define DBORDINATEDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbOrdinateDimension : public DbDimension
{
public:
    DbOrdinateDimension();
    DbOrdinateDimension(
        Adesk::Boolean     useXAxis,
        const GePoint3d& definingPoint,
        const GePoint3d& leaderEndPoint);
    DbOrdinateDimension(
        Adesk::Boolean     useXAxis,
        const GePoint3d& definingPoint,
        const GePoint3d& leaderEndPoint,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~DbOrdinateDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbOrdinateDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    Adesk::Boolean      isUsingXAxis() const;
    Adesk::Boolean      isUsingYAxis() const;
    Acad::ErrorStatus   useXAxis();
    Acad::ErrorStatus   useYAxis();
    Acad::ErrorStatus   setUsingXAxis(bool value);
    Acad::ErrorStatus   setUsingYAxis(bool value);

    GePoint3d         origin() const;
    Acad::ErrorStatus   setOrigin(const GePoint3d&);

    GePoint3d         definingPoint() const;
    Acad::ErrorStatus   setDefiningPoint(const GePoint3d&);

    GePoint3d         leaderEndPoint() const;
    Acad::ErrorStatus   setLeaderEndPoint(const GePoint3d&);

};

#endif
