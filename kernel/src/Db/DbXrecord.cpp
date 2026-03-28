#include "DbXrecord.h"
#include "DbDwgFiler.h"
#include "DbDxfFiler.h"
#include "DbImpl.h"

DbXrecord::DbXrecord() {
	this->m_pImpl = new DbXrecordImpl();
}
DbXrecord::~DbXrecord() {

}

Acad::ErrorStatus DbXrecord::rbChain(resbuf** ppRb, DbDatabase* auxDb) const {
	if (DB_IMP_XRECORD(this->m_pImpl)->resbufs.length() == 0) {
		*ppRb = NULL;
		return Acad::ErrorStatus::eOk;
	}
	acutRbDup(DB_IMP_XRECORD(this->m_pImpl)->resbufs[0], ppRb);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXrecord::setFromRbChain(const resbuf& pRb, DbDatabase* auxDb) {

	if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
	{
		return Acad::ErrorStatus::eFail;
	}

	for (int i = DB_IMP_XRECORD(this->m_pImpl)->resbufs.length() - 1; i >= 0; i--) {
		resbuf* p = DB_IMP_XRECORD(this->m_pImpl)->resbufs.at(i);
		acutRelRb(p);
	}
	DB_IMP_XRECORD(this->m_pImpl)->resbufs.removeAll();

	return this->appendRbChain(pRb);
}
Acad::ErrorStatus DbXrecord::appendRbChain(const resbuf& pRb) {

	if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
	{
		return Acad::ErrorStatus::eFail;
	}

	resbuf* result = NULL;
	acutRbDup(&pRb, &result);
	DB_IMP_XRECORD(this->m_pImpl)->resbufs.append(result);

	return Acad::ErrorStatus::eOk;
}
bool DbXrecord::isXlateReferences() const {
	return DB_IMP_XRECORD(this->m_pImpl)->isXlateReferences;
}
void DbXrecord::setXlateReferences(bool translate) {
	DB_IMP_XRECORD(this->m_pImpl)->isXlateReferences = translate;
}

Db::DuplicateRecordCloning DbXrecord::mergeStyle()const {
	return DB_IMP_XRECORD(this->m_pImpl)->mergeStyle;
}
void DbXrecord::setMergeStyle(Db::DuplicateRecordCloning style){
	DB_IMP_XRECORD(this->m_pImpl)->mergeStyle = style;
}

Acad::ErrorStatus DbXrecord::dwgInFields(DbDwgFiler* /*pFiler*/) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbXrecord::dwgOutFields(DbDwgFiler* /*pFiler*/) const {
	return Acad::ErrorStatus::eOk;
}

