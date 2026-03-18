#include "RxSystemVariable.h"
#include "arxHeaders.h"

RxSystemVariable::RxSystemVariable()
{
	this->m_onGet = NULL;
	this->m_onSet = NULL;
	this->m_variable = NULL;
}
RxSystemVariable::~RxSystemVariable()
{
}

const AcString &RxSystemVariable::name()
{
	return this->m_name;
}
void RxSystemVariable::setName(const AcString &_name)
{
	this->m_name = _name;
}

Acad::ErrorStatus RxSystemVariable::variable(resbuf*& _val)
{
	if (this->m_onGet != NULL)
	{
		void(*fun)(resbuf*&) = (void(*)(resbuf*&))this->m_onGet;
		fun(_val);
	}
	else {
		if (this->m_variable != NULL) {
			acutRbDup(this->m_variable, &_val);
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus RxSystemVariable::setVariable(const resbuf *_val)
{
	if (this->m_onSet != NULL) {
		Acad::ErrorStatus(*fun)(const resbuf*) = (Acad::ErrorStatus(*)(const resbuf*))this->m_onSet;
		return fun(_val);
	}
	else {
		if(this->m_variable != NULL){
			acutRelRb(this->m_variable);
		}
		this->m_variable = new resbuf();
		acutRbDup(_val, &this->m_variable);
	}
	return Acad::eOk;
}

void RxSystemVariable::on_get(void(*fun)(resbuf*&)) {
	this->m_onGet = (void*)fun;
}
void RxSystemVariable::on_set(Acad::ErrorStatus(*fun)(const resbuf*)) {
	this->m_onSet = (void*)fun;
}