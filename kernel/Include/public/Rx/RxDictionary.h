#ifndef RXDICTIONARY_H
#define RXDICTIONARY_H


#include "adesk.h"
#include "Rx.h"
#include "RxObject.h"


class RxDictionaryIterator;
class RxDictionary : public RxObject
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(RxDictionary);

#if EMSDK
    virtual RxObject* emsdk_at(const emstring& key) const = 0;
#endif
    virtual RxObject* at(const ACHAR* key) const = 0;
    virtual RxObject* at(Adesk::UInt32 id) const = 0;

#if EMSDK
    virtual RxObject* emsdk_atPut(const emstring& key, RxObject* pObject) = 0;
#endif
    virtual RxObject* atPut(const ACHAR* key, RxObject* pObject, Adesk::UInt32& retId) = 0;
    virtual RxObject* atPut(Adesk::UInt32 id, RxObject* pObject) = 0;

#if EMSDK
    virtual Adesk::Boolean emsdk_resetKey(const emstring& oldKey, const emstring& newKey) = 0;
#endif
    virtual Adesk::Boolean resetKey(const ACHAR* oldKey, const ACHAR* newKey) = 0;
    virtual Adesk::Boolean resetKey(Adesk::UInt32 id, const ACHAR* newKey) = 0;

#if EMSDK
    virtual Adesk::Boolean emsdk_atKeyAndIdPut(const emstring& newKey, Adesk::UInt32 id, RxObject* pObject) = 0;
#endif
    virtual Adesk::Boolean atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id, RxObject* pObject) = 0;

#if EMSDK
    virtual RxObject* emsdk_remove(const emstring& key) = 0;
#endif
    virtual RxObject* remove(const ACHAR* key) = 0;
    virtual RxObject* remove(Adesk::UInt32 id) = 0;

#if EMSDK
    virtual Adesk::Boolean emsdk_has(const emstring& entryName) const = 0;
#endif
    virtual Adesk::Boolean has(const ACHAR* entryName) const = 0;
    virtual Adesk::Boolean has(Adesk::UInt32 id) const = 0;

#if EMSDK
    virtual Adesk::UInt32  emsdk_idAt(const emstring& key) const = 0;
#endif
    virtual Adesk::UInt32  idAt(const ACHAR* key) const = 0;

    virtual const ACHAR* keyAt(Adesk::UInt32 id) const = 0;

    virtual Adesk::UInt32  numEntries() const = 0;

    virtual RxDictionaryIterator* newIterator(Rx::DictIterType type = Rx::kDictSorted) = 0;

    virtual Adesk::Boolean deletesObjects() const = 0;
    virtual Adesk::Boolean isCaseSensitive() const = 0;
    virtual Adesk::Boolean isSorted() const = 0;
};


extern RxDictionary * acrxSysRegistry();


#endif