#ifndef EDTOUCHARRAY_H
#define EDTOUCHARRAY_H


#include "EdTouch.h"
#include "AcArray.h"



#if !EMSDK
typedef AcArray<EdTouch*> EdTouchArray;
#endif


// emsdk
#if EMSDK
class EdTouchArray
{
public:
	EdTouchArray() {}
	virtual ~EdTouchArray() {}

public:
	ACARRAY_PTR_METHODS(EdTouch*);
};
#endif



#endif