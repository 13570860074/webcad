#include "DbPlotSettings.h"



DbPlotSettings::DbPlotSettings() {

}
DbPlotSettings::~DbPlotSettings() {

}



Acad::ErrorStatus DbPlotSettings::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbPlotSettings::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
