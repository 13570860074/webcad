#include "GiTextStyle.h"
#include "GePoint2d.h"
#include "DbDatabase.h"
#include "GiWorldDraw.h"
#include "DbObjectManager.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "GiImpl.h"


GiTextStyle::GiTextStyle(DbDatabase* pDb) {
	this->m_pImpl = new GiTextStyleImpl();
	if (pDb == NULL) {
		pDb = acdbHostApplicationServices()->workingDatabase();
	}

	DbObjectId textstyleId = pDb->textstyle();
	DbTextStyleTableRecord *pTextStyle = (DbTextStyleTableRecord*)::kernel()->acdbObjectManager()->openDbObject(textstyleId);
	if (pTextStyle != NULL) {
		AcString fontName;
		pTextStyle->fileName(fontName);
		this->setFileName(fontName.kACharPtr());

		AcString bigFontFileName;
		pTextStyle->bigFontFileName(bigFontFileName);
		this->setBigFontFileName(bigFontFileName.kACharPtr());
	}
}
GiTextStyle::~GiTextStyle() {

}


GiTextStyle::GiTextStyle(
	const ACHAR* fontName,
	const ACHAR* bigFontName,
	const double textSize,
	const double xScale,
	const double obliqueAngle,
	const double trPercent,

	const Adesk::Boolean isBackward,
	const Adesk::Boolean isUpsideDown,
	const Adesk::Boolean isVertical,

	const Adesk::Boolean isOverlined,
	const Adesk::Boolean isUnderlined,
	const Adesk::Boolean isStrikethrough,

	const ACHAR* styleName) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->bigFontName = bigFontName;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->fontName = fontName;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->textSize = textSize;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->xScale = xScale;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->obliqueAngle = obliqueAngle;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->trPercent = trPercent;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isBackward = isBackward;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isUpsideDown = isUpsideDown;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isVertical = isVertical;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isOverlined = isOverlined;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isUnderlined = isUnderlined;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isStrikethrough = isStrikethrough;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->styleName = styleName;
}

// Unicode: change from char to int, since in the implementation, the returned
// result is actually bitmask indicating some status result.
int GiTextStyle::loadStyleRec(DbDatabase* pDb) const {
	return 0;
}
void GiTextStyle::setTextSize(const double size) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->textSize = size;
}
void GiTextStyle::setXScale(const double xScale) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->xScale = xScale;
}
void GiTextStyle::setObliquingAngle(const double obliquingAngle) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->obliqueAngle = obliquingAngle;
}

void GiTextStyle::setTrackingPercent(const double trPercent) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->trPercent = trPercent;
}

void GiTextStyle::setBackward(const Adesk::Boolean isBackward) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isBackward = isBackward;
}
void GiTextStyle::setUpsideDown(const Adesk::Boolean isUpsideDown) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isUpsideDown = isUpsideDown;
}
void GiTextStyle::setVertical(const Adesk::Boolean isVertical) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isVertical = isVertical;
}

void GiTextStyle::setUnderlined(const Adesk::Boolean isUnderlined) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isUnderlined = isUnderlined;
}
void GiTextStyle::setOverlined(const Adesk::Boolean isOverlined) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isOverlined = isOverlined;
}
void GiTextStyle::setStrikethrough(const Adesk::Boolean isStrikethrough) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->isStrikethrough = isStrikethrough;
}

void GiTextStyle::setFileName(const ACHAR* fontName) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->fontName = fontName;
}
void GiTextStyle::setBigFontFileName(const ACHAR* bigFontFileName) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->bigFontName = bigFontFileName;
}
double GiTextStyle::textSize() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->textSize;
}
double GiTextStyle::xScale() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->xScale;
}
double GiTextStyle::obliquingAngle() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->obliqueAngle;
}

double GiTextStyle::trackingPercent() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->trPercent;
}

Adesk::Boolean GiTextStyle::isBackward() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->isBackward;
}
Adesk::Boolean GiTextStyle::isUpsideDown() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->isUpsideDown;
}
Adesk::Boolean GiTextStyle::isVertical() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->isVertical;
}

Adesk::Boolean GiTextStyle::isUnderlined() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->isUnderlined;
}
Adesk::Boolean GiTextStyle::isOverlined() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->isOverlined;
}
Adesk::Boolean GiTextStyle::isStrikethrough() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->isStrikethrough;
}

const ACHAR* GiTextStyle::fileName() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->fontName.constPtr();
}
const ACHAR* GiTextStyle::bigFontFileName() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->bigFontName.constPtr();
}

GePoint2d GiTextStyle::extents(const ACHAR* pStr,
	const Adesk::Boolean penups,
	const int len,
	const Adesk::Boolean raw,
	GiWorldDraw* ctxt) const {
	return GePoint2d::kOrigin;
}

const ACHAR* GiTextStyle::styleName() const {
	return GI_IMP_TEXTSTYLE(this->m_pImpl)->styleName.constPtr();
}
Acad::ErrorStatus GiTextStyle::setStyleName(const ACHAR* styleName) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->styleName = styleName;
	return Acad::eOk;
}


Acad::ErrorStatus GiTextStyle::setFont(
	const ACHAR* pTypeface,
	Adesk::Boolean bold,
	Adesk::Boolean italic, Charset charset,
	FontUtils::FontPitch pitch,
	FontUtils::FontFamily family) {
	GI_IMP_TEXTSTYLE(this->m_pImpl)->pTypeface = pTypeface;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->bold = bold;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->italic = italic;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->charset = charset;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->pitch = pitch;
	GI_IMP_TEXTSTYLE(this->m_pImpl)->family = family;
	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus GiTextStyle::font(
	AcString& sTypeface,
	bool& bold,
	bool& italic,
	Charset& charset,
	FontUtils::FontPitch& pitch,
	FontUtils::FontFamily& family) const {
	sTypeface = GI_IMP_TEXTSTYLE(this->m_pImpl)->pTypeface;
	bold = GI_IMP_TEXTSTYLE(this->m_pImpl)->bold;
	italic = GI_IMP_TEXTSTYLE(this->m_pImpl)->italic;
	charset = GI_IMP_TEXTSTYLE(this->m_pImpl)->charset;
	pitch = GI_IMP_TEXTSTYLE(this->m_pImpl)->pitch;
	family = GI_IMP_TEXTSTYLE(this->m_pImpl)->family;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus GiTextStyle::font(ACHAR*& pTypeface,
	Adesk::Boolean& bold,
	Adesk::Boolean& italic,
	Charset& charset,
	FontUtils::FontPitch& pitch,
	FontUtils::FontFamily& family) const {
	::acutNewString(GI_IMP_TEXTSTYLE(this->m_pImpl)->pTypeface, pTypeface);
	bold = GI_IMP_TEXTSTYLE(this->m_pImpl)->bold;
	italic = GI_IMP_TEXTSTYLE(this->m_pImpl)->italic;
	charset = GI_IMP_TEXTSTYLE(this->m_pImpl)->charset;
	pitch = GI_IMP_TEXTSTYLE(this->m_pImpl)->pitch;
	family = GI_IMP_TEXTSTYLE(this->m_pImpl)->family;
	return Acad::ErrorStatus::eOk;
}


Acad::ErrorStatus  GiTextStyle::extentsBox(const ACHAR* pStr,
	const Adesk::Boolean penups,
	const int len,
	const Adesk::Boolean raw,
	GePoint2d& extMin,
	GePoint2d& extMax,
	GiWorldDraw* ctxt) const {
	return Acad::ErrorStatus::eOk;
}

void GiTextStyle::setTrackKerning(double trackPercent) const {

}

bool GiTextStyle::preLoaded() const {
	return false;
}
void GiTextStyle::setPreLoaded(bool) {

}



