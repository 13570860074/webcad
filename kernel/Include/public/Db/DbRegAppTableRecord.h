#ifndef DBREGAPPTABLERECORD_H
#define DBREGAPPTABLERECORD_H

#include "Db.h"
#include "DbSymbolTableRecord.h"



class DbRegAppTableRecord : public DbSymbolTableRecord
{
public:
    DbRegAppTableRecord();
    virtual ~DbRegAppTableRecord();
    DB_DECLARE_MEMBERS(DbRegAppTableRecord);
};


#endif