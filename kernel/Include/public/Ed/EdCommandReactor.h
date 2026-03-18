#ifndef EDCOMMANDREACTOR_H
#define EDCOMMANDREACTOR_H

#include "adesk.h"
#include "RxObject.h"


class EdCommand;
class EdCommandEventArgs;
class EdCommandReactor : public RxObject
{
public:
    RX_DECLARE_MEMBERS(EdCommandReactor);

    virtual void commandWillStart(EdCommand* pCmd, EdCommandEventArgs* data);
    virtual void     commandEnded(EdCommand* pCmd, EdCommandEventArgs* data);
    virtual void          goodbye(EdCommand* pCmd, EdCommandEventArgs* data); //sent just before cmd is to be deleted

#if EMSDK
    EdCommandReactor();
    virtual ~EdCommandReactor();

    void onCommandWillStart(int ptr);
    void onCommandEnded(int ptr);
    void onGoodbye(int ptr);
#endif
};

#endif