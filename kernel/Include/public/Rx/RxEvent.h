#ifndef RXEVENT_H
#define RXEVENT_H

#include "RxObject.h"

#define ACRX_EVENT_OBJ "AcRxEvent"


#define acrxEvent RxEvent::cast(acrxSysRegistry()->at(ACRX_EVENT_OBJ))

class RxEventReactor;
class RxEvent : public RxObject
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(RxEvent);
    virtual void addReactor(RxEventReactor* newObj) = 0;
    virtual void removeReactor(RxEventReactor* delObj) = 0;
};



#endif