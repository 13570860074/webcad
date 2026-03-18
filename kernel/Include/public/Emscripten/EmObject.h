#ifndef   EMOBJECT_H
#define   EMOBJECT_H

#include "export.h"

class EmObject {
public:
	EmObject() {}
	virtual ~EmObject() {};
};

#if EMSDK
#define INHERIT_EM_OBJECT :public EmObject
#else
#define INHERIT_EM_OBJECT
#endif

#endif //_ADESK_H
