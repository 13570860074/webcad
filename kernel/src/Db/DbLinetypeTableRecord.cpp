#include "DbLinetypeTableRecord.h"
#include "DbLinetypeDashe.h"
#include "DbImpl.h"



DbLinetypeTableRecord::DbLinetypeTableRecord()
{
	this->m_pImpl = new DbLinetypeTableRecordImpl();
}
DbLinetypeTableRecord::~DbLinetypeTableRecord()
{

}



Acad::ErrorStatus DbLinetypeTableRecord::comments(char*& pString) const {
    ::acutNewString(DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->comments, pString);
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLinetypeTableRecord::comments(const char*& pString) const {
    pString = DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->comments;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLinetypeTableRecord::comments(AcString& sComments) const {
    sComments = DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->comments;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLinetypeTableRecord::setComments(const char* pString) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->comments = pString;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}


double DbLinetypeTableRecord::patternLength() const {
    return DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->patternLength;
}
Acad::ErrorStatus DbLinetypeTableRecord::setPatternLength(double patternLength) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->patternLength = patternLength;
    }
    return Acad::ErrorStatus::eOk;
}
int DbLinetypeTableRecord::numDashes() const {
    return DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length();
}
Acad::ErrorStatus DbLinetypeTableRecord::setNumDashes(int count) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (count < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
			for (int i = DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length() - 1; i >= count; i--) {
				realloc_arry_at(DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe, i);
			}
		}
		else if (count == DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length() + 1) {
			DbLinetypeDashe* pLinetypeDashe = new DbLinetypeDashe();
			DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.append(pLinetypeDashe);
		}
		else if(count > DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()){
			for (int i = DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length(); i < count; i++) {
				DbLinetypeDashe* pLinetypeDashe = new DbLinetypeDashe();
				DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.append(pLinetypeDashe);
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


double DbLinetypeTableRecord::dashLengthAt(int index) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		return DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->dashLength();
	}
	return 0.0;
}
Acad::ErrorStatus DbLinetypeTableRecord::setDashLengthAt(int index, double value) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
			DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->setDashLength(value);
		}
	}
	return Acad::ErrorStatus::eFail;
}

DbObjectId DbLinetypeTableRecord::shapeStyleAt(int index) const {
	return NULL;
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeStyleAt(int index, DbObjectId id) {
	return Acad::ErrorStatus::eFail;
}

int DbLinetypeTableRecord::shapeNumberAt(int index) const {
	return 0;
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeNumberAt(int index, int shapeNumber) {
	return Acad::ErrorStatus::eOk;
}

GeVector2d DbLinetypeTableRecord::shapeOffsetAt(int index) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		return DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->shapeOffset();
	}
	return GeVector2d(0, 0);
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeOffsetAt(int index, const GeVector2d& offset) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
			DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->setShapeOffset(offset);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbLinetypeTableRecord::shapeScaleAt(int index) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		return DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->shapeScale();
	}
	return 0.0;
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeScaleAt(int index, double scale) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
			DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->setShapeScale(scale);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbLinetypeTableRecord::isScaledToFit() const {
	return false;
}
void DbLinetypeTableRecord::setIsScaledToFit(bool scaledToFit) {

}

bool DbLinetypeTableRecord::shapeIsUcsOrientedAt(int index) const {
	return false;
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeIsUcsOrientedAt(int index, bool isUcsOriented) {
	return Acad::ErrorStatus::eFail;
}

bool DbLinetypeTableRecord::shapeIsUprightAt(int index) const {
	return false;
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeIsUprightAt(int index, bool isUpright) {
	return Acad::ErrorStatus::eFail;
}

double DbLinetypeTableRecord::shapeRotationAt(int index) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		return DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->shapeRotation();
	}
	return 0.0;
}
Acad::ErrorStatus DbLinetypeTableRecord::setShapeRotationAt(int index, double rotation) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
			DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->setShapeRotation(rotation);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbLinetypeTableRecord::textAt(int index, AcString& sText) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		sText = DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->text();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLinetypeTableRecord::textAt(int index, char*& text) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		::acutNewString(DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->text(), text);
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLinetypeTableRecord::textAt(int index, const char*& text) const {
	if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
		text = DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->text();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbLinetypeTableRecord::setTextAt(int index, const char* text) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (index < DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.length()) {
			DB_IMP_LINETYPETABLERECORD(this->m_pImpl)->linetypeDashe.at(index)->setText(text);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
const emstring DbLinetypeTableRecord::emsdk_comments()const {
	AcString str;
	this->comments(str);
	return str.constPtr();
}
Acad::ErrorStatus DbLinetypeTableRecord::emsdk_setComments(const emstring& pString) {
	return this->setComments(pString.c_str());
}
const emstring DbLinetypeTableRecord::emsdk_asciiDescription()const {
	ACHAR* sOutput;
	this->asciiDescription(sOutput);
	emstring val = sOutput;
	delete sOutput;
	return val;
}
Acad::ErrorStatus DbLinetypeTableRecord::emsdk_setAsciiDescription(const emstring& pString) {
	return this->setAsciiDescription(pString.c_str());
}
const emstring DbLinetypeTableRecord::emsdk_textAt(int index)const {
	ACHAR* sOutput;
	this->textAt(index, sOutput);
	emstring val = sOutput;
	delete sOutput;
	return val;
}
Acad::ErrorStatus DbLinetypeTableRecord::emsdk_setTextAt(int index, const emstring& text) {
	return this->setTextAt(index, text.c_str());
}
#endif





Acad::ErrorStatus DbLinetypeTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	char *comments;
	pFiler->readString(&comments);
	this->setComments(comments);

	double patternLength;
	pFiler->readDouble(&patternLength);
	this->setPatternLength(patternLength);

	int numDashes;
	pFiler->readInt32(&numDashes);
	this->setNumDashes(numDashes);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLinetypeTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	char* pComments = NULL;
	this->comments(pComments);
	pFiler->writeString(pComments);

	pFiler->writeDouble(this->patternLength());

	pFiler->writeInt32(this->numDashes());

	return Acad::ErrorStatus::eOk;
}