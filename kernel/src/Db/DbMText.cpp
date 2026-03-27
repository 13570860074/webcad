#include "DbMText.h"
#include "GePlane.h"
#include "DbTextStyleTable.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "kernel.h"
#include "DbObjectManager.h"
#include "DbExtents.h"
#include "DbGripData.h"
#include "DbImpl.h"
#include <cmath>


DbMText::DbMText() {
	this->m_pImpl = new DbMTextImpl();
}
DbMText::~DbMText() {

}





Acad::ErrorStatus DbMText::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	GePoint3d location;
	pFiler->readPoint3d(&location);
	this->setLocation(location);

	GeVector3d normal;
	pFiler->readVector3d(&normal);
	this->setNormal(normal);

	GeVector3d direction;
	pFiler->readVector3d(&direction);
	this->setDirection(direction);

	double rotation;
	pFiler->readDouble(&rotation);
	this->setRotation(rotation);

	double width;
	pFiler->readDouble(&width);
	this->setWidth(width);

	double height;
	pFiler->readDouble(&height);
	this->setHeight(height);

	double textHeight;
	pFiler->readDouble(&textHeight);
	this->setTextHeight(textHeight);

	short attachment;
	pFiler->readInt16(&attachment);
	this->setAttachment((DbMText::AttachmentPoint)attachment);

	short flowDirection;
	pFiler->readInt16(&flowDirection);
	this->setFlowDirection((DbMText::FlowDirection)flowDirection);

	AcString contents;
	pFiler->readString(contents);
	this->setContents(contents.constPtr());

	AcString contentsRTF;
	pFiler->readString(contentsRTF);
	this->setContentsRTF(contentsRTF.constPtr());

	DbHandle handle;
	pFiler->readDbHandle(&handle);
	DbObjectId textStyleId = NULL;
	this->database()->getDbObjectId(textStyleId, false, handle);
	DB_IMP_MTEXT(this->m_pImpl)->textStyleId.setFromOldId(textStyleId.asOldId());

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writePoint3d(this->location());
	pFiler->writeVector3d(this->normal());
	pFiler->writeVector3d(this->direction());
	pFiler->writeDouble(this->rotation());
	pFiler->writeDouble(this->width());
	pFiler->writeDouble(this->height());
	pFiler->writeDouble(this->textHeight());
	pFiler->writeInt16(this->attachment());
	pFiler->writeInt16(this->flowDirection());
	pFiler->writeString(this->contents());
	pFiler->writeString(this->contentsRTF());
	pFiler->writeDbHandle(DB_IMP_MTEXT(this->m_pImpl)->textStyleId.handle());

	return Acad::ErrorStatus::eOk;
}
bool DbMText::subWorldDraw(GiWorldDraw* pWd) const {

	/* 求方向 */
	GeVector3d direction = this->direction();
	if (abs(this->rotation()) > 0.000001) {
		direction.rotateBy(this->rotation(), this->normal());
	}

	/* 文字样式 */
	GiTextStyle textStyle;

	/* 获得字体 */
	AcString fontName;
	AcString bigFontFileName;
	AcString pTypeface;
	Adesk::Boolean bold = false;
	Adesk::Boolean italic = false;
	Charset charset = Charset::kChineseSimpCharset;
	FontUtils::FontPitch pitch = FontUtils::FontPitch::kVariable;
	FontUtils::FontFamily family = FontUtils::FontFamily::kDecorative;
	if (this->database() != NULL) {
		DbObjectId textstyleId = this->textStyle();
		if (textstyleId == NULL) {
			textstyleId = this->database()->textstyle();
		}
		DbTextStyleTableRecord* pTextStyle = (DbTextStyleTableRecord*)::kernel()->acdbObjectManager()->openDbObject(textstyleId);
		if (pTextStyle != NULL) {
			pTextStyle->fileName(fontName);
			pTextStyle->bigFontFileName(bigFontFileName);
			pTextStyle->font(pTypeface, bold, italic, charset, pitch, family);
		}
	}
	textStyle.setTextSize(2.5);
	textStyle.setFileName(fontName);
	textStyle.setBigFontFileName(bigFontFileName);
	textStyle.setFont(pTypeface, bold, italic, charset, pitch, family);
	pWd->geometry().text(
		this->location(),
		this->normal(),
		direction,
		this->contents(),
		strlen(this->contents()),
		false,
		textStyle);
	return true;
}





//  Insertion point
//
GePoint3d DbMText::location() const {
	return DB_IMP_MTEXT(this->m_pImpl)->location;
}
Acad::ErrorStatus DbMText::setLocation(const GePoint3d& _location) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->location = _location;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

//  Normal vector
// 
GeVector3d DbMText::normal() const {
	return DB_IMP_MTEXT(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbMText::setNormal(const GeVector3d& _normal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->normal = _normal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbMText::getPlane(GePlane& plane, Db::Planarity& type) const {
	if (this->normal().length() > GeContext::gTol.equalVector()) {
		plane.set(this->location(), this->normal());
		type = Db::Planarity::kPlanar;
		return Acad::ErrorStatus::eOk;
	}

	type = Db::Planarity::kNonPlanar;
	return Acad::ErrorStatus::eFail;
}
GeVector3d DbMText::direction() const {
	return DB_IMP_MTEXT(this->m_pImpl)->direction;
}
Acad::ErrorStatus DbMText::setDirection(const GeVector3d& _direction) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->direction = _direction;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbMText::rotation() const {
	return DB_IMP_MTEXT(this->m_pImpl)->rotation;
}
Acad::ErrorStatus DbMText::setRotation(double _rotation) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->rotation = _rotation;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbMText::width() const {

	double width = 0;
	for (int i = 0; i < this->columnCount(); i++) {
		width += this->columnWidth();
		if (i < this->columnCount() - 1) {
			width += this->columnGutterWidth();
		}
	}
	return width;
}
Acad::ErrorStatus DbMText::setWidth(double _width) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		DB_IMP_MTEXT(this->m_pImpl)->columnWidth = _width;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbMText::ascent() const {
	return 0.0;
}
double DbMText::descent() const {
	return 0.0;
}
//
DbObjectId DbMText::textStyle() const {
	return DB_IMP_MTEXT(this->m_pImpl)->textStyleId;
}
Acad::ErrorStatus DbMText::setTextStyle(DbObjectId _textStyleId) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		if (this->database() == NULL) {
			DB_IMP_MTEXT(this->m_pImpl)->textStyleId = _textStyleId;
		}
		else {
			DbTextStyleTableRecord* pTextStyleTableRecord = NULL;
			if (::acdbOpenObject(pTextStyleTableRecord, _textStyleId) == Acad::ErrorStatus::eOk) {
				if (pTextStyleTableRecord->database() == this->database()) {
					DB_IMP_MTEXT(this->m_pImpl)->textStyleId = _textStyleId;
				}
				pTextStyleTableRecord->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
//
double DbMText::textHeight() const {
	return DB_IMP_MTEXT(this->m_pImpl)->textHeight;
}
Acad::ErrorStatus DbMText::setTextHeight(double _textHeight) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->textHeight = _textHeight;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


DbMText::AttachmentPoint DbMText::attachment() const {
	return DB_IMP_MTEXT(this->m_pImpl)->attachment;
}
Acad::ErrorStatus DbMText::setAttachment(AttachmentPoint _attachment) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->attachment = _attachment;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

DbMText::FlowDirection DbMText::flowDirection() const {
	return DB_IMP_MTEXT(this->m_pImpl)->flowDirection;
}
Acad::ErrorStatus DbMText::setFlowDirection(DbMText::FlowDirection _flowDirection) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->flowDirection = _flowDirection;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::contents(AcString& sContents) const {
	sContents = DB_IMP_MTEXT(this->m_pImpl)->contents;
	return Acad::ErrorStatus::eOk;
}
ACHAR* DbMText::contents() const {
	char* str = NULL;
	::acutNewString(DB_IMP_MTEXT(this->m_pImpl)->contents.constPtr(), str);
	return str;
}
int DbMText::setContents(const ACHAR* sText) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->contents = sText;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbMText::contentsRTF(AcString& sContentsRTF) const {
	sContentsRTF = DB_IMP_MTEXT(this->m_pImpl)->contents;
	return Acad::ErrorStatus::eOk;
}
ACHAR* DbMText::contentsRTF() const {
	char* str = NULL;
	::acutNewString(DB_IMP_MTEXT(this->m_pImpl)->contents.constPtr(), str);
	return str;
}

int DbMText::setContentsRTF(const ACHAR* RTFString) {
	RTFString = DB_IMP_MTEXT(this->m_pImpl)->contents.constPtr();
	return 0;
}

Acad::ErrorStatus DbMText::text(AcString& sText) const {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->contents = sText;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
ACHAR* DbMText::text() const {
	char *str = NULL;
	::acutNewString(DB_IMP_MTEXT(this->m_pImpl)->contents.constPtr(), str);
	return str;
}

//  Return points marking text box
//
void DbMText::getBoundingPoints(GePoint3dArray&) const {

}


//  These strings can be used instead of sprinkling your code 
//  with string constants.
const ACHAR* const DbMText::nonBreakSpace() {
	return NULL;
}
const ACHAR* const DbMText::overlineOn() {
	return NULL;
}
const ACHAR* const DbMText::overlineOff() {
	return NULL;
}
const ACHAR* const DbMText::underlineOn() {
	return NULL;
}
const ACHAR* const DbMText::underlineOff() {
	return NULL;
}
const ACHAR* const DbMText::colorChange() {
	return NULL;
}
const ACHAR* const DbMText::fontChange() {
	return NULL;
}
const ACHAR* const DbMText::heightChange() {
	return NULL;
}
const ACHAR* const DbMText::widthChange() {
	return NULL;
}
const ACHAR* const DbMText::obliqueChange() {
	return NULL;
}
const ACHAR* const DbMText::trackChange() {
	return NULL;
}
const ACHAR* const DbMText::lineBreak() {
	return NULL;
}
const ACHAR* const DbMText::paragraphBreak() {
	return NULL;
}
const ACHAR* const DbMText::stackStart() {
	return NULL;
}
const ACHAR* const DbMText::alignChange() {
	return NULL;
}
const ACHAR* const DbMText::blockBegin() {
	return NULL;
}
const ACHAR* const DbMText::blockEnd() {
	return NULL;
}
const ACHAR* const DbMText::strikethroughOn() {
	return NULL;
}
const ACHAR* const DbMText::strikethroughOff() {
	return NULL;
}

Acad::ErrorStatus DbMText::setLineSpacingStyle(Db::LineSpacingStyle eStyle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->lineSpacingStyle = eStyle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Db::LineSpacingStyle DbMText::lineSpacingStyle() const {
	return DB_IMP_MTEXT(this->m_pImpl)->lineSpacingStyle;
}
Acad::ErrorStatus DbMText::setLineSpacingFactor(double dFactor) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->lineSpacingFactor = dFactor;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
double DbMText::lineSpacingFactor() const {
	return DB_IMP_MTEXT(this->m_pImpl)->lineSpacingFactor;
}


bool DbMText::showBorders() const {
	return DB_IMP_MTEXT(this->m_pImpl)->showBorders;
}


Acad::ErrorStatus DbMText::setShowBorders(bool enable) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->showBorders = enable;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbMText::backgroundFillOn() const {
	return DB_IMP_MTEXT(this->m_pImpl)->backgroundFill;
}
Acad::ErrorStatus DbMText::setBackgroundFill(bool enable) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->backgroundFill = enable;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbMText::getBackgroundFillColor(CmColor& color) const {
	color = DB_IMP_MTEXT(this->m_pImpl)->backgroundFillColor;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setBackgroundFillColor(const CmColor& color) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->backgroundFillColor = color;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbMText::getBackgroundScaleFactor(double& scale) const {
	scale = DB_IMP_MTEXT(this->m_pImpl)->backgroundScaleFactor;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setBackgroundScaleFactor(const double scale) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->backgroundScaleFactor = scale;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbMText::getBackgroundTransparency(CmTransparency& transp) const {
	transp = DB_IMP_MTEXT(this->m_pImpl)->backgroundTransparency;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setBackgroundTransparency(const CmTransparency& transp) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->backgroundTransparency = transp;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbMText::useBackgroundColorOn() const {
	return DB_IMP_MTEXT(this->m_pImpl)->useBackgroundColor;
}
Acad::ErrorStatus DbMText::setUseBackgroundColor(bool enable) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->useBackgroundColor = enable;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

DbMText::ColumnType DbMText::columnType() const {
	DbMText::ColumnType type;
	this->getColumnType(type);
	return type;
}
bool DbMText::columnAutoHeight() const {
	bool val = false;
	this->getColumnAutoHeight(val);
	return val;
}
int DbMText::columnCount() const {
	int val = 0;
	this->getColumnCount(val);
	return val;
}
double DbMText::columnWidth() const {
	double val = 0.0;
	this->getColumnWidth(val);
	return val;
}
double DbMText::columnGutterWidth() const {
	double val = 0.0;
	this->getColumnGutterWidth(val);
	return val;
}
bool DbMText::columnFlowReversed() const {
	bool val = false;
	this->getColumnFlowReversed(val);
	return val;
}
double DbMText::columnHeight(int _index) const {
	double val = 0.0;
	this->getColumnHeight(_index, val);
	return val;
}
Acad::ErrorStatus DbMText::getColumnType(DbMText::ColumnType& _columnType) const {
	_columnType = DB_IMP_MTEXT(this->m_pImpl)->columnType;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setColumnType(ColumnType _columnType) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->columnType = _columnType;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::getColumnAutoHeight(bool& _columnAutoHeight) const {
	_columnAutoHeight = DB_IMP_MTEXT(this->m_pImpl)->columnAutoHeight;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setColumnAutoHeight(bool _columnAutoHeight) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->columnAutoHeight = _columnAutoHeight;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::getColumnCount(int& _columnCount) const {
	_columnCount = DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length();
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setColumnCount(int _columnCount) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length() < _columnCount) {
			for (int i = DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length(); i < _columnCount; i++) {
				DB_IMP_MTEXT(this->m_pImpl)->columnHeights.append(0);
			}
		}
		else if (DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length() > _columnCount) {
			DB_IMP_MTEXT(this->m_pImpl)->columnHeights.splice(_columnCount);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::getColumnWidth(double& _columnWidth) const {
	_columnWidth = DB_IMP_MTEXT(this->m_pImpl)->columnWidth;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setColumnWidth(double _columnWidth) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->columnWidth = _columnWidth;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbMText::subGetGeomExtents(DbExtents& extents) const {
	GePoint3d loc = this->location();
	double h = this->textHeight();
	if (h < 1e-10) h = 2.5;
	double rot = this->rotation();
	// 估算文本宽度
	AcString str;
	this->contents(str);
	int len = str.length();
	double w = h * len * 0.6;
	double aw = DB_IMP_MTEXT(this->m_pImpl)->actualWidth;
	if (aw > 0) w = aw;
	double ah = DB_IMP_MTEXT(this->m_pImpl)->actualHeight;
	if (ah > 0) h = ah;

	double cosR = cos(rot);
	double sinR = sin(rot);
	GeVector3d dirX(cosR, sinR, 0.0);
	GeVector3d dirY(-sinR, cosR, 0.0);

	GePoint3d corners[4];
	corners[0] = loc;
	corners[1] = loc + dirX * w;
	corners[2] = loc + dirX * w + dirY * (-h);
	corners[3] = loc + dirY * (-h);
	for (int i = 0; i < 4; i++) {
		extents.addPoint(corners[i]);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::subTransformBy(const GeMatrix3d& xform) {
	DbMTextImpl* imp = DB_IMP_MTEXT(this->m_pImpl);
	imp->location.transformBy(xform);
	imp->direction.transformBy(xform);
	double scale = imp->direction.length();
	if (scale > 1e-14) {
		imp->direction.normalize();
		imp->textHeight *= scale;
		imp->actualWidth *= scale;
		imp->actualHeight *= scale;
	}
	imp->normal.transformBy(xform);
	imp->normal.normalize();
	imp->rotation = atan2(imp->direction.y, imp->direction.x);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::subGetGripPoints(DbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize, const GeVector3d& curViewDir, const int bitflags) const {
	DbGripData* grip = new DbGripData();
	grip->setGripPoint(this->location());
	grips.append(grip);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::subGetOsnapPoints(Db::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const GePoint3d& pickPoint, const GePoint3d& lastPoint, const GeMatrix3d& viewXform, GePoint3dArray& snapPoints, DbIntArray& geomIds) const {
	if (osnapMode == Db::OsnapMode::kOsModeIns) {
		snapPoints.append(this->location());
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset) {
	DbMTextImpl* imp = DB_IMP_MTEXT(this->m_pImpl);
	for (int i = 0; i < indices.length(); i++) {
		if (indices[i] == 0) {
			imp->location += offset;
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags) {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::getColumnGutterWidth(double& _columnGutterWidth) const {
	_columnGutterWidth = DB_IMP_MTEXT(this->m_pImpl)->columnGutterWidth;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setColumnGutterWidth(double _columnGutterWidth) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->columnGutterWidth = _columnGutterWidth;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::getColumnFlowReversed(bool& _columnFlowReversed) const {
	_columnFlowReversed = DB_IMP_MTEXT(this->m_pImpl)->columnFlowReversed;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbMText::setColumnFlowReversed(bool _columnFlowReversed) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_MTEXT(this->m_pImpl)->columnFlowReversed = _columnFlowReversed;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::getColumnHeight(int index, double& val) const {
	if (index < DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length()) {
		val = DB_IMP_MTEXT(this->m_pImpl)->columnHeights[index];
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbMText::setColumnHeight(int index, double _height) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (index < DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length()) {
			DB_IMP_MTEXT(this->m_pImpl)->columnHeights[index] = _height;
			return Acad::ErrorStatus::eOk;
		}
	}
	return Acad::ErrorStatus::eFail;
}
double DbMText::height() const {
	double height = 0;
	for (int i = 0; i < this->columnCount(); i++) {
		if (this->columnHeight(i) > height) {
			height += this->columnHeight(i);
		}
	}
	return height;
}
Acad::ErrorStatus DbMText::setHeight(double _height) {
	
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {

		// 获得当前高度
		double height = this->height();

		// 获得增量
		double increment = (_height - height) / this->columnCount();

		// 设置高度
		for (int i = 0; i < DB_IMP_MTEXT(this->m_pImpl)->columnHeights.length(); i++) {
			DB_IMP_MTEXT(this->m_pImpl)->columnHeights[i] = DB_IMP_MTEXT(this->m_pImpl)->columnHeights[i] + increment;
		}

	}
	return Acad::ErrorStatus::eOk;
}
