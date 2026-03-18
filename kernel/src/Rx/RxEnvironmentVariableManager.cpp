#include "RxEnvironmentVariableManager.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "RxImpl.h"

RxEnvironmentVariableManager::RxEnvironmentVariableManager()
{
	this->m_pImpl = new RxEnvironmentVariableManagerImpl();
}
RxEnvironmentVariableManager::~RxEnvironmentVariableManager()
{
}

void RxEnvironmentVariableManager::onInit()
{

}
bool RxEnvironmentVariableManager::append(const AcString &_name, RxEnvironmentVariable *&variable)
{
	RxEnvironmentVariable *environmentVariable = RX_IMP_ENVIRONMENTVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
	if (environmentVariable != NULL)
	{
		return false;
	}
	RX_IMP_ENVIRONMENTVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert(_name, variable);
	return true;
}
void RxEnvironmentVariableManager::remove(const AcString &_name)
{
	RxEnvironmentVariable *environmentVariable = RX_IMP_ENVIRONMENTVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
	if (environmentVariable == NULL)
	{
		return;
	}
	RX_IMP_ENVIRONMENTVARIABLEMANAGER(this->m_pImpl)->recordMaps.remove(_name);
	delete environmentVariable;
}
RxEnvironmentVariable *RxEnvironmentVariableManager::get(const AcString &_name)
{
	return RX_IMP_ENVIRONMENTVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
}