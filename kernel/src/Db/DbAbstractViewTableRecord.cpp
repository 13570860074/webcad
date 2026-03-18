#include "DbAbstractViewTableRecord.h"
#include "acadstrc.h"
#include "GeMatrix3d.h"
#include "DbUCSTableRecord.h"
#include "EdUserInputManager.h"
#include "DbDwgFiler.h"
#include "kernel.h"
#include "DbImpl.h"


DbAbstractViewTableRecord::DbAbstractViewTableRecord()
{
	this->m_pImpl = new DbAbstractViewTableRecordImpl();
}
DbAbstractViewTableRecord::~DbAbstractViewTableRecord()
{

}



Acad::ErrorStatus DbAbstractViewTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	double height = 0.0;
	pFiler->readDouble(&height);
	this->setHeight(height);

	double width = 0.0;
	pFiler->readDouble(&width);
	this->setWidth(width);

	GePoint2d centerPoint;
	pFiler->readPoint2d(&centerPoint);
	this->setCenterPoint(centerPoint);

	GePoint3d target;
	pFiler->readPoint3d(&target);
	this->setTarget(target);

	GeVector3d viewDirection;
	pFiler->readVector3d(&viewDirection);
	this->setViewDirection(viewDirection);

	double viewTwist = 0.0;
	pFiler->readDouble(&viewTwist);
	this->setViewTwist(viewTwist);

	double lensLength = 0.0;
	pFiler->readDouble(&lensLength);
	this->setLensLength(lensLength);

	double frontClipDistance = 0.0;
	pFiler->readDouble(&frontClipDistance);
	this->setFrontClipDistance(frontClipDistance);

	double backClipDistance = 0.0;
	pFiler->readDouble(&backClipDistance);
	this->setBackClipDistance(backClipDistance);

	bool perspectiveEnabled = false;
	pFiler->readBoolean(&perspectiveEnabled);
	this->setPerspectiveEnabled(perspectiveEnabled);

	bool frontClipEnabled = false;
	pFiler->readBoolean(&frontClipEnabled);
	this->setFrontClipEnabled(frontClipEnabled);

	bool backClipEnabled = false;
	pFiler->readBoolean(&backClipEnabled);
	this->setBackClipEnabled(backClipEnabled);

	bool frontClipAtEye = false;
	pFiler->readBoolean(&frontClipAtEye);
	this->setFrontClipAtEye(frontClipAtEye);


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbAbstractViewTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}

	pFiler->writeDouble(this->height());
	pFiler->writeDouble(this->width());
	pFiler->writePoint2d(this->centerPoint());
	pFiler->writePoint3d(this->target());
	pFiler->writeVector3d(this->viewDirection());
	pFiler->writeDouble(this->viewTwist());
	pFiler->writeDouble(this->lensLength());
	pFiler->writeDouble(this->frontClipDistance());
	pFiler->writeDouble(this->backClipDistance());
	pFiler->writeBoolean(this->perspectiveEnabled());
	pFiler->writeBoolean(this->frontClipEnabled());
	pFiler->writeBoolean(this->backClipEnabled());
	pFiler->writeBoolean(this->frontClipAtEye());
	
	return Acad::ErrorStatus::eOk;
}




GePoint2d DbAbstractViewTableRecord::centerPoint() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->centerPoint;
}
void DbAbstractViewTableRecord::setCenterPoint(const GePoint2d& centerPoint) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->centerPoint = centerPoint;
}

double DbAbstractViewTableRecord::height() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->height;
}
void DbAbstractViewTableRecord::setHeight(double height) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->height = height;
}

double DbAbstractViewTableRecord::width() const {
	return this->height() * ::acedUserInputManager()->screenAspect();
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->width;
}
void DbAbstractViewTableRecord::setWidth(double width) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->width = width;
}

GePoint3d DbAbstractViewTableRecord::target() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->target;
}
void DbAbstractViewTableRecord::setTarget(const GePoint3d& target) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->target = target;
}

GeVector3d DbAbstractViewTableRecord::viewDirection() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection;
}
void DbAbstractViewTableRecord::setViewDirection(const GeVector3d& viewDirection) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = viewDirection;
}

double DbAbstractViewTableRecord::viewTwist() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->twistAngle;
}
void DbAbstractViewTableRecord::setViewTwist(double angle) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->twistAngle = angle;
}

double DbAbstractViewTableRecord::lensLength() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->lensLength;
}
void DbAbstractViewTableRecord::setLensLength(double length) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->lensLength = length;
}

double DbAbstractViewTableRecord::frontClipDistance() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->frontClipDistance;
}
void DbAbstractViewTableRecord::setFrontClipDistance(double distance) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->frontClipDistance = distance;
}

double DbAbstractViewTableRecord::backClipDistance() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->backClipDistance;
}
void DbAbstractViewTableRecord::setBackClipDistance(double distance) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->backClipDistance = distance;
}

bool DbAbstractViewTableRecord::perspectiveEnabled() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isPerspectiveEnabled;
}
void DbAbstractViewTableRecord::setPerspectiveEnabled(bool enabled) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isPerspectiveEnabled = enabled;
}

bool DbAbstractViewTableRecord::frontClipEnabled() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isFrontClipEnabled;
}
void DbAbstractViewTableRecord::setFrontClipEnabled(bool enabled) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isFrontClipEnabled = enabled;
}

bool DbAbstractViewTableRecord::backClipEnabled() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isBackClipEnabled;
}
void DbAbstractViewTableRecord::setBackClipEnabled(bool enabled) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isBackClipEnabled = enabled;
}

bool DbAbstractViewTableRecord::frontClipAtEye() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isFrontClipAtEye;
}
void DbAbstractViewTableRecord::setFrontClipAtEye(bool atEye) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->isFrontClipAtEye = atEye;
}

// Background
const DbObjectId& DbAbstractViewTableRecord::background() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->background;
}
DbObjectId& DbAbstractViewTableRecord::background() {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->background;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setBackground(const DbObjectId& backgroundId) {
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->background = backgroundId;
	return Acad::ErrorStatus::eOk;
}

// VisualStyle
DbObjectId DbAbstractViewTableRecord::visualStyle() const {
	return NULL;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setVisualStyle(const DbObjectId& visualStyleId) {
	return Acad::ErrorStatus::eFail;
}


// Brightness controls the relative intensity of lights.
double DbAbstractViewTableRecord::brightness() const {
	return 0.0;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setBrightness(double) {
	return Acad::ErrorStatus::eFail;
}
// Contrast controls intensity of ambient light, relative to other lights.
double DbAbstractViewTableRecord::contrast() const {
	return 0.0;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setContrast(double) {
	return Acad::ErrorStatus::eFail;
}

CmColor DbAbstractViewTableRecord::ambientLightColor() const {
	return CmColor();
}
Acad::ErrorStatus DbAbstractViewTableRecord::setAmbientLightColor(const CmColor& clr) {
	return Acad::ErrorStatus::eFail;
}

// A single sun (distant light) can be associated with each viewport.
DbObjectId DbAbstractViewTableRecord::sunId() const {
	return NULL;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setSun(DbObjectId& retId, DbObject* pSun) {
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus setSun(DbObjectId& retId, DbObject* pSun, bool eraseOldSun) {
	return Acad::ErrorStatus::eFail;
}

// UCS query methods.
//
Acad::ErrorStatus DbAbstractViewTableRecord::getUcs(GePoint3d& origin, GeVector3d& xAxis, GeVector3d& yAxis) const {
	origin = DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->origin;
	xAxis = DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis;
	yAxis = DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis;
	return Acad::ErrorStatus::eOk;
}
bool DbAbstractViewTableRecord::isUcsOrthographic(Db::OrthographicView& view) const {
	view = (Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView;
	if ((Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView == Db::OrthographicView::kTopView ||
		(Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView == Db::OrthographicView::kBottomView ||
		(Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView == Db::OrthographicView::kFrontView ||
		(Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView == Db::OrthographicView::kBackView ||
		(Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView == Db::OrthographicView::kLeftView ||
		(Db::OrthographicView)DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView == Db::OrthographicView::kRightView) {
		return true;
	}
	return false;
}
DbObjectId DbAbstractViewTableRecord::ucsName() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->ucsId;
}
double DbAbstractViewTableRecord::elevation() const {
	return DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->elevation;
}

// UCS set methods.
//
Acad::ErrorStatus DbAbstractViewTableRecord::setUcs(const GePoint3d& origin, const GeVector3d& xAxis, const GeVector3d& yAxis) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->origin = origin;
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = xAxis;
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = yAxis;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setUcs(Db::OrthographicView view) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->orthographicView = view;
		if (view == Db::OrthographicView::kTopView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d(1, 0, 0);
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d(0, 1, 0);
		}
		else if (view == Db::OrthographicView::kBottomView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d(1, 0, 0);
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d(0, -1, 0);
		}
		else if (view == Db::OrthographicView::kLeftView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d(0, 1, 0);
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kRightView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d(0, -1, 0);
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kFrontView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d(-1, 0, 0);
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kBackView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d(1, 0, 0);
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d(0, 0, 1);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setUcs(const DbObjectId& ucsId) {

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
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->ucsId = ucsId;

	//设置轴
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->origin = pUCSTableRecord->origin();
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = pUCSTableRecord->xAxis();
	DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = pUCSTableRecord->yAxis();

	pUCSTableRecord->close();

	return Acad::ErrorStatus::eOk;

}
Acad::ErrorStatus DbAbstractViewTableRecord::setUcsToWorld() {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->origin = GePoint3d::kOrigin;
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->xAxis = GeVector3d::kXAxis;
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->yAxis = GeVector3d::kYAxis;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setElevation(double elev) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->elevation = elev;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

// Orthographic view methods.
//
bool DbAbstractViewTableRecord::isViewOrthographic(Db::OrthographicView& view) const {

	if (DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection.isEqualTo(GeVector3d(0, 0, 1)) == true) {
		view = Db::OrthographicView::kTopView;
		return true;
	}
	else if (DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection.isEqualTo(GeVector3d(0, 0, -1)) == true) {
		view = Db::OrthographicView::kBackView;
		return true;
	}
	else if (DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection.isEqualTo(GeVector3d(0, -1, 0)) == true) {
		view = Db::OrthographicView::kFrontView;
		return true;
	}
	else if (DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection.isEqualTo(GeVector3d(0, 1, 0)) == true) {
		view = Db::OrthographicView::kBackView;
		return true;
	}
	else if (DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection.isEqualTo(GeVector3d(-1, 0, 0)) == true) {
		view = Db::OrthographicView::kLeftView;
		return true;
	}
	else if (DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection.isEqualTo(GeVector3d(1, 0, 0)) == true) {
		view = Db::OrthographicView::kRightView;
		return true;
	}
	view = Db::OrthographicView::kNonOrthoView;
	return false;
}
Acad::ErrorStatus DbAbstractViewTableRecord::setViewDirection(Db::OrthographicView view) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (view == Db::OrthographicView::kTopView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = GeVector3d(0, 0, 1);
		}
		else if (view == Db::OrthographicView::kBottomView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = GeVector3d(0, 0, -1);
		}
		else if (view == Db::OrthographicView::kLeftView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = GeVector3d(-1, 0, 0);
		}
		else if (view == Db::OrthographicView::kRightView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = GeVector3d(1, 0, 0);
		}
		else if (view == Db::OrthographicView::kFrontView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = GeVector3d(0, -1, 0);
		}
		else if (view == Db::OrthographicView::kBackView) {
			DB_IMP_ABSTRACTVIEWTABLERECORD(this->m_pImpl)->viewDirection = GeVector3d(0, 1, 0);
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
