#include "ApWorkDocManager.h"
#include "ApWorkDocument.h"
#include "ApDocumentIterator.h"
#include "ReRendererManager.h"
#include "ReRendererManager.h"
#include "EvEventManager.h"
#include "GiEntityManager.h"
#include "RxDictionary.h"
#include "EdCommandStack.h"
#include "EdRegistryCommandStack.h"
#include "DbDatabase.h"
#include "DbHostApplicationServices.h"
#include "kernel.h"
#include "ApImpl.h"



ApWorkDocManager::ApWorkDocManager() {
	this->m_pImpl = new ApWorkDocManagerImpl();
}
ApWorkDocManager::~ApWorkDocManager() {

}

void ApWorkDocManager::onBeginDrag(ApWorkDocManager* pointer) {
	AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isDrag = true;
}
void ApWorkDocManager::onEndDrag(ApWorkDocManager* pointer) {
	AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isDrag = false;
}
void ApWorkDocManager::onBeginAnimationFrame(ApWorkDocManager* pointer) {

	/** 创建默认文档 */
	if (pointer->documentCount() == 0) {
		pointer->appContextNewDocument("test");
	}

	/** 执行命令 */
	if (AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isCommand == false) {
		AcString command = pointer->extractStack(' ');
		if (command.length() == 0) {
			command = pointer->extractStack('\n');
		}
		if (command.length() > 0) {
			((EdRegistryCommandStack*)::acrxSysRegistry()->at(ACRX_COMMAND_DOCK))->sendCommand(command);
		}
	}
}
void ApWorkDocManager::onCommandWillStart(ApWorkDocManager* pointer, const ACHAR* cmdStr) {
	AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isCommand = true;
	ApWorkDocument* document = (ApWorkDocument*)pointer->curDocument();
	if (document == NULL) {
		return;
	}
	AP_IMP_WORKDOCUMENT(document->m_pImpl)->isQuiescent = false;
}
void ApWorkDocManager::onCommandEnded(ApWorkDocManager* pointer, const ACHAR* cmdStr) {
	AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isCommand = false;
	ApWorkDocument* document = (ApWorkDocument*)pointer->curDocument();
	if (document == NULL) {
		return;
	}
	AP_IMP_WORKDOCUMENT(document->m_pImpl)->isQuiescent = true;
}
void ApWorkDocManager::onCommandCancelled(ApWorkDocManager* pointer, const ACHAR* cmdStr) {
	AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isCommand = false;
	ApWorkDocument* document = (ApWorkDocument*)pointer->curDocument();
	if (document == NULL) {
		return;
	}
	AP_IMP_WORKDOCUMENT(document->m_pImpl)->isQuiescent = true;
}
void ApWorkDocManager::onCommandFailed(ApWorkDocManager* pointer, const ACHAR* cmdStr) {
	AP_IMP_WORKDOCMANAGER(pointer->m_pImpl)->isCommand = false;
	ApWorkDocument* document = (ApWorkDocument*)pointer->curDocument();
	if (document == NULL) {
		return;
	}
	AP_IMP_WORKDOCUMENT(document->m_pImpl)->isQuiescent = true;
}
void ApWorkDocManager::onInit() {
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)onBeginAnimationFrame, this);
}
AcString ApWorkDocManager::extractStack(const ACHAR ch)const {
	if (AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack.length() == 0) {
		return "";
	}

	AcString cmd;
	AcString newStack;
	bool isFind = false;
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack.length(); i++) {
		if (isFind == false) {
			cmd += AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack[i];
		}
		else if (isFind == true) {
			newStack += AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack[i];
		}
		if (AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack[i] == ch) {
			isFind = true;
		}
	}
	if (isFind == false) {
		return "";
	}

	AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack = newStack;
	return cmd;
}
ApDocument* ApWorkDocManager::curDocument() const {
	return AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->curDocument;
}
ApDocument* ApWorkDocManager::mdiActiveDocument() const {
	return AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->curDocument;
}
bool ApWorkDocManager::isApplicationContext() const {
	if (this->curDocument() == NULL)
	{
		return false;
	}
	return true;
}
ApDocument* ApWorkDocManager::document(const DbDatabase* db) const {
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length(); i++) {
		if (AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents[i]->database() == db) {
			return AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents[i];
		}
	}
	return NULL;
}
Acad::ErrorStatus ApWorkDocManager::lockDocument(ApDocument* pDoc, Ap::DocLockMode, const char* pGlobalCmdName, const char* pLocalCmdName, bool prompt) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus ApWorkDocManager::unlockDocument(ApDocument* pDoc) {
	return Acad::ErrorStatus::eOk;
}
ApDocumentIterator* ApWorkDocManager::newApDocumentIterator() {
	ApDocumentIterator* documentIterator = new ApDocumentIterator();
	AP_IMP_DOCUMENTITERATORIMPL(documentIterator->m_pImpl)->iterator = AcIterator<ApDocument*>(AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents);
	return documentIterator;
}
void ApWorkDocManager::addReactor(ApDocManagerReactor* _pReactor) {
	AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.append(_pReactor);
}
void ApWorkDocManager::removeReactor(ApDocManagerReactor* _pReactor) {
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++)
	{
		if (AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i) == _pReactor)
		{
			AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.removeAt(i);
			break;
		}
	}
}

Acad::ErrorStatus ApWorkDocManager::setCurDocument(ApDocument* pDoc, Ap::DocLockMode, bool activate) {

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	// 触发激活文档开始事件
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++) {
		AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i)->documentToBeActivated(pDoc);
	}

	// 设置当前文档
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length(); i++)
	{
		if (AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.at(i) == pDoc)
		{
			AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->curDocument = pDoc;
			es = Acad::ErrorStatus::eOk;
			break;
		}
	}

	// 设置工作数据库
	::acdbHostApplicationServices()->setWorkingDatabase(pDoc->database());

	// 触发激活文档事件
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++) {
		AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i)->documentActivated(pDoc);
	}

	return es;
}
Acad::ErrorStatus ApWorkDocManager::activateDocument(ApDocument* pAcTargetDocument, bool bPassScript) {
	return this->setCurDocument(pAcTargetDocument);
}


Acad::ErrorStatus ApWorkDocManager::sendStringToExecute(ApDocument* pAcTargetDocument, const ACHAR* pszExecute, bool bActivate, bool bWrapUpInactiveDoc, bool bEchoString) {
	AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->executeStack += pszExecute;
	return Acad::ErrorStatus::eOk;
}
#if EMSDK
Acad::ErrorStatus ApWorkDocManager::emsdk_sendStringToExecute(ApDocument* pAcTargetDocument, const emstring& pszExecute) {
	return this->sendStringToExecute(pAcTargetDocument, pszExecute.c_str(), true, false, true);
}
#endif


Acad::ErrorStatus ApWorkDocManager::appContextNewDocument(const ACHAR* pszTemplateName) {

	ApWorkDocument* document = new ApWorkDocument();
	document->setDocTitle(pszTemplateName);

	// 触发将要创建文档事件
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++) {
		AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i)->documentCreateStarted(document);
	}

	// 设置文档名称
	AcString title;
	title.format("Drawing%d.fwg", AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length() + 1);

	// 创建数据库
	DbDatabase* database = new DbDatabase();
	document->setDocTitle(title);
	AP_IMP_WORKDOCUMENT(document->m_pImpl)->database = database;

	// 添加到列表
	unsigned int id = 1;
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length(); i++) {
		if (AP_IMP_WORKDOCUMENT(AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents[i]->m_pImpl)->id >= id) {
			id = AP_IMP_WORKDOCUMENT(AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents[i]->m_pImpl)->id + 1;
		}
	}
	AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.append(document);

	// 触发创建文档事件
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++) {
		AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i)->documentCreated(document);
	}

	// 激活文档
	this->activateDocument(document);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus ApWorkDocManager::appContextOpenDocument(const ACHAR* pszDrawingName) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus ApWorkDocManager::appContextRecoverDocument(const ACHAR* pszDrawingName) {
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
Acad::ErrorStatus ApWorkDocManager::emsdk_appContextNewDocument(const emstring& pszTemplateName) {
	return this->appContextNewDocument(pszTemplateName.c_str());
}
Acad::ErrorStatus ApWorkDocManager::emsdk_appContextOpenDocument(const emstring& pszDrawingName) {
	return this->appContextOpenDocument(pszDrawingName.c_str());
}
Acad::ErrorStatus ApWorkDocManager::emsdk_appContextRecoverDocument(const emstring& pszDrawingName) {
	return this->appContextRecoverDocument(pszDrawingName.c_str());
}
#endif

Acad::ErrorStatus ApWorkDocManager::appContextOpenDocument(ApDocument* _document) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus ApWorkDocManager::appDatabaseNewDocument(const DbDatabase* database, const ACHAR* pszExecute) {
	ApWorkDocument* document = new ApWorkDocument();
	document->setDocTitle(pszExecute);

	// 触发将要创建文档事件
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++) {
		AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i)->documentCreateStarted(document);
	}

	// 设置文档名称
	AcString title;
	title.format("Drawing%d.fwg", AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length() + 1);

	// 创建数据库
	AP_IMP_WORKDOCUMENT(document->m_pImpl)->database = (DbDatabase*)database;

	// 添加到列表
	unsigned int id = 1;
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length(); i++) {
		if (AP_IMP_WORKDOCUMENT(AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents[i]->m_pImpl)->id >= id) {
			id = AP_IMP_WORKDOCUMENT(AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents[i]->m_pImpl)->id + 1;
		}
	}
	AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.append(document);

	// 触发创建文档事件
	for (int i = 0; i < AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.length(); i++) {
		AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->docManagerReactors.at(i)->documentCreated(document);
	}

	// 激活文档
	this->activateDocument(document);

	return Acad::ErrorStatus::eOk;
}

int ApWorkDocManager::documentCount() const {
	return AP_IMP_DOCMANAGERIMPL(this->m_pImpl)->documents.length();
}






