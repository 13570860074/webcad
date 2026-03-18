#include "DbBlockBegin.h"



DbBlockBegin::DbBlockBegin() {

}
DbBlockBegin::~DbBlockBegin() {

}



Acad::ErrorStatus DbBlockBegin::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockBegin::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
bool DbBlockBegin::subWorldDraw(GiWorldDraw* pWd) const {
    return false;
}

