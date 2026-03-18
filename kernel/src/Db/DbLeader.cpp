#include "DbLeader.h"



DbLeader::DbLeader() {

}
DbLeader::~DbLeader() {

}





Acad::ErrorStatus DbLeader::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLeader::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
bool DbLeader::subWorldDraw(GiWorldDraw* pWd) const {
    return false;
}

