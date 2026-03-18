#ifndef EDREGISTRYCOMMANDSTACK_H
#define EDREGISTRYCOMMANDSTACK_H

#include "EdCommandStack.h"


class EdCommandIterator;
class EdRegistryCommandStack : public EdCommandStack
{
public:
    EdRegistryCommandStack();
    virtual ~EdRegistryCommandStack();
    RX_DECLARE_MEMBERS(EdRegistryCommandStack);

    bool isCommand();
    virtual Acad::ErrorStatus sendCommand(const AcString& command);

    virtual Acad::ErrorStatus addCommand(
        const ACHAR* cmdGroupName, const ACHAR* cmdGlobalName, const ACHAR* cmdLocalName, Adesk::Int32 commandFlags, int FunctionAddr,
        void* UIContext = NULL, int fcode = -1, void* resourceInstance = NULL, EdCommand** cmdPtrRet = NULL);
    virtual Acad::ErrorStatus addCommand(
        const ACHAR* cmdGroupName,const ACHAR* cmdGlobalName,const ACHAR* cmdLocalName,Adesk::Int32 commandFlags,
        RxFunctionPtr FunctionAddr, void* UIContext = NULL, int fcode = -1, void* resourceInstance = NULL, EdCommand** cmdPtrRet = NULL);

    virtual EdCommandIterator* iterator();

    virtual EdCommand* lookupGlobalCmd(const ACHAR* cmdName);
    virtual EdCommand* lookupLocalCmd(const ACHAR* cmdName);

    virtual Acad::ErrorStatus removeGroup(const ACHAR* groupName);
};



#endif


