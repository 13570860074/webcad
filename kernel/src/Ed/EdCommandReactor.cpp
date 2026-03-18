
#include "EdCommandReactor.h"
#include "EdImpl.h"

enum EdCommandReactorEvent
{
	kCommandWillStart,
	kCommandEnded,
	kGoodbye,
};



void EdCommandReactor::commandWillStart(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandReactorEvent::kCommandWillStart, pCmd, data);
}
void EdCommandReactor::commandEnded(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandReactorEvent::kCommandEnded, pCmd, data);
}
void EdCommandReactor::goodbye(EdCommand* pCmd, EdCommandEventArgs* data) {
	RX_IMP_COMMANDREACTOR(this->m_pImpl)->pEmsdkEvent->_emit(EdCommandReactorEvent::kGoodbye, pCmd, data);
}


#if EMSDK
EdCommandReactor::EdCommandReactor() {
	this->m_pImpl = new EdCommandReactorImpl();
}
EdCommandReactor::~EdCommandReactor() {

}
void EdCommandReactor::onCommandWillStart(int ptr) {
	RX_IMP_COMMANDREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandReactorEvent::kCommandWillStart, ptr);
}
void EdCommandReactor::onCommandEnded(int ptr) {
	RX_IMP_COMMANDREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandReactorEvent::kCommandEnded, ptr);
}
void EdCommandReactor::onGoodbye(int ptr) {
	RX_IMP_COMMANDREACTOR(this->m_pImpl)->pEmsdkEvent->on(EdCommandReactorEvent::kGoodbye, ptr);
}
#endif