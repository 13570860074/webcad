#include "RxSystemVariableManager.h"
#include "EvEventManager.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "RxImpl.h"
#include "DbImpl.h"

RxSystemVariableManager::RxSystemVariableManager()
{
	this->m_pImpl = new RxSystemVariableManagerImpl();
}
RxSystemVariableManager::~RxSystemVariableManager()
{
}

void RxSystemVariableManager::get_viewHeight(resbuf*& val) {
	val->restype = Db::kDxfXdReal;

	DbObject* object = NULL;
	if (::acdbOpenObject(object, acedActiveViewportId(), Db::OpenMode::kForRead) == Acad::ErrorStatus::eOk)
	{
		if (DB_IMP_OBJECT(object->m_pImpl)->isViewportTableRecord() == true) {
			DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)object;
			val->resval.rreal = pViewportTableRecord->height();
		}
		else if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == true) {
			DbViewport* pViewport = (DbViewport*)object;
			val->resval.rreal = pViewport->viewHeight();
		}
		object->close();
	}
}
Acad::ErrorStatus RxSystemVariableManager::set_viewHeight(const resbuf* val)
{
	DbObject* object = NULL;
	if (::acdbOpenObject(object, acedActiveViewportId(), Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk)
	{
		if (DB_IMP_OBJECT(object->m_pImpl)->isViewportTableRecord() == true) {
			DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)object;
			pViewportTableRecord->setHeight(val->resval.rreal);
		}
		else if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == true) {
			DbViewport* pViewport = (DbViewport*)object;
			pViewport->setViewHeight(val->resval.rreal);
		}
		object->close();
	}
	return Acad::eOk;
}
void RxSystemVariableManager::get_ctab(resbuf*& val)
{
	std::string layoutName = acdbHostApplicationServices()->layoutManager()->findActiveLayout(true);

	val->restype = Db::kDxfXdAsciiString;
	acutNewString(layoutName.c_str(), val->resval.rstring);
}
Acad::ErrorStatus RxSystemVariableManager::set_ctab(const resbuf* val)
{
	return acdbHostApplicationServices()->layoutManager()->setCurrentLayout(val->resval.rstring);
}

void RxSystemVariableManager::get_tilemode(resbuf*& val)
{
	AcString layoutName;
	acdbHostApplicationServices()->layoutManager()->getActiveLayoutName(layoutName, true);

	val->restype = Db::kDxfInt8;
	if (layoutName.isEqual("Model", true) == true) {
		val->resval.rint = 1;
	}
	else {
		val->resval.rint = 0;
	}
}
Acad::ErrorStatus RxSystemVariableManager::set_tilemode(const resbuf* val)
{
	return acdbHostApplicationServices()->layoutManager()->setCurrentLayout(val->resval.rint == 1 ? "Model" : "Layout1");
}

void RxSystemVariableManager::get_viewctr(resbuf*& val)
{
	val->restype = Db::kDwg3Real;

	// 获得当前激活视口
	DbObjectId activeViewportId = acedActiveViewportId();

	// 打开实体
	DbObject* viewport = NULL;
	if (::acdbOpenObject(viewport, activeViewportId) == Acad::ErrorStatus::eOk)
	{
		DbViewportTableRecord* viewportTableRecord = DbViewportTableRecord::cast(viewport);
		if (viewportTableRecord != NULL) {
			GePoint2d centerPoint = viewportTableRecord->centerPoint();

			val->resval.rpoint[0] = centerPoint.x;
			val->resval.rpoint[1] = centerPoint.y;
			val->resval.rpoint[2] = 0.0;
		}

		DbViewport* pViewport = DbViewport::cast(viewport);
		if (pViewport != NULL) {
			GePoint2d centerPoint = pViewport->viewCenter();

			val->resval.rpoint[0] = centerPoint.x;
			val->resval.rpoint[1] = centerPoint.y;
			val->resval.rpoint[2] = 0.0;
		}

		// 关闭对象
		viewport->close();
	}
}
Acad::ErrorStatus RxSystemVariableManager::set_viewctr(const resbuf* val)
{
	// 获得当前激活视口
	DbObjectId activeViewportId = acedActiveViewportId();

	// 打开实体
	DbObject* viewport = NULL;
	if (::acdbOpenObject(viewport, activeViewportId, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk)
	{
		DbViewportTableRecord* viewportTableRecord = DbViewportTableRecord::cast(viewport);
		if (viewportTableRecord != NULL)
		{
			viewportTableRecord->setCenterPoint(GePoint2d(val->resval.rpoint[0], val->resval.rpoint[1]));
		}

		DbViewport* pViewport = DbViewport::cast(viewport);
		if (pViewport != NULL)
		{
			pViewport->setViewCenter(GePoint2d(val->resval.rpoint[0], val->resval.rpoint[1]));
		}

		// 关闭对象
		viewport->close();
	}

	return Acad::ErrorStatus::eOk;
}
void RxSystemVariableManager::get_plineWid(resbuf*& val) {
	val->restype = Db::kDwgReal;
	val->resval.rreal = 0.0;
	DbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	if (db == NULL) {
		return;
	}
	val->resval.rreal = db->plinewid();
}
Acad::ErrorStatus RxSystemVariableManager::set_plineWid(const resbuf* val) {
	DbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	if (db == NULL) {
		return Acad::ErrorStatus::eFail;
	}
	db->setPlinewid(val->resval.rreal);
	return Acad::ErrorStatus::eOk;
}
void RxSystemVariableManager::get_lastPoint(resbuf*& val) {

}
Acad::ErrorStatus RxSystemVariableManager::set_lastPoint(const resbuf* val) {
	return Acad::ErrorStatus::eOk;
}

void RxSystemVariableManager::onInit()
{

	resbuf* val = NULL;
	RxSystemVariable* variable = NULL;

	// 版本
	variable = new RxSystemVariable();
	variable->setName("_VERNUM");
	val = acutNewRb(Db::kDxfXdAsciiString);
	val->resval.rstring = "1.0.0";
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("_VERNUM", variable);

	// 视口尺寸
	variable = new RxSystemVariable();
	variable->setName("VIEWSIZE");
	variable->on_get(RxSystemVariableManager::get_viewHeight);
	variable->on_set(RxSystemVariableManager::set_viewHeight);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("VIEWSIZE", variable);

	// 视口中心点
	variable = new RxSystemVariable();
	variable->setName("VIEWCTR");
	variable->on_get(RxSystemVariableManager::get_viewctr);
	variable->on_set(RxSystemVariableManager::set_viewctr);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("VIEWCTR", variable);

	// 当前默认关键字(acedInitGet输入)
	variable = new RxSystemVariable();
	variable->setName("ADSJIGKWORD");
	val = acutNewRb(Db::kDxfXdAsciiString);
	val->resval.rstring = "";
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("ADSJIGKWORD", variable);

	// 当前JIG输入
	variable = new RxSystemVariable();
	variable->setName("ADSJIGINPUT");
	val = acutNewRb(Db::kDxfXdAsciiString);
	val->resval.rstring = "";
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("ADSJIGINPUT", variable);

	// 命令行可见性
	variable = new RxSystemVariable();
	variable->setName("COMMANDLINEVISIBILITY");
	val = acutNewRb(Db::kDwgInt8);
	val->resval.rint = 1;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("COMMANDLINEVISIBILITY", variable);

	// 当前空间名称
	variable = new RxSystemVariable();
	variable->setName("CTAB");
	variable->on_get(RxSystemVariableManager::get_ctab);
	variable->on_set(RxSystemVariableManager::set_ctab);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("CTAB", variable);

	// 是否是模型空间
	variable = new RxSystemVariable();
	variable->setName("TILEMODE");
	variable->on_get(RxSystemVariableManager::get_tilemode);
	variable->on_set(RxSystemVariableManager::set_tilemode);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("TILEMODE", variable);

	// 光标样式
	variable = new RxSystemVariable();
	variable->setName("CORSORSTYLE");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 0;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("CORSORSTYLE", variable);

	// 拾取框尺寸
	variable = new RxSystemVariable();
	variable->setName("PICKBOX");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 6;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("PICKBOX", variable);

	// 靶框尺寸
	variable = new RxSystemVariable();
	variable->setName("APERTURE");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 10;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("APERTURE", variable);

	// 十字光标尺寸
	variable = new RxSystemVariable();
	variable->setName("CURSORSIZE");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 5;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("CURSORSIZE", variable);

	// 夹点尺寸
	variable = new RxSystemVariable();
	variable->setName("GRIPSIZE");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 5;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("GRIPSIZE", variable);

	// 触控笔模式(0表无模式,1表智能绘图,2表拾取,3表删除)
	variable = new RxSystemVariable();
	variable->setName("TOUCHPENMODE");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 1;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("TOUCHPENMODE", variable);

	// 是否开启正交
	variable = new RxSystemVariable();
	variable->setName("ORTHO");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 0;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("ORTHO", variable);

	// 存储上一个输入点
	variable = new RxSystemVariable();
	variable->setName("LASTPOINT");
	val = acutNewRb(Db::DwgDataType::kDwg3Real);
	val->resval.rpoint[0] = 0;
	val->resval.rpoint[1] = 0;
	val->resval.rpoint[2] = 0;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("LASTPOINT", variable);

	// 是否多段线默认宽度
	variable = new RxSystemVariable();
	variable->setName("PLINEWID");
	variable->on_get(RxSystemVariableManager::get_plineWid);
	variable->on_set(RxSystemVariableManager::set_plineWid);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("PLINEWID", variable);

	/*
	0:关闭
	1:打开
	*/
	// 打开或关闭动态提示
	variable = new RxSystemVariable();
	variable->setName("DYNPROMPT");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 1;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("DYNPROMPT", variable);

	/*
	0.关闭所有动态输入功能（包括动态提示）
	1.打开指针输入
	2.打开标注输入
	3.同时打开指针和标注输入
	负值表示同时关闭动态提示和动态输入
	*/
	// 动态输入模式
	variable = new RxSystemVariable();
	variable->setName("DYNMODE");
	val = acutNewRb(Db::DwgDataType::kDwgInt8);
	val->resval.rint = 3;
	variable->setVariable(val);
	acutRelRb(val);
	RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.insert("DYNMODE", variable);
}

RxSystemVariable* RxSystemVariableManager::get(const AcString& _name) {
	return RX_IMP_SYSTEMVARIABLEMANAGER(this->m_pImpl)->recordMaps.find(_name.makeUpper());
}