#include "DbBlockEnd.h"



DbBlockEnd::DbBlockEnd() {

}
DbBlockEnd::~DbBlockEnd() {

}


Acad::ErrorStatus DbBlockEnd::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbBlockEnd::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
bool DbBlockEnd::subWorldDraw(GiWorldDraw* pWd) const {
    return false;
}

