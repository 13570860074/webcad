#ifndef APDOCUMENTITERATOR_H
#define APDOCUMENTITERATOR_H

#include "AcArray.h"
#include "RxObject.h"


class ApDocument;
class ApDocumentIterator :public RxObject
{
public:
    ApDocumentIterator();
    virtual ~ApDocumentIterator();
    RX_DECLARE_MEMBERS(ApDocumentIterator);


    void start();
    bool done();
    void step();
    ApDocument* document();
};



#endif
