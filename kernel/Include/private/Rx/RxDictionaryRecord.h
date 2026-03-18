#ifndef RXDICTIONARYRECORD_H
#define RXDICTIONARYRECORD_H


#include "AcArray.h"
#include "AcString.h"
#include "RxObject.h"

class RxDictionaryRecord :public RxObject {
public:
    RxDictionaryRecord() {
        this->id = 0;
        this->pObject = NULL;
    }
    ~RxDictionaryRecord() {}
    RX_DECLARE_MEMBERS(RxDictionaryRecord);


public:
    unsigned int id;
    AcString key;
    RxObject* pObject;
};




#endif




