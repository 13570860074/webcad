#ifndef DBBLOCKEND_H
#define DBBLOCKEND_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"


class DbBlockEnd : public DbEntity
{
public:
    DbBlockEnd();
    virtual ~DbBlockEnd();
    DB_ENTITY_DECLARE_MEMBERS(DbBlockEnd);

};

#endif
