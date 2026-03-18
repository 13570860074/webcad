#ifndef EDCOMMAND_H
#define EDCOMMAND_H

#include "adesk.h"
#include "RxObject.h"
#include "EdCommandReactor.h"




typedef void (*RxFunctionPtr) ();



class EdCommand;
struct EdCommandStruc {
    RxFunctionPtr fcnAddr;
    long            flags;
    void* app;

    //This member should obviate the need to the ones above
    //since they are already contained in AcEdCommand.
    EdCommand* cmd;
};






class EdCommand : public RxObject
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(EdCommand);

    virtual void            commandUndef(bool undefIt) = 0;
    virtual Adesk::Int32    commandFlags() const = 0;
    virtual RxFunctionPtr functionAddr() const = 0;
    virtual void            functionAddr(RxFunctionPtr fhdl) = 0;
    virtual void* commandApp() const = 0;
    virtual int             functionCode() const = 0;
    virtual void            functionCode(int fcode) = 0;
    virtual const ACHAR* globalName() const = 0;
    virtual const ACHAR* localName() const = 0;
#if EMSDK
    virtual const emstring emsdk_globalName() const = 0;
    virtual const emstring emsdk_localName() const = 0;
#endif


    
    void addReactor(EdCommandReactor* pReactor);
    void removeReactor(EdCommandReactor* pReactor);
    

    Adesk::UInt8 defaultCommandVersion() const;
};


#endif