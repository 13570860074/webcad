#include "ApDocManagerReactor.h"
#include "AcEvent.h"
#include "ApImpl.h"



enum ApDocManagerReactorEvent
{
	kDocumentCreateStarted,
	kDocumentCreated,
	kDocumentToBeDestroyed,
	kDocumentDestroyed,
	kDocumentCreateCanceled,
	kDocumentLockModeWillChange,
	kDocumentLockModeChangeVetoed,
	kDocumentLockModeChanged,
	kDocumentBecameCurrent,
	kDocumentToBeActivated,
	kDocumentToBeDeactivated,
	kDocumentActivationModified,
	kDocumentActivated,
	kDocumentTitleUpdated,
};





void ApDocManagerReactor::documentCreateStarted(ApDocument* pDocCreating) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentCreateStarted, pDocCreating);
}
void ApDocManagerReactor::documentCreated(ApDocument* pDocCreating) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentCreated, pDocCreating);
}
void ApDocManagerReactor::documentToBeDestroyed(ApDocument* pDocToDestroy) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentToBeDestroyed, pDocToDestroy);
}
void ApDocManagerReactor::documentDestroyed(const ACHAR* fileName) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentDestroyed, fileName);
}
void ApDocManagerReactor::documentCreateCanceled(ApDocument* pDocCreateCancelled) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentCreateCanceled, pDocCreateCancelled);
}


void ApDocManagerReactor::documentLockModeWillChange(
	ApDocument* document,
	Ap::DocLockMode myCurrentMode,
	Ap::DocLockMode myNewMode,
	Ap::DocLockMode currentMode,
	const ACHAR* pGlobalCmdName) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentLockModeWillChange, document, myCurrentMode, myNewMode, currentMode, pGlobalCmdName);
}
void ApDocManagerReactor::documentLockModeChangeVetoed(ApDocument *pDocument, const ACHAR* pGlobalCmdName) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentLockModeChangeVetoed, pDocument, pGlobalCmdName);
}
void ApDocManagerReactor::documentLockModeChanged(
	ApDocument* document,
	Ap::DocLockMode myPreviousMode,
	Ap::DocLockMode myCurrentMode,
	Ap::DocLockMode currentMode,
	const ACHAR* pGlobalCmdName) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentLockModeChanged, document, myPreviousMode, myCurrentMode, currentMode, pGlobalCmdName);
}

void ApDocManagerReactor::documentBecameCurrent(ApDocument *pDocument) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentBecameCurrent, pDocument);
}

void ApDocManagerReactor::documentToBeActivated(ApDocument* pActivatingDoc) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentToBeActivated, pActivatingDoc);
}
void ApDocManagerReactor::documentToBeDeactivated(ApDocument* pDeActivatedDoc) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentToBeDeactivated, pDeActivatedDoc);
}
void ApDocManagerReactor::documentActivationModified(bool bActivation) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentActivationModified, bActivation);
}
void ApDocManagerReactor::documentActivated(ApDocument* pActivatedDoc) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentActivated, pActivatedDoc);
}

void ApDocManagerReactor::documentTitleUpdated(ApDocument *pDocument) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->_emit(ApDocManagerReactorEvent::kDocumentTitleUpdated, pDocument);
}

#if EMSDK

ApDocManagerReactor::ApDocManagerReactor() {
	this->m_pImpl = new ApDocManagerReactorImpl();
}
ApDocManagerReactor::~ApDocManagerReactor() {

}

void ApDocManagerReactor::onDocumentCreateStarted(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentCreateStarted, ptr);
}
void ApDocManagerReactor::onDocumentCreated(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentCreated, ptr);
}
void ApDocManagerReactor::onDocumentToBeDestroyed(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentToBeDestroyed, ptr);
}
void ApDocManagerReactor::onDocumentDestroyed(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentDestroyed, ptr);
}
void ApDocManagerReactor::onDocumentCreateCanceled(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentCreateCanceled, ptr);
}
void ApDocManagerReactor::onDocumentLockModeWillChange(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentLockModeWillChange, ptr);
}
void ApDocManagerReactor::onDocumentLockModeChangeVetoed(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentLockModeChangeVetoed, ptr);
}
void ApDocManagerReactor::onDocumentLockModeChanged(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentLockModeChanged, ptr);
}
void ApDocManagerReactor::onDocumentBecameCurrent(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentBecameCurrent, ptr);
}
void ApDocManagerReactor::onDocumentToBeActivated(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentToBeActivated, ptr);
}
void ApDocManagerReactor::onDocumentToBeDeactivated(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentToBeDeactivated, ptr);
}
void ApDocManagerReactor::onDocumentActivationModified(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentActivationModified, ptr);
}
void ApDocManagerReactor::onDocumentActivated(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentActivated, ptr);
}
void ApDocManagerReactor::onDocumentTitleUpdated(int ptr) {
	AP_IMP_DOCMANAGERREACTORIMPL(this->m_pImpl)->pEmsdkEvent->on(ApDocManagerReactorEvent::kDocumentTitleUpdated, ptr);
}
#endif

