#include "DbEntityReactor.h"
#include "arxHeaders.h"
#include "DbImpl.h"

enum DbEntityReactorEvent
{
    kModifiedGraphics,
    kDragCloneToBeDeleted,
};

void DbEntityReactor::modifiedGraphics(const DbEntity* dbObj) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbEntityReactorEvent::kModifiedGraphics, dbObj);
}
void DbEntityReactor::dragCloneToBeDeleted(const DbEntity* pOriginalObj, const DbEntity* pClone) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbEntityReactorEvent::kDragCloneToBeDeleted, pOriginalObj, pClone);
}


#if EMSDK

DbEntityReactor::DbEntityReactor() {

}
DbEntityReactor::~DbEntityReactor() {

}
void DbEntityReactor::onModifiedGraphics(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbEntityReactorEvent::kModifiedGraphics, ptr);
}
void DbEntityReactor::onDragCloneToBeDeleted(int ptr) {
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbEntityReactorEvent::kDragCloneToBeDeleted, ptr);
}

#endif
