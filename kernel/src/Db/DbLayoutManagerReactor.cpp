#include "DbLayoutManagerReactor.h"
#include "AcEvent.h"
#include "DbImpl.h"


enum DbLayoutManagerReactorEvent
{
	kLayoutCreated,
	kLayoutToBeRemoved,
	kLayoutRemoved,
	kAbortLayoutRemoved,
	kLayoutToBeCopied,
	kLayoutCopied,
	kAbortLayoutCopied,
	kLayoutToBeRenamed,
	kLayoutRenamed,
	kAbortLayoutRename,
	kLayoutSwitched,
	kPlotStyleTableChanged,
	kLayoutsReordered,
	kRefreshLayoutTabs,
};



void DbLayoutManagerReactor::layoutCreated(const char* newLayoutName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutCreated, newLayoutName, layoutId);
}
void DbLayoutManagerReactor::layoutToBeRemoved(const char* layoutName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutToBeRemoved, layoutName, layoutId);
}
void DbLayoutManagerReactor::layoutRemoved(const char* layoutName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutRemoved, layoutName, layoutId);
}
void DbLayoutManagerReactor::abortLayoutRemoved(const char* layoutName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kAbortLayoutRemoved, layoutName, layoutId);
}
void DbLayoutManagerReactor::layoutToBeCopied(const char* layoutName, const DbObjectId& oldLayoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutToBeCopied, layoutName, oldLayoutId);
}
void DbLayoutManagerReactor::layoutCopied(const char* oldLayoutName, const DbObjectId& oldLayoutId,
	const char* newLayoutname, const DbObjectId& newLayoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutCopied, oldLayoutName, oldLayoutId, newLayoutname, newLayoutId);
}
void DbLayoutManagerReactor::abortLayoutCopied(const char* layoutName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kAbortLayoutCopied, layoutName, layoutId);
}
void DbLayoutManagerReactor::layoutToBeRenamed(const char* oldName, const char* newName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutToBeRenamed, oldName, newName, layoutId);
}
void DbLayoutManagerReactor::layoutRenamed(const char* oldName, const char* newName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutRenamed, oldName, newName, layoutId);
}
void DbLayoutManagerReactor::abortLayoutRename(const char* oldName, const char* newName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kAbortLayoutRename, oldName, newName, layoutId);
}
void DbLayoutManagerReactor::layoutSwitched(const char* newLayoutname, const DbObjectId& newLayoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutSwitched, newLayoutname, newLayoutId);
}
void DbLayoutManagerReactor::plotStyleTableChanged(const char* newTableName, const DbObjectId& layoutId) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kPlotStyleTableChanged, newTableName, layoutId);
}
void DbLayoutManagerReactor::layoutsReordered() {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kLayoutsReordered);
}
void DbLayoutManagerReactor::refreshLayoutTabs() {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(DbLayoutManagerReactorEvent::kRefreshLayoutTabs);
}


#if EMSDK
DbLayoutManagerReactor::DbLayoutManagerReactor() {
	this->m_pImpl = new DbLayoutManagerReactorImpl();
}
DbLayoutManagerReactor::~DbLayoutManagerReactor() {

}
void DbLayoutManagerReactor::onLayoutCreated(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutCreated, ptr);
}
void DbLayoutManagerReactor::onLayoutToBeRemoved(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutToBeRemoved, ptr);
}
void DbLayoutManagerReactor::onLayoutRemoved(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutRemoved, ptr);
}
void DbLayoutManagerReactor::onAbortLayoutRemoved(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kAbortLayoutRemoved, ptr);
}
void DbLayoutManagerReactor::onLayoutToBeCopied(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutToBeCopied, ptr);
}
void DbLayoutManagerReactor::onLayoutCopied(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutCopied, ptr);
}
void DbLayoutManagerReactor::onAbortLayoutCopied(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kAbortLayoutCopied, ptr);
}
void DbLayoutManagerReactor::onLayoutToBeRenamed(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutToBeRenamed, ptr);
}
void DbLayoutManagerReactor::onLayoutRenamed(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutRenamed, ptr);
}
void DbLayoutManagerReactor::onAbortLayoutRename(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kAbortLayoutRename, ptr);
}
void DbLayoutManagerReactor::onLayoutSwitched(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutSwitched, ptr);
}
void DbLayoutManagerReactor::onPlotStyleTableChanged(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kPlotStyleTableChanged, ptr);
}
void DbLayoutManagerReactor::onLayoutsReordered(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kLayoutsReordered, ptr);
}
void DbLayoutManagerReactor::onRefreshLayoutTabs(int ptr) {
	DB_IMP_LAYOUTMANAGERREACTOR(this->m_pImpl)->pEmsdkEvent->on(DbLayoutManagerReactorEvent::kRefreshLayoutTabs, ptr);
}
#endif
