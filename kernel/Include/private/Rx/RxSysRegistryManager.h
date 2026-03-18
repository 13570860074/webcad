#ifndef RXSYSREGISTRYMANAGER_H
#define RXSYSREGISTRYMANAGER_H


#include "AcArray.h"
#include "AcString.h"
#include "RxDictionary.h"
#include "RxManager.h"

class RxSysRegistryManager : public RxManager, public RxDictionary{
public:
	RxSysRegistryManager();
    virtual ~RxSysRegistryManager();
    RX_MANAGER_DECLARE_MEMBERS(RxSysRegistryManager);
    
private:
    virtual void onInit();

public:

#if EMSDK
    virtual RxObject* emsdk_at(const emstring& key) const;
#endif
    virtual RxObject* at(const ACHAR* key) const;
    virtual RxObject* at(Adesk::UInt32 id) const;

#if EMSDK
    virtual RxObject* emsdk_atPut(const emstring& key, RxObject* pObject);
#endif
    virtual RxObject* atPut(const ACHAR* key, RxObject* pObject, Adesk::UInt32& retId);
    virtual RxObject* atPut(Adesk::UInt32 id, RxObject* pObject);

#if EMSDK
    virtual Adesk::Boolean emsdk_resetKey(const emstring& oldKey, const emstring& newKey);
#endif
    virtual Adesk::Boolean resetKey(const ACHAR* oldKey, const ACHAR* newKey);
    virtual Adesk::Boolean resetKey(Adesk::UInt32 id, const ACHAR* newKey);

#if EMSDK
    virtual Adesk::Boolean emsdk_atKeyAndIdPut(const emstring& newKey, Adesk::UInt32 id, RxObject* pObject);
#endif
    virtual Adesk::Boolean atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id, RxObject* pObject);

#if EMSDK
    virtual RxObject* emsdk_remove(const emstring& key);
#endif
    virtual RxObject* remove(const ACHAR* key);
    virtual RxObject* remove(Adesk::UInt32 id);

#if EMSDK
    virtual Adesk::Boolean emsdk_has(const emstring& entryName) const;
#endif
    virtual Adesk::Boolean has(const ACHAR* entryName) const;
    virtual Adesk::Boolean has(Adesk::UInt32 id) const;

#if EMSDK
    virtual Adesk::UInt32  emsdk_idAt(const emstring& key) const;
#endif
    virtual Adesk::UInt32  idAt(const ACHAR* key) const;

    virtual const ACHAR* keyAt(Adesk::UInt32 id) const;

    virtual Adesk::UInt32  numEntries() const;

    virtual RxDictionaryIterator* newIterator(Rx::DictIterType type = Rx::kDictSorted);

    virtual Adesk::Boolean deletesObjects() const;
    virtual Adesk::Boolean isCaseSensitive() const;
    virtual Adesk::Boolean isSorted() const;


};




#endif




