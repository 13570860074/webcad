#ifndef DBVIEWTABLERECORD_H
#define DBVIEWTABLERECORD_H


#include "Db.h"
#include "DbAbstractViewTableRecord.h"

class DbViewTableRecord : public  DbAbstractViewTableRecord
{
public:
    DbViewTableRecord();
    virtual ~DbViewTableRecord();
    DB_DECLARE_MEMBERS(DbViewTableRecord);

    virtual void close();
};


#endif

