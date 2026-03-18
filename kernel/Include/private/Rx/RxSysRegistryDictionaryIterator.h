#ifndef RXSYSREGISTRYDICTIONARYITERATOR_H
#define RXSYSREGISTRYDICTIONARYITERATOR_H

#include "RxDictionaryIterator.h"

class RxSysRegistryDictionaryIterator : public RxDictionaryIterator
{
public:
    RxSysRegistryDictionaryIterator();
    virtual ~RxSysRegistryDictionaryIterator();
    RX_DECLARE_MEMBERS_VIRTUAL(RxDictionaryIterator);

    virtual bool done() const;
    virtual bool next();
#if EMSDK
    virtual const std::string emsdk_key() const;
#else
    virtual const ACHAR* key() const;
#endif
    RxObject* object() const override;
    virtual Adesk::UInt32 id() const;
};


#endif
