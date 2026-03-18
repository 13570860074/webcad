#ifndef RXCLASSMANAGER_H
#define RXCLASSMANAGER_H

#include "export.h"
#include <functional>

class RxClass;
class RxObject;
class RxClassManagerImpl;
class RxClassManager {
public:
	RxClassManager();
	virtual ~RxClassManager();

	// 注册初始类
	static void registerInitClass();

	// 注册类
	static void registerClass(const char* name);
	static void registerClass(const char* name, std::function<RxObject* ()> _fun);

	// 注册实例化类
	static void registerMallocClass(std::function<RxObject* ()> _fun);

	// 实例化类
	static void mallocClass();

	// 创建类
	static RxObject* create(const char* name);

	// 查找类
	static RxClass *acrxFindAcRxClass(const char* name);

private:
	RxClassManagerImpl* m_Impl;
};


#define RX_REGISTER_CLASS(CLASSNAME) \
class CLASSNAME##_Register {\
public:\
	CLASSNAME##_Register() {\
		RxClassManager::registerClass(#CLASSNAME, [] { return new CLASSNAME();});\
	}\
};\
CLASSNAME##_Register();\


#define RX_REGISTER_CLASS_VIRTUAL(CLASSNAME) \
class CLASSNAME##_Register {\
public:\
	CLASSNAME##_Register() {\
		RxClassManager::registerClass(#CLASSNAME);\
	}\
};\
CLASSNAME##_Register();\


#define RX_REGISTER_MALLOC(CLASSNAME) \
    class CLASSNAME##_registerMalloc {\
    public:\
        CLASSNAME##_registerMalloc() {\
            std::function<RxObject* ()> _fun = [] { return new CLASSNAME(); };\
            RxClassManager::registerMallocClass(_fun);\
        }\
};\
CLASSNAME##_registerMalloc _registerMalloc##CLASSNAME = CLASSNAME##_registerMalloc();\


#endif