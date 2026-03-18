#ifndef RXOBJECT_H
#define RXOBJECT_H


#include "adesk.h"
#include "acadstrc.h"
#include "EmObject.h"
#include "RxDeclareMembers.h"
#include "RxObjectImpl.h"
#include "AcDebug.h"
#include "AcString.h"

class RxClass;
class DbDwgFiler;
class RxObject INHERIT_EM_OBJECT {
public:
	RxObject();
	virtual ~RxObject();

	static RxObject* cast(const RxObject* inPtr)
	{
		return const_cast<RxObject*>(inPtr);
	}

public:
	virtual RxClass* isA() const = 0;
	virtual void className(AcString& name)const { name = ""; }
	virtual RxObject* clone() const;
	Acad::ErrorStatus         dwgIn(DbDwgFiler* pFiler);
	virtual Acad::ErrorStatus dwgInFields(DbDwgFiler* pFiler);
	Acad::ErrorStatus         dwgOut(DbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgOutFields(DbDwgFiler* pFiler) const;

public:
	RxObjectImpl* m_pImpl;
};




#endif