#ifndef GIDRAWABLE_H
#define GIDRAWABLE_H

#include "RxObject.h"


class GiDrawable :public RxObject
{
public:
	GiDrawable();
	virtual ~GiDrawable();
	RX_DECLARE_MEMBERS(GiDrawable);
};



#endif