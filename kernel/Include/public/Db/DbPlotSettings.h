#ifndef DBPLOTSETTINGS_H
#define DBPLOTSETTINGS_H


#include "Db.h"
#include "DbObject.h"



class DbPlotSettings : public DbObject
{
public:
    DbPlotSettings();
    virtual ~DbPlotSettings();
    DB_DECLARE_MEMBERS(DbPlotSettings);
};



#endif