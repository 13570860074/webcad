#include "RxManager.h"
#include "kernel.h"
#include "RxManagerDictionary.h"
#include "RxImpl.h"

RxManager::RxManager()
{
	RxMangerStub *stub = new RxMangerStub();
	stub->isInit = false;
	stub->pt = this;
	stub->fun = NULL;
	::acrxManagerDictionary()->appendManager(stub);
}
RxManager::~RxManager()
{
}

void RxManager::instantiate()
{
	::acrxManagerDictionary()->instantiate();
}
RxManager *RxManager::queryManager(const AcString &_className)
{
	return ::acrxManagerDictionary()->query(_className);
}

void RxManager::onInit()
{
}
