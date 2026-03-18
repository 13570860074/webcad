#ifndef RXMANAGER_H
#define RXMANAGER_H

#include "RxObject.h"
#include "AcArray.h"
#include <functional>


class RxMangerStub;
class RxManager
{
public:
	RxManager();
	virtual ~RxManager();
	
public:

	/* 初始化 */
	static void instantiate();

	/* 通过类名查询管理器 */
	static RxManager* queryManager(const AcString& _className);

public:

	virtual void onInit();
	virtual const AcString moduleName()const { return "RxManager"; }
};

#define RX_MANAGER_DECLARE_MEMBERS(CLASSNAME)\
public:\
	const AcString moduleName()const { return #CLASSNAME; }\

#define acrxManager(CLASSNAME)\
((CLASSNAME*)RxManager::acrxManager(#CLASSNAME))\


#endif
