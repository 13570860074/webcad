#ifndef EDREGISTRYCOMMAND_H
#define EDREGISTRYCOMMAND_H

#include "adesk.h"
#include "RxObject.h"
#include "EdCommand.h"


class EdRegistryCommand : public EdCommand
{
public:
    EdRegistryCommand();
    virtual ~EdRegistryCommand();
    RX_DECLARE_MEMBERS(EdRegistryCommand);

    virtual void commandUndef(bool undefIt);

    virtual void commandFlags(Adesk::Int32 flags) const;
    virtual Adesk::Int32 commandFlags() const;

    virtual RxFunctionPtr functionAddr() const;
    virtual void functionAddr(RxFunctionPtr fhdl);

    virtual void commandApp(void *app)const;
    virtual void* commandApp() const;

    virtual int functionCode() const;
    virtual void functionCode(int fcode);

    virtual void groupName(const ACHAR* name) const;
    virtual const ACHAR* groupName() const;
    virtual void globalName(const ACHAR* name);
    virtual const ACHAR* globalName() const;
    virtual void localName(const ACHAR* name);
    virtual const ACHAR* localName() const;

#if EMSDK
    virtual void emsdk_groupName(const emstring& name) const;
    virtual const emstring emsdk_groupName() const;
    virtual void emsdk_globalName(const emstring& name) const;
    virtual const emstring emsdk_globalName() const;
    virtual void emsdk_localName(const emstring& name) const;
    virtual const emstring emsdk_localName() const;
#endif
};


#endif