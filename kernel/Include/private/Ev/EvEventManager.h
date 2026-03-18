#ifndef EVEVENTMANAGER_H
#define EVEVENTMANAGER_H


#include "RxObject.h"
#include "Ev.h"
#include "AcEvent.h"
#include "RxManager.h"




class ApDocument;
class EvEventManager : public RxObject, public RxManager, public AcEvent {
public:
    EvEventManager();
    virtual ~EvEventManager();
    RX_DECLARE_MEMBERS(EvEventManager);
    RX_MANAGER_DECLARE_MEMBERS(EvEventManager);

public:
    static void onDocumentCreated(EvEventManager * pointer, ApDocument* pDocCreating);

public:

    // 初始化
    void onInit();

public:

};




#endif




