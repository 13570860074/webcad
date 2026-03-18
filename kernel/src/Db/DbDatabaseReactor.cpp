#include "DbDatabaseReactor.h"
#include "DbObject.h"
#include "DbImpl.h"


enum DbDatabaseReactorEvent
{
    kObjectAppended,
    kObjectUnAppended,
    kObjectReAppended,
    kObjectOpenedForModify,
    kObjectModified,
    kObjectErased,
    kHeaderSysVarWillChange,
    kHeaderSysVarChanged,
    kProxyResurrectionCompleted,
    kGoodbye,
};


#if EMSDK
DbDatabaseReactor::DbDatabaseReactor() {
    this->m_pImpl = new DbDatabaseReactorImpl();
}
DbDatabaseReactor::~DbDatabaseReactor() {

}
#endif

void DbDatabaseReactor::objectAppended(const DbDatabase *dwg, const DbObject *dbObj)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kObjectAppended, dwg, dbObj);
}
void DbDatabaseReactor::objectUnAppended(const DbDatabase *dwg, const DbObject *dbObj)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kObjectUnAppended, dwg, dbObj);
}
void DbDatabaseReactor::objectReAppended(const DbDatabase *dwg, const DbObject *dbObj)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kObjectReAppended, dwg, dbObj);
}
void DbDatabaseReactor::objectOpenedForModify(const DbDatabase *dwg, const DbObject *dbObj)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kObjectOpenedForModify, dwg, dbObj);
}
void DbDatabaseReactor::objectModified(const DbDatabase *dwg, const DbObject *dbObj)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kObjectModified, dwg, dbObj);
}
void DbDatabaseReactor::objectErased(const DbDatabase *dwg, const DbObject *dbObj, Adesk::Boolean bErased)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kObjectErased, dwg, dbObj, bErased);
}
void DbDatabaseReactor::headerSysVarWillChange(const DbDatabase *dwg, const ACHAR *name)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kHeaderSysVarWillChange, dwg, name);
}
void DbDatabaseReactor::headerSysVarChanged(const DbDatabase *dwg, const ACHAR *name, Adesk::Boolean bSuccess)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kHeaderSysVarChanged, dwg, name, bSuccess);
}
void DbDatabaseReactor::proxyResurrectionCompleted(const DbDatabase *dwg, const ACHAR *appname, DbObjectIdArray &objects)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kProxyResurrectionCompleted, dwg, appname, objects);
}
void DbDatabaseReactor::goodbye(const DbDatabase *dwg)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbDatabaseReactorEvent::kGoodbye, dwg);
}

#if EMSDK

void DbDatabaseReactor::onObjectAppended(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kObjectAppended, ptr);
}
void DbDatabaseReactor::onObjectUnAppended(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kObjectUnAppended, ptr);
}
void DbDatabaseReactor::onObjectReAppended(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kObjectReAppended, ptr);
}
void DbDatabaseReactor::onObjectOpenedForModify(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kObjectOpenedForModify, ptr);
}
void DbDatabaseReactor::onObjectModified(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kObjectModified, ptr);
}
void DbDatabaseReactor::onObjectErased(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kObjectErased, ptr);
}
void DbDatabaseReactor::onHeaderSysVarWillChange(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kHeaderSysVarWillChange, ptr);
}
void DbDatabaseReactor::onHeaderSysVarChanged(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kHeaderSysVarChanged, ptr);
}
void DbDatabaseReactor::onProxyResurrectionCompleted(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kProxyResurrectionCompleted, ptr);
}
void DbDatabaseReactor::onGoodbye(int ptr)
{
    DB_IMP_DATABASEREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbDatabaseReactorEvent::kGoodbye, ptr);
}

#endif