#ifndef APIMPL_H
#define APIMPL_H


#include "RxImpl.h"
#include "AcString.h"
#include "AcArray.h"
#include "ApDocument.h"
#include "AcEvent.h"
#include "AcEmsdkEvent.h"
#include "AcIterator.h"
#include "ApDocManagerReactor.h"


#define AP_IMP_DOCUMENTIMPL(x) ((ApDocumentImpl*)x)
#define AP_IMP_WORKDOCUMENT(x) ((ApWorkDocumentImpl*)x)
#define AP_IMP_DOCMANAGERIMPL(x) ((ApDocManagerImpl*)x)
#define AP_IMP_DOCUMENTITERATORIMPL(x) ((ApDocumentIteratorImpl*)x)
#define AP_IMP_DOCMANAGERREACTORIMPL(x) ((ApDocManagerReactorImpl*)x)
#define AP_IMP_WORKDOCMANAGER(x) ((ApWorkDocManagerImpl*)x)


class ApDocumentImpl :public RxObjectImpl
{
public:
	ApDocumentImpl() {
		this->id = 0;
		this->isQuiescent = true;
		this->DocLockMode = 0;
		this->isReadOnly = false;
		this->isModified = false;
	}
	virtual ~ApDocumentImpl()
	{

	}

	unsigned int id;
	int DocLockMode;
	bool isQuiescent;
	AcString fileName;
	AcString docTitle;
	bool isReadOnly;
	bool isModified;
};
class ApWorkDocumentImpl :public ApDocumentImpl
{
public:
	ApWorkDocumentImpl() {
		this->database = NULL;
	}
	virtual ~ApWorkDocumentImpl()
	{
	}

	DbDatabase* database;
};


class ApDocManagerImpl :public RxObjectImpl {
public:
	ApDocManagerImpl() {
		this->curDocument = NULL;
	}
	virtual ~ApDocManagerImpl() {
	}

	AcString executeStack;
	AcArray<ApDocument*> documents;
	ApDocument* curDocument;
	AcArray<ApDocManagerReactor*> docManagerReactors;
};
class ApWorkDocManagerImpl :public ApDocManagerImpl {
public:
	ApWorkDocManagerImpl() {
		this->isDrag = false;
		this->isCommand = false;
	}
	virtual ~ApWorkDocManagerImpl() {

	}

	bool isDrag;
	bool isCommand;
};


class ApDocumentIteratorImpl :public RxObjectImpl {
public:
	ApDocumentIteratorImpl() {

	}
	virtual ~ApDocumentIteratorImpl() {

	}

	AcIterator<ApDocument*> iterator;
};


class ApDocManagerReactorImpl :public RxObjectReactorImpl {
public:
	ApDocManagerReactorImpl() {
	}
	virtual ~ApDocManagerReactorImpl() {
	}
};



#endif
