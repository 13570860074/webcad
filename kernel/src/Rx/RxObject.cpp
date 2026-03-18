#include "RxObject.h"
#include "kernel.h"



RxObject::RxObject() {
	this->m_pImpl = NULL;
}
RxObject::~RxObject() {
	if (this->m_pImpl != NULL) {
		delete this->m_pImpl;
	}
}

RxClass* RxObject::isA() const {
	return NULL;
}
RxObject* RxObject::clone() const {
	return NULL;
}
Acad::ErrorStatus RxObject::dwgIn(DbDwgFiler* pFiler) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus RxObject::dwgInFields(DbDwgFiler* pFiler) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus RxObject::dwgOut(DbDwgFiler* pFiler) const {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus RxObject::dwgOutFields(DbDwgFiler* pFiler) const {
	return Acad::ErrorStatus::eOk;
}

