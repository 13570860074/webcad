#ifndef EVDATABASEREACTOR_H
#define EVDATABASEREACTOR_H

#include "DbDatabaseReactor.h"

class EvDatabaseReactor : public DbDatabaseReactor
{
public:
    EvDatabaseReactor();
    virtual ~EvDatabaseReactor();

    virtual void objectAppended(const DbDatabase *dwg, const DbObject *dbObj);
    virtual void objectUnAppended(const DbDatabase *dwg, const DbObject *dbObj);
    virtual void objectReAppended(const DbDatabase *dwg, const DbObject *dbObj);
    virtual void objectOpenedForModify(const DbDatabase *dwg, const DbObject *dbObj);
    virtual void objectModified(const DbDatabase *dwg, const DbObject *dbObj);
    virtual void objectErased(const DbDatabase *dwg, const DbObject *dbObj, Adesk::Boolean bErased);
    virtual void headerSysVarWillChange(const DbDatabase *dwg, const ACHAR *name);
    virtual void headerSysVarChanged(const DbDatabase *dwg, const ACHAR *name, Adesk::Boolean bSuccess);
    virtual void proxyResurrectionCompleted(const DbDatabase *dwg, const ACHAR *appname, DbObjectIdArray &objects);
    virtual void goodbye(const DbDatabase *dwg);
};

#endif
