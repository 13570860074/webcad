#include "DbSpline.h"



DbSpline::DbSpline() {

}
DbSpline::~DbSpline() {

}





Acad::ErrorStatus DbSpline::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbCurve::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbSpline::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbCurve::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
bool DbSpline::subWorldDraw(GiWorldDraw* pWd) const {
    return false;
}

