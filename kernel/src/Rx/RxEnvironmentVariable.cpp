#include "RxEnvironmentVariable.h"
#include "arxHeaders.h"

RxEnvironmentVariable::RxEnvironmentVariable()
{
	this->m_name = "";
	this->m_variable = "";
}
RxEnvironmentVariable::~RxEnvironmentVariable()
{
}

const AcString &RxEnvironmentVariable::name()const
{
	return this->m_name;
}
void RxEnvironmentVariable::setName(const AcString &_name)
{
	this->m_name = _name;
}

const AcString& RxEnvironmentVariable::variable()const
{
	return this->m_variable;
}
void RxEnvironmentVariable::setVariable(const AcString &_val)
{
	this->m_variable = _val;
}