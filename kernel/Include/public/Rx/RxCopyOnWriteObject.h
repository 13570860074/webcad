#ifndef RXCOPYONWRITEOBJECT_H
#define RXCOPYONWRITEOBJECT_H

#include "RxObject.h"


class RxCopyOnWriteObject :public RxObject
{
public:
	RxCopyOnWriteObject();
	virtual ~RxCopyOnWriteObject();
};



#endif