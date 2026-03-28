#include "GiEntDraw.h"
#include "arxHeaders.h"
#include "AcDebug.h"
#include "GiWorldGeometry.h"
#include "DbObjectId.h"
#include "DbEntity.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTableRecordIterator.h"
#include "GiSubEntityTraits.h"
#include "DbObjectManager.h"
#include "EvEventManager.h"
#include "GiWorldDrawManager.h"
#include "GiEntityManager.h"
#include "GiPoints.h"
#include "GiLineSegment.h"
#include "GiTriangleMeshGeometry.h"
#include "GiMesh.h"
#include "kernel.h"
#include "GiImpl.h"
#include "DbImpl.h"

GiEntDraw::GiEntDraw()
{
	this->m_pImpl = new GiEntDrawImpl();
}
GiEntDraw::~GiEntDraw()
{
}
GiWorldGeometry& GiEntDraw::geometry() const
{
	return *GI_IMP_ENTDRAW(this->m_pImpl)->pWorldGeometry;
}
void GiEntDraw::draw()
{

	do
	{

		// 获得要更新的对象
		AcArray<DbEntity*> updateEntitys;
		AcArray<DbLayerTableRecord*> updateLayerTableRecords;
		for (int i = 0; i < GI_IMP_ENTDRAW(this->m_pImpl)->entityIds.length(); i++)
		{
			DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(GI_IMP_ENTDRAW(this->m_pImpl)->entityIds[i]);
			if (object == NULL) {
				continue;
			}
			updateEntitys.append((DbEntity*)object);
		}
		for (int i = 0; i < GI_IMP_ENTDRAW(this->m_pImpl)->layerTableRecordIds.length(); i++)
		{
			DbObject* object = ::kernel()->acdbObjectManager()->openDbObject(GI_IMP_ENTDRAW(this->m_pImpl)->layerTableRecordIds[i]);
			if (object == NULL) {
				continue;
			}
			updateLayerTableRecords.append((DbLayerTableRecord*)object);
		}
		GI_IMP_ENTDRAW(this->m_pImpl)->entityIds.removeAll();
		GI_IMP_ENTDRAW(this->m_pImpl)->layerTableRecordIds.removeAll();

		// 获得所有实体
		AcArray<DbEntity*> allEntitys;
		if (GI_IMP_ENTDRAW(this->m_pImpl)->isInit == false || updateLayerTableRecords.length() > 0) {
			const AcArray<DbObject*>* allObjects = ::kernel()->acdbObjectManager()->queryObjects();
			for (int i = 0; i < allObjects->length(); i++) {
				if (allObjects->at(i) == NULL) {
					continue;
				}
				if (allObjects->at(i)->database() != this->document()->database()) {
					continue;
				}
				if (DB_IMP_OBJECT(allObjects->at(i)->m_pImpl)->isEntity() == true) {
					allEntitys.append((DbEntity*)allObjects->at(i));
				}
			}
		}

		// 初始化
		if (GI_IMP_ENTDRAW(this->m_pImpl)->isInit == false) {
			GI_IMP_ENTDRAW(this->m_pImpl)->isInit = true;

			for (int i = 0; i < allEntitys.length(); i++) {
				DB_IMP_OBJECT(allEntitys[i]->m_pImpl)->isWrite = true;
				updateEntitys.append(allEntitys[i]);
			}
		}

		// 判断是否需要更新
		if (updateEntitys.length() == 0 && updateLayerTableRecords.length() == 0) {
			break;
		}

		// 更新图层
		if (updateLayerTableRecords.length() > 0)
		{
			for (int i = 0; i < allEntitys.length(); i++)
			{
				if (DB_IMP_OBJECT(allEntitys[i]->m_pImpl)->isWrite == true)
				{
					continue;
				}

				// 获得实体的图层对象
				DbLayerTableRecord* pLayerTableRecord = (DbLayerTableRecord*)::kernel()->acdbObjectManager()->openDbObject(DB_IMP_ENTITY(allEntitys[i]->m_pImpl)->layerId);
				if (pLayerTableRecord == NULL)
				{
					continue;
				}

				// 如果图层被标记修改则标记实体需要被修改
				if (DB_IMP_OBJECT(pLayerTableRecord->m_pImpl)->isWrite == true)
				{
					DB_IMP_OBJECT(allEntitys[i]->m_pImpl)->isWrite = true;
					updateEntitys.append(allEntitys[i]);
				}
			}
		}

		// 绘制实体
		bool isDraw = false;
		for (int i = 0; i < updateEntitys.length(); i++)
		{
			if (DB_IMP_OBJECT(updateEntitys[i]->m_pImpl)->isWrite == false)
			{
				continue;
			}
			DB_IMP_OBJECT(updateEntitys[i]->m_pImpl)->isWrite = false;

			::kernel()->acevEventManager()->_emit(Ev::DbEntityDraw::kEntityDrawBegin, updateEntitys[i]);

			DbObjectId blockId = updateEntitys[i]->blockId();
			DbBlockTableRecord* pBlockTableRecord = (DbBlockTableRecord*)::kernel()->acdbObjectManager()->openDbObject(blockId);
			if (pBlockTableRecord->isLayout() == true)
			{
				isDraw = true;
				updateEntitys[i]->draw();
			}

			::kernel()->acevEventManager()->_emit(Ev::DbEntityDraw::kEntityDrawEnd, updateEntitys[i]);
		}

		// 将几何实体添加到实体对象中进行渲染
		if (isDraw == true)
		{
			AcArray<GiWorldGeometryStake*> worldGeometryStake;
			GI_IMP_ENTDRAW(this->m_pImpl)->pWorldGeometry->worldGeometryStakes(worldGeometryStake);
			GiGeometryPool* pool = ::kernel()->acgiEntityManager()->pool();
			for (int i = 0; i < worldGeometryStake.length(); i++)
			{
				GiEntityGeometry* entityGeometry = worldGeometryStake[i]->geometry();
				if (entityGeometry->type() == Gi::EntityGeometryType::kLineSegmentGeometry)
				{
					GiLineSegment* entity = (GiLineSegment*)::kernel()->acgiEntityManager()->queryEntity(Gi::EntityType::kLineSegment, Gi::ElementType::kElement, worldGeometryStake[i]->material());
					if (entity == NULL)
					{
						entity = new GiLineSegment();
						entity->setElement(Gi::ElementType::kElement);
						entity->setMaterial(worldGeometryStake[i]->material());
						::kernel()->acgiEntityManager()->addEntity(entity);
					}
					entity->appendLineSegment((GiLineSegmentGeometry*)entityGeometry);
				}
				else if (entityGeometry->type() == Gi::EntityGeometryType::kTriangleMeshGeometry)
				{
					GiMesh* entity = (GiMesh*)::kernel()->acgiEntityManager()->queryEntity(Gi::EntityType::kMesh, Gi::ElementType::kElement, worldGeometryStake[i]->material());
					if (entity == NULL)
					{
						entity = new GiMesh();
						entity->setElement(Gi::ElementType::kElement);
						entity->setMaterial(worldGeometryStake[i]->material());
						::kernel()->acgiEntityManager()->addEntity(entity);
					}
					entity->appendTriangleMeshGeometry((GiTriangleMeshGeometry*)entityGeometry);
				}
				else if (entityGeometry->type() == Gi::EntityGeometryType::kPointGeometry)
				{
					GiPoints* entity = (GiPoints*)::kernel()->acgiEntityManager()->queryEntity(Gi::EntityType::kPoints, Gi::ElementType::kElement, worldGeometryStake[i]->material());
					if (entity == NULL)
					{
						entity = new GiPoints();
						entity->setElement(Gi::ElementType::kElement);
						entity->setMaterial(worldGeometryStake[i]->material());
						::kernel()->acgiEntityManager()->addEntity(entity);
					}
					entity->appendPointGeometry((GiPointGeometry*)entityGeometry);
				}
				// stake 归还对象池
				pool->releaseStake(worldGeometryStake[i]);
			}
			GI_IMP_ENTDRAW(this->m_pImpl)->pWorldGeometry->removeWorldGeometryStakesOnly();
		}

		// 高亮实体
		if (isDraw == true) {
			for (int i = 0; i < updateEntitys.length(); i++)
			{
				if (DB_IMP_ENTITY(updateEntitys[i]->m_pImpl)->highlight == true) {
					::kernel()->acgiWorldDrawManager()->entityHighlander()->highlight(updateEntitys[i]);
				}
			}
		}

		for (int i = 0; i < updateEntitys.length(); i++)
		{
			DB_IMP_OBJECT(updateEntitys[i]->m_pImpl)->isWrite = false;
		}
		for (int i = 0; i < updateLayerTableRecords.length(); i++)
		{
			DB_IMP_OBJECT(updateLayerTableRecords[i]->m_pImpl)->isWrite = false;
		}

	} while (false);

}

ApDocument* GiEntDraw::document() const
{
	return GI_IMP_ENTDRAW(this->m_pImpl)->pDocument;
}
void GiEntDraw::setDocument(ApDocument* pDocument)
{
	GI_IMP_ENTDRAW(this->m_pImpl)->pDocument = pDocument;
}

GiRegenType GiEntDraw::regenType() const
{
	return GI_IMP_ENTDRAW(this->m_pImpl)->regenType;
}
bool GiEntDraw::regenAbort() const
{
	return false;
}
GiSubEntityTraits& GiEntDraw::subEntityTraits() const
{
	return *GI_IMP_ENTDRAW(this->m_pImpl)->pEntityTraits;
}
bool GiEntDraw::isDragging() const
{
	return false;
}

// This function operates against the current active viewport
//
double GiEntDraw::deviation(const GiDeviationType, const GePoint3d&) const
{
	return 0.0;
}
UInt32 GiEntDraw::numberOfIsolines() const
{
	return 0;
}

bool GiEntDraw::secondaryCall()
{
	return false;
}
AcArray<unsigned int>& GiEntDraw::entityIds()
{
	return GI_IMP_ENTDRAW(this->m_pImpl)->entityIds;
}
AcArray<unsigned int>& GiEntDraw::layerTableRecordIds()
{
	return GI_IMP_ENTDRAW(this->m_pImpl)->layerTableRecordIds;
}
