#include "DbTextStyleTableRecord.h"
#include "DbImpl.h"



DbTextStyleTableRecord::DbTextStyleTableRecord()
{
	this->m_pImpl = new DbTextStyleTableRecordImpl();
}
DbTextStyleTableRecord::~DbTextStyleTableRecord()
{

}


Adesk::Boolean DbTextStyleTableRecord::isShapeFile() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->isShapeFile;
}
void DbTextStyleTableRecord::setIsShapeFile(Adesk::Boolean shape) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->isShapeFile = shape;
	}
}

Adesk::Boolean DbTextStyleTableRecord::isVertical() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->vertical;
}
void DbTextStyleTableRecord::setIsVertical(Adesk::Boolean vertical) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->vertical = vertical;
	}
}

double DbTextStyleTableRecord::textSize() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->textSize;
}
Acad::ErrorStatus DbTextStyleTableRecord::setTextSize(double size) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->textSize = size;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbTextStyleTableRecord::xScale() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->xScale;
}
Acad::ErrorStatus DbTextStyleTableRecord::setXScale(double xScale) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->xScale = xScale;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbTextStyleTableRecord::obliquingAngle() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->obliquingAngle;
}
Acad::ErrorStatus DbTextStyleTableRecord::setObliquingAngle(double obliquingAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->obliquingAngle = obliquingAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Adesk::UInt8 DbTextStyleTableRecord::flagBits() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->flagBits;
}
void DbTextStyleTableRecord::setFlagBits(Adesk::UInt8 flagBits) {
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->flagBits = flagBits;
}

double DbTextStyleTableRecord::priorSize() const {
	return DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->priorSize;
}
Acad::ErrorStatus DbTextStyleTableRecord::setPriorSize(double priorSize) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->priorSize = priorSize;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbTextStyleTableRecord::fileName(AcString& sFileName) const {
	sFileName = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->fileName.constPtr();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::fileName(ACHAR*& fileName) const {
	::acutNewString(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->fileName.constPtr(), fileName);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::fileName(const ACHAR*& fileName) const {
	fileName = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->fileName.constPtr();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::setFileName(const ACHAR* fileName) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->fileName = fileName;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbTextStyleTableRecord::bigFontFileName(AcString& sFileName) const {
	sFileName = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bigFontFileName;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::bigFontFileName(ACHAR*& fileName) const {
	::acutNewString(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bigFontFileName.constPtr(), fileName);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::bigFontFileName(const ACHAR*& fileName) const {
	fileName = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bigFontFileName.constPtr();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::setBigFontFileName(const ACHAR* fileName) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bigFontFileName = fileName;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
const emstring DbTextStyleTableRecord::emskd_fileName()const {
	AcString str;
	this->fileName(str);
	return str.constPtr();
}
const emstring DbTextStyleTableRecord::emskd_bigFontFileName()const {
	AcString str;
	this->bigFontFileName(str);
	return str.constPtr();
}
Acad::ErrorStatus DbTextStyleTableRecord::emskd_setFileName(const emstring& fileName) {
	return this->setFileName(fileName.c_str());
}
Acad::ErrorStatus DbTextStyleTableRecord::emskd_setBigFontFileName(const emstring& fileName) {
	return this->setBigFontFileName(fileName.c_str());
}
#endif



Acad::ErrorStatus DbTextStyleTableRecord::setFont(
	const ACHAR* pTypeface,
	Adesk::Boolean bold,
	Adesk::Boolean italic, Charset charset,
	FontUtils::FontPitch pitch,
	FontUtils::FontFamily family,
	bool bAllowMissingFont) {
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pTypeface = pTypeface;
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bold = bold;
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->italic = italic;
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->charset = charset;
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pitch = pitch;
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->family = family;
	DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bAllowMissingFont = bAllowMissingFont;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbTextStyleTableRecord::font(
	AcString& sTypeface,
	bool& bold,
	bool& italic,
	Charset& charset,
	FontUtils::FontPitch& pitch,
	FontUtils::FontFamily& family) const {
	sTypeface = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pTypeface;
	bold = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bold;
	italic = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->italic;
	charset = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->charset;
	pitch = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pitch;
	family = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->family;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::font(ACHAR*& pTypeface,
	Adesk::Boolean& bold,
	Adesk::Boolean& italic,
	Charset& charset,
	FontUtils::FontPitch& pitch,
	FontUtils::FontFamily& family) const {
	::acutNewString(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pTypeface, pTypeface);
	bold = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bold;
	italic = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->italic;
	charset = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->charset;
	pitch = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pitch;
	family = DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->family;
	return Acad::ErrorStatus::eOk;
}




Acad::ErrorStatus DbTextStyleTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	bool isShapeFile;
	pFiler->readBoolean(&isShapeFile);
	this->setIsShapeFile(isShapeFile);

	bool isVertical;
	pFiler->readBoolean(&isVertical);
	this->setIsVertical(isVertical);

	double textSize = 0.0;
	pFiler->readDouble(&textSize);
	this->setTextSize(textSize);

	double xScale = 0.0;
	pFiler->readDouble(&xScale);
	this->setXScale(xScale);

	double obliquingAngle = 0.0;
	pFiler->readDouble(&obliquingAngle);
	this->setObliquingAngle(obliquingAngle);

	char* pFileName = NULL;
	pFiler->readString(&pFileName);
	this->setFileName(pFileName);

	char* pBigFontFileName = NULL;
	pFiler->readString(&pBigFontFileName);
	this->setBigFontFileName(pBigFontFileName);

	char* pTypeface = NULL;
	pFiler->readString(&pTypeface);
	bool bold;
	pFiler->readBool(&bold);
	bool italic;
	pFiler->readBool(&italic);
	int charset;
	pFiler->readInt32(&charset);
	int pitch;
	pFiler->readInt32(&pitch);
	int family;
	pFiler->readInt32(&family);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeBoolean(this->isShapeFile());
	pFiler->writeBoolean(this->isVertical());
	pFiler->writeDouble(this->textSize());
	pFiler->writeDouble(this->xScale());
	pFiler->writeDouble(this->obliquingAngle());

	char* pFileName = NULL;
	this->fileName(pFileName);
	pFiler->writeString(pFileName);

	char* pBigFontFileName = NULL;
	this->bigFontFileName(pBigFontFileName);
	pFiler->writeString(pBigFontFileName);

	pFiler->writeString(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pTypeface);
	pFiler->writeBool(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->bold);
	pFiler->writeBool(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->italic);
	pFiler->writeInt32(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->charset);
	pFiler->writeInt32(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->pitch);
	pFiler->writeInt32(DB_IMP_TEXTSTYLETABLERECORD(this->m_pImpl)->family);

	return Acad::ErrorStatus::eOk;
}