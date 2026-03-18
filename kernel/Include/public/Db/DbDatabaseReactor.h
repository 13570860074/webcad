#ifndef DBDATABASEREACTOR_H
#define DBDATABASEREACTOR_H

#include "DbObject.h"
#include "DbObjectIdArray.h"

class DbDatabase;
class DbDatabaseReactor : public RxObject
{
public:
    RX_DECLARE_MEMBERS(DbDatabaseReactor);

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

    //emsdk
#if EMSDK

public:
    DbDatabaseReactor();
    virtual ~DbDatabaseReactor();

    void onObjectAppended(int ptr);
    void onObjectUnAppended(int ptr);
    void onObjectReAppended(int ptr);
    void onObjectOpenedForModify(int ptr);
    void onObjectModified(int ptr);
    void onObjectErased(int ptr);
    void onHeaderSysVarWillChange(int ptr);
    void onHeaderSysVarChanged(int ptr);
    void onProxyResurrectionCompleted(int ptr);
    void onGoodbye(int ptr);
#endif

};

#endif