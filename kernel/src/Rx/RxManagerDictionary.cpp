#include "RxManager.h"
#include "RxManagerDictionary.h"

RxManagerDictionary::RxManagerDictionary()
{
	this->pManagers = new AcArray<RxMangerStub *>();
}
RxManagerDictionary::~RxManagerDictionary()
{
	realloc_all(this->pManagers);
}
void RxManagerDictionary::instantiate()
{
	for (int i = 0; i < this->pManagers->length(); i++)
	{
		RxMangerStub *stub = this->pManagers->at(i);
		if (stub->disabledInit == true) {
			continue;
		}
		if (stub->pt == NULL)
		{
			stub->pt = stub->fun();
			stub->pt->onInit();
		}
		else if (stub->pt != NULL && stub->isInit == false)
		{
			stub->isInit = true;
			stub->pt->onInit();
		}
	}
}
RxManager *RxManagerDictionary::query(const AcString &name)
{
	for (int i = 0; i < this->pManagers->length(); i++)
	{
		RxMangerStub *stub = this->pManagers->at(i);
		if (stub->pt != NULL)
		{
			if(stub->pt->moduleName() == name) {
				return stub->pt;
			}
		}
	}
	return NULL;
}
void RxManagerDictionary::appendManager(RxMangerStub *manager)
{
	this->pManagers->append(manager);
}
void RxManagerDictionary::disabledInit(const AcString& name, const bool v) {
	for (int i = 0; i < this->pManagers->length(); i++)
	{
		RxMangerStub* stub = this->pManagers->at(i);
		if (stub->pt->moduleName().isEqual(name, true) == true) {
			std::cout<< stub->pt->moduleName().constPtr()<<":"<<v << std::endl;
			stub->disabledInit = v;
			break;
		}
	}
}