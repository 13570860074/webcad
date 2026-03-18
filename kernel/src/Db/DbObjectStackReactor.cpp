#include "DbObjectStackReactor.h"
#include "DbDatabaseObjectStack.h"
#include "DbImpl.h"

enum DbObjectStackReactorEvent
{
    kCreateUndoStackWillStart,
    kCreateUndoStackEnd,
    kAppendRedoStack,
    kRemoveRedoStacks,
};
void DbObjectStackReactor::createUndoStackWillStart(const DbDatabaseObjectStack *dbObjStack)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectStackReactorEvent::kCreateUndoStackWillStart, dbObjStack);
}
void DbObjectStackReactor::createUndoStackEnd(const DbDatabaseObjectStack *dbObjStack, DbObjectStackContainer *stackContainers)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectStackReactorEvent::kCreateUndoStackEnd, dbObjStack, stackContainers);
}
void DbObjectStackReactor::appendRedoStack(const DbDatabaseObjectStack *dbObjStack, DbObjectStackContainer *stackContainer)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectStackReactorEvent::kAppendRedoStack, dbObjStack, stackContainer);
}
void DbObjectStackReactor::removeRedoStacks(const DbDatabaseObjectStack *dbObjStack, AcArray<DbObjectStackContainer *> *stackContainers)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbObjectStackReactorEvent::kRemoveRedoStacks, dbObjStack, stackContainers);
}

#if EMSDK

DbObjectStackReactor::DbObjectStackReactor()
{
    this->m_pImpl = new DbObjectStackReactorImpl();
}
DbObjectStackReactor::~DbObjectStackReactor()
{
}
void DbObjectStackReactor::createUndoStackWillStart(int ptr)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectStackReactorEvent::kCreateUndoStackWillStart, ptr);
}
void DbObjectStackReactor::createUndoStackEnd(int ptr)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectStackReactorEvent::kCreateUndoStackEnd, ptr);
}
void DbObjectStackReactor::onAppendRedoStack(int ptr)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectStackReactorEvent::kAppendRedoStack, ptr);
}
void DbObjectStackReactor::onRemoveRedoStacks(int ptr)
{
    DB_IMP_OBJECTREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbObjectStackReactorEvent::kRemoveRedoStacks, ptr);
}

#endif
