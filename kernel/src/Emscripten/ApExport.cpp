#include "export.h"
#include "RxExport.h"
#include "AcIterator.h"
#include "RxObject.h"
#include "ApDocument.h"
#include "ApWorkDocument.h"
#include "ApDocManager.h"
#include "ApDocumentIterator.h"
#include "ApDocManagerReactor.h"
#include "Ap.h"




#if EMSDK


EMSCRIPTEN_BINDINGS(Ap) {

	function("acDocManagerPtr", &acDocManagerPtr, allow_raw_pointers());
	function("curDoc", &curDoc, allow_raw_pointers());


	//DocLockMode
	enum_<Ap::DocLockMode>("DocLockMode")
		.value("kNone", Ap::kNone)
		.value("kAutoWrite", Ap::kNotLocked)
		.value("kWrite", Ap::kWrite)
		.value("kProtectedAutoWrite", Ap::kProtectedAutoWrite)
		.value("kRead", Ap::kRead)
		.value("kXWrite", Ap::kXWrite);

	//ApDocument
	emscripten::class_<ApDocument, base<RxObject>>("ApDocument")
		RX_EMSDK_EXPORT_METHODS(ApDocument)
		.function("fileName", &ApDocument::emsdk_fileName, pure_virtual())
		.function("docTitle", &ApDocument::emsdk_docTitle, pure_virtual())
		.function("setDocTitle", &ApDocument::emsdk_setDocTitle)
		.function("isReadOnly", &ApDocument::isReadOnly, pure_virtual())
		.function("upgradeDocOpen", &ApDocument::upgradeDocOpen)
		.function("downgradeDocOpen", &ApDocument::downgradeDocOpen);

	//ApDocManager
	emscripten::class_<ApDocManager, base<RxObject>>("ApDocManager")
		RX_EMSDK_EXPORT_METHODS(ApDocManager)
		.function("curDocument", &ApDocManager::curDocument, allow_raw_pointers(), pure_virtual())
		.function("mdiActiveDocument", &ApDocManager::mdiActiveDocument, allow_raw_pointers(), pure_virtual())
		.function("isApplicationContext", &ApDocManager::isApplicationContext, pure_virtual())
		.function("lockDocument", &ApDocManager::lockDocument, allow_raw_pointers(), pure_virtual())
		.function("unlockDocument", &ApDocManager::unlockDocument, allow_raw_pointers(), pure_virtual())
		.function("newApDocumentIterator", &ApDocManager::newApDocumentIterator, allow_raw_pointers(), pure_virtual())
		.function("addReactor", &ApDocManager::addReactor, allow_raw_pointers(), pure_virtual())
		.function("removeReactor", &ApDocManager::removeReactor, allow_raw_pointers(), pure_virtual())
		.function("setCurDocument", &ApDocManager::setCurDocument, allow_raw_pointers(), pure_virtual())
		.function("activateDocument", &ApDocManager::activateDocument, allow_raw_pointers(), pure_virtual())
		.function("sendStringToExecute", select_overload <Acad::ErrorStatus(ApDocument*, const std::string&)>(&ApDocManager::emsdk_sendStringToExecute), allow_raw_pointers(), pure_virtual())
		.function("sendStringToExecute", select_overload <Acad::ErrorStatus(ApDocument*, const ACHAR*, bool, bool, bool)>(&ApDocManager::sendStringToExecute), allow_raw_pointers(), pure_virtual())
		.function("appContextNewDocument", &ApDocManager::emsdk_appContextNewDocument)
        .function("appContextOpenDocument", &ApDocManager::emsdk_appContextOpenDocument)
		.function("appContextRecoverDocument", &ApDocManager::emsdk_appContextRecoverDocument)
		.function("appContextPromptNewDocument", &ApDocManager::appContextPromptNewDocument, allow_raw_pointers())
		.function("appContextPromptOpenDocument", &ApDocManager::appContextPromptOpenDocument, allow_raw_pointers())
		.function("appContextCloseDocument", &ApDocManager::appContextCloseDocument, allow_raw_pointers())
		.function("documentCount", &ApDocManager::documentCount, allow_raw_pointers(), pure_virtual());

	//ApMaterialReactor
	emscripten::class_<ApDocManagerReactor, base<RxObject>>("ApDocManagerReactor")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(ApDocManagerReactor)
		.function("onDocumentCreateStarted", select_overload<void(int)>(&ApDocManagerReactor::onDocumentCreateStarted))
		.function("onDocumentCreated", select_overload<void (int)>(&ApDocManagerReactor::onDocumentCreated))
		.function("onDocumentToBeDestroyed", select_overload<void(int)>(&ApDocManagerReactor::onDocumentToBeDestroyed))
		.function("onDocumentDestroyed", select_overload<void(int)>(&ApDocManagerReactor::onDocumentDestroyed))
		.function("onDocumentCreateCanceled", select_overload<void(int)>(&ApDocManagerReactor::onDocumentCreateCanceled))
		.function("onDocumentLockModeWillChange", select_overload<void(int)>(&ApDocManagerReactor::onDocumentLockModeWillChange))
		.function("onDocumentLockModeChangeVetoed", select_overload<void(int)>(&ApDocManagerReactor::onDocumentLockModeChangeVetoed))
		.function("onDocumentLockModeChanged", select_overload<void(int)>(&ApDocManagerReactor::onDocumentLockModeChanged))
		.function("onDocumentBecameCurrent", select_overload<void(int)>(&ApDocManagerReactor::onDocumentBecameCurrent))
		.function("onDocumentToBeActivated", select_overload<void(int)>(&ApDocManagerReactor::onDocumentToBeActivated))
		.function("onDocumentToBeDeactivated", select_overload<void(int)>(&ApDocManagerReactor::onDocumentToBeDeactivated))
		.function("onDocumentActivationModified", select_overload<void(int)>(&ApDocManagerReactor::onDocumentActivationModified))
		.function("onDocumentActivated", select_overload<void(int)>(&ApDocManagerReactor::onDocumentActivated))
		.function("onDocumentTitleUpdated", select_overload<void(int)>(&ApDocManagerReactor::onDocumentTitleUpdated));


	//ApDocumentIterator
	emscripten::class_<ApDocumentIterator, base<RxObject>>("ApDocumentIterator")
		.constructor<>()
		RX_EMSDK_EXPORT_METHODS(ApDocumentIterator)
		.function("start", &ApDocumentIterator::start)
		.function("step", &ApDocumentIterator::step)
		.function("done", &ApDocumentIterator::done)
		.function("document", select_overload<ApDocument* ()>(&ApDocumentIterator::document), allow_raw_pointers());
}



#endif
