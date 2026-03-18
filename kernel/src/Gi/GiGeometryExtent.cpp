#include "GiGeometryExtent.h"
#include "GiEntityManager.h"
#include "GiComputeExtent.h"
#include "GiPointGeometry.h"
#include "GiComputeExtent.h"
#include "GiComputeDisplay.h"
#include "GiEntityIterator.h"
#include "RxWorkerThread.h"
#include "RxWorkerThreadManager.h"
#include "DbObjectManager.h"
#include "EvEventManager.h"
#include "ApDocument.h"
#include "arxHeaders.h"
#include "acedTrans.h"
#include "AcDebug.h"
#include "kernel.h"
#include "GeMath.h"
#include "GiImpl.h"
#include "DbImpl.h"
#include "kernel.h"
#include <thread>
#include <mutex>



GiGeometryExtent::GiGeometryExtent()
{
	this->m_pImpl = new GiGeometryExtentImpl();
}
GiGeometryExtent::~GiGeometryExtent()
{
}

ApDocument* GiGeometryExtent::document() const {
	return GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->document;
}
void GiGeometryExtent::setDocument(ApDocument* doc)
{
	GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->document = doc;
}

DbObjectId GiGeometryExtent::modelSpace() const {
	return GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->modelSpace;
}
void GiGeometryExtent::setModelSpace(const DbObjectId& modelSpace) {
	GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->modelSpace = modelSpace;
}

GeMatrix3d GiGeometryExtent::displayMatrix() const {
	return GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->displayMatrix;
}
void GiGeometryExtent::setDisplayMatrix(const GeMatrix3d& mat) {
	GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->displayMatrix = mat;
}
void GiGeometryExtent::refreshMatrix(const bool bRefresh) {
	GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->markRefresh = bRefresh;
}
bool GiGeometryExtent::isRefreshMatrix() const {
	return GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->markRefresh;
}
AcArray<GiGeometryExtentView*>* GiGeometryExtent::views() {
	return GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents;
}

AcArray<GiEntityGeometry*>* GiGeometryExtent::geometrys(const DbObjectId& viewportId) const {
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->length(); i++) {
		if (GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->at(i)->viewportId() == viewportId) {
			return GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->at(i)->geometrys();
		}
	}
	return NULL;
}
void GiGeometryExtent::updateDisplayMatrix(const AcArray<GiEntityGeometry*>& targetGeometrys, const DbObjectId& viewportId)
{
	bool isModelSpace = true;
	GeMatrix3d newMatrix;
	DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(viewportId);
	if (DB_IMP_OBJECT(object->m_pImpl)->isViewportTableRecord() == true) {
		newMatrix = ::trans_dcs_mat(viewportId);
	}
	if (DB_IMP_OBJECT(object->m_pImpl)->isViewport() == true)
	{
		newMatrix = ::trans_dcs_mat(viewportId);
		if (((DbViewport*)object)->number() == 1) {
			isModelSpace = false;
		}
	}

	/** 模型空间 */
	if (isModelSpace == true) {

		GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->displayMatrix = newMatrix;

		/** 获得所有工作线程 */
		unsigned int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
		unsigned int countGeometryThread = int(targetGeometrys.length() / ((numWorkerThread + 1) * 1.0));

		/** 判断是否是默认矩阵 */
		bool bDefaultMatrix = true;
		if (GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->displayMatrix.isEqualTo(GeMatrix3d::kIdentity) == false)
		{
			bDefaultMatrix = false;
		}

		/** 创建线程并分配任务 */
		AcArray<GiComputeDisplay*> computeDisplays;
		if (targetGeometrys.length() > 10000)
		{
			for (int i = 0; i < numWorkerThread; i++)
			{
				int start = i * countGeometryThread;
				int end = (i + 1) * countGeometryThread;

				GiComputeDisplay* computeDisplay = new GiComputeDisplay();
				computeDisplay->matrix = GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->displayMatrix;
				computeDisplay->defaultMatrix = bDefaultMatrix;
				computeDisplay->isMainThread = false;
				for (int u = start; u < end; u++)
				{
					computeDisplay->allGeometrys.append(targetGeometrys[u]);
				}
				computeDisplays.append(computeDisplay);
			}
		}
		GiComputeDisplay* computeDisplay = new GiComputeDisplay();
		computeDisplay->matrix = GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->displayMatrix;
		computeDisplay->defaultMatrix = bDefaultMatrix;
		computeDisplay->isMainThread = true;
		if (targetGeometrys.length() <= 10000)
		{
			for (int u = 0; u < targetGeometrys.length(); u++)
			{
				computeDisplay->allGeometrys.append(targetGeometrys[u]);
			}
		}
		else
		{
			int start = numWorkerThread * countGeometryThread;
			int end = targetGeometrys.length();
			for (int u = start; u < end; u++)
			{
				computeDisplay->allGeometrys.append(targetGeometrys[u]);
			}
		}
		computeDisplays.append(computeDisplay);

		/** 计算 */
		for (int i = 0; i < computeDisplays.length(); i++)
		{
			if (computeDisplays.at(i)->isMainThread == false) {
				::kernel()->acrxWorkerThreadManager()->addTask((void (*)(void*))GiComputeDisplay::compute, computeDisplays.at(i), false);
			}
		}
		for (int i = 0; i < computeDisplays.length(); i++)
		{
			if (computeDisplays.at(i)->isMainThread == true)
			{
				GiComputeDisplay::compute(computeDisplay);
			}
		}

		/** 等待计算完成 */
		while (true)
		{
			bool isDone = true;
			for (int i = 0; i < computeDisplays.length(); i++)
			{
				if (computeDisplays.at(i)->isDone.load() == false)
				{
					isDone = false;
					break;
				}
			}
			if (isDone == true)
			{
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		/** 删除对象 */
		realloc_arry_all(computeDisplays);
	}
}
void GiGeometryExtent::updateDisplayExtent(const AcArray<GiEntityGeometry*>& targetGeometrys, const DbObjectId& viewportId) {

	// 获得范围视图
	GiGeometryExtentView* viewItem = NULL;
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->length(); i++) {
		if (viewportId == GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->at(i)->viewportId()) {
			viewItem = GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->at(i);
			break;
		}
	}
	if (viewItem == NULL) {
		return;
	}

	// 设置新的视图高度和中心
	DbObject* viewport = ::kernel()->acdbObjectManager()->openDbObject(viewportId);
	if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewportTableRecord() == true)
	{
		DbViewportTableRecord* pViewportTableRecord = DbViewportTableRecord::cast(viewport);
		viewItem->setViewHeight(pViewportTableRecord->height());
		viewItem->setViewCenter(pViewportTableRecord->centerPoint());
	}
	if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewport() == true)
	{
		DbViewport* pViewport = DbViewport::cast(viewport);
	}

	GePoint2d startPoint = ::trans_vcs_to_dcs(GePoint2d(1, 1), viewItem->viewportId());
	GePoint2d endPoint = ::trans_vcs_to_dcs(GePoint2d(0, 0), viewItem->viewportId());

	AcArray<GiEntityGeometry*> geometrys;
	this->extentGeometrys(geometrys, targetGeometrys, startPoint, endPoint, viewItem->viewportId());
	viewItem->geometrys()->removeAll();
	for (int i = 0; i < geometrys.length(); i++) {
		viewItem->geometrys()->append(geometrys[i]);
	}
}

void GiGeometryExtent::extentGeometrys(AcArray<GiEntityGeometry*>& geometrys, const GePoint2d& startPoint, const GePoint2d& endPoint, const DbObjectId& viewportId)
{
	for (int i = 0; i < GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->length(); i++) {
		if (GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->at(i)->viewportId() == viewportId) {
			return this->extentGeometrys(geometrys, *GI_IMP_GEOMETRYSELECTOR(this->m_pImpl)->viewExtents->at(i)->geometrys(), startPoint, endPoint, viewportId);
		}
	}
}
void GiGeometryExtent::extentGeometrys(AcArray<GiEntityGeometry*>& geometrys, const AcArray<GiEntityGeometry*>& targetGeometrys, const GePoint2d& startPoint, const GePoint2d& endPoint, const DbObjectId& viewportId)
{
	// 判断是否是模型空间
	bool isModelSpace = true;
	DbObject* viewport = ::kernel()->acdbObjectManager()->openDbObject(viewportId);
	if (viewport == NULL)
	{
		return;
	}
	if (DB_IMP_OBJECT(viewport->m_pImpl)->isViewport() == true)
	{
		if (((DbViewport*)viewport)->number() == 1)
		{
			isModelSpace = false;
		}
	}


	bool isOnlyMainThread = true;
	if (targetGeometrys.length() > 5000)
	{
		isOnlyMainThread = false;
	}

	// 获得所有工作线程
	unsigned int numWorkerThread = ::kernel()->acrxWorkerThreadManager()->numWorkerThread();
	unsigned int countGeometryThread = int(targetGeometrys.length() / ((numWorkerThread + 1) * 1.0));

	// 创建线程
	AcArray<GiComputeExtent*> computeExtents;
	if (isOnlyMainThread == false)
	{
		// 创建计算线程
		for (int i = 0; i < numWorkerThread; i++)
		{
			int start = i * countGeometryThread;
			int end = (i + 1) * countGeometryThread;

			GiComputeExtent* computeExtent = new GiComputeExtent();
			computeExtent->isMainThread = false;
			computeExtent->isModelSpace = isModelSpace;
			computeExtent->startPoint = startPoint;
			computeExtent->endPoint = endPoint;
			for (int u = start; u < end; u++)
			{
				computeExtent->geometrys.append(targetGeometrys[u]);
			}
			computeExtents.append(computeExtent);
		}
	}
	GiComputeExtent* computeExtent = new GiComputeExtent();
	computeExtent->isMainThread = true;
	computeExtent->isModelSpace = isModelSpace;
	computeExtent->startPoint = startPoint;
	computeExtent->endPoint = endPoint;
	if (isOnlyMainThread == true)
	{
		int start = 0;
		int end = targetGeometrys.length();
		for (int u = start; u < end; u++)
		{
			computeExtent->geometrys.append(targetGeometrys[u]);
		}
	}
	else
	{
		int start = numWorkerThread * countGeometryThread;
		int end = targetGeometrys.length();
		for (int u = start; u < end; u++)
		{
			computeExtent->geometrys.append(targetGeometrys[u]);
		}
	}
	computeExtents.append(computeExtent);

	// 计算
	for (int i = 0; i < computeExtents.length(); i++)
	{
		if (computeExtents[i]->isMainThread == true)
		{
			GiComputeExtent::compute(computeExtents[i]);
		}
		else
		{
			::kernel()->acrxWorkerThreadManager()->addTask((void (*)(void*))GiComputeExtent::compute, computeExtents[i], false);
		}
	}

	// 等待计算完成
	while (true)
	{
		bool isDone = true;
		for (int i = 0; i < computeExtents.length(); i++)
		{
			if (computeExtents[i]->isDone.load() == false)
			{
				isDone = false;
				break;
			}
		}
		if (isDone == true)
		{
			break;
		}
	};

	// 获得选取的几何对象
	for (int i = 0; i < computeExtents.length(); i++)
	{
		for (int u = 0; u < computeExtents[i]->selectGeometrys.length(); u++)
		{
			geometrys.append(computeExtents[i]->selectGeometrys[u]);
		}
	}

	// 释放内存
	realloc_arry_all(computeExtents);
}
