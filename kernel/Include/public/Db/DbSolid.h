#ifndef DBSOLID_H
#define DBSOLID_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"


class DbSolid : public DbEntity
{
public:
    DbSolid();
    DbSolid(const GePoint3d& pt0,
        const GePoint3d& pt1,
        const GePoint3d& pt2,
        const GePoint3d& pt3);
    DbSolid(const GePoint3d& pt0,
        const GePoint3d& pt1,
        const GePoint3d& pt2);
    virtual ~DbSolid();
    DB_ENTITY_DECLARE_MEMBERS(DbSolid);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;


    Acad::ErrorStatus getPointAt(Adesk::UInt16 idx, GePoint3d& pntRes) const;
    Acad::ErrorStatus setPointAt(Adesk::UInt16 idx, const GePoint3d&);

    double              thickness() const;
    Acad::ErrorStatus   setThickness(double);

    GeVector3d        normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

    double              elevation() const;
    void                setElevation(double);
};


#endif
