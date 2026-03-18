#ifndef APWORKDOCMANAGER_H
#define APWORKDOCMANAGER_H


#include "ApDocManager.h"
#include "RxManager.h"


class ApWorkDocManager : public RxManager, public ApDocManager
{
public:
    ApWorkDocManager();
    virtual ~ApWorkDocManager();
    RX_DECLARE_MEMBERS(ApWorkDocManager);
    RX_MANAGER_DECLARE_MEMBERS(ApWorkDocManager);

public:

    static void onBeginDrag(ApWorkDocManager* pointer);
    static void onEndDrag(ApWorkDocManager* pointer);

    static void onBeginAnimationFrame(ApWorkDocManager* pointer);
    static void onCommandWillStart(ApWorkDocManager *pointer, const ACHAR* cmdStr);
    static void onCommandEnded(ApWorkDocManager* pointer, const ACHAR* cmdStr);
    static void onCommandCancelled(ApWorkDocManager* pointer, const ACHAR* cmdStr);
    static void onCommandFailed(ApWorkDocManager* pointer, const ACHAR* cmdStr);

    virtual void onInit();

public:

    AcString extractStack(const ACHAR ch)const;

    virtual ApDocument* curDocument() const;
    virtual ApDocument* mdiActiveDocument() const;

    virtual bool isApplicationContext() const;

    virtual ApDocument* document(const DbDatabase*) const;

    virtual Acad::ErrorStatus lockDocument(ApDocument* pDoc, Ap::DocLockMode = Ap::kWrite, const char* pGlobalCmdName = NULL, const char* pLocalCmdName = NULL, bool prompt = true);
    virtual Acad::ErrorStatus unlockDocument(ApDocument* pDoc);

    virtual ApDocumentIterator* newApDocumentIterator();
    virtual void addReactor(ApDocManagerReactor*);
    virtual void removeReactor(ApDocManagerReactor*);

    virtual Acad::ErrorStatus setCurDocument(ApDocument* pDoc, Ap::DocLockMode = Ap::kNone, bool activate = false);
    virtual Acad::ErrorStatus activateDocument(ApDocument* pAcTargetDocument, bool bPassScript = false);

    virtual Acad::ErrorStatus sendStringToExecute(ApDocument* pAcTargetDocument, const ACHAR* pszExecute, bool bActivate, bool bWrapUpInactiveDoc, bool bEchoString);
#if EMSDK
    virtual Acad::ErrorStatus emsdk_sendStringToExecute(ApDocument* pAcTargetDocument, const emstring& pszExecute);
#endif

    virtual Acad::ErrorStatus appContextNewDocument(const ACHAR* pszTemplateName);
    virtual Acad::ErrorStatus appContextOpenDocument(const ACHAR* pszDrawingName);
    virtual Acad::ErrorStatus appContextRecoverDocument(const ACHAR* pszDrawingName);
#if EMSDK
    virtual Acad::ErrorStatus emsdk_appContextNewDocument(const emstring& pszTemplateName);
    virtual Acad::ErrorStatus emsdk_appContextOpenDocument(const emstring& pszDrawingName);
    virtual Acad::ErrorStatus emsdk_appContextRecoverDocument(const emstring& pszDrawingName);
#endif

    virtual Acad::ErrorStatus appContextOpenDocument(ApDocument* _document);

    virtual Acad::ErrorStatus appDatabaseNewDocument(const DbDatabase *database, const ACHAR* pszExecute);


    virtual int documentCount() const;
};





#endif
