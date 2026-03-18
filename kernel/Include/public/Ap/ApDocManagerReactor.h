#ifndef APDOCMANAGERREACTOR_H
#define APDOCMANAGERREACTOR_H


#include "Ap.h"
#include "RxObject.h"


class ApDocument;
class ApDocManagerReactor : public RxObject
{
public:
    RX_DECLARE_MEMBERS(ApDocManagerReactor);

    // Fired when a document is first instantiated no database is available.
    virtual void documentCreateStarted(ApDocument* pDocCreating);
    // Fired when a document has been successfully created replaces kLoadDwgMsg.
    virtual void documentCreated(ApDocument* pDocCreating);
    // Fired when a document is about to be destroyed replaces kUnloadDwgMsg
    virtual void documentToBeDestroyed(ApDocument* pDocToDestroy);
    // Fired when a document is completely destroyed
    virtual void documentDestroyed(const ACHAR* fileName);
    // Fired when a user action has cancelled a documents creation.
    // Only fired in MDI mode and after a documentCreateStarted
    virtual void documentCreateCanceled(ApDocument* pDocCreateCancelled);


    virtual void documentLockModeWillChange(
        ApDocument* document,
        Ap::DocLockMode myCurrentMode,
        Ap::DocLockMode myNewMode,
        Ap::DocLockMode currentMode,
        const ACHAR* pGlobalCmdName);
    virtual void documentLockModeChangeVetoed(ApDocument*, const ACHAR* pGlobalCmdName);
    virtual void documentLockModeChanged(
        ApDocument* document,
        Ap::DocLockMode myPreviousMode,
        Ap::DocLockMode myCurrentMode,
        Ap::DocLockMode currentMode,
        const ACHAR* pGlobalCmdName);

    virtual void documentBecameCurrent(ApDocument*);

    // This reactor is fired PRIOR to a document becoming activated NOTE: The document
    // context will not be set yet see also documentActivated()
    virtual void documentToBeActivated(ApDocument* pActivatingDoc);
    // Parallel to documentToBeActivated
    virtual void documentToBeDeactivated(ApDocument* pDeActivatedDoc);

    // Notify  of changes caused by disableDocumentActivation() or enableDocumentActivation().
    // bActivation == TRUE when document activation is enabled FALSE when disabled.
    virtual void documentActivationModified(bool bActivation);

    // This reactor is fired once a document has been activated and its context
    // successfully switched. NOTE: There are situations where document is
    // "activated" outside the normal UI user interaction. See also documentToBeActivated
    virtual void documentActivated(ApDocument* pActivatedDoc);

    virtual void documentTitleUpdated(ApDocument*);

    //emsdk
#if EMSDK
public:
    ApDocManagerReactor();
    virtual ~ApDocManagerReactor();

    void onDocumentCreateStarted(int ptr);
    void onDocumentCreated(int ptr);
    void onDocumentToBeDestroyed(int ptr);
    void onDocumentDestroyed(int ptr);
    void onDocumentCreateCanceled(int ptr);
    void onDocumentLockModeWillChange(int ptr);
    void onDocumentLockModeChangeVetoed(int ptr);
    void onDocumentLockModeChanged(int ptr);
    void onDocumentBecameCurrent(int ptr);
    void onDocumentToBeActivated(int ptr);
    void onDocumentToBeDeactivated(int ptr);
    void onDocumentActivationModified(int ptr);
    void onDocumentActivated(int ptr);
    void onDocumentTitleUpdated(int ptr);
#endif


};


#endif


