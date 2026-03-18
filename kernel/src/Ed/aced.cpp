#include "aced.h"
#include "Db.h"
#include "DbLayout.h"
#include "DbViewport.h"
#include "DbObjectManager.h"
#include "DbLayoutManager.h"
#include "DbHostApplicationServices.h"
#include "DbViewportTable.h"
#include "DbViewportTableIterator.h"
#include "DbDictionaryIterator.h"
#include "kernel.h"
#include "DbImpl.h"



Acad::ErrorStatus acedMspace() {

	// 获得当前布局
	AcString activeLayoutName;
	acdbHostApplicationServices()->layoutManager()->getActiveLayoutName(activeLayoutName, true);

	// 判断当前是否是模型空间
	bool isModelSpace = true;
	if (activeLayoutName.isEqual("Model", true) == true)
	{
		isModelSpace = true;
	}
	else
	{
		isModelSpace = false;
	}

	// 如果是模型空间则退出
	if (isModelSpace == true) {
		return Acad::ErrorStatus::eFail;
	}

	// 获得布局词典
	DbDictionary* pLayoutDictionary = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayoutDictionary(pLayoutDictionary);

	//获得当前布局
	DbObjectId layoutId = NULL;
	DbDictionaryIterator* pDictionaryIterator = pLayoutDictionary->newIterator();
	for (; !pDictionaryIterator->done(); pDictionaryIterator->next()) {

		DbLayout* pLayout = NULL;
		if (pDictionaryIterator->getObject(pLayout) != 0) {
			continue;
		}

		if (pLayout->getTabSelected() == true) {
			layoutId = pLayout->objectId();
		}

		pLayout->close();
	}
	delete pDictionaryIterator;

	// 关闭布局词典
	pLayoutDictionary->close();

	// 获得当前激活视口所有视口
	DbObjectIdArray viewportArray;
	DbObjectId activationViewportId = NULL;
	if (layoutId != NULL) {
		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId) == Acad::ErrorStatus::eOk) {
			activationViewportId = DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId;
			viewportArray = pLayout->getViewportArray();
			pLayout->close();
		}
	}

	// 获得需要切换的视口
	DbObjectId viewportId = NULL;
	if (viewportArray.length() > 0) {
		for (int i = 0; i < viewportArray.length(); i++) {
			DbViewport* pViewport = NULL;
			if (::acdbOpenObject(pViewport, viewportArray[i]) == Acad::ErrorStatus::eOk) {
				if (pViewport->number() == 1 && activationViewportId == viewportArray[i]) {
					viewportId = NULL;
					pViewport->close();
					break;
				}
				else if(viewportId == NULL){
					viewportId = viewportArray[i];
					pViewport->close();
				}
			}
		}
	}

	// 设置视口
	DbViewport* pViewport = NULL;
	if (::acdbOpenObject(pViewport, viewportId, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {
		acedSetCurrentVPort(pViewport);
		pViewport->close();
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus acedPspace() {

	// 获得当前布局
	AcString activeLayoutName;
	acdbHostApplicationServices()->layoutManager()->getActiveLayoutName(activeLayoutName, true);

	// 判断当前是否是模型空间
	bool isModelSpace = true;
	if (activeLayoutName.isEqual("Model", true) == true)
	{
		isModelSpace = true;
	}
	else
	{
		isModelSpace = false;
	}

	// 如果是模型空间则退出
	if (isModelSpace == true) {
		return Acad::ErrorStatus::eFail;
	}

	// 获得当前图纸空间的视口id
	DbObjectId paperSpaceVportId = acdbHostApplicationServices()->workingDatabase()->paperSpaceVportId();

	// 设置视口
	DbViewport* pViewport = NULL;
	if (::acdbOpenObject(pViewport, paperSpaceVportId, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {
		acedSetCurrentVPort(pViewport);
		pViewport->close();
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus acedSetCurrentVPort(int vpnumber) {

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	// 获得当前布局
	AcString activeLayoutName;
	acdbHostApplicationServices()->layoutManager()->getActiveLayoutName(activeLayoutName, true);

	// 判断当前是否是模型空间
	bool isModelSpace = true;
	if (activeLayoutName.isEqual("Model", true) == true)
	{
		isModelSpace = true;
	}
	else
	{
		isModelSpace = false;
	}

	// 如果是模型空间则退出
	if (isModelSpace == true) {
		return Acad::ErrorStatus::eFail;
	}

	// 获得布局中所有视口和被激活视口
	DbLayout* pLayout = NULL;
	DbObjectIdArray viewportArray = NULL;
	if (::acdbOpenObject(pLayout, acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId()) == Acad::ErrorStatus::eOk) {
		
		// 获得所有视口
		viewportArray = pLayout->getViewportArray();

		// 设置视口是否激活
		for (int i = 0; i < viewportArray.length(); i++) {
			DbViewport* pViewport = NULL;
			if (::acdbOpenObject(pViewport, viewportArray[i]) == Acad::ErrorStatus::eOk) {
				if (pViewport->number() == vpnumber) {
					pViewport->subOpen(Db::OpenMode::kForWrite);
					DB_IMP_LAYOUT(pViewport->m_pImpl)->activationViewportId = viewportArray[i];
					es = Acad::ErrorStatus::eOk;
				}
				pViewport->close();
			}
		}

		// 关闭布局
		pLayout->close();
	}

	return es;
}
Acad::ErrorStatus acedSetCurrentVPort(const DbViewport* pVp) {

	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do {

		if (pVp == NULL) {
			break;
		}

		// 获得当前布局
		AcString activeLayoutName;
		acdbHostApplicationServices()->layoutManager()->getActiveLayoutName(activeLayoutName, true);

		// 判断当前是否是模型空间
		bool isModelSpace = true;
		if (activeLayoutName.isEqual("Model", true) == true)
		{
			isModelSpace = true;
		}
		else
		{
			isModelSpace = false;
		}

		// 如果是模型空间则退出
		if (isModelSpace == true) {
			break;
		}

		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId()) == Acad::ErrorStatus::eOk) {

			// 获得所有视口
			DbObjectIdArray viewportArray = pLayout->getViewportArray();

			// 判断视口是否存在
			for (int i = 0; i < viewportArray.length(); i++) {
				if (viewportArray[i] == pVp->objectId()) {
					DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId = pVp->objectId();
					es = Acad::ErrorStatus::eOk;
					break;
				}
			}

			pLayout->close();
		}

	} while (false);

	return es;
}


short acedGetCurVportCircleSides() {

	short val = 50;

	// 获得当前视口id
	DbObjectId id = acedActiveViewportId();

	bool isRecord = false;
	if (isRecord == false) {
		DbViewportTableRecord* pViewportTableRecord = NULL;
		if (::acdbOpenObject(pViewportTableRecord, id) == Acad::ErrorStatus::eOk) {
			val = pViewportTableRecord->circleSides();
			pViewportTableRecord->close();
			isRecord = true;
		}
	}
	if(isRecord == false) {
		DbViewport* pViewport = NULL;
		if (::acdbOpenObject(pViewport, id) == Acad::ErrorStatus::eOk) {
			val = pViewport->circleSides();
			pViewport->close();
			isRecord = true;
		}
	}

	return val;
}
Acad::ErrorStatus acedSetCurVportCircleSides(short value) {

	// 获得当前视口id
	DbObjectId id = acedActiveViewportId();

	bool isRecord = false;
	if (isRecord == false) {
		DbViewportTableRecord* pViewportTableRecord = NULL;
		if (::acdbOpenObject(pViewportTableRecord, id, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {
			pViewportTableRecord->setCircleSides(value);
			pViewportTableRecord->close();
			isRecord = true;
		}
	}
	if (isRecord == false) {
		DbViewport* pViewport = NULL;
		if (::acdbOpenObject(pViewport, id, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {
			pViewport->setCircleSides(value);
			pViewport->close();
			isRecord = true;
		}
	}
	
	if (isRecord == false) {
		return Acad::ErrorStatus::eFail;
	}
	return Acad::ErrorStatus::eOk;
}


Acad::ErrorStatus acedSetCurrentUCS(const GeMatrix3d& mat) {

	Acad::ErrorStatus error = Acad::ErrorStatus::eFail;

	DbObjectId activeViewportId = acedActiveViewportId();

	DbViewportTableRecord* pViewportTableRecord = NULL;
	if (::acdbOpenObject(pViewportTableRecord, activeViewportId, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {

		GePoint3d origin = mat.getCsOrigin();
		GeVector3d xAxis = mat.getCsXAxis();
		GeVector3d yAxis = mat.getCsYAxis();
		pViewportTableRecord->setUcs(origin, xAxis, yAxis);
		pViewportTableRecord->close();

		error = Acad::ErrorStatus::eOk;
	}
	else {
		DbViewport* pViewport = NULL;
		if (::acdbOpenObject(pViewport, activeViewportId, Db::OpenMode::kForWrite) == Acad::ErrorStatus::eOk) {

			GePoint3d origin = mat.getCsOrigin();
			GeVector3d xAxis = mat.getCsXAxis();
			GeVector3d yAxis = mat.getCsYAxis();
			pViewport->setUcs(origin, xAxis, yAxis);
			pViewport->close();

			error = Acad::ErrorStatus::eOk;
		}
	}

	return error;
}
Acad::ErrorStatus acedGetCurrentUCS(GeMatrix3d& mat) {

	Acad::ErrorStatus error = Acad::ErrorStatus::eFail;

	DbObjectId activeViewportId = acedActiveViewportId();

	DbObject* viewport = ::kernel()->acdbObjectManager()->openDbObject(activeViewportId);
	if (viewport == NULL) {
		return error;
	}

	if (viewport->isA() == DbViewportTableRecord::desc()) {
		DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)viewport;

		GePoint3d origin;
		GeVector3d xAxis;
		GeVector3d yAxis;
		GeVector3d zAxis;
		pViewportTableRecord->getUcs(origin, xAxis, yAxis);
		zAxis = xAxis.crossProduct(yAxis);

		mat.setToAlignCoordSys(
			GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
			origin, xAxis, yAxis, zAxis);

		error = Acad::ErrorStatus::eOk;
	}
	else if (viewport->isA() == DbViewport::desc()) {
		DbViewport* pViewport = (DbViewport*)viewport;

		GePoint3d origin;
		GeVector3d xAxis;
		GeVector3d yAxis;
		GeVector3d zAxis;
		pViewport->getUcs(origin, xAxis, yAxis);
		zAxis = xAxis.crossProduct(yAxis);

		mat.setToAlignCoordSys(
			GePoint3d::kOrigin, GeVector3d::kXAxis, GeVector3d::kYAxis, GeVector3d::kZAxis,
			origin, xAxis, yAxis, zAxis);
	}

	return error;
}


//  AutoCAD color to RGB conversion function
//
Adesk::UInt32     acedGetRGB(int color) {
	CmColor col;
	col.setColorIndex(color);
	return col.color();
}

Acad::ErrorStatus acedGetCurrentSelectionSet(DbObjectIdArray& sset) {
	sset.removeAll();
	//DbObjectIdArray selectionSet = ::kernel()->acedSelectionSetManager()->selectionSetEnts();
	//for (int i = 0; i < selectionSet.length(); i++) {
	//	sset.append(selectionSet[i]);
	//}
	return Acad::ErrorStatus::eOk;
}



// Get the current viewport id.  Returns null id if no vports open.
// The id returned will refer to a viewport entity (DbViewport) if
// the editor is in layout mode.  Otherwise, in tilemode, it will
// refer to an DbViewportTableRecord object.
//
DbObjectId acedActiveViewportId() {

	DbObjectId viewportId = NULL;

	do {
		// 获得当前布局
		DbObjectId layoutId = acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId();
		if (layoutId == NULL) {
			break;
		}

		// 获得被激活的视口
		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId) == Acad::ErrorStatus::eOk) {
			viewportId = DB_IMP_LAYOUT(pLayout->m_pImpl)->activationViewportId;
			pLayout->close();
		}

	} while (false);

	return viewportId;
}

// Converts viewport number into an id.  The id refers to a Viewport
// entity (in layout mode) or to a ViewportTableRecord (in tile mode).
// The int number is one returned by the CVPORT sysvar or by the
// DbViewport::number() method.
//
DbObjectId acedViewportIdFromNumber(int nVpNum) {

	DbObjectId id = NULL;

	do {
		//获得当前布局名称
		DbObjectId layoutId = acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId();
		if (layoutId == NULL) {
			break;
		}

		// 获得布局对象
		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId) != Acad::ErrorStatus::eOk) {
			break;
		}

		// 获得布局名称
		AcString layoutName;
		pLayout->getName(layoutName);

		if (layoutName.isEqual("Model", true) == false) {
			DbObjectIdArray ViewportArray = pLayout->getViewportArray();
			if (ViewportArray.length() > 0 && nVpNum < ViewportArray.length()) {
				id = ViewportArray[nVpNum];
			}
		}

		// 关闭布局
		pLayout->close();

	} while (false);

	return id;

}

// Returns the number of viewports in the current editor window.
// Includes the paperspace viewport, when we're in layout mode.
//
int acedNumberOfViewports() {

	int count = 0;

	do {
		//获得当前布局名称
		DbObjectId layoutId = acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId();
		if (layoutId == NULL) {
			break;
		}

		// 获得布局对象
		DbLayout* pLayout = NULL;
		if (::acdbOpenObject(pLayout, layoutId) != Acad::ErrorStatus::eOk) {
			break;
		}

		// 获得布局名称
		AcString layoutName;
		pLayout->getName(layoutName);

		if (layoutName == "Model") {
			DbViewportTable* pViewportTable = NULL;
			acdbHostApplicationServices()->workingDatabase()->getViewportTable(pViewportTable);

			DbViewportTableIterator* pViewportTableIterator = NULL;
			pViewportTable->newIterator(pViewportTableIterator);
			for (pViewportTableIterator->start(); !pViewportTableIterator->done(); pViewportTableIterator->step()) {
				count++;
			}
			delete pViewportTableIterator;

			pViewportTable->close();
		}
		else {
			count = pLayout->getViewportArray().length();
		}

		// 关闭布局
		pLayout->close();

	} while (false);

	return count;
}

// Get the current viewport ObjectId for the current layout's viewport.
// Returns null id in tile mode.  Note that acedActiveViewportId()
// has the same functionality as this when in layout mode.
DbObjectId acedGetCurViewportObjectId() {
	DbObjectId id = acedActiveViewportId();
	DbObject* object = NULL;
	if (acdbOpenObject(object, id) == Acad::ErrorStatus::eOk) {
		if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == false) {
			id = NULL;
		}
		object->close();
	}
	return id;
}

