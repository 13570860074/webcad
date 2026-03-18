#include "DbDimension.h"
#include "GePlane.h"
#include "DbBlockTable.h"
#include "DbDimStyleTableRecord.h"
#include "DbObjectManager.h"
#include "GiWorldDraw.h"
#include "GiEntityTraits.h"
#include "DbBlockTableRecordIterator.h"
#include "GiWorldGeometry.h"
#include "GiWorldGeometry.h"
#include "GiEntityTraits.h"
#include "kernel.h"
#include "DbLine.h"
#include "DbImpl.h"
#include "GiImpl.h"


DbDimension::DbDimension() {
	
}
DbDimension::~DbDimension() {

}


void DbDimension::textDefinedSize(double& width, double& height) const {

}
#if EMSDK
double DbDimension::textDefinedWidth()const {
	return 0.0;
}
double DbDimension::textDefinedHeight()const {
	return 0.0;
}
#endif

void DbDimension::setTextDefinedSize(double width, double height) {

}

void DbDimension::resetTextDefinedSize() {

}

GePoint3d DbDimension::textPosition() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->textPosition;
}
Acad::ErrorStatus DbDimension::setTextPosition(const GePoint3d& textPosition) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->textPosition = textPosition;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Adesk::Boolean DbDimension::isUsingDefaultTextPosition() const {
    return false;
}
Acad::ErrorStatus DbDimension::useSetTextPosition() {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::useDefaultTextPosition() {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setUsingDefaultTextPosition(bool) {
    return Acad::ErrorStatus::eOk;
}

GeVector3d DbDimension::normal() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->normal;
}
Acad::ErrorStatus DbDimension::setNormal(const GeVector3d& normal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->normal = normal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::getPlane(GePlane& _plane, Db::Planarity& _planarity) const {
    if (this->normal().length() < GeContext::gTol.equalVector()) {
        _planarity = Db::Planarity::kNonPlanar;
        return Acad::ErrorStatus::eFail;
    }

    _planarity = Db::Planarity::kPlanar;
    _plane.set(this->textPosition(), this->normal());
    return Acad::ErrorStatus::eOk;
}
#if EMSDK
GePlane DbDimension::emsdk_getPlane()const {
	GePlane plane;
	Db::Planarity planarity;
	this->getPlane(plane, planarity);
	return plane;
}
#endif

double DbDimension::elevation() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->elevation;
}
Acad::ErrorStatus DbDimension::setElevation(double elevation) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->elevation = elevation;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::dimensionText(AcString& sDimText) const {
	sDimText = DB_IMP_DIMENSION(this->m_pImpl)->dimensionText;
    return Acad::ErrorStatus::eOk;
}
ACHAR* DbDimension::dimensionText() const {
	ACHAR* text = NULL;
	::acutNewString(DB_IMP_DIMENSION(this->m_pImpl)->dimensionText.constPtr(), text);
    return text;
}
#if EMSDK
std::string DbDimension::emsdk_dimensionText()const {
	return this->dimensionText();
}
Acad::ErrorStatus DbDimension::emsdk_setDimensionText(const std::string text) {
	return this->setDimensionText(text.c_str());
}
#endif
Acad::ErrorStatus DbDimension::setDimensionText(const ACHAR* str) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimensionText = str;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbDimension::textRotation() const {
    return  DB_IMP_DIMENSION(this->m_pImpl)->textRotation;
}
Acad::ErrorStatus DbDimension::setTextRotation(double textRotation) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->textRotation = textRotation;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

DbObjectId DbDimension::dimensionStyle() const {
    return  DB_IMP_DIMENSION(this->m_pImpl)->dimensionStyle;
}
Acad::ErrorStatus DbDimension::setDimensionStyle(DbObjectId newVal) {
    Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

    do
    {
        if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false)
        {
            break;
        }

        if (this->database() != NULL)
        {
            // 获得标注样式表记录
            DbDimStyleTableRecord* pDimStyleTableRecord = NULL;
            ::acdbOpenObject(pDimStyleTableRecord, newVal);
            if (pDimStyleTableRecord != NULL)
            {
                // 判断数据库是否相同
                if (pDimStyleTableRecord->database() == this->database())
                {
                    DB_IMP_DIMENSION(this->m_pImpl)->dimensionStyle = newVal;
                    es = Acad::ErrorStatus::eOk;
                }

                pDimStyleTableRecord->close();
            }
        }
        else
        {
            DB_IMP_DIMENSION(this->m_pImpl)->dimensionStyle = newVal;
            es = Acad::ErrorStatus::eOk;
        }

    } while (false);

    return es;
}

DbMText::AttachmentPoint DbDimension::textAttachment() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->textAttachment;
}
Acad::ErrorStatus DbDimension::setTextAttachment(DbMText::AttachmentPoint eAtt) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->textAttachment = eAtt;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Db::LineSpacingStyle DbDimension::textLineSpacingStyle() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->textLineSpacingStyle;
}
Acad::ErrorStatus DbDimension::setTextLineSpacingStyle(Db::LineSpacingStyle eStyle) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->textLineSpacingStyle = eStyle;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

double DbDimension::textLineSpacingFactor() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->textLineSpacingFactor;
}
Acad::ErrorStatus DbDimension::setTextLineSpacingFactor(double dFactor) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->textLineSpacingFactor = dFactor;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::getDimstyleData(DbDimStyleTableRecord*& pRecord) const {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setDimstyleData(DbDimStyleTableRecord* pNewData) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setDimstyleData(DbObjectId newDataId) {
    return Acad::ErrorStatus::eOk;
}

void DbDimension::erased(DbObject* dbObj, Adesk::Boolean bErasing) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		
	}
}
void DbDimension::objectClosed(const DbObjectId dbObj) {

}
void DbDimension::dragStatus(const Db::DragStat status) {

}
void DbDimension::close() {

	DbEntity::close();

	if (this->database() == NULL) {
		return;
	}
	if (this->isNotifyEnabled() == false && this->isWriteEnabled() == false) {
		return;
	}

	DbObjectId dimBlockId = this->dimBlockId();
	DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)::kernel()->acdbObjectManager()->openDbObject(dimBlockId);
	if (pBlockTableRecord != NULL) {

		/* 删除实体 */
		DbBlockTableRecordIterator* pBlockTableRecordIterator = NULL;
		pBlockTableRecord->newIterator(pBlockTableRecordIterator);
		for (pBlockTableRecordIterator->start(); !pBlockTableRecordIterator->done(); pBlockTableRecordIterator->step()) {
			DbEntity* pEntity = NULL;
			pBlockTableRecordIterator->getEntity(pEntity);
			if (pEntity == NULL) {
				continue;
			}
			pEntity->erase();
			pEntity->close();
		}
	}
}

double DbDimension::horizontalRotation() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->horizontalRotation;
}
Acad::ErrorStatus DbDimension::setHorizontalRotation(double newVal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->horizontalRotation = newVal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}


DbObjectId DbDimension::dimBlockId() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimBlockId;
}
Acad::ErrorStatus DbDimension::setDimBlockId(const DbObjectId& blk) {

	if (this->database() == NULL) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimBlockId = blk;
		return Acad::ErrorStatus::eOk;
	}

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	DbObject *obj = ::kernel()->acdbObjectManager()->openDbObject(blk);
	if (obj != NULL) {
		if (obj->database() == this->database()) {
			DB_IMP_DIMENSION(this->m_pImpl)->dimBlockId = blk;
			es = Acad::ErrorStatus::eOk;
		}
	}

    return es;
}
GePoint3d DbDimension::dimBlockPosition() const {
    return GePoint3d::kOrigin;
}
Acad::ErrorStatus DbDimension::setDimBlockPosition(const GePoint3d&) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::recomputeDimBlock(bool forceUpdate) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::generateLayout() {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::measurement(double& val) {
	val = DB_IMP_DIMENSION(this->m_pImpl)->measurement;
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDimension::formatMeasurement(AcString& sMTextContent, double measurement, const ACHAR* dimensionText) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::formatMeasurement(ACHAR* MTextContentBuffer,
    size_t contentBufferLen,
    double measurement,
    ACHAR* dimensionText) {
    return Acad::ErrorStatus::eOk;
}

bool DbDimension::isDynamicDimension() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->isDynamicDimension;
}
Acad::ErrorStatus DbDimension::setDynamicDimension(bool newVal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->isDynamicDimension = newVal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::getOsnapPointsAtSubentPath(Db::OsnapMode osnapMode,
    const DbFullSubentPath& subentId,
    const GePoint3d& pickPoint,
    const GePoint3d& lastPoint,
    const GeMatrix3d& viewXform,
    GePoint3dArray& snapPoints,
    DbIntArray& geomIds) const {
    return Acad::ErrorStatus::eOk;
}


DbObjectId DbDimension::dimLineLinetype() const {
    return NULL;
}
Acad::ErrorStatus DbDimension::setDimLineLinetype(const DbObjectId linetype) {
    return Acad::ErrorStatus::eOk;
}
DbObjectId DbDimension::dimExt1Linetype() const {
    return NULL;
}
Acad::ErrorStatus DbDimension::setDimExt1Linetype(const DbObjectId linetype) {
    return Acad::ErrorStatus::eOk;
}
DbObjectId DbDimension::dimExt2Linetype() const {
    return NULL;
}
Acad::ErrorStatus DbDimension::setDimExt2Linetype(const DbObjectId linetype) {
    return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbDimension::removeTextField() {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::fieldToMText(DbMText* pDimMText) {
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::fieldFromMText(DbMText* pDimMText) {
    return Acad::ErrorStatus::eOk;
}



int DbDimension::dimfit() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->dimfit;
}
int DbDimension::dimunit() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->dimunit;
}

Acad::ErrorStatus DbDimension::setDimfit(int dimfit) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->dimfit = dimfit;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimunit(int dimunit) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->dimunit = dimunit;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::isHorizontalRefTextRotation() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->isHorizontalRefTextRotation;
}
Acad::ErrorStatus DbDimension::setHorizontalRefTextRotation(bool newVal) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->isHorizontalRefTextRotation = newVal;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::getArrowFirstIsFlipped() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->arrowFirstIsFlipped;
}
bool DbDimension::getArrowSecondIsFlipped() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->arrowSecondIsFlipped;
}
Acad::ErrorStatus DbDimension::setArrowFirstIsFlipped(bool bIsFlipped) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->arrowFirstIsFlipped = bIsFlipped;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setArrowSecondIsFlipped(bool bIsFlipped) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->arrowSecondIsFlipped = bIsFlipped;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

GeMatrix3d DbDimension::blockTransform() const {
    GeMatrix3d mat;
    mat.setToPlaneToWorld(GePlane(this->textPosition(), this->normal()));

    GeMatrix3d tempMat;
    tempMat.setToRotation(this->textRotation(), this->normal(), this->textPosition());
    mat.preMultBy(tempMat);

    return mat;
}


bool DbDimension::inspection() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->inspection;
}
Acad::ErrorStatus DbDimension::setInspection(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->inspection = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

int DbDimension::inspectionFrame() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->inspectionFrame;
}
Acad::ErrorStatus DbDimension::setInspectionFrame(int frame) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->inspectionFrame = frame;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

const ACHAR* DbDimension::inspectionLabel() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->inspectionLabel.constPtr();
}
Acad::ErrorStatus DbDimension::setInspectionLabel(const ACHAR* label) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->inspectionLabel = label;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

const ACHAR* DbDimension::inspectionRate() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->inspectionRate.constPtr();
}
Acad::ErrorStatus DbDimension::setInspectionRate(const ACHAR* label) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->inspectionRate = label;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::isConstraintObject() const {
    return false;
}
bool DbDimension::isConstraintObject(bool& hasExpression, bool& isReferenceConstraint) const {
    return false;
}
bool DbDimension::isConstraintDynamic(void) const {
    return false;
}
Acad::ErrorStatus DbDimension::setConstraintDynamic(bool bDynamic) {
    return Acad::ErrorStatus::eOk;
}

bool DbDimension::shouldParticipateInOPM(void) const {
    return false;
}
void DbDimension::setShouldParticipateInOPM(bool bShouldParticipate) {

}
double DbDimension::centerMarkSize() const {
    return 0.0;
}
Acad::ErrorStatus DbDimension::prefix(AcString& sPrefix) const {
    sPrefix = DB_IMP_DIMENSION(this->m_pImpl)->prefix;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setPrefix(const AcString& sPrefix) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->prefix = sPrefix;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::suffix(AcString& val) const {
    val = DB_IMP_DIMENSION(this->m_pImpl)->suffix;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setSuffix(const AcString& val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suffix = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::alternateSuffix(AcString&val) const {
    val = DB_IMP_DIMENSION(this->m_pImpl)->alternateSuffix;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setAlternateSuffix(const AcString& val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->alternateSuffix = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::alternatePrefix(AcString& val) const {
    val = DB_IMP_DIMENSION(this->m_pImpl)->alternatePrefix;
    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::setAlternatePrefix(const AcString& val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->alternatePrefix = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::suppressAngularLeadingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->suppressAngularLeadingZeros;
}
Acad::ErrorStatus DbDimension::setSuppressAngularLeadingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suppressAngularLeadingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::suppressAngularTrailingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->suppressAngularTrailingZeros;
}
Acad::ErrorStatus DbDimension::setSuppressAngularTrailingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suppressAngularTrailingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::altSuppressZeroInches() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altSuppressZeroInches;
}
Acad::ErrorStatus DbDimension::setAltSuppressZeroInches(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altSuppressZeroInches = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::altSuppressZeroFeet() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altSuppressZeroFeet;
}
Acad::ErrorStatus DbDimension::setAltSuppressZeroFeet(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altSuppressZeroFeet = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::altSuppressTrailingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altSuppressTrailingZeros;
}
Acad::ErrorStatus DbDimension::setAltSuppressTrailingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altSuppressTrailingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::altToleranceSuppressLeadingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressLeadingZeros;
}
Acad::ErrorStatus DbDimension::setAltToleranceSuppressLeadingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressLeadingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::altToleranceSuppressZeroInches() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressZeroInches;
}
Acad::ErrorStatus DbDimension::setAltToleranceSuppressZeroInches(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressZeroInches = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::altToleranceSuppressZeroFeet() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressZeroFeet;
}
Acad::ErrorStatus DbDimension::setAltToleranceSuppressZeroFeet(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressZeroFeet = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::altToleranceSuppressTrailingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressTrailingZeros;
}
Acad::ErrorStatus DbDimension::setAltToleranceSuppressTrailingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altToleranceSuppressTrailingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::suppressZeroFeet() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->suppressZeroFeet;
}
Acad::ErrorStatus DbDimension::setSuppressZeroFeet(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suppressZeroFeet = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::suppressTrailingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->suppressTrailingZeros;
}
Acad::ErrorStatus DbDimension::setSuppressTrailingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suppressTrailingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::suppressLeadingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->suppressLeadingZeros;
}
Acad::ErrorStatus DbDimension::setSuppressLeadingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suppressLeadingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::suppressZeroInches() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->suppressZeroInches;
}
Acad::ErrorStatus DbDimension::setSuppressZeroInches(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->suppressZeroInches = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::altSuppressLeadingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->altSuppressLeadingZeros;
}
Acad::ErrorStatus DbDimension::setAltSuppressLeadingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->altSuppressLeadingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::toleranceSuppressZeroFeet() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressZeroFeet;
}
Acad::ErrorStatus DbDimension::setToleranceSuppressZeroFeet(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressZeroFeet = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::toleranceSuppressTrailingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressTrailingZeros;
}
Acad::ErrorStatus DbDimension::setToleranceSuppressTrailingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressTrailingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}
bool DbDimension::toleranceSuppressLeadingZeros() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressLeadingZeros;
}
Acad::ErrorStatus DbDimension::setToleranceSuppressLeadingZeros(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressLeadingZeros = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

bool DbDimension::toleranceSuppressZeroInches() const {
    return DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressZeroInches;
}
Acad::ErrorStatus DbDimension::setToleranceSuppressZeroInches(bool val) {
    if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
        DB_IMP_DIMENSION(this->m_pImpl)->toleranceSuppressZeroInches = val;
        return Acad::ErrorStatus::eOk;
    }
    return Acad::ErrorStatus::eFail;
}

DbDimension::CenterMarkType DbDimension::centerMarkType() const {
    return DbDimension::CenterMarkType::kMark;
}






int DbDimension::dimadec() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimadec;
}
bool DbDimension::dimalt() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimalt;
}

int DbDimension::dimaltd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimaltd;
}
double DbDimension::dimaltf() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimaltf;
}
double DbDimension::dimaltrnd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimaltrnd;
}
int DbDimension::dimalttd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimalttd;
}
int DbDimension::dimalttz() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimalttz;
}
int DbDimension::dimaltu() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimaltu;
}
int DbDimension::dimaltz() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimaltz;
}
const char* DbDimension::dimapost() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimapost.constPtr();
}
int DbDimension::dimarcsym() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimarcsym;
	return 0;
}
double DbDimension::dimasz() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimasz;
}
int DbDimension::dimatfit() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimatfit;
}
int DbDimension::dimaunit() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimaunit;
}
int DbDimension::dimazin() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimazin;
}
DbObjectId DbDimension::dimblk() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimblk;
}
DbObjectId DbDimension::dimblk1() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimblk1;
}
DbObjectId DbDimension::dimblk2() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimblk2;
}
double DbDimension::dimcen() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimcen;
}
CmColor DbDimension::dimclrd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimclrd;
}
CmColor DbDimension::dimclre() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimclre;
}
CmColor DbDimension::dimclrt() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimclrt;
}
int DbDimension::dimdec() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimdec;
}
double DbDimension::dimdle() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimdle;
}
double DbDimension::dimdli() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimdli;
}
char DbDimension::dimdsep() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimdsep;
}
double DbDimension::dimexe() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimexe;
}
double DbDimension::dimexo() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimexo;
}
int DbDimension::dimfrac() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimfrac;
}
double DbDimension::dimgap() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimgap;
}
double DbDimension::dimjogang() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimjogang;
	return 0.0;
}
int DbDimension::dimjust() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimjust;
}
DbObjectId DbDimension::dimldrblk() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimldrblk;
}
double DbDimension::dimlfac() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimlfac;
}
bool DbDimension::dimlim() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimlim;
}
DbObjectId DbDimension::dimltex1() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimltex1;
	return NULL;
}
DbObjectId DbDimension::dimltex2() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimltex2;
	return NULL;
}
DbObjectId DbDimension::dimltype() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimltype;
	return NULL;
}
int DbDimension::dimlunit() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimlunit;
}
Db::LineWeight DbDimension::dimlwd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimlwd;
}
Db::LineWeight DbDimension::dimlwe() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimlwe;
}
const char* DbDimension::dimpost() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimpost;
}
double DbDimension::dimrnd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimrnd;
}
bool DbDimension::dimsah() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimsah;
}
double DbDimension::dimscale() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimscale;
}
bool DbDimension::dimsd1() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimsd1;
}
bool DbDimension::dimsd2() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimsd2;
}
bool DbDimension::dimse1() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimse1;
}
bool DbDimension::dimse2() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimse2;
}
bool DbDimension::dimsoxd() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimsoxd;
}
int DbDimension::dimtad() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtad;
}
int DbDimension::dimtdec() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtdec;
}
double DbDimension::dimtfac() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtfac;
}
int DbDimension::dimtfill() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimtfill;
	return 0;
}
CmColor DbDimension::dimtfillclr() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimtfillclr;
	return CmColor();
}
bool DbDimension::dimtih() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtih;
}
bool DbDimension::dimtix() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtix;
}
double DbDimension::dimtm() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtm;
}
int DbDimension::dimtmove() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtmove;
}
bool DbDimension::dimtofl() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtofl;
}
bool DbDimension::dimtoh() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtoh;
}
bool DbDimension::dimtol() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtol;
}
int DbDimension::dimtolj() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtolj;
}
double DbDimension::dimtp() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtp;
}
double DbDimension::dimtsz() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtsz;
}
double DbDimension::dimtvp() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtvp;
}
DbObjectId DbDimension::dimtxsty() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtxsty;
}
double DbDimension::dimtxt() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtxt;
}
int DbDimension::dimtzin() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimtzin;
}
bool DbDimension::dimupt() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimupt;
}
int DbDimension::dimzin() const {
	return DB_IMP_DIMENSION(this->m_pImpl)->dimzin;
}

bool DbDimension::dimfxlenOn() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimfxlenOn;
	return false;
}
double DbDimension::dimfxlen() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimfxlen;
	return 0.0;
}

bool DbDimension::dimtxtdirection() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimtxtdirection;
	return false;
}
double DbDimension::dimmzf() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimmzf;
	return 0.0;
}
const char* DbDimension::dimmzs() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimmzs;
	return "";
}
double DbDimension::dimaltmzf() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimaltmzf;
	return 0.0;
}
const char* DbDimension::dimaltmzs() const {
	//return DB_IMP_DIMENSION(this->m_pImpl)->dimaltmzs;
	return "";
}

Acad::ErrorStatus DbDimension::setDimadec(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimadec = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimalt(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimalt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltd(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimaltd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltf(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimaltf = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltmzf(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimaltmzf = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltmzs(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimaltmzs = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltrnd(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimaltrnd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimalttd(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimalttd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimalttz(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimalttz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltu(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimaltu = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaltz(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimaltz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimapost(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimapost = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimarcsym(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimarcsym = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimasz(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimasz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimatfit(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimatfit = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimaunit(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimaunit = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimazin(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimazin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimblk(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimblk = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimblk1(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimblk1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimblk2(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimblk2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimcen(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimcen = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimclrd(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimclrd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimclre(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimclre = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimclrt(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimclrt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimdec(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimdec = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimdle(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimdle = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimdli(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimdli = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimdsep(char  v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimdsep = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimexe(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimexe = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimexo(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimexo = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimfrac(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimfrac = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimgap(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimgap = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimjogang(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimjogang = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimjust(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimjust = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimldrblk(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimldrblk = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimlfac(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimlfac = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimlim(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimlim = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimltex1(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimltex1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimltex2(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimltex2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimltype(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimltype = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimlunit(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimlunit = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimlwd(Db::LineWeight v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimlwd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimlwe(Db::LineWeight v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimlwe = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimmzf(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimmzf = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimmzs(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimmzs = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimpost(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimpost = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimrnd(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimrnd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimsah(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimsah = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimscale(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimscale = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimsd1(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimsd1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimsd2(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimsd2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimse1(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimse1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimse2(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimse2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimsoxd(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimsoxd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtad(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtad = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtdec(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtdec = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtfac(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtfac = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtfill(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimtfill = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtfillclr(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimtfillclr = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtih(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtih = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtix(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtix = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtm(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtm = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtmove(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtmove = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtofl(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtofl = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtoh(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtoh = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtol(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtol = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtolj(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtolj = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtp(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtp = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtsz(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtsz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtvp(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtvp = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtxsty(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtxsty = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtxt(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtxt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtxtdirection(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimtxtdirection = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimtzin(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimtzin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimupt(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimupt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimension::setDimzin(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMENSION(this->m_pImpl)->dimzin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::setDimblk(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMENSION(this->m_pImpl)->dimblk = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::setDimblk1(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMENSION(this->m_pImpl)->dimblk1 = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::setDimblk2(const char* v) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMENSION(this->m_pImpl)->dimblk2 = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::setDimldrblk(const char* v) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMENSION(this->m_pImpl)->dimldrblk = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


Acad::ErrorStatus DbDimension::setDimfxlenOn(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimfxlenOn = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimension::setDimfxlen(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMENSION(this->m_pImpl)->dimfxlen = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}






bool DbDimension::subWorldDraw(GiWorldDraw* pWd) const {


	return true;
}
Acad::ErrorStatus DbDimension::dwgInFields(DbDwgFiler* pFiler) {
	DbEntity::dwgInFields(pFiler);

	DbHandle handle;

	pFiler->readDbHandle(&handle);
	DbObjectId dimBlockId = NULL;
	this->database()->getDbObjectId(dimBlockId, false, handle);
	DB_IMP_DIMENSION(this->m_pImpl)->dimBlockId.setFromOldId(dimBlockId.asOldId());

	GePoint3d textPosition;
	pFiler->readPoint3d(&textPosition);
	this->setTextPosition(textPosition);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimension::dwgOutFields(DbDwgFiler* pFiler) const {
	DbEntity::dwgOutFields(pFiler);

	pFiler->writeDbHandle(this->dimBlockId().handle());
	pFiler->writePoint3d(this->textPosition());

	return Acad::ErrorStatus::eOk;
}