#include "RxSysRegistryManager.h"
#include "EditorReactorManager.h"
#include "EdRegistryCommandStack.h"
#include "RxSysRegistryDictionaryIterator.h"
#include "DbObjectId.h"
#include "kernel.h"
#include "export.h"
#include "RxImpl.h"


RxSysRegistryManager::RxSysRegistryManager() {
    this->m_pImpl = new RxSysRegistryDictionaryImpl();
}
RxSysRegistryManager::~RxSysRegistryManager() {

}

void RxSysRegistryManager::onInit() {

    // 创建命令堆载
    Adesk::UInt32 retId;
    EdRegistryCommandStack* pCommandStack = new EdRegistryCommandStack();
    this->atPut(ACRX_COMMAND_DOCK, pCommandStack, retId);

    // 创建编辑反应器管理器
    EditorReactorManager* pReactorManager = new EditorReactorManager();
    this->atPut(ACED_EDITOR_OBJ, pReactorManager, retId);
}



#if EMSDK
RxObject* RxSysRegistryManager::emsdk_at(const emstring& key) const {
    return this->at(key.c_str());
}
#endif
RxObject* RxSysRegistryManager::at(const ACHAR* key) const {
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key == key) {
            return RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->pObject;
        }
    }
    return NULL;
}
RxObject* RxSysRegistryManager::at(Adesk::UInt32 id) const {
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id == id) {
            return RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->pObject;
            break;
        }
    }
    return NULL;
}
#if EMSDK
RxObject* RxSysRegistryManager::emsdk_atPut(const emstring& key, RxObject* pObject) {
    Adesk::UInt32 retId;
    return this->atPut(key.c_str(), pObject, retId);
}
#endif

RxObject* RxSysRegistryManager::atPut(const ACHAR* key, RxObject* pObject, Adesk::UInt32& retId) {
    if (this->has(key) == true) {
        return NULL;
    }

    // 分配一个id
    int id = 1;
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id > id) {
            id = RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id + 1;
        }
    }

    retId = id;

    RxDictionaryRecord* pDictionaryRecord = new RxDictionaryRecord();
    pDictionaryRecord->id = id;
    pDictionaryRecord->key = key;
    pDictionaryRecord->pObject = pObject;
    RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->append(pDictionaryRecord);
    return pObject;
}
RxObject* RxSysRegistryManager::atPut(Adesk::UInt32 id, RxObject* pObject) {
    if (this->has(id) == true) {
        return NULL;
    }
    RxDictionaryRecord* pDictionaryRecord = new RxDictionaryRecord();
    pDictionaryRecord->id = id;
    pDictionaryRecord->pObject = pObject;
    RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->append(pDictionaryRecord);
    return pObject;
}
#if EMSDK
Adesk::Boolean RxSysRegistryManager::emsdk_resetKey(const emstring& oldKey, const emstring& newKey) {
    return this->resetKey(oldKey.c_str(), newKey.c_str());
}
#endif
Adesk::Boolean RxSysRegistryManager::resetKey(const ACHAR* oldKey, const ACHAR* newKey) {
    bool isEs = false;
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key == oldKey) {
            RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key = newKey;
            isEs = true;
            break;
        }
    }
    return isEs;
}
Adesk::Boolean RxSysRegistryManager::resetKey(Adesk::UInt32 id, const ACHAR* newKey) {
    bool isEs = false;
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id == id) {
            RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key = newKey;
            isEs = true;
            break;
        }
    }
    return isEs;
}

#if EMSDK
Adesk::Boolean RxSysRegistryManager::emsdk_atKeyAndIdPut(const emstring& newKey, Adesk::UInt32 id, RxObject* pObject) {
    return this->atKeyAndIdPut(newKey.c_str(), id, pObject);
}
#endif
Adesk::Boolean RxSysRegistryManager::atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id, RxObject* pObject) {
    bool isEs = false;
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id == id && 
            RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->pObject == pObject) {
            RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key = newKey;
            isEs = true;
            break;
        }
    }
    return isEs;
}
#if EMSDK
RxObject* RxSysRegistryManager::emsdk_remove(const emstring& key) {
    return this->remove(key.c_str());
}
#endif
RxObject* RxSysRegistryManager::remove(const ACHAR* key) {
    RxObject *object = NULL;
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key == key) {
            object = RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->pObject;
            break;
        }
    }
    return object;
}
RxObject* RxSysRegistryManager::remove(Adesk::UInt32 id) {
    RxObject* object = NULL;
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id == id) {
            object = RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->pObject;
            break;
        }
    }
    return object;
}

#if EMSDK
Adesk::Boolean RxSysRegistryManager::emsdk_has(const emstring& entryName) const {
    return this->has(entryName.c_str());
}
#endif
Adesk::Boolean RxSysRegistryManager::has(const ACHAR* entryName) const {
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key == entryName) {
            return true;
        }
    }
    return false;
}
Adesk::Boolean RxSysRegistryManager::has(Adesk::UInt32 id) const {
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id == id) {
            return true;
        }
    }
    return false;
}

#if EMSDK
Adesk::UInt32  RxSysRegistryManager::emsdk_idAt(const emstring& key) const {
    return this->idAt(key.c_str());
}
#endif
Adesk::UInt32  RxSysRegistryManager::idAt(const ACHAR* key) const {
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key == key) {
            return RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id;
        }
    }
    return 0;
}

const ACHAR* RxSysRegistryManager::keyAt(Adesk::UInt32 id) const {
    for (int i = 0; i < RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length(); i++) {
        if (RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->id == id) {
            return RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->at(i)->key.constPtr();
        }
    }
    return "";
}

Adesk::UInt32  RxSysRegistryManager::numEntries() const {
    return RX_IMP_SYSREGISTRYDICTIONARY(this->m_pImpl)->pRecords->length();
}

RxDictionaryIterator* RxSysRegistryManager::newIterator(Rx::DictIterType type) {
    RxDictionaryIterator* pIterator = new RxSysRegistryDictionaryIterator();
    RX_IMP_ITERATOR(pIterator->m_pImpl)->pRecords = RX_IMP_SYSREGISTRYDICTIONARYITERATOR(this->m_pImpl)->pRecords;
    return pIterator;
}

Adesk::Boolean RxSysRegistryManager::deletesObjects() const {
    return false;
}
Adesk::Boolean RxSysRegistryManager::isCaseSensitive() const {
    return false;
}
Adesk::Boolean RxSysRegistryManager::isSorted() const {
    return false;
}



RxDictionary* acrxSysRegistry() {
    return ::kernel()->acrxSysRegistry();
}