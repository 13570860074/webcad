#ifndef APDOCMANAGER_H
#define APDOCMANAGER_H

#include "Ap.h"
#include "Db.h"
#include "RxObject.h"

class ApDocument;
class DbDatabase;
class ApDocumentIterator;
class ApDocManagerReactor;
class ApDocManager : public RxObject
{
public:
	RX_DECLARE_MEMBERS_VIRTUAL(ApDocManager);


	// curDocument() returns the document having current context.
	// mdiActiveDocument() returns the MDI active document.
	// curDocument() and mdiActiveDocument() can be different.
	// You can call curDocument() to make a document "current" 
	// without actually activating it. After finish your AcDbDatabase
	// operation under the temporary current document, call
	// setCurDocument(acDocManager->mdiActiveDocument())
	// to reset the MDI active document as the current document.
	virtual ApDocument* curDocument() const = 0;
	virtual ApDocument* mdiActiveDocument() const = 0;

	virtual bool isApplicationContext() const = 0;

	virtual ApDocument* document(const DbDatabase*) const = 0;

	virtual Acad::ErrorStatus lockDocument(
		ApDocument* pDoc,
		Ap::DocLockMode = Ap::kWrite,
		const char* pGlobalCmdName = NULL,
		const char* pLocalCmdName = NULL,
		bool prompt = true) = 0;
	virtual Acad::ErrorStatus unlockDocument(ApDocument* pDoc) = 0;

	virtual ApDocumentIterator* newApDocumentIterator() = 0;

	virtual void addReactor(ApDocManagerReactor*) = 0;
	virtual void removeReactor(ApDocManagerReactor*) = 0;

	//          Allows an external application running under the Application context
	//          to make a document "current" without actually activating it.
	//          This current state is only valid during the context of the function
	//          calling this API. This enables safe manipulation of a documents
	//          data without the document actually being the top most or
	//          active document. 
	//          The default parameters "mode" and "activate" allow document locking
	//          and activation to occur. These are shortcuts to allow several
	//          steps to be accomplished with a single API and can also
	//          be accomplished with the individual API calls.  
	virtual Acad::ErrorStatus setCurDocument(ApDocument* pDoc, Ap::DocLockMode = Ap::kNone, bool activate = false) = 0;

	// Activate target document
	virtual Acad::ErrorStatus activateDocument(ApDocument* pAcTargetDocument, bool bPassScript = false) = 0;

	// Send string to target document to execute by default
	// activate document. Allow the string to be executed as soon as
	// possible if "bActivate" is false" and "bWrapUpInactiveDoc" is true.
	// "bEchoString" determines whether the sent string is echoed on the
	// command line.
	virtual Acad::ErrorStatus sendStringToExecute(ApDocument* pAcTargetDocument, const ACHAR* pszExecute, bool bActivate = true, bool bWrapUpInactiveDoc = false, bool bEchoString = true) = 0;
#if EMSDK
	virtual Acad::ErrorStatus emsdk_sendStringToExecute(ApDocument* pAcTargetDocument, const emstring& pszExecute) = 0;
#endif

	// These two member functions allow synchronous access to NEW OPEN
	// They may only be called from the application context 
	// See also executeInApplicationContext() These APIs only
	// function in MDI mode.
	virtual Acad::ErrorStatus appContextNewDocument(const ACHAR* pszTemplateName) = 0;
	virtual Acad::ErrorStatus appContextOpenDocument(const ACHAR* pszDrawingName) = 0;
	virtual Acad::ErrorStatus appContextRecoverDocument(const ACHAR* pszDrawingName) = 0;
#if EMSDK
	virtual Acad::ErrorStatus emsdk_appContextNewDocument(const emstring& pszTemplateName) = 0;
	virtual Acad::ErrorStatus emsdk_appContextOpenDocument(const emstring& pszDrawingName) = 0;
	virtual Acad::ErrorStatus emsdk_appContextRecoverDocument(const emstring& pszDrawingName) = 0;
#endif

	//exactly the same as appContextNewDocument (including preconditions) but prompts for template name
	Acad::ErrorStatus appContextPromptNewDocument();
	//exactly the same as appContextOpenDocument (including preconditions) but prompts for drawing name
	Acad::ErrorStatus appContextPromptOpenDocument();
	// Caution:  This function immediately destroys the active document and
	// a lot of related data, so beware what you reference after calling.
	// It can only be invoked from the application context.
	Acad::ErrorStatus appContextCloseDocument(ApDocument* pDoc);

	// Return the number of currently open documents
	virtual int documentCount() const = 0;
};



ApDocManager* acDocManagerPtr();
#define acDocManager acDocManagerPtr()

inline ApDocument* curDoc() { return ::acDocManager->curDocument(); }


#endif
