
#include "EvDatabaseReactor.h"
#include "EvEventManager.h"
#include "kernel.h"

EvDatabaseReactor::EvDatabaseReactor()
{
}
EvDatabaseReactor::~EvDatabaseReactor()
{
}
void EvDatabaseReactor::objectAppended(const DbDatabase *dwg, const DbObject *dbObj)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kObjectAppended, dwg, dbObj);
}
void EvDatabaseReactor::objectUnAppended(const DbDatabase *dwg, const DbObject *dbObj)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kObjectUnAppended, dwg, dbObj);
}
void EvDatabaseReactor::objectReAppended(const DbDatabase *dwg, const DbObject *dbObj)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kObjectReAppended, dwg, dbObj);
}
void EvDatabaseReactor::objectOpenedForModify(const DbDatabase *dwg, const DbObject *dbObj)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kObjectOpenedForModify, dwg, dbObj);
}
void EvDatabaseReactor::objectModified(const DbDatabase *dwg, const DbObject *dbObj)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kObjectModified, dwg, dbObj);
}
void EvDatabaseReactor::objectErased(const DbDatabase *dwg, const DbObject *dbObj, Adesk::Boolean bErased)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kObjectErased, dwg, dbObj);
}
void EvDatabaseReactor::headerSysVarWillChange(const DbDatabase *dwg, const ACHAR *name)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kHeaderSysVarWillChange, dwg, name);
}
void EvDatabaseReactor::headerSysVarChanged(const DbDatabase *dwg, const ACHAR *name, Adesk::Boolean bSuccess)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kHeaderSysVarChanged, dwg, name, bSuccess);
}
void EvDatabaseReactor::proxyResurrectionCompleted(const DbDatabase *dwg, const ACHAR *appname, DbObjectIdArray &objects)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kProxyResurrectionCompleted, appname, objects);
}
void EvDatabaseReactor::goodbye(const DbDatabase *dwg)
{
	::kernel()->acevEventManager()->_emit(Ev::DatabaseReactor::kGoodbye, dwg);
}