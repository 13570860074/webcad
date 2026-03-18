#ifndef DBLEADER_H
#define DBLEADER_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbCurve.h"


class DbLeader : public DbCurve
{
public:
    DbLeader();
    virtual ~DbLeader();
    DB_ENTITY_DECLARE_MEMBERS(DbLeader);

};



#endif
