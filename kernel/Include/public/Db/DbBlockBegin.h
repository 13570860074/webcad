#ifndef DBBLOCKBEGIN_H
#define DBBLOCKBEGIN_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"


class DbBlockBegin : public DbEntity
{
public:
    DbBlockBegin();
    virtual ~DbBlockBegin();
    DB_ENTITY_DECLARE_MEMBERS(DbBlockBegin);

};

#endif
