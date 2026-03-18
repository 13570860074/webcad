#include "ApDocManager.h"
#include "ApWorkDocument.h"
#include "ApDocumentIterator.h"
#include "GiEntityManager.h"
#include "ReRendererManager.h"
#include "kernel.h"
#include "ApImpl.h"


Acad::ErrorStatus ApDocManager::appContextPromptNewDocument() {
	return this->appContextNewDocument("");
}
Acad::ErrorStatus ApDocManager::appContextPromptOpenDocument() {

	AcString fileName;
	if (fileName == "") {
		return Acad::ErrorStatus::eFail;
	}
	return this->appContextOpenDocument(fileName.constPtr());
}
Acad::ErrorStatus ApDocManager::appContextCloseDocument(ApDocument* pDoc) {
	return Acad::ErrorStatus::eOk;
}











ApDocManager* acDocManagerPtr() {
	return ::kernel()->acapDocManager();
}