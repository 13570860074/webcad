#include "EvDocManagerReactor.h"
#include "EvEventManager.h"
#include "kernel.h"


EvDocManagerReactor::EvDocManagerReactor() {

}
EvDocManagerReactor::~EvDocManagerReactor() {

}



void EvDocManagerReactor::documentCreateStarted(ApDocument* pDocCreating) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentCreateStarted, pDocCreating);
}
void EvDocManagerReactor::documentCreated(ApDocument* pDocCreating) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentCreated, pDocCreating);
}
void EvDocManagerReactor::documentToBeDestroyed(ApDocument* pDocToDestroy) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentToBeDestroyed, pDocToDestroy);
}
void EvDocManagerReactor::documentDestroyed(const char* fileName) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentDestroyed, fileName);
}
void EvDocManagerReactor::documentCreateCanceled(ApDocument* pDocCreateCancelled) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentCreateCanceled, pDocCreateCancelled);
}

void EvDocManagerReactor::documentLockModeWillChange(ApDocument* document, Ap::DocLockMode myCurrentMode, Ap::DocLockMode myNewMode, Ap::DocLockMode currentMode, const char* pGlobalCmdName) {
	::kernel()->acevEventManager()->_emit(
		Ev::DocManagerReactor::kDocumentLockModeWillChange, document, myCurrentMode, myNewMode, currentMode, pGlobalCmdName);
}
void EvDocManagerReactor::documentLockModeChangeVetoed(ApDocument*, const char* pGlobalCmdName) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentLockModeChangeVetoed, pGlobalCmdName);
}
void EvDocManagerReactor::documentLockModeChanged(ApDocument* document, Ap::DocLockMode myPreviousMode, Ap::DocLockMode myCurrentMode, Ap::DocLockMode currentMode, const char* pGlobalCmdName) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentLockModeChanged, document, myPreviousMode, myCurrentMode, currentMode, pGlobalCmdName);
}
void EvDocManagerReactor::documentBecameCurrent(ApDocument* document) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentBecameCurrent, document);
}
void EvDocManagerReactor::documentToBeActivated(ApDocument* pActivatingDoc) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentToBeActivated, pActivatingDoc);
}
void EvDocManagerReactor::documentToBeDeactivated(ApDocument* pDeActivatedDoc) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentToBeDeactivated, pDeActivatedDoc);
}
void EvDocManagerReactor::documentActivationModified(bool bActivation) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentActivationModified, bActivation);
}
void EvDocManagerReactor::documentActivated(ApDocument* pActivatedDoc) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentActivated, pActivatedDoc);
}

void EvDocManagerReactor::documentTitleUpdated(ApDocument* document) {
	::kernel()->acevEventManager()->_emit(Ev::DocManagerReactor::kDocumentTitleUpdated, document);
}


