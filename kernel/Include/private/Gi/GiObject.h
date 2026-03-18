#ifndef GIOBJECT_H
#define GIOBJECT_H


#include "export.h"
#include "RxObject.h"
#include "GiObjectId.h"



class GiObject : public RxObject
{
public:
    virtual ~GiObject();
    RX_DECLARE_MEMBERS(GiObject);

    GiObjectId objectId() const;
};




#endif




