#include "DbUCSTableRecord.h"
#include "DbDwgFiler.h"
#include "DbImpl.h"



DbUCSTableRecord::DbUCSTableRecord()
{
	this->m_pImpl = new DbUCSTableRecordImpl();
}
DbUCSTableRecord::~DbUCSTableRecord()
{

}


GePoint3d DbUCSTableRecord::origin() const {
	return DB_IMP_UCSTABLERECORD(this->m_pImpl)->origin;
}
void DbUCSTableRecord::setOrigin(const GePoint3d& newOrigin) {
	DB_IMP_UCSTABLERECORD(this->m_pImpl)->origin = newOrigin;
}

GeVector3d DbUCSTableRecord::xAxis() const {
	return DB_IMP_UCSTABLERECORD(this->m_pImpl)->xAxis;
}
void DbUCSTableRecord::setXAxis(const GeVector3d& xAxis) {
	DB_IMP_UCSTABLERECORD(this->m_pImpl)->xAxis = xAxis;
}

GeVector3d DbUCSTableRecord::yAxis() const {
	return DB_IMP_UCSTABLERECORD(this->m_pImpl)->yAxis;
}
void DbUCSTableRecord::setYAxis(const GeVector3d& yAxis) {
	DB_IMP_UCSTABLERECORD(this->m_pImpl)->yAxis = yAxis;
}

GePoint3d DbUCSTableRecord::ucsBaseOrigin(Db::OrthographicView view) const {
	return DB_IMP_UCSTABLERECORD(this->m_pImpl)->origin;
}
Acad::ErrorStatus DbUCSTableRecord::setUcsBaseOrigin(const GePoint3d& origin, Db::OrthographicView view) {
	DB_IMP_UCSTABLERECORD(this->m_pImpl)->origin = origin;
	return Acad::ErrorStatus::eOk;
}





Acad::ErrorStatus DbUCSTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	GePoint3d origin;
	pFiler->readPoint3d(&origin);
	this->setOrigin(origin);

	GeVector3d xAxis;
	pFiler->readVector3d(&xAxis);
	this->setXAxis(xAxis);

	GeVector3d yAxis;
	pFiler->readVector3d(&yAxis);
	this->setYAxis(yAxis);


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbUCSTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->origin());
	pFiler->writeVector3d(this->xAxis());
	pFiler->writeVector3d(this->yAxis());

	return Acad::ErrorStatus::eOk;
}