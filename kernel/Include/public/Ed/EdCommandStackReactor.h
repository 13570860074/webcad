#ifndef EDCOMMANDSTACKREACTOR_H
#define EDCOMMANDSTACKREACTOR_H

#include "RxObject.h"

class EdCommand;
class EdCommandEventArgs;
class EdCommandStackReactor : public RxObject
{
public:
    RX_DECLARE_MEMBERS(EdCommandStackReactor);

    virtual void commandAdded(EdCommand* pCmd, EdCommandEventArgs* data);
    virtual void commandRemoved(EdCommand* pCmd, EdCommandEventArgs* data);
    virtual void commandWillStart(EdCommand* pCmd, EdCommandEventArgs* data);
    virtual void commandEnded(EdCommand* pCmd, EdCommandEventArgs* data);

#if EMSDK
    EdCommandStackReactor();
    virtual ~EdCommandStackReactor();

    void onCommandAdded(int ptr);
    void onCommandRemoved(int ptr);
    void onCommandWillStart(int ptr);
    void onCommandEnded(int ptr);
#endif
};


#endif