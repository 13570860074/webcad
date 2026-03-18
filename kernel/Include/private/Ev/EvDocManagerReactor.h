#ifndef EVDOCMANAGERREACTOR_H
#define EVDOCMANAGERREACTOR_H


#include "ApDocManagerReactor.h"

class EvDocManagerReactor : public ApDocManagerReactor
{
public:
	EvDocManagerReactor();
	virtual ~EvDocManagerReactor();

public:

	virtual void documentCreateStarted(ApDocument* pDocCreating);
	virtual void documentCreated(ApDocument* pDocCreating);
	virtual void documentToBeDestroyed(ApDocument* pDocToDestroy);
	virtual void documentDestroyed(const char* fileName);
	virtual void documentCreateCanceled(ApDocument* pDocCreateCancelled);

	virtual void documentLockModeWillChange(ApDocument* document, Ap::DocLockMode myCurrentMode, Ap::DocLockMode myNewMode, Ap::DocLockMode currentMode, const char* pGlobalCmdName);
	virtual void documentLockModeChangeVetoed(ApDocument*, const char* pGlobalCmdName);
	virtual void documentLockModeChanged(ApDocument* document, Ap::DocLockMode myPreviousMode, Ap::DocLockMode myCurrentMode, Ap::DocLockMode currentMode, const char* pGlobalCmdName);

	virtual void documentBecameCurrent(ApDocument*);
	virtual void documentToBeActivated(ApDocument* pActivatingDoc);
	virtual void documentToBeDeactivated(ApDocument* pDeActivatedDoc);
	virtual void documentActivationModified(bool bActivation);
	virtual void documentActivated(ApDocument* pActivatedDoc);

	virtual void documentTitleUpdated(ApDocument*);

};


#endif


