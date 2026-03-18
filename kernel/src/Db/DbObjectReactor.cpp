#include "DbObjectReactor.h"
#include "arxHeaders.h"
#include "DbImpl.h"



enum DbObjectReactorEvent
{
    kCancelled,
    kCopied,
    kErased,
    kGoodbye,
    kOpenedForModify,
    kModified,
    kSubObjModified,
    kModifyUndone,
    kModifiedXData,
    kUnappended,
    kReappended,
    kObjectClosed,
};



void DbObjectReactor::cancelled(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kCancelled, dbObj);
}
void DbObjectReactor::copied(const DbObject* dbObj, const DbObject* newObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kCopied, dbObj, newObj);
}
void DbObjectReactor::erased(const DbObject* dbObj, Adesk::Boolean bErasing) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kErased, dbObj, bErasing);
}
void DbObjectReactor::goodbye(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kGoodbye, dbObj);
}
void DbObjectReactor::openedForModify(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kOpenedForModify, dbObj);
}
void DbObjectReactor::modified(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kModified, dbObj);
}
void DbObjectReactor::subObjModified(const DbObject* dbObj, const DbObject* subObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kSubObjModified, dbObj, subObj);
}
void DbObjectReactor::modifyUndone(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kModifyUndone, dbObj);
}
void DbObjectReactor::modifiedXData(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kModifiedXData, dbObj);
}
void DbObjectReactor::unappended(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kUnappended, dbObj);
}
void DbObjectReactor::reappended(const DbObject* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kReappended, dbObj);
}
void DbObjectReactor::objectClosed(const DbObjectId objId) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectReactorEvent::kObjectClosed, objId);
}




#if EMSDK


DbObjectReactor::DbObjectReactor() {
    this->m_pImpl = new DbObjectReactorImpl();
}
DbObjectReactor::~DbObjectReactor() {

}

void DbObjectReactor::onCancelled(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kCancelled, ptr);
}
void DbObjectReactor::onCopied(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kCopied, ptr);
}
void DbObjectReactor::onErased(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kErased, ptr);
}
void DbObjectReactor::onGoodbye(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kGoodbye, ptr);
}
void DbObjectReactor::onOpenedForModify(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kOpenedForModify, ptr);
}
void DbObjectReactor::onModified(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kModified, ptr);
}
void DbObjectReactor::onSubObjModified(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kSubObjModified, ptr);
}
void DbObjectReactor::onModifyUndone(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kModifyUndone, ptr);
}
void DbObjectReactor::onModifiedXData(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kModifiedXData, ptr);
}
void DbObjectReactor::onUnappended(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kUnappended, ptr);
}
void DbObjectReactor::onReappended(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kReappended, ptr);
}
void DbObjectReactor::onObjectClosed(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectReactorEvent::kObjectClosed, ptr);
}

#endif
