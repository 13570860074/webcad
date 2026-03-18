#include "RxConfigurationVariable.h"
#include "arxHeaders.h"

RxConfigurationVariable::RxConfigurationVariable()
{
	this->m_name = "";
	this->m_variable = "";
}
RxConfigurationVariable::~RxConfigurationVariable()
{
}

const AcString &RxConfigurationVariable::name()const
{
	return this->m_name;
}
void RxConfigurationVariable::setName(const AcString &_name)
{
	this->m_name = _name;
}

const AcString& RxConfigurationVariable::variable()const
{
	return this->m_variable;
}
void RxConfigurationVariable::setVariable(const AcString &_val)
{
	this->m_variable = _val;
}