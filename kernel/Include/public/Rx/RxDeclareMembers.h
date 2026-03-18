#ifndef RXDECLAREMEMBERS_H
#define RXDECLAREMEMBERS_H


#include "AcString.h"
#include "RxClassManager.h"

extern RxClass* acrxFindAcRxClass(const char* name);

#if EMSDK

#define _RX_DECLARE_MEMBERS(CLASS_NAME)\
public:\
	static CLASS_NAME* cast(const RxObject* inPtr) {\
		if(inPtr == NULL){\
			return NULL;\
		}\
		RxObject *p = const_cast<RxObject*>(inPtr);\
		return dynamic_cast<CLASS_NAME*>(p);\
	}\
	static CLASS_NAME* cast(const unsigned int inPtr) {\
		if(inPtr == 0){\
			return NULL;\
		}\
		RxObject *p = (RxObject*)inPtr;\
		return dynamic_cast<CLASS_NAME*>(p);\
	}\
	virtual RxClass *isA() const{\
		return ::acrxFindAcRxClass(#CLASS_NAME); \
	}\
	static RxClass* desc(){\
		return ::acrxFindAcRxClass(#CLASS_NAME); \
	}\
	static const char* className() { return #CLASS_NAME; } \
	virtual void className(AcString& name)const { name = #CLASS_NAME; }\

#else

#define _RX_DECLARE_MEMBERS(CLASS_NAME)\
public:\
	static CLASS_NAME* cast(const RxObject* inPtr) {\
		if(inPtr == NULL){\
			return NULL;\
		}\
		RxObject *p = const_cast<RxObject*>(inPtr);\
		return dynamic_cast<CLASS_NAME*>(p);\
	}\
	static CLASS_NAME* cast(const long long inPtr) {\
		if(inPtr == 0){\
			return NULL; \
		}\
		RxObject *p = (RxObject*)inPtr;\
		return dynamic_cast<CLASS_NAME*>(p);\
	}\
	virtual RxClass *isA() const{\
		return ::acrxFindAcRxClass(#CLASS_NAME); \
	}\
	static RxClass* desc(){\
		return ::acrxFindAcRxClass(#CLASS_NAME); \
	}\
	static const char* className() { return #CLASS_NAME; } \
	virtual void className(AcString& name)const { name = #CLASS_NAME; }\

#endif


#define RX_DECLARE_MEMBERS(CLASS_NAME)\
public:\
	_RX_DECLARE_MEMBERS(CLASS_NAME);\
	static void rxInit() { RX_REGISTER_CLASS(CLASS_NAME); } \


#define RX_DECLARE_MEMBERS_VIRTUAL(CLASS_NAME)\
public:\
	_RX_DECLARE_MEMBERS(CLASS_NAME);\
	static void rxInit() { RX_REGISTER_CLASS_VIRTUAL(CLASS_NAME); } \




#endif