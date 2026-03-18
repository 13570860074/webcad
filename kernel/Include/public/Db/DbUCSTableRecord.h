#ifndef DBUCSTABLERECORD_H
#define DBUCSTABLERECORD_H


#include "Db.h"
#include "GePoint3d.h"
#include "DbSymbolTableRecord.h"


class DbUCSTableRecord : public DbSymbolTableRecord
{
public:
    DbUCSTableRecord();
    virtual ~DbUCSTableRecord();
    DB_DECLARE_MEMBERS(DbUCSTableRecord);

public:
    GePoint3d  origin() const;
    void setOrigin(const GePoint3d& newOrigin);

    GeVector3d xAxis() const;
    void setXAxis(const GeVector3d& xAxis);

    GeVector3d yAxis() const;
    void setYAxis(const GeVector3d& yAxis);

    GePoint3d ucsBaseOrigin(Db::OrthographicView view) const;
    Acad::ErrorStatus setUcsBaseOrigin(const GePoint3d& origin, Db::OrthographicView view);
};

#endif