#include "ReRendererManager.h"
#include "ReRendererReactor.h"
#include "EvEventManager.h"
#include "ApDocManager.h"
#include "RxManager.h"
#include "RxClassManager.h"
#include "RxAsyncManager.h"
#include "RxManagerDictionary.h"
#include "DbObject.h"
#include "DbLayout.h"
#include "DbLayoutManager.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbViewportTableIterator.h"
#include "DbHostApplicationServices.h"
#include "GiMaterial.h"
#include "GiObjectManager.h"
#include "GiMaterialManager.h"
#include "EdUserInputManager.h"
#include "AcDebug.h"
#include "AcSystem.h"
#include "kernel.h"
#include "DbImpl.h"
#include "ReImpl.h"
#include "GiImpl.h"
#include "aced.h"



/*
*
* 1:定时器模式
* 2:wasm中emscripten_set_main_loop产生的循环模式
* 3.手动定义的循环模式
/
/* 渲染模式 */
#define RENDERERMODE 3




/* 定时器循环模式 */
#if (RENDERERMODE == 1 && EMSDK)

/* 定时器循环的回调函数 */
EM_PORT_API(void) callbackRendererInterval() {
	::kernel()->acreRendererManager()->animationFrame();
}
/* 启动定时器循环 */
EM_JS(void, CallbackRendererInterval, (), {
	setInterval(function() {
		Module._callbackRendererInterval();
	}, 10);
	});

#endif


/* wasm中emscripten_set_main_loop产生的循环模式 */
#if (RENDERERMODE == 2 && EMSDK)


/* emscripten_set_main_loop回调 */
void main_loop(void) {
	::kernel()->acreRendererManager()->animationFrame();
}
/* 加载emscripten_set_main_loop */
EM_PORT_API(void) callbackRendererTimeout() {
	emscripten_set_main_loop(main_loop, 0, 1);
}
/* 启动循环 */
EM_JS(void, CallbackRendererTimeout, (), {
	setTimeout(function() {
		Module._callbackRendererTimeout();
	}, 100);
	});

#endif


void ReRendererManager::onBeginDrag(ReRendererManager* pointer) {

}
void ReRendererManager::onEndDrag(ReRendererManager* pointer) {

}

void ReRendererManager::onViewChanged(ReRendererManager* pointer)
{

}
void ReRendererManager::onAsyncBegin(ReRendererManager* pointer)
{

}
void ReRendererManager::onAsyncEnd(ReRendererManager* pointer)
{

}
void ReRendererManager::onAsyncAwaitBegin(ReRendererManager* pointer)
{
	RE_IMP_RENDERERMANAGER(pointer->m_pImpl)->isAsyncAwait = true;
}
void ReRendererManager::onAsyncAwaitEnd(ReRendererManager* pointer)
{
	RE_IMP_RENDERERMANAGER(pointer->m_pImpl)->isAsyncAwait = false;

	// 触发事件
	AcEventTrigger::emitAllEvent();

	// 触发渲染事件
	::kernel()->acreRendererManager()->animationFrame();
}

void ReRendererManager::onInit()
{
	//::kernel()->acevEventManager()->on(Ev::Drag::kBeginDrag, (void*)ReRendererManager::onBeginDrag, this);
	//::kernel()->acevEventManager()->on(Ev::Drag::kEndDrag, (void*)ReRendererManager::onEndDrag, this);

	//::kernel()->acevEventManager()->on(Ev::EditorReactor::kViewChanged, (void*)ReRendererManager::onViewChanged, this);

	::kernel()->acevEventManager()->on(Ev::Async::kAsyncBegin, (void*)ReRendererManager::onAsyncBegin, this);
	::kernel()->acevEventManager()->on(Ev::Async::kAsyncEnd, (void*)ReRendererManager::onAsyncEnd, this);
	::kernel()->acevEventManager()->on(Ev::Async::kAsyncAwaitBegin, (void*)ReRendererManager::onAsyncAwaitBegin, this);
	::kernel()->acevEventManager()->on(Ev::Async::kAsyncAwaitEnd, (void*)ReRendererManager::onAsyncAwaitEnd, this);
}

ReRendererManager::ReRendererManager()
{
	this->m_pImpl = new ReRendererManagerImpl();
}
ReRendererManager::~ReRendererManager()
{
}

void ReRendererManager::addReactor(ReRendererReactor* pReactor)
{
	RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.append(pReactor);
}
void ReRendererManager::removeReactor(ReRendererReactor* reactor) {
	for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++) {
		if (RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i] == reactor) {
			realloc_arry_at(RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors, i);
			break;
		}
	}
}
ReRendererManager::ERendererMode ReRendererManager::rendererMode() {
	return ReRendererManager::ERendererMode::kAllViewport;
}
void ReRendererManager::setRendererMode(ReRendererManager::ERendererMode mode) {

}


void ReRendererManager::renderer() {


	// 删除渲染对象
	realloc_arry_all(RE_IMP_RENDERERMANAGER(this->m_pImpl)->renderers);

	// 渲染
	this->rendererViewports();
}
void ReRendererManager::animationFrame() {

	/** 如果在异步等待种则中断异步 */
	if (RE_IMP_RENDERERMANAGER(this->m_pImpl)->isAsyncAwait == true) {
		::kernel()->acrxAsyncManager()->resolve();
		return;
	}

	/* 获得当前被激活视口 */
	DbObjectId activeViewportId = acedActiveViewportId();

	/* 触发切换视口事件 */
	if (activeViewportId != RE_IMP_RENDERERMANAGER(this->m_pImpl)->lastActiveViewportId) {
		::kernel()->acevEventManager()->_emit(Ev::Viewport::kSwitchViewport, activeViewportId);
		RE_IMP_RENDERERMANAGER(this->m_pImpl)->lastActiveViewportId = activeViewportId;
	}

	/** 触发帧开始事件 */
	for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++) {
		RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->beginAnimationFrame();
	}

	/* 渲染 */
	if (true)
	{
		// 触发渲染循环开始反应器
		for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++)
		{
			RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->beginRenderer();
		}

		// 渲染
		this->renderer();

		// 触发渲染循环完成反应器
		for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++)
		{
			RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->endRenderer();
		}
	}

	/** 触发帧结束事件 */
	for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++) {
		RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->endAnimationFrame();
	}
}

/**
* 1.有实体被修改则渲染所有视口
* 2.有视口被切换则渲染所有视口
* 3.在图纸空间中视口变化渲染所有视口
*/
void ReRendererManager::rendererWorkViewport() {

#if 0

	// 如果视口有修改同时当前视口是图纸空间则设置所有视口渲染
	if (RE_IMP_RENDERERMANAGER(this->m_pImpl)->isAllRendererViewports == false &&
		RE_IMP_RENDERERMANAGER(this->m_pImpl)->isViewChanged == true) {
		RE_IMP_RENDERERMANAGER(this->m_pImpl)->isAllRendererViewports = true;
	}

	if (RE_IMP_RENDERERMANAGER(this->m_pImpl)->isAllRendererViewports == true) {
		this->rendererViewports();
	}
	else {

		// 触发清空工作视口事件
		for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++)
		{
			RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.at(i)->clearWorkCanvas();
		}

		// 获得块表记录
		DbLayout* pLayout = NULL;
		DbObjectId blockTableRecordId = NULL;
		if (::acdbOpenObject(pLayout, acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId()) == Acad::ErrorStatus::eOk) {
			blockTableRecordId = pLayout->getBlockTableRecordId();
			pLayout->close();
		}

		// 获得激活的视口
		DbObjectId activeViewportId = acedActiveViewportId();

		// 打开视口
		DbObject* object = NULL;
		if (::acdbOpenObject(object, activeViewportId) == Acad::ErrorStatus::eOk) {

			// 视口渲染
			this->rendererViewport(object, activeViewportId, blockTableRecordId);

			// 关闭视口
			object->close();
		}
	}

#endif
}
void ReRendererManager::rendererViewports()
{

	// 触发空事件清理布局
	for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++)
	{
		RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.at(i)->clearCanvas();
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

	// 获得激活的视口
	DbObjectId activeViewportId = acedActiveViewportId();

	// 设置渲染数据
	if (isModelSpace == true)
	{

		// 获得布局的块表记录
		DbLayout* pLayout = NULL;
		DbObjectId blockTableRecordId = NULL;
		if (::acdbOpenObject(pLayout, acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId()) == Acad::ErrorStatus::eOk) {
			blockTableRecordId = pLayout->getBlockTableRecordId();
			pLayout->close();
		}

		// 获得视口表
		DbViewportTable* pViewportTable = NULL;
		acdbHostApplicationServices()->workingDatabase()->getViewportTable(pViewportTable);

		// 遍历视口
		DbViewportTableIterator* pViewportTableIterator = NULL;
		pViewportTable->newIterator(pViewportTableIterator);
		for (pViewportTableIterator->start(); !pViewportTableIterator->done(); pViewportTableIterator->step())
		{
			DbViewportTableRecord* pViewportTableRecord = NULL;
			if (pViewportTableIterator->getRecord(pViewportTableRecord, Db::OpenMode::kForRead) != 0)
			{
				continue;
			}

			this->rendererViewport(pViewportTableRecord, activeViewportId, blockTableRecordId);

			pViewportTableRecord->close();
		}
		delete pViewportTableIterator;

		// 关闭视口
		pViewportTable->close();
	}
	else
	{
#if 0
		// 获得当前布局id
		DbObjectId layoutId = acdbHostApplicationServices()->layoutManager()->getActiveLayoutBTRId();

		// 打开布局
		DbLayout* pLayout = NULL;
		::acdbOpenObject(pLayout, layoutId);

		// 获得所有视口
		DbObjectIdArray viewportIds = pLayout->getViewportArray();

		// 获得块表记录id
		DbObjectId blockTableRecordId = pLayout->getBlockTableRecordId();

		// 如果没有找到视口则创建默认的视口
		if (viewportIds.length() <= 0)
		{
			// 打开块表记录
			DbBlockTableRecord* pBlockTableRecord = NULL;
			if (::acdbOpenObject(pBlockTableRecord, blockTableRecordId, Db::OpenMode::kForWrite) == 0)
			{

				DbObjectId viewportId = NULL;

				// 创建布局视口
				DbViewport* pViewport = new DbViewport();
				pViewport->setOn();
				pViewport->setCenterPoint(GePoint3d());
				pBlockTableRecord->appendDbEntity(viewportId, pViewport);
				pViewport->close();
				viewportIds.append(viewportId);

				// 创建默认视口
				pViewport = new DbViewport();
				pViewport->setCenterPoint(GePoint3d(5.25, 4.0, 0.0));
				pViewport->setViewHeight(9.226);
				pViewport->setViewCenter(GePoint2d(6.0, 4.5));
				pBlockTableRecord->appendDbEntity(viewportId, pViewport);
				pViewport->close();
				viewportIds.append(viewportId);

				pBlockTableRecord->close();
			}
		}

		// 关闭布局
		pLayout->close();

		// 打开所有视口对象
		for (int i = 0; i < viewportIds.length(); i++)
		{
			DbViewport* pViewport = NULL;
			if (::acdbOpenObject(pViewport, viewportIds[i], Db::OpenMode::kForRead) == 0)
			{
				this->rendererViewport(pViewport, activeViewportId, blockTableRecordId);
				pViewport->close();
			}
		}

#endif
	}

}
void ReRendererManager::rendererViewport(DbObject* object, const DbObjectId& activeViewportId, const DbObjectId& blockTableRecordId) {

#if 1
	do {

		if (DB_IMP_OBJECT(object->m_pImpl)->isViewportTableRecord() == true) {

			DbViewportTableRecord* viewportTableRecord = (DbViewportTableRecord*)object;

			// 触发渲染视口开始反应器
			for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++) {
				RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->beginRendererViewport(viewportTableRecord);
			}

			// 获得视口左下和右上角
			GePoint2d lowerLeftCorner, upperRightCorner;
			lowerLeftCorner = viewportTableRecord->lowerLeftCorner();
			upperRightCorner = viewportTableRecord->upperRightCorner();

			// 获得视图高度
			double viewHeight = viewportTableRecord->height();
			double viewWidth = viewHeight * ::kernel()->acedUserInputManager()->screenAspect();
			viewHeight = viewHeight * (upperRightCorner.y - lowerLeftCorner.y);
			viewWidth = viewWidth * (upperRightCorner.x - lowerLeftCorner.x);

			// 创建相机
			ReOrthogonalCamera* pCamera = new ReOrthogonalCamera();
			pCamera->setLowerLeftCorner(lowerLeftCorner);
			pCamera->setUpperRightCorner(upperRightCorner);

			// 设置相机矩阵
			GePoint3d position = GePoint3d(viewportTableRecord->centerPoint().x, viewportTableRecord->centerPoint().y, 50);
			GeMatrix3d viewMatrix;
			viewMatrix.setToTranslation(GeVector3d(position.x, position.y, position.z));
			pCamera->setMatrix(viewMatrix);

			// 设置范围
			pCamera->setLeft(-viewWidth / 2);
			pCamera->setRight(viewWidth / 2);
			pCamera->setTop(viewHeight / 2);
			pCamera->setBottom(-viewHeight / 2);

			// 创建渲染对象
			ReRenderer* renderer = new ReRenderer();
			renderer->setCamera(pCamera);
			if (viewportTableRecord->objectId() == activeViewportId)
			{
				RE_IMP_RENDERER(renderer->m_pImpl)->isWorkArea = true;
			}
			else
			{
				RE_IMP_RENDERER(renderer->m_pImpl)->isWorkArea = false;
			}

			// 关闭视口
			viewportTableRecord->close();

			// 触发渲染视口事件
			for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++)
			{
				RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->rendererViewport(renderer);
			}

			// 触发渲染视口完成反应器
			for (int i = 0; i < RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors.length(); i++)
			{
				RE_IMP_RENDERERMANAGER(this->m_pImpl)->reactors[i]->endRendererViewport(viewportTableRecord);
			}

			// 将渲染对象添加到列表中
			RE_IMP_RENDERERMANAGER(this->m_pImpl)->renderers.append(renderer);
		}
		else if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == true) {


		}

	} while (false);
#endif
}