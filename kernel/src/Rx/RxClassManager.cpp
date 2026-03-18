#include "RxClassManager.h"
#include "RxObject.h"
#include "RxImpl.h"

bool RxClassManagerImpl::isInitClass = false;
AcMap<AcString, RxClass*> RxClassManagerImpl::classMaps = AcMap<AcString, RxClass*>();
AcArray<std::function<RxObject* ()>> pRxObjectMallocClass;

RxClassManager::RxClassManager()
{
	this->m_Impl = new RxClassManagerImpl();
}
RxClassManager::~RxClassManager()
{
	if (this->m_Impl != NULL)
	{
		delete this->m_Impl;
		this->m_Impl = NULL;
	}
};
void RxClassManager::registerInitClass()
{
	do
	{
		if (RxClassManagerImpl::isInitClass == true)
		{
			break;
		}
		RxClassManagerImpl::isInitClass = true;

		

	} while (false);
}
void RxClassManager::registerClass(const char* name) {

}
void RxClassManager::registerClass(const char* name, std::function<RxObject* ()> _fun)
{

}
void RxClassManager::registerMallocClass(std::function<RxObject* ()> _fun)
{
	pRxObjectMallocClass.append(_fun);
}
void RxClassManager::mallocClass()
{
	for (int i = 0; i < pRxObjectMallocClass.length(); i++)
	{
		pRxObjectMallocClass[i]();
	}
	pRxObjectMallocClass.removeAll();
}
RxObject* RxClassManager::create(const char* name)
{
	return NULL;
}
RxClass* RxClassManager::acrxFindAcRxClass(const char* name)
{
	return RxClassManagerImpl::classMaps.find(name);
}
