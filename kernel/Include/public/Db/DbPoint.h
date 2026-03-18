#ifndef DBPOINT_H
#define DBPOINT_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbPoint : public DbEntity
{
public:
    DbPoint();
    DbPoint(const GePoint3d& position);
    virtual ~DbPoint();
    DB_ENTITY_DECLARE_MEMBERS(DbPoint);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    
    GePoint3d         position() const;
    Acad::ErrorStatus   setPosition(const GePoint3d&);

    double              thickness() const;
    Acad::ErrorStatus   setThickness(double);

    GeVector3d        normal() const;
    Acad::ErrorStatus   setNormal(const GeVector3d&);

    double              ecsRotation() const;
    Acad::ErrorStatus   setEcsRotation(double);

    // DbEntity overrides
    Adesk::Boolean    isPlanar() const override { return Adesk::kTrue; }
    Acad::ErrorStatus getPlane(GePlane&, Db::Planarity&) const override;
};


#endif
