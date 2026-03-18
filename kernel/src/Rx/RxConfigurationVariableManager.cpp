#include "RxConfigurationVariableManager.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "RxImpl.h"

RxConfigurationVariableManager::RxConfigurationVariableManager()
{
	this->m_pImpl = new RxConfigurationVariableManagerImpl();
}
RxConfigurationVariableManager::~RxConfigurationVariableManager()
{
}

void RxConfigurationVariableManager::onInit()
{

}
bool RxConfigurationVariableManager::append(const AcString &_name, RxConfigurationVariable *&variable)
{
	RxConfigurationVariable *configurationVariable = RX_IMP_CONFIGURATIONVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
	if (configurationVariable != NULL)
	{
		return false;
	}
	RX_IMP_CONFIGURATIONVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert(_name, variable);
	return true;
}
void RxConfigurationVariableManager::remove(const AcString &_name)
{
	RxConfigurationVariable *configurationVariable = RX_IMP_CONFIGURATIONVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
	if (configurationVariable == NULL)
	{
		return;
	}
	RX_IMP_CONFIGURATIONVARIABLEMANAGER(this->m_pImpl)->recordMaps.remove(_name);
	delete configurationVariable;
}
RxConfigurationVariable *RxConfigurationVariableManager::get(const AcString &_name)
{
	return RX_IMP_CONFIGURATIONVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
}