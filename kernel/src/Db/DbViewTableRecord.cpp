#include "DbViewTableRecord.h"
#include "DbImpl.h"



DbViewTableRecord::DbViewTableRecord()
{
	this->m_pImpl = new DbViewTableRecordImpl();
}
DbViewTableRecord::~DbViewTableRecord()
{

}


void DbViewTableRecord::close() {

}







Acad::ErrorStatus DbViewTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
