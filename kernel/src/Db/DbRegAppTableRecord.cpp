#include "DbRegAppTableRecord.h"
#include "DbImpl.h"



DbRegAppTableRecord::DbRegAppTableRecord()
{
	this->m_pImpl = new DbRegAppTableRecordImpl();
}
DbRegAppTableRecord::~DbRegAppTableRecord()
{

}




Acad::ErrorStatus DbRegAppTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRegAppTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}