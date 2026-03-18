#ifndef DBSPLINE_H
#define DBSPLINE_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbSpline : public DbCurve
{
public:
    DbSpline();
    virtual ~DbSpline();
    DB_ENTITY_DECLARE_MEMBERS(DbSpline);

};



#endif
