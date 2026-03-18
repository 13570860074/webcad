//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2019 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//        
// DESCRIPTION:  header file for class RxDictIterator
// This is a layer of commonality between the two iterators needed for
// RxIDDictionary; RxDictSkipListIterator, and RxDictArrayIterator.

#ifndef RXDICTIONARYITERATOR_H
#define RXDICTIONARYITERATOR_H


#include "RxIterator.h"

class RxDictionaryIterator: public RxIterator
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(RxDictionaryIterator);

#if EMSDK
    virtual const emstring emsdk_key() const = 0;
#else
    virtual const ACHAR * key   () const = 0;
#endif
    RxObject*   object() const override = 0;
    virtual Adesk::UInt32 id    () const = 0;
};


#endif
