#include "DbLinetypeDashe.h"
#include "AcString.h"
#include "DbImpl.h"

DbLinetypeDashe::DbLinetypeDashe() {
	this->m_pImpl = new DbLinetypeDasheImpl();
}
DbLinetypeDashe::~DbLinetypeDashe() {

}

AcString DbLinetypeDashe::text() {
	return DB_IMP_LINETYPEDASHE(this->m_pImpl)->text;
}
double DbLinetypeDashe::dashLength() {
	return DB_IMP_LINETYPEDASHE(this->m_pImpl)->dashLength;
}
GeVector2d DbLinetypeDashe::shapeOffset() const {
	return DB_IMP_LINETYPEDASHE(this->m_pImpl)->shapeOffset;
}
double DbLinetypeDashe::shapeScale() {
	return DB_IMP_LINETYPEDASHE(this->m_pImpl)->shapeScale;
}
double DbLinetypeDashe::shapeRotation() {
	return DB_IMP_LINETYPEDASHE(this->m_pImpl)->shapeRotation;
}

void DbLinetypeDashe::setText(const AcString& _text) {
	DB_IMP_LINETYPEDASHE(this->m_pImpl)->text = _text;
}
void DbLinetypeDashe::setDashLength(double _v) {
	DB_IMP_LINETYPEDASHE(this->m_pImpl)->dashLength = _v;
}
void DbLinetypeDashe::setShapeOffset(const GeVector2d& point) {
	DB_IMP_LINETYPEDASHE(this->m_pImpl)->shapeOffset = point;
}
void DbLinetypeDashe::setShapeScale(double _v) {
	DB_IMP_LINETYPEDASHE(this->m_pImpl)->shapeScale = _v;
}
void DbLinetypeDashe::setShapeRotation(double _v) {
	DB_IMP_LINETYPEDASHE(this->m_pImpl)->shapeRotation = _v;
}