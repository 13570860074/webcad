#include "DbText.h"
#include "GePlane.h"
#include "GiWorldDraw.h"
#include "GiWorldGeometry.h"
#include "DbObjectManager.h"
#include "DbTextStyleTableRecord.h"
#include "GiTextStyle.h"
#include "kernel.h"
#include "DbImpl.h"


DbText::DbText() {
    this->m_pImpl = new DbTextImpl();
}
DbText::DbText(const GePoint3d& position, const ACHAR* text, DbObjectId style, double height, double rotation) {
    DB_IMP_TEXT(this->m_pImpl)->position = position;
    DB_IMP_TEXT(this->m_pImpl)->textString = text;
    DB_IMP_TEXT(this->m_pImpl)->textStyleId = style;
    DB_IMP_TEXT(this->m_pImpl)->height = height;
    DB_IMP_TEXT(this->m_pImpl)->rotation = rotation;
}
DbText::~DbText() {

}





Acad::ErrorStatus DbText::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    GePoint3d position;
    pFiler->readPoint3d(&position);
    this->setPosition(position);

    GePoint3d alignmentPoint;
    pFiler->readPoint3d(&alignmentPoint);
    this->setAlignmentPoint(alignmentPoint);

    GeVector3d normal;
    pFiler->readVector3d(&normal);
    this->setNormal(normal);

    double thickness;
    pFiler->readDouble(&thickness);
    this->setThickness(thickness);

    double oblique;
    pFiler->readDouble(&oblique);
    this->setOblique(oblique);

    double rotation;
    pFiler->readDouble(&rotation);
    this->setRotation(rotation);

    double height;
    pFiler->readDouble(&height);
    this->setHeight(height);

    double widthFactor;
    pFiler->readDouble(&widthFactor);
    this->setWidthFactor(widthFactor);

    AcString textString;
    pFiler->readString(textString);
    this->setTextString(textString);

    bool isMirroredInX;
    pFiler->readBool(&isMirroredInX);
    this->mirrorInX(isMirroredInX);

    bool isMirroredInY;
    pFiler->readBool(&isMirroredInY);
    this->mirrorInY(isMirroredInY);

    Adesk::Int16 horizontalMode;
    pFiler->readInt16(&horizontalMode);
    this->setHorizontalMode((Db::TextHorzMode)horizontalMode);

    Adesk::Int16 verticalMode;
    pFiler->readInt16(&verticalMode);
    this->setVerticalMode((Db::TextVertMode)verticalMode);

    Adesk::Int16 justification;
    pFiler->readInt16(&justification);
    this->setJustification((AcTextAlignment)justification);

    DbHandle handle;
    pFiler->readDbHandle(&handle);
    DbObjectId textStyleId = NULL;
    this->database()->getDbObjectId(textStyleId, false, handle);
    DB_IMP_TEXT(this->m_pImpl)->textStyleId.setFromOldId(textStyleId.asOldId());

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbText::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }

    pFiler->writePoint3d(this->position());

    pFiler->writePoint3d(this->alignmentPoint());

    pFiler->writeVector3d(this->normal());

    pFiler->writeDouble(this->thickness());

    pFiler->writeDouble(this->oblique());

    pFiler->writeDouble(this->rotation());

    pFiler->writeDouble(this->height());

    pFiler->writeDouble(this->widthFactor());

    pFiler->writeString(this->textString());

    pFiler->writeBool(this->isMirroredInX());
    pFiler->writeBool(this->isMirroredInY());

    pFiler->writeInt16(this->horizontalMode());

    pFiler->writeInt16(this->verticalMode());

    pFiler->writeInt16(this->justification());

    pFiler->writeDbHandle(DB_IMP_TEXT(this->m_pImpl)->textStyleId.handle());

    return Acad::ErrorStatus::eOk;
}
bool DbText::subWorldDraw(GiWorldDraw* pWd) const {

    /* 求方向 */
    GeVector3d direction = GeVector3d::kXAxis;
    if (this->normal().isEqualTo(GeVector3d::kZAxis) == false) {
        direction = GeVector3d::kZAxis.crossProduct(this->normal());
    }
    if (abs(this->rotation()) > 0.000001) {
        direction.rotateBy(this->rotation(), this->normal());
    }

    /* 文字样式 */
    GiTextStyle textStyle;
    textStyle.setXScale(this->widthFactor());

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
    textStyle.setTextSize(this->height());
    textStyle.setFileName(fontName);
    textStyle.setBigFontFileName(bigFontFileName);
    textStyle.setFont(pTypeface, bold, italic, charset, pitch, family);
    pWd->geometry().text(
        this->position(), 
        this->normal(), 
        direction,
        this->textStringConst(),
        strlen(this->textStringConst()),
        false,
        textStyle);
    return true;
}






GePoint3d DbText::position() const {
    return DB_IMP_TEXT(this->m_pImpl)->position;
}
Acad::ErrorStatus DbText::setPosition(const GePoint3d& _position) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->position = _position;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GePoint3d DbText::alignmentPoint() const {
    return DB_IMP_TEXT(this->m_pImpl)->alignmentPoint;
}
Acad::ErrorStatus DbText::setAlignmentPoint(const GePoint3d& _alignmentPoint) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->alignmentPoint = _alignmentPoint;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
Adesk::Boolean DbText::isDefaultAlignment() const {
    return false;
}

GeVector3d DbText::normal() const {
    return DB_IMP_TEXT(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbText::setNormal(const GeVector3d& _normal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->normal = _normal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbText::getPlane(GePlane& _plane, Db::Planarity& planarity) const {
    _plane.set(this->position(), this->normal());
    planarity = Db::Planarity::kPlanar;
    return Acad::ErrorStatus::eOk;
}

double DbText::thickness() const {
    return DB_IMP_TEXT(this->m_pImpl)->thickness;
}
Acad::ErrorStatus DbText::setThickness(double _thickness) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->thickness = _thickness;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

double DbText::oblique() const {
    return DB_IMP_TEXT(this->m_pImpl)->oblique;
}
Acad::ErrorStatus DbText::setOblique(double _oblique) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->oblique = _oblique;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

double DbText::rotation() const {
    return DB_IMP_TEXT(this->m_pImpl)->rotation;
}
Acad::ErrorStatus DbText::setRotation(double _rotation) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->rotation = _rotation;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

double DbText::height() const {
    return DB_IMP_TEXT(this->m_pImpl)->height;
}
Acad::ErrorStatus DbText::setHeight(double _height) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->height = _height;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

double DbText::widthFactor() const {
    return DB_IMP_TEXT(this->m_pImpl)->widthFactor;
}
Acad::ErrorStatus DbText::setWidthFactor(double _widthFactor) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->widthFactor = _widthFactor;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}


Acad::ErrorStatus DbText::textString(AcString& sText) const {
    sText = DB_IMP_TEXT(this->m_pImpl)->textString;
    return Acad::ErrorStatus::eOk;
}
ACHAR* DbText::textString() const {
    char* str = NULL;
    ::acutNewString(DB_IMP_TEXT(this->m_pImpl)->textString.constPtr(), str);
    return str;
}
const ACHAR* DbText::textStringConst() const {
    return DB_IMP_TEXT(this->m_pImpl)->textString.constPtr();
}
Acad::ErrorStatus DbText::setTextString(const ACHAR* _str) {
    DB_IMP_TEXT(this->m_pImpl)->textString = _str;
    return Acad::ErrorStatus::eOk;
}

DbObjectId DbText::textStyle() const {
    return DB_IMP_TEXT(this->m_pImpl)->textStyleId;
}
Acad::ErrorStatus DbText::setTextStyle(DbObjectId _styleId) {
    
    Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

    do {

        if (this->database() == NULL) {
            DB_IMP_TEXT(this->m_pImpl)->textStyleId = _styleId;
            es = Acad::ErrorStatus::eOk;
            break;
        }

        // 打开样式
        DbTextStyleTableRecord* pTextStyleTableRecord = NULL;
        if (::acdbOpenObject(pTextStyleTableRecord, _styleId) != 0) {
            break;
        }

        if (this->database() == pTextStyleTableRecord->database()) {
            DB_IMP_TEXT(this->m_pImpl)->textStyleId = _styleId;
            es = Acad::ErrorStatus::eOk;
        }

        pTextStyleTableRecord->close();

    } while (false);

    return es;
}

Adesk::Boolean DbText::isMirroredInX() const {
    return DB_IMP_TEXT(this->m_pImpl)->mirroredInX;
}
Acad::ErrorStatus DbText::mirrorInX(Adesk::Boolean _val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->mirroredInX = _val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Adesk::Boolean DbText::isMirroredInY() const {
    return DB_IMP_TEXT(this->m_pImpl)->mirroredInY;
}
Acad::ErrorStatus DbText::mirrorInY(Adesk::Boolean _val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->mirroredInY = _val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Db::TextHorzMode DbText::horizontalMode() const {

    Db::TextHorzMode horzMode  = Db::TextHorzMode::kTextLeft;

    if (this->justification() == AcTextAlignment::kTextAlignmentAligned) {
        horzMode = Db::TextHorzMode::kTextAlign;
    }
    else if (this->justification() == AcTextAlignment::kTextAlignmentCenter) {
        horzMode = Db::TextHorzMode::kTextCenter;
    }
    else if (this->justification() == AcTextAlignment::kTextAlignmentFit) {
        horzMode = Db::TextHorzMode::kTextFit;
    }
    else if (this->justification() == AcTextAlignment::kTextAlignmentBottomLeft ||
        this->justification() == AcTextAlignment::kTextAlignmentLeft ||
        this->justification() == AcTextAlignment::kTextAlignmentMiddleLeft ||
        this->justification() == AcTextAlignment::kTextAlignmentTopLeft) {
        horzMode = Db::TextHorzMode::kTextLeft;
    }
    else if (this->justification() == AcTextAlignment::kTextAlignmentBottomCenter ||
        this->justification() == AcTextAlignment::kTextAlignmentMiddleCenter ||
        this->justification() == AcTextAlignment::kTextAlignmentTopCenter ||
        this->justification() == AcTextAlignment::kTextAlignmentMiddle) {
        horzMode = Db::TextHorzMode::kTextMid;
    }
    else if (this->justification() == AcTextAlignment::kTextAlignmentBottomRight ||
        this->justification() == AcTextAlignment::kTextAlignmentMiddleRight ||
        this->justification() == AcTextAlignment::kTextAlignmentRight ||
        this->justification() == AcTextAlignment::kTextAlignmentTopRight) {
        horzMode = Db::TextHorzMode::kTextRight;
    }

    return horzMode;
}
Acad::ErrorStatus DbText::setHorizontalMode(Db::TextHorzMode _horizontalMode) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        
        Db::TextHorzMode horzMode = _horizontalMode;
        Db::TextVertMode verticalMode = this->verticalMode();
        if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }
        else if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }
        else if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }
        else if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }

        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentFit;
        }
        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleLeft;
        }

        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddle;
        }
        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentRight;
        }
        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomRight;
        }
        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopRight;
        }
        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleRight;
        }

        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Db::TextVertMode DbText::verticalMode() const {

    Db::TextVertMode verticalMode = Db::TextVertMode::kTextTop;

    if (this->justification() == DbText::AcTextAlignment::kTextAlignmentBottomCenter ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentBottomLeft ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentBottomRight) {
        verticalMode = Db::TextVertMode::kTextBottom;
    }
    else if (this->justification() == DbText::AcTextAlignment::kTextAlignmentMiddle ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentMiddleCenter ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentMiddleLeft ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentMiddleRight) {
        verticalMode = Db::TextVertMode::kTextVertMid;
    }
    else if (this->justification() == DbText::AcTextAlignment::kTextAlignmentTopCenter ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentTopLeft ||
        this->justification() == DbText::AcTextAlignment::kTextAlignmentTopRight) {
        verticalMode = Db::TextVertMode::kTextTop;
    }
    else {
        verticalMode = Db::TextVertMode::kTextBase;
    }

    return verticalMode;
}
Acad::ErrorStatus DbText::setVerticalMode(Db::TextVertMode _verticalMode) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        Db::TextHorzMode horzMode = this->horizontalMode();
        Db::TextVertMode verticalMode = _verticalMode;
        if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }
        else if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }
        else if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }
        else if (horzMode == Db::TextHorzMode::kTextAlign && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentAligned;
        }

        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextCenter && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentFit;
        }
        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextFit && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopLeft;
        }
        else if (horzMode == Db::TextHorzMode::kTextLeft && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleLeft;
        }

        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddle;
        }
        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopCenter;
        }
        else if (horzMode == Db::TextHorzMode::kTextMid && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleCenter;
        }

        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextBase) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentRight;
        }
        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextBottom) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentBottomRight;
        }
        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextTop) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentTopRight;
        }
        else if (horzMode == Db::TextHorzMode::kTextRight && verticalMode == Db::TextVertMode::kTextVertMid) {
            DB_IMP_TEXT(this->m_pImpl)->textAlignment = DbText::AcTextAlignment::kTextAlignmentMiddleRight;
        }

        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbText::adjustAlignment(const DbDatabase* pDb) {
    return Acad::ErrorStatus::eFail;
}


DbText::AcTextAlignment DbText::justification() const {
    return (DbText::AcTextAlignment)DB_IMP_TEXT(this->m_pImpl)->textAlignment;
}
Acad::ErrorStatus DbText::setJustification(AcTextAlignment _textAlignment) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_TEXT(this->m_pImpl)->textAlignment = _textAlignment;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}