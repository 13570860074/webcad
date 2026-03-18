#ifndef EDCOMMANDSTACK_H
#define EDCOMMANDSTACK_H

#include "acadstrc.h"
#include "EdCommand.h"
#include "EdCommandStackReactor.h"


#define ACRX_COMMAND_DOCK  	"ACAD_REGISTERED_COMMANDS"



class EdCommandIterator;
class EdCommandStack : public RxObject
{
public:
    virtual ~EdCommandStack();
    RX_DECLARE_MEMBERS_VIRTUAL(EdCommandStack);


    virtual Acad::ErrorStatus addCommand(
        const ACHAR* cmdGroupName,const ACHAR* cmdGlobalName,const ACHAR* cmdLocalName,Adesk::Int32 commandFlags,
        int FunctionAddr, void* UIContext = NULL, int fcode = -1, void* resourceInstance = NULL, EdCommand** cmdPtrRet = NULL) = 0;
    virtual Acad::ErrorStatus addCommand(
        const ACHAR* cmdGroupName,const ACHAR* cmdGlobalName,const ACHAR* cmdLocalName,Adesk::Int32 commandFlags,
        RxFunctionPtr FunctionAddr, void* UIContext = NULL, int fcode = -1, void* resourceInstance = NULL, EdCommand** cmdPtrRet = NULL) = 0;

    virtual EdCommandIterator* iterator() = 0;

    virtual EdCommand* lookupGlobalCmd(const ACHAR* cmdName) = 0;
    virtual EdCommand* lookupLocalCmd(const ACHAR* cmdName) = 0;

    virtual Acad::ErrorStatus removeGroup(const ACHAR* groupName) = 0;

    void addReactor(EdCommandStackReactor* pReactor);
    void removeReactor(EdCommandStackReactor* pReactor);
};

#define acedRegCmds \
EdCommandStack::cast(::acrxSysRegistry()->at(ACRX_COMMAND_DOCK))\


#endif


