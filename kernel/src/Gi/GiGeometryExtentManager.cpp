#include "GiGeometryExtentManager.h"
#include "DbObjectManager.h"
#include "GiEntityManager.h"
#include "GiGeometryExtent.h"
#include "EvEventManager.h"
#include "RxWorkerThreadManager.h"
#include "acedTrans.h"
#include "GeMath.h"
#include "AcDebug.h"
#include "AcSystem.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "GiImpl.h"
#include "DbImpl.h"

int bbbbb = 0;
GiGeometryExtentManager::GiGeometryExtentManager() {
	this->m_pImpl = new GiGeometryExtentManagerImpl();
}
GiGeometryExtentManager::~GiGeometryExtentManager() {
}


void GiGeometryExtentManager::onDocumentCreated(GiGeometryExtentManager* pointer, ApDocument* document)
{
	// 获得模型空间
	DbBlockTable* pBlockTable = NULL;
	DbObjectId modelSpaceId = NULL;
	document->database()->getBlockTable(pBlockTable);
	if (pBlockTable != NULL) {
		pBlockTable->getIdAt(ACDB_MODEL_SPACE, modelSpaceId);
		pBlockTable->close();
	}

	// 创建几何范围对象
	GiGeometryExtent* pGeometryExtent = new GiGeometryExtent();
	pGeometryExtent->setDocument(document);
	pGeometryExtent->setModelSpace(modelSpaceId);
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.append(pGeometryExtent);

	// 标记更新矩阵和范围
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->bUpdateMatrix.store(true);
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->bUpdateExtent.store(true);
}
void GiGeometryExtentManager::onDocumentToBeDestroyed(GiGeometryExtentManager* pointer, ApDocument* document)
{
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.length(); i++)
	{
		if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.at(i)->document() == document) {
			realloc_arry_at(GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents, i);
			break;
		}
	}
}
void GiGeometryExtentManager::onMoveViewBegin(GiGeometryExtentManager* pointer, GePoint2d _mouse) {

	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag = true;
}
void GiGeometryExtentManager::onMoveViewEnd(GiGeometryExtentManager* pointer, GePoint2d _mouse) {
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag = false;
}
void GiGeometryExtentManager::onZoomViewBegin(GiGeometryExtentManager* pointer) {
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag = true;
}
void GiGeometryExtentManager::onZoomViewEnd(GiGeometryExtentManager* pointer) {
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag = false;
}
void GiGeometryExtentManager::onTwistBegin(GiGeometryExtentManager* pointer) {
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag = true;
}
void GiGeometryExtentManager::onTwistEnd(GiGeometryExtentManager* pointer) {
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag = false;
}
void GiGeometryExtentManager::onBeginAnimationFrame(GiGeometryExtentManager* pointer) {


	/** 如果处于拖拽状态则退出 */
	if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isDrag == true) {
		return;
	}

	/** 如果处于锁定状态则退出 */
	if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isLock.load() == true) {
		return;
	}

	/** 获得当前范围对象 */
	GiGeometryExtent* pGeometryExtent = NULL;
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.length(); i++)
	{
		if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->document() != acDocManager->curDocument()) {
			continue;
		}
		pGeometryExtent = GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i];
		break;
	}
	if (pGeometryExtent == NULL) {
		pGeometryExtent = new GiGeometryExtent();
		pGeometryExtent->setDocument(acDocManager->curDocument());
		GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.append(pGeometryExtent);
	}

	/** 获得当前视口 */
	DbObjectId activeViewportId = acedActiveViewportId();

	/** 添加视口 */
	GiGeometryExtentView* view = NULL;
	for (int i = 0; i < pGeometryExtent->views()->length(); i++) {
		if (pGeometryExtent->views()->at(i)->viewportId() == activeViewportId) {
			view = pGeometryExtent->views()->at(i);
			break;
		}
	}
	if (view == NULL) {
		view = new GiGeometryExtentView();
		view->setViewportId(activeViewportId);
		pGeometryExtent->views()->append(view);
	}

	/** 标记更新 */
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->drawEntitys.length(); i++) {
		ApDocument* document = acDocManager->document(GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->drawEntitys[i]->database());
		if (document == NULL) {
			continue;
		}
		for (int j = 0; j < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.length(); j++) {
			if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[j]->document() != document) {
				continue;
			}
			GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[j]->refreshMatrix();
			for (int k = 0; k < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[j]->views()->length(); k++) {
				GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[j]->views()->at(k)->markRefresh();
			}
			break;
		}
	}
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->drawEntitys.removeAll();

	/** 获得当前视口矩阵,视口高度,视口中心和是否模型空间 */
	GeMatrix3d newMatrix;
	double newViewHeight = 0;
	bool isModelSpace = true;
	GePoint2d newViewCenter = GePoint2d(0, 0);
	DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(activeViewportId);
	if (DB_IMP_OBJECT(object->m_pImpl)->isViewportTableRecord() == true)
	{
		DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)object;

		newMatrix = ::trans_dcs_mat(activeViewportId);
		newViewHeight = pViewportTableRecord->height();
		newViewCenter = pViewportTableRecord->centerPoint();
	}
	else if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == true) {
		DbViewport* pViewport = (DbViewport*)object;

		newMatrix = ::trans_dcs_mat(activeViewportId);
		newViewHeight = pViewport->viewHeight();
		newViewCenter = pViewport->viewCenter();
		if (pViewport->number() == 1)
		{
			isModelSpace = false;
		}
	}

	/** 判断当前文档是否被标记重新计算矩阵 */
	bool bRefreshMatrix = false;
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.length(); i++) {
		if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->document() != curDoc()) {
			continue;
		}
		if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->isRefreshMatrix() == true) {
			bRefreshMatrix = true;
		}
		else if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->displayMatrix().isEqualTo(newMatrix) == false) {
			bRefreshMatrix = true;
		}
		GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->setDisplayMatrix(newMatrix);
		GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->refreshMatrix(false);
		break;
	}

	/** 判断是否需要重新计算范围 */
	bool bRefreshExtent = false;
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents.length(); i++) {
		if (GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->document() != curDoc()) {
			continue;
		}
		AcArray<GiGeometryExtentView*>* views = GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->extents[i]->views();
		for (int j = 0; j < views->length(); j++) {
			if (views->at(j)->viewportId() != activeViewportId) {
				continue;
			}

			if (views->at(j)->isRefresh() == true) {
				bRefreshExtent = true;
			}
			else if (abs(views->at(j)->viewHeight() - newViewHeight) > 0.00002) {
				bRefreshExtent = true;
			}
			else if (views->at(j)->viewCenter().isEqualTo(newViewCenter) == false) {
				bRefreshExtent = true;
			}

			views->at(j)->markRefresh(false);
			views->at(j)->setViewHeight(newViewHeight);
			views->at(j)->setViewCenter(newViewCenter);
			break;
		}
		break;
	}
	if (bRefreshMatrix == true) {
		bRefreshExtent = true;
	}
	if (bRefreshMatrix == false && bRefreshExtent == false) {
		return;
	}

	/** 锁定 */
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->isLock.store(true);

	/** 更新 */
	if (bRefreshMatrix == true && bRefreshExtent == false) {
		pointer->asyncUpdateDisplayMatrix(activeViewportId);
	}
	else if (bRefreshMatrix == false && bRefreshExtent == true) {
		pointer->asyncUpdateDisplayExtent(activeViewportId);
	}
	else if (bRefreshMatrix == true && bRefreshExtent == true) {
		pointer->asyncUpdateDisplayMatrixAddExtent(activeViewportId);
	}
	AcDebug::print("");
}
void GiGeometryExtentManager::onEntityDrawEnd(GiGeometryExtentManager* pointer, DbEntity* pEntity) {
	GI_IMP_GEOMETRYSELECTORMANAGER(pointer->m_pImpl)->drawEntitys.append(pEntity);
}

void GiGeometryExtentManager::onInit()
{
	/** 监听创建文档事件 */
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentCreated, (void*)GiGeometryExtentManager::onDocumentCreated, this);

	/** 监听文档关闭事件 */
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentDestroyed, (void*)GiGeometryExtentManager::onDocumentToBeDestroyed, this);


	/** 监听消息循环开始事件 */
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)GiGeometryExtentManager::onBeginAnimationFrame, this);


	/** 监听拖动开始事件 */
	::kernel()->acevEventManager()->on(Ev::Controller::kMoveViewBegin, (void*)GiGeometryExtentManager::onMoveViewBegin, this);

	/** 监听拖动完成事件 */
	::kernel()->acevEventManager()->on(Ev::Controller::kMoveViewEnd, (void*)GiGeometryExtentManager::onMoveViewEnd, this);

	/** 监听缩放开始事件 */
	::kernel()->acevEventManager()->on(Ev::Controller::kZoomViewBegin, (void*)GiGeometryExtentManager::onZoomViewBegin, this);

	/** 监听缩放完成事件 */
	::kernel()->acevEventManager()->on(Ev::Controller::kZoomViewEnd, (void*)GiGeometryExtentManager::onZoomViewEnd, this);

	/** 监听扭转视图开始事件 */
	::kernel()->acevEventManager()->on(Ev::Controller::kTwistBegin, (void*)GiGeometryExtentManager::onTwistBegin, this);

	/** 监听扭转视图完成事件 */
	::kernel()->acevEventManager()->on(Ev::Controller::kTwistEnd, (void*)GiGeometryExtentManager::onTwistEnd, this);


	/** 监听绘制实体完成事件 */
	::kernel()->acevEventManager()->on(Ev::DbEntityDraw::kEntityDrawEnd, (void*)GiGeometryExtentManager::onEntityDrawEnd, this);
}

bool GiGeometryExtentManager::isLocked() const {
	if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.load() == true) {
		return true;
	}
	return false;
}

/** 获得需要计算的所有几何对象 */
void GiGeometryExtentManager::allGeometrys(AcArray<GiEntityGeometry*>& geometrys, const DbObjectId viewportId) const {

	// 获得工作数据库
	DbDatabase* workingDatabase = acdbHostApplicationServices()->workingDatabase();

	// 获得视口对应的块表记录
	bool isModelSpace = true;
	DbObjectId blockTableRecordId = NULL;
	DbObject* viewport = ::kernel()->acdbObjectManager()->openDbObject(viewportId);
	if (viewport == NULL) {
		isModelSpace = true;
		DbBlockTable* pBlockTable = NULL;
		workingDatabase->getBlockTable(pBlockTable);
		if (pBlockTable != NULL) {
			pBlockTable->getIdAt(ACDB_MODEL_SPACE, blockTableRecordId);
			pBlockTable->close();
		}
	}
	else if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewportTableRecord() == true)
	{
		DbBlockTable* pBlockTable = NULL;
		workingDatabase->getBlockTable(pBlockTable);
		if (pBlockTable != NULL) {
			pBlockTable->getIdAt(ACDB_MODEL_SPACE, blockTableRecordId);
			pBlockTable->close();
		}
	}
	else if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewport() == true)
	{

		if (((DbViewport*)viewport)->number() == 1)
		{
			isModelSpace = false;
			blockTableRecordId = ((DbViewport*)viewport)->blockId();
		}
		else
		{
			DbBlockTable* pBlockTable = NULL;
			workingDatabase->getBlockTable(pBlockTable);
			if (pBlockTable != NULL) {
				pBlockTable->getIdAt(ACDB_MODEL_SPACE, blockTableRecordId);
				pBlockTable->close();
			}
		}
	}

	// 查询当前图纸所有对象
	const AcArray<DbObject*>* objects = ::kernel()->acdbObjectManager()->queryObjects();
	for (int i = 0; i < objects->length(); i++)
	{
		if (objects->at(i) == NULL)
		{
			continue;
		}
		if (objects->at(i)->database() != workingDatabase)
		{
			continue;
		}
		if (objects->at(i)->isErased() == true)
		{
			continue;
		}
		if (DB_IMP_OBJECT(objects->at(i)->m_pImpl)->isEntity() == false)
		{
			continue;
		}

		// 获得实体
		DbEntity* entity = DbEntity::cast(objects->at(i));
		if (entity->blockId() != blockTableRecordId)
		{
			continue;
		}
		if (entity->objectId() == NULL)
		{
			continue;
		}

		// 获得实体的几何对象
		const AcArray<GiEntityGeometry*> geometryEntitys = DB_IMP_ENTITY(entity->m_pImpl)->entitys;
		for (int u = 0; u < geometryEntitys.length(); u++)
		{
			geometrys.append(geometryEntitys.at(u));
		}
	}
}

/** 更新在视口下几何实体的显示坐标 */
void GiGeometryExtentManager::updateDisplayMatrix(const DbObjectId& viewportId) {

	/** 锁定 */
	GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(true);

	/** 锁定线程 */
	::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

	AcArray<GiEntityGeometry*> geometrys;
	this->allGeometrys(geometrys, viewportId);

	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.length(); i++)
	{
		if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents[i]->document() == acDocManager->curDocument())
		{
			GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents[i]->updateDisplayMatrix(geometrys, viewportId);
			break;
		}
	}

	/** 解锁线程 */
	::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

	/** 解锁 */
	GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(false);
}
void GiGeometryExtentManager::asyncUpdateDisplayMatrix(const DbObjectId& viewportId) {

	::kernel()->acrxWorkerThreadManager()->addTask([this, viewportId]()->void {
		this->updateDisplayMatrix(viewportId);
		}, true);
}

/** 更新在视口下几何实体的显示范围 */
void GiGeometryExtentManager::updateDisplayExtent(const DbObjectId& viewportId) {

	/** 锁定 */
	GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(true);

	/** 锁定线程 */
	::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

	AcArray<GiEntityGeometry*> geometrys;
	this->allGeometrys(geometrys, viewportId);

	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.length(); i++) {
		if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents[i]->document() == acDocManager->curDocument()) {
			GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents[i]->updateDisplayExtent(geometrys, viewportId);
			break;
		}
	}

	/** 锁解锁线程 */
	::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

	/** 解锁 */
	GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(false);
}
void GiGeometryExtentManager::asyncUpdateDisplayExtent(const DbObjectId& viewportId) {

	::kernel()->acrxWorkerThreadManager()->addTask([this, viewportId]()->void {
		this->updateDisplayExtent(viewportId);
		}, true);
}

/** 更新在视口下几何实体的显示坐标和范围 */
void GiGeometryExtentManager::updateDisplayMatrixAddExtent(const DbObjectId& viewportId) {

	/** 锁定 */
	GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(true);

	/** 锁定线程 */
	::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

	AcArray<GiEntityGeometry*> geometrys;
	this->allGeometrys(geometrys, viewportId);

	for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.length(); i++)
	{
		if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i)->document() == acDocManager->curDocument())
		{
			GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i)->updateDisplayMatrix(geometrys, viewportId);
			GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i)->updateDisplayExtent(geometrys, viewportId);
			break;
		}
	}

	/** 解锁线程 */
	::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

	/** 解锁 */
	GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(false);
}
void GiGeometryExtentManager::asyncUpdateDisplayMatrixAddExtent(const DbObjectId& viewportId) {

	::kernel()->acrxWorkerThreadManager()->addTask([this, viewportId]()->void {
		this->updateDisplayMatrixAddExtent(viewportId);
		}, true);
}

/** 获得范围内的实体 */
void GiGeometryExtentManager::extentGeometryEntitys(AcArray<GiEntityGeometry*>& geometrys, const GePoint2d& _startPoint, const GePoint2d& _endPoint, const DbObjectId viewportId)
{

	do
	{
		// 等待解锁
		while (true) {
			if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.load() == false) {
				break;
			}
			AcSystem::sleep(5);
		}

		// 锁定
		GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(true);

		// 锁定线程
		::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

		GiGeometryExtent* pGeometrySelector = NULL;
		for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.length(); i++) {
			if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i)->document() == ::curDoc()) {
				pGeometrySelector = GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i);
				break;
			}
		}
		if (pGeometrySelector == NULL) {
			break;
		}
		pGeometrySelector->extentGeometrys(geometrys, _startPoint, _endPoint, viewportId);

		// 解锁线程
		::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

		// 解锁
		GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(false);

	} while (false);
}
void GiGeometryExtentManager::extentGeometryEntitys(AcArray<GiEntityGeometry*>& geometrys, const AcArray<GiEntityGeometry*>& targetGeometrys, const GePoint2d& _startPoint, const GePoint2d& _endPoint, const DbObjectId viewportId) {

	do
	{
		// 等待解锁
		while (true) {
			if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.load() == false) {
				break;
			}
			AcSystem::sleep(5);
		}

		// 锁定
		GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(true);

		/** 锁定线程 */
		::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

		GiGeometryExtent* pGeometrySelector = NULL;
		for (int i = 0; i < GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.length(); i++)
		{
			if (GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i)->document() == ::curDoc())
			{
				pGeometrySelector = GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->extents.at(i);
				break;
			}
		}
		if (pGeometrySelector == NULL) {
			break;
		}

		pGeometrySelector->extentGeometrys(geometrys, targetGeometrys, _startPoint, _endPoint, viewportId);

		/** 解锁线程 */
		::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

		// 解锁
		GI_IMP_GEOMETRYSELECTORMANAGER(this->m_pImpl)->isLock.store(false);

	} while (false);
}

/** 获得范围内的实体(异步) */
void GiGeometryExtentManager::asyncExtentGeometryEntitys(void (*fun)(AcArray<GiEntityGeometry*>&), const GePoint2d& startPoint, const GePoint2d& endPoint, const DbObjectId viewportId) {
	::kernel()->acrxWorkerThreadManager()->addTask([this, fun, startPoint, endPoint, viewportId]()->void {
		AcArray<GiEntityGeometry*>geometrys;
		this->extentGeometryEntitys(geometrys, startPoint, endPoint, viewportId);
		fun(geometrys);
		}, true);
}
void GiGeometryExtentManager::asyncExtentGeometryEntitys(void (*fun)(AcArray<GiEntityGeometry*>&), const AcArray<GiEntityGeometry*>& targetGeometrys, const GePoint2d& startPoint, const GePoint2d& endPoint, const DbObjectId viewportId) {
	::kernel()->acrxWorkerThreadManager()->addTask([this, fun, targetGeometrys, startPoint, endPoint, viewportId]()->void {
		AcArray<GiEntityGeometry*>geometrys;
		this->extentGeometryEntitys(geometrys, targetGeometrys, startPoint, endPoint, viewportId);
		fun(geometrys);
		}, true);
}