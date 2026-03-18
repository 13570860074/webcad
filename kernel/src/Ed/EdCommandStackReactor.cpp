#include "EdCommandStackReactor.h"
#include "EdImpl.h"

enum EdCommandStackReactorEvent
{
	kCommandAdded,
	kCommandRemoved,
	kCommandWillStart,
	kCommandEnded,
};



void EdCommandStackReactor::commandAdded(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandStackReactorEvent::kCommandAdded, pCmd, data);
}
void EdCommandStackReactor::commandRemoved(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandStackReactorEvent::kCommandRemoved, pCmd, data);
}
void EdCommandStackReactor::commandWillStart(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandStackReactorEvent::kCommandWillStart, pCmd, data);
}
void EdCommandStackReactor::commandEnded(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandStackReactorEvent::kCommandEnded, pCmd, data);
}

#if EMSDK
EdCommandStackReactor::EdCommandStackReactor() {
	this->m_pImpl = new EdCommandStackReactorImpl();
}
EdCommandStackReactor::~EdCommandStackReactor() {

}

void EdCommandStackReactor::onCommandAdded(int ptr) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandStackReactorEvent::kCommandAdded, ptr);
}
void EdCommandStackReactor::onCommandRemoved(int ptr) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandStackReactorEvent::kCommandRemoved, ptr);
}
void EdCommandStackReactor::onCommandWillStart(int ptr) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandStackReactorEvent::kCommandWillStart, ptr);
}
void EdCommandStackReactor::onCommandEnded(int ptr) {
	RX_IMP_COMMANDSTACKREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandStackReactorEvent::kCommandEnded, ptr);
}
#endif
