#include "kernel.h"
#include "ReRendererManager.h"
#include "DbDatabase.h"
#include "DbViewport.h"
#include "DbUCSTableRecord.h"
#include "DbObjectManager.h"
#include "DbImpl.h"



DbViewport::DbViewport()
{
	this->m_pImpl = new DbViewportImpl();
}
DbViewport::~DbViewport()
{

}



double DbViewport::height() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->height;
}
Acad::ErrorStatus DbViewport::setHeight(double _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->height = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbViewport::width() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->width;
}
Acad::ErrorStatus DbViewport::setWidth(double _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->width = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbViewport::centerPoint() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->centerPoint;
}
Acad::ErrorStatus DbViewport::setCenterPoint(const GePoint3d& _center) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->centerPoint = _center;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Int16 DbViewport::number() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->number;
}

bool DbViewport::isOn() const {
	return  DB_IMP_VIEWPORT(this->m_pImpl)->bOn;
}
Acad::ErrorStatus DbViewport::setOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->bOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->bOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setIsOn(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->bOn = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint3d DbViewport::viewTarget() const {
	return  DB_IMP_VIEWPORT(this->m_pImpl)->target;
}
Acad::ErrorStatus DbViewport::setViewTarget(const GePoint3d& _target) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->target = _target;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector3d DbViewport::viewDirection() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection;
}
Acad::ErrorStatus DbViewport::setViewDirection(const GeVector3d& _viewDirection) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = _viewDirection;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Model Space height, and center in Display coordinates
//
double DbViewport::viewHeight() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->viewHeight;
}
Acad::ErrorStatus DbViewport::setViewHeight(double ht) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->viewHeight = ht;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint2d DbViewport::viewCenter() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->viewCenter;
}
Acad::ErrorStatus DbViewport::setViewCenter(const GePoint2d& pt) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->viewCenter = pt;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbViewport::twistAngle() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->twistAngle;
}
Acad::ErrorStatus DbViewport::setTwistAngle(double _twistAngle) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->twistAngle = _twistAngle;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbViewport::lensLength() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->lensLength;
}
Acad::ErrorStatus DbViewport::setLensLength(double _lensLength) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->lensLength = _lensLength;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isFrontClipOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipOn;
}
Acad::ErrorStatus DbViewport::setFrontClipOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setFrontClipOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setFrontClipOn(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipOn = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isBackClipOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isBackClipOn;
}
Acad::ErrorStatus DbViewport::setBackClipOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isBackClipOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setBackClipOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isBackClipOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setBackClipOn(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isBackClipOn = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isFrontClipAtEyeOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipAtEyeOn;
}
Acad::ErrorStatus DbViewport::setFrontClipAtEyeOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipAtEyeOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setFrontClipAtEyeOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipAtEyeOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setFrontClipAtEyeOn(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isFrontClipAtEyeOn = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// FrontZ 
double DbViewport::frontClipDistance() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->frontClipDistance;
}
Acad::ErrorStatus DbViewport::setFrontClipDistance(double newVal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->frontClipDistance = newVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// BackZ 
double DbViewport::backClipDistance() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->backClipDistance;
}
Acad::ErrorStatus DbViewport::setBackClipDistance(double newVal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->backClipDistance = newVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isPerspectiveOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isPerspectiveOn;
}
Acad::ErrorStatus DbViewport::setPerspectiveOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isPerspectiveOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setPerspectiveOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isPerspectiveOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setPerspectiveOn(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isPerspectiveOn = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
bool DbViewport::isUcsFollowModeOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isUcsFollowModeOn;
}
Acad::ErrorStatus DbViewport::setUcsFollowModeOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsFollowModeOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcsFollowModeOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsFollowModeOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcsFollowModeOn(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsFollowModeOn = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isUcsIconVisible() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconVisible;
}
Acad::ErrorStatus DbViewport::setUcsIconVisible() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconVisible = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcsIconInvisible() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconVisible = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcsIconVisible(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconVisible = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isUcsIconAtOrigin() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconAtOrigin;
}
Acad::ErrorStatus DbViewport::setUcsIconAtOrigin() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconAtOrigin = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcsIconAtCorner() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconAtOrigin = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcsIconAtOrigin(bool bOn) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsIconAtOrigin = bOn;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbViewport::setFastZoomOn(bool) {
	return Acad::ErrorStatus::eOk;
}

UInt16 DbViewport::circleSides() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->circleSides;
}
Acad::ErrorStatus DbViewport::setCircleSides(UInt16 _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->circleSides = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isSnapOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isSnapOn;
}
Acad::ErrorStatus DbViewport::setSnapOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isSnapOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setSnapOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isSnapOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setSnapOn(bool _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isSnapOn = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isSnapIsometric() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isSnapIsometric;
}
Acad::ErrorStatus DbViewport::setSnapIsometric() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isSnapIsometric = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setSnapStandard() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isSnapIsometric = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setSnapIsometric(bool _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isSnapIsometric = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

double DbViewport::snapAngle() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->snapAngle;
}
Acad::ErrorStatus DbViewport::setSnapAngle(double _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->snapAngle = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GePoint2d DbViewport::snapBasePoint() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->snapBasePoint;
}
Acad::ErrorStatus DbViewport::setSnapBasePoint(const GePoint2d& _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->snapBasePoint = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector2d DbViewport::snapIncrement() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->snapIncrement;
}
Acad::ErrorStatus DbViewport::setSnapIncrement(const GeVector2d& _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->snapIncrement = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

UInt16 DbViewport::snapIsoPair() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->snapIsoPair;
}
Acad::ErrorStatus DbViewport::setSnapIsoPair(UInt16 _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->snapIsoPair = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isGridOn() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isGridOn;
}
Acad::ErrorStatus DbViewport::setGridOn() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridOn = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setGridOff() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridOn = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setGridOn(bool _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridOn = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isGridBoundToLimits() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isGridBoundToLimits;
}
Acad::ErrorStatus DbViewport::setGridBoundToLimits(bool bNewVal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridBoundToLimits = bNewVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isGridAdaptive() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isGridAdaptive;
}
Acad::ErrorStatus DbViewport::setGridAdaptive(bool bNewVal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridAdaptive = bNewVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isGridSubdivisionRestricted() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isGridSubdivisionRestricted;
}
Acad::ErrorStatus DbViewport::setGridSubdivisionRestricted(bool bNewVal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridSubdivisionRestricted = bNewVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isGridFollow() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isGridFollow;
}
Acad::ErrorStatus DbViewport::setGridFollow(bool bNewVal) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isGridFollow = bNewVal;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

UInt16 DbViewport::gridMajor() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->gridMajor;
}
Acad::ErrorStatus DbViewport::setGridMajor(UInt16 _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->gridMajor = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

GeVector2d DbViewport::gridIncrement() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->gridIncrement;
}
Acad::ErrorStatus DbViewport::setGridIncrement(const GeVector2d& _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->gridIncrement = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbViewport::freezeLayersInViewport(const DbObjectIdArray& ids) {
	DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.removeAll();
	for (int i = 0; i < ids.length(); i++) {
		DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.append(ids.at(i));
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::thawLayersInViewport(const DbObjectIdArray& ids) {
	for (int i = 0; i < ids.length(); i++) {
		for (int u = 0; u < DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.length(); u++) {
			if (DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers[u] == ids.at(i)) {
				DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.removeAt(u);
				break;
			}
		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::thawAllLayersInViewport() {
	DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.removeAll();
	return Acad::ErrorStatus::eOk;
}
bool DbViewport::isLayerFrozenInViewport(const DbObjectId& layerId) const {
	for (int i = 0; i < DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.length(); i++) {
		if (layerId == DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers[i]) {
			return true;
		}
	}
	return false;
}
Acad::ErrorStatus DbViewport::getFrozenLayerList(DbObjectIdArray& ids) const {
	for (int i = 0; i < DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.length(); i++) {
		ids.append(DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers[i]);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::updateDisplay() const {
	return Acad::ErrorStatus::eOk;
}

bool DbViewport::isLocked() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isLocked;
}
Acad::ErrorStatus DbViewport::setLocked() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isLocked = true;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUnlocked() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isLocked = false;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setLocked(bool _v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isLocked = _v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


Acad::ErrorStatus DbViewport::getUcs(GePoint3d& origin, GeVector3d& xAxis, GeVector3d& yAxis) const {
	origin.set(DB_IMP_VIEWPORT(this->m_pImpl)->origin.x, DB_IMP_VIEWPORT(this->m_pImpl)->origin.y, DB_IMP_VIEWPORT(this->m_pImpl)->origin.z);
	xAxis.set(DB_IMP_VIEWPORT(this->m_pImpl)->xAxis.x, DB_IMP_VIEWPORT(this->m_pImpl)->xAxis.y, DB_IMP_VIEWPORT(this->m_pImpl)->xAxis.z);
	yAxis.set(DB_IMP_VIEWPORT(this->m_pImpl)->yAxis.x, DB_IMP_VIEWPORT(this->m_pImpl)->yAxis.y, DB_IMP_VIEWPORT(this->m_pImpl)->yAxis.z);
	return Acad::ErrorStatus::eOk;
}
bool DbViewport::isUcsOrthographic(Db::OrthographicView& view) const {

	view = (Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView;
	if ((Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kTopView ||
		(Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kBottomView ||
		(Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kFrontView ||
		(Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kBackView ||
		(Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kLeftView ||
		(Db::OrthographicView)DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kRightView) {
		return true;
	}
	return false;
}
DbObjectId DbViewport::ucsName() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->ucsId;
}
double DbViewport::elevation() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->elevation;
}
Acad::ErrorStatus DbViewport::setUcs(const GePoint3d& origin, const GeVector3d& xAxis, const GeVector3d& yAxis) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->origin = origin;
		DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = xAxis;
		DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = yAxis;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcs(Db::OrthographicView view) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView = view;
		if (view == Db::OrthographicView::kTopView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d(1, 0, 0);
			DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d(0, 1, 0);
		}
		else if (view == Db::OrthographicView::kBottomView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d(1, 0, 0);
			DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d(0, -1, 0);
		}
		else if (view == Db::OrthographicView::kLeftView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d(0, 1, 0);
			DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kRightView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d(0, -1, 0);
			DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kFrontView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d(-1, 0, 0);
			DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kBackView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d(1, 0, 0);
			DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setUcs(const DbObjectId& ucsId) {
	if (this->isWriteEnabled() != true && this->isNotifyEnabled() != true) {
		return Acad::ErrorStatus::eFail;
	}

	//打开对象
	DbUCSTableRecord* pUCSTableRecord = NULL;
	if (::acdbOpenObject(pUCSTableRecord, ucsId) != 0) {
		return Acad::ErrorStatus::eFail;
	}

	//判断是否在数据库上
	if (pUCSTableRecord->database() != this->database()) {
		pUCSTableRecord->close();
		return Acad::ErrorStatus::eFail;
	}

	//设置坐标系
	DB_IMP_VIEWPORT(this->m_pImpl)->ucsId = ucsId;

	//设置轴
	DB_IMP_VIEWPORT(this->m_pImpl)->origin = pUCSTableRecord->origin();
	DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = pUCSTableRecord->xAxis();
	DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = pUCSTableRecord->yAxis();

	pUCSTableRecord->close();

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::setUcsToWorld() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->origin = GePoint3d::kOrigin;
		DB_IMP_VIEWPORT(this->m_pImpl)->xAxis = GeVector3d::kXAxis;
		DB_IMP_VIEWPORT(this->m_pImpl)->yAxis = GeVector3d::kYAxis;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbViewport::setElevation(double elev) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->elevation = elev;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbViewport::isViewOrthographic(Db::OrthographicView& view) const {
	if (DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kTopView) {
		view = Db::OrthographicView::kTopView;
		return true;
	}
	else if (DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kBottomView) {
		view = Db::OrthographicView::kBottomView;
		return true;
	}
	else if (DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kLeftView) {
		view = Db::OrthographicView::kLeftView;
		return true;
	}
	else if (DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kRightView) {
		view = Db::OrthographicView::kRightView;
		return true;
	}
	else if (DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kFrontView) {
		view = Db::OrthographicView::kFrontView;
		return true;
	}
	else if (DB_IMP_VIEWPORT(this->m_pImpl)->orthographicView == Db::OrthographicView::kBackView) {
		view = Db::OrthographicView::kBackView;
		return true;
	}
	view = Db::OrthographicView::kNonOrthoView;
	return false;
}
Acad::ErrorStatus DbViewport::setViewDirection(Db::OrthographicView view) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (view == Db::OrthographicView::kTopView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kBottomView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = GeVector3d(0, 0, -1);
		}
		else if (view == Db::OrthographicView::kLeftView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = GeVector3d(-1, 0, 0);
		}
		else if (view == Db::OrthographicView::kRightView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = GeVector3d(1, 0, 0);
		}
		else if (view == Db::OrthographicView::kFrontView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = GeVector3d(0, -1, 0);
		}
		else if (view == Db::OrthographicView::kBackView) {
			DB_IMP_VIEWPORT(this->m_pImpl)->viewDirection = GeVector3d(0, 1, 0);
		}
	}
	return Acad::ErrorStatus::eOk;
}


bool DbViewport::isUcsSavedWithViewport() const {
	return DB_IMP_VIEWPORT(this->m_pImpl)->isUcsSavedWithViewport;
}
void DbViewport::setUcsPerViewport(bool ucsvp) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_VIEWPORT(this->m_pImpl)->isUcsSavedWithViewport = ucsvp;
	}
}





Acad::ErrorStatus DbViewport::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	double height;
	pFiler->readDouble(&height);
	this->setHeight(height);

	double width;
	pFiler->readDouble(&width);
	this->setWidth(height);

	GePoint3d centerPoint;
	pFiler->readPoint3d(&centerPoint);
	this->setCenterPoint(centerPoint);

	short number;
	pFiler->readInt16(&number);
	DB_IMP_VIEWPORT(this->m_pImpl)->number = number;

	GePoint3d target;
	pFiler->readPoint3d(&target);
	this->setViewTarget(target);

	GeVector3d viewDirection;
	pFiler->readVector3d(&viewDirection);
	this->setViewDirection(viewDirection);

	double viewHeight;
	pFiler->readDouble(&viewHeight);
	this->setViewHeight(viewHeight);

	GePoint2d viewCenter;
	pFiler->readPoint2d(&viewCenter);
	this->setViewCenter(viewCenter);

	double twistAngle;
	pFiler->readDouble(&twistAngle);
	this->setTwistAngle(twistAngle);

	double lensLength;
	pFiler->readDouble(&lensLength);
	this->setLensLength(lensLength);

	bool isFrontClipOn;
	pFiler->readBool(&isFrontClipOn);
	this->setFrontClipOn(isFrontClipOn);

	bool isBackClipOn;
	pFiler->readBool(&isBackClipOn);
	this->setBackClipOn(isBackClipOn);

	bool isFrontClipAtEyeOn;
	pFiler->readBool(&isFrontClipAtEyeOn);
	this->setFrontClipAtEyeOn(isFrontClipAtEyeOn);

	double frontClipDistance;
	pFiler->readDouble(&frontClipDistance);
	this->setFrontClipDistance(frontClipDistance);

	double backClipDistance;
	pFiler->readDouble(&backClipDistance);
	this->setBackClipDistance(backClipDistance);

	bool isPerspectiveOn;
	pFiler->readBool(&isPerspectiveOn);
	this->setPerspectiveOn(isPerspectiveOn);

	bool isUcsFollowModeOn;
	pFiler->readBool(&isUcsFollowModeOn);
	this->setUcsFollowModeOn(isUcsFollowModeOn);

	bool isUcsIconVisible;
	pFiler->readBool(&isUcsIconVisible);
	this->setUcsIconVisible(isUcsIconVisible);

	bool isUcsIconAtOrigin;
	pFiler->readBool(&isUcsIconAtOrigin);
	this->setUcsIconAtOrigin(isUcsIconAtOrigin);

	unsigned short circleSides;
	pFiler->readUInt16(&circleSides);
	this->setCircleSides(circleSides);

	bool isSnapOn;
	pFiler->readBool(&isSnapOn);
	this->setSnapOn(isSnapOn);

	bool isSnapIsometric;
	pFiler->readBool(&isSnapIsometric);
	this->setSnapIsometric(isSnapIsometric);

	bool isLocked;
	pFiler->readBool(&isLocked);
	this->setLocked(isLocked);

	DbObjectIdArray frozenLayers;
	unsigned short numFrozenLayers = 0;
	pFiler->readUInt16(&numFrozenLayers);
	for (int i = 0; i < numFrozenLayers; i++) {
		DbHandle handle;
		pFiler->readDbHandle(&handle);

		DbObjectId id;
		this->database()->getDbObjectId(id, false, handle);
		if (id != NULL) {
			DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.append(id);
		}
	}

	unsigned short gridMajor;
	pFiler->readUInt16(&gridMajor);
	this->setGridMajor(gridMajor);

	GeVector2d gridIncrement;
	pFiler->readVector2d(&gridIncrement);
	this->setGridIncrement(gridIncrement);

	bool isGridFollow;
	pFiler->readBool(&isGridFollow);
	this->setGridFollow(isGridFollow);

	bool isGridSubdivisionRestricted;
	pFiler->readBool(&isGridSubdivisionRestricted);
	this->setGridSubdivisionRestricted(isGridSubdivisionRestricted);

	bool isGridAdaptive;
	pFiler->readBool(&isGridAdaptive);
	this->setGridAdaptive(isGridAdaptive);

	bool isGridBoundToLimits;
	pFiler->readBool(&isGridBoundToLimits);
	this->setGridBoundToLimits(isGridBoundToLimits);

	bool isGridOn;
	pFiler->readBool(&isGridOn);
	this->setGridOn(isGridOn);

	unsigned short snapIsoPair;
	pFiler->readUInt16(&snapIsoPair);
	this->setSnapIsoPair(snapIsoPair);

	GeVector2d snapIncrement;
	pFiler->readVector2d(&snapIncrement);
	this->setSnapIncrement(snapIncrement);

	GePoint2d snapBasePoint;
	pFiler->readPoint2d(&snapBasePoint);
	this->setSnapBasePoint(snapBasePoint);

	double snapAngle;
	pFiler->readDouble(&snapAngle);
	this->setSnapAngle(snapAngle);

	GePoint3d origin;
	pFiler->readPoint3d(&origin);
	DB_IMP_VIEWPORT(this->m_pImpl)->origin.set(origin.x, origin.y, origin.z);

	GeVector3d xAxis;
	pFiler->readVector3d(&xAxis);
	DB_IMP_VIEWPORT(this->m_pImpl)->xAxis.set(xAxis.x, xAxis.y, xAxis.z);

	GeVector3d yAxis;
	pFiler->readVector3d(&yAxis);
	DB_IMP_VIEWPORT(this->m_pImpl)->yAxis.set(yAxis.x, yAxis.y, yAxis.z);

	double elevation;
	pFiler->readDouble(&elevation);
	this->setElevation(elevation);

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeDouble(this->height());

	pFiler->writeDouble(this->width());

	pFiler->writePoint3d(this->centerPoint());

	pFiler->writeInt16(this->number());

	pFiler->writePoint3d(this->viewTarget());

	pFiler->writeVector3d(this->viewDirection());

	pFiler->writeDouble(this->viewHeight());

	pFiler->writePoint2d(this->viewCenter());

	pFiler->writeDouble(this->twistAngle());

	pFiler->writeDouble(this->lensLength());

	pFiler->writeBool(this->isFrontClipOn());

	pFiler->writeBool(this->isBackClipOn());

	pFiler->writeBool(this->isFrontClipAtEyeOn());

	pFiler->writeDouble(this->frontClipDistance());

	pFiler->writeDouble(this->backClipDistance());

	pFiler->writeBool(this->isPerspectiveOn());

	pFiler->writeBool(this->isUcsFollowModeOn());

	pFiler->writeBool(this->isUcsIconVisible());

	pFiler->writeBool(this->isUcsIconAtOrigin());

	pFiler->writeUInt16(this->circleSides());

	pFiler->writeBool(this->isSnapOn());

	pFiler->writeBool(this->isSnapIsometric());

	pFiler->writeBool(this->isLocked());

	pFiler->writeUInt16(this->circleSides());

	pFiler->writeUInt16(DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.length());
	for (int i = 0; i < DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers.length(); i++) {
		pFiler->writeDbHandle(DB_IMP_VIEWPORT(this->m_pImpl)->frozenLayers[i].handle());
	}

	pFiler->writeUInt16(this->gridMajor());

	pFiler->writeVector2d(this->gridIncrement());

	pFiler->writeBool(this->isGridFollow());

	pFiler->writeBool(this->isGridSubdivisionRestricted());

	pFiler->writeBool(this->isGridAdaptive());

	pFiler->writeBool(this->isGridBoundToLimits());

	pFiler->writeBool(this->isGridOn());

	pFiler->writeUInt16(this->snapIsoPair());

	pFiler->writeVector2d(this->snapIncrement());

	pFiler->writePoint2d(this->snapBasePoint());

	pFiler->writeDouble(this->snapAngle());

	pFiler->writePoint3d(DB_IMP_VIEWPORT(this->m_pImpl)->origin);

	pFiler->writeVector3d(DB_IMP_VIEWPORT(this->m_pImpl)->xAxis);

	pFiler->writeVector3d(DB_IMP_VIEWPORT(this->m_pImpl)->yAxis);

	pFiler->writeDouble(this->elevation());

	return Acad::ErrorStatus::eOk;
}

bool DbViewport::subWorldDraw(GiWorldDraw* pWd) const {
	return true;
}






Acad::ErrorStatus DbViewport::subGetGeomExtents(DbExtents& extents) const
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::subTransformBy(const GeMatrix3d& xform)
{
	GePoint3d startPoint = DB_IMP_VIEWPORT(this->m_pImpl)->centerPoint;
	startPoint.x -= DB_IMP_VIEWPORT(this->m_pImpl)->width / 2.0;
	startPoint.y -= DB_IMP_VIEWPORT(this->m_pImpl)->height / 2.0;
	GePoint3d endPoint = DB_IMP_VIEWPORT(this->m_pImpl)->centerPoint;
	endPoint.x += DB_IMP_VIEWPORT(this->m_pImpl)->width / 2.0;
	endPoint.y += DB_IMP_VIEWPORT(this->m_pImpl)->height / 2.0;
	startPoint.transformBy(xform);
	endPoint.transformBy(xform);
	DB_IMP_VIEWPORT(this->m_pImpl)->centerPoint = startPoint + (endPoint - startPoint) / 2.0;
	DB_IMP_VIEWPORT(this->m_pImpl)->centerPoint.z = 0.0;
	DB_IMP_VIEWPORT(this->m_pImpl)->width = endPoint.x - startPoint.x;
	DB_IMP_VIEWPORT(this->m_pImpl)->height = endPoint.y - startPoint.y;
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::subGetGripPoints(
	DbGripDataPtrArray& grips,
	const double curViewUnitSize,
	const int gripSize,
	const GeVector3d& curViewDir,
	const int bitflags) const
{

	GePoint3d ps;

	DbGripData* gripData = new DbGripData();
	gripData->setGripPoint(this->centerPoint());
	grips.append(gripData);

	gripData = new DbGripData();
	ps = this->centerPoint();
	ps.x -= this->width() / 2.0;
	ps.y -= this->height() / 2.0;
	gripData->setGripPoint(ps);
	grips.append(gripData);

	gripData = new DbGripData();
	ps = this->centerPoint();
	ps.x += this->width() / 2.0;
	ps.y -= this->height() / 2.0;
	gripData->setGripPoint(ps);
	grips.append(gripData);

	gripData = new DbGripData();
	ps = this->centerPoint();
	ps.x += this->width() / 2.0;
	ps.y += this->height() / 2.0;
	gripData->setGripPoint(ps);
	grips.append(gripData);

	gripData = new DbGripData();
	ps = this->centerPoint();
	ps.x -= this->width() / 2.0;
	ps.y += this->height() / 2.0;
	gripData->setGripPoint(ps);
	grips.append(gripData);

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbViewport::subGetOsnapPoints(
	Db::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const GePoint3d& pickPoint,
	const GePoint3d& lastPoint,
	const GeMatrix3d& viewXform,
	GePoint3dArray& snapPoints,
	DbIntArray& geomIds) const
{

	if (osnapMode == Db::OsnapMode::kOsModeNear)
	{

	}
	else if (osnapMode == Db::OsnapMode::kOsModeMid)
	{

	}
	else if (osnapMode == Db::OsnapMode::kOsModeEnd)
	{

	}
	else if (osnapMode == Db::OsnapMode::kOsModePerp)
	{

	}

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbViewport::subMoveGripPointsAt(const DbIntArray& indices, const GeVector3d& offset)
{

	for (int i = 0; i < indices.length(); i++)
	{
		if (indices.at(i) == 0)
		{

		}
		else if (indices.at(i) == 1)
		{

		}
		else if (indices.at(i) == 2)
		{

		}
		else if (indices.at(i) == 3)
		{

		}
		else if (indices.at(i) == 4)
		{

		}
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::subMoveGripPointsAt(const DbVoidPtrArray& gripAppData, const GeVector3d& offset, const int bitflags)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewport::subIntersectWith(const DbEntity* pEnt, Db::Intersect intType, GePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const {

	if (pEnt == NULL) {
		return Acad::ErrorStatus::eFail;
	}

	if (DB_IMP_ENTITY(pEnt->m_pImpl)->isLine()) {
		//const DbLine* argLine = DbViewport::cast(pEnt);
		//if (intType == Db::Intersect::kOnBothOperands) {
		//    GeLineSeg3d line1(this->startPoint(), this->endPoint());
		//    GeLineSeg3d line2(argLine->startPoint(), argLine->endPoint());

		//    GePoint3d intersect;
		//    if (line1.intersectWith(line2, intersect) == true) {
		//        points.append(intersect);
		//    }
		//}
		//else if (intType == Db::Intersect::kExtendThis) {
		//    GeLine3d line1(this->startPoint(), this->endPoint());
		//    GeLineSeg3d line2(argLine->startPoint(), argLine->endPoint());

		//    GePoint3d intersect;
		//    if (line1.intersectWith(line2, intersect) == true) {
		//        points.append(intersect);
		//    }
		//}
		//else if (intType == Db::Intersect::kExtendArg) {
		//    GeLineSeg3d line1(this->startPoint(), this->endPoint());
		//    GeLine3d line2(argLine->startPoint(), argLine->endPoint());

		//    GePoint3d intersect;
		//    if (line1.intersectWith(line2, intersect) == true) {
		//        points.append(intersect);
		//    }
		//}
		//else if (intType == Db::Intersect::kExtendBoth) {
		//    GeLine3d line1(this->startPoint(), this->endPoint());
		//    GeLine3d line2(argLine->startPoint(), argLine->endPoint());

		//    GePoint3d intersect;
		//    if (line1.intersectWith(line2, intersect) == true) {
		//        points.append(intersect);
		//    }
		//}
	}
	else if (DB_IMP_ENTITY(pEnt->m_pImpl)->isCircle()) {
		//const DbCircle* argCircle = DbCircle::cast(pEnt);
		//if (intType == Db::Intersect::kOnBothOperands) {
		//    GeLineSeg3d line1(this->startPoint(), this->endPoint());
		//    GeCircArc3d circArc(argCircle->center(), argCircle->radius());

		//    GePoint3d intersect;
		//    if (line1.intersectWith(circArc, intersect) == true) {
		//        points.append(intersect);
		//    }
		//}
	}

	return Acad::ErrorStatus::eOk;

}







void DbViewport::close() {
	DbEntity::close();
}