//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2019 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RXITERATOR_H
#define RXITERATOR_H

#include "adesk.h"
#include "RxObject.h"
#include "AcArray.h"

class RxIterator: public RxObject
{
protected:
    RX_DECLARE_MEMBERS_VIRTUAL(RxIterator);

    virtual bool done() const = 0;
    virtual bool next() = 0;
    virtual RxObject* object() const = 0;
};



#endif
