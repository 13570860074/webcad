#ifndef GSELEMENT_H
#define GSELEMENT_H


#include "RxObject.h"

class ApDocument;
class GsElement : public RxObject {
public:
    virtual ~GsElement();

protected:
    GsElement();

public:
    virtual void onInit();

    /** 获得文档 */
    ApDocument* document() const;
};




#endif