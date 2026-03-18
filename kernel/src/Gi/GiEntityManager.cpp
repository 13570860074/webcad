#include "kernel.h"
#include "EvEventManager.h"
#include "ReRendererReactor.h"
#include "GiEntityIndex.h"
#include "GiObjectManager.h"
#include "GiEntityGeometry.h"
#include "GiEntityReactor.h"
#include "GiEntityIterator.h"
#include "GiEntityManager.h"
#include "GiMaterialManager.h"
#include "GiLineSegment.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "GiEntity.h"
#include "AcArray.h"
#include "GiImpl.h"




GiEntityManager::GiEntityManager()
{
	this->m_pImpl = new GiEntityManagerImpl();
}
GiEntityManager::~GiEntityManager()
{
}

void GiEntityManager::onBeginAnimationFrame(GiEntityManager* pointer) {


	AcArray<GiEntity*> createElementEntitys = AcArray<GiEntity*>();
	AcArray<GiEntity*> createDraggingEntitys = AcArray<GiEntity*>();
	AcArray<GiEntity*> createOtherEntitys = AcArray<GiEntity*>();
	AcArray<GiEntity*> updateElementEntitys = AcArray<GiEntity*>();
	AcArray<GiEntity*> updateDraggingEntitys = AcArray<GiEntity*>();
	AcArray<GiEntity*> updateOtherEntitys = AcArray<GiEntity*>();
	AcArray<GiEntity*> transformationEntitys = AcArray<GiEntity*>();
	for (int i = 0; i < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entitys.length(); i++) {

		GiEntity* entity = GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entitys[i];
		if (entity == NULL) {
			continue;
		}

		// 判断实体是否需要创建
		if (GI_IMP_ENTITY(entity->m_pImpl)->bCreate == false) {

			GI_IMP_ENTITY(entity->m_pImpl)->bCreate = true;
			if (entity->element() == Gi::ElementType::kElement) {
				createElementEntitys.append(entity);
			}
			else if (entity->element() == Gi::ElementType::kDragging) {
				createDraggingEntitys.append(entity);
			}
			else {
				createOtherEntitys.append(entity);
			}
		}

		// 判断实体是否需要更新顶点
		if (GI_IMP_ENTITY(entity->m_pImpl)->bUpdateVertex == true) {

			GI_IMP_ENTITY(entity->m_pImpl)->bUpdateVertex = false;
			if (entity->element() == Gi::ElementType::kElement) {
				updateElementEntitys.append(entity);
			}
			else if (entity->element() == Gi::ElementType::kDragging) {
				updateDraggingEntitys.append(entity);
			}
			else {
				updateOtherEntitys.append(entity);
			}
		}

		// 判断实体是否需要矩阵变换
		if (GI_IMP_ENTITY(entity->m_pImpl)->bTransformation == true) {
			GI_IMP_ENTITY(entity->m_pImpl)->bTransformation = false;
			transformationEntitys.append(entity);
		}
	}

	// 创建实体 
	if (createElementEntitys.length() > 0 || createDraggingEntitys.length() > 0 || createOtherEntitys.length() > 0)
	{
		::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kCreateEntityWillStart);
		if (createElementEntitys.length() > 0) {

			/** 触发图元实体将要创建开始事件 */
			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kCreateElementEntityWillStart);

			/** 锁定线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

			for (int i = 0; i < createElementEntitys.length(); i++) {
				for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
					GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->createEntity(createElementEntitys[i]);
				}
			}

			/** 解锁线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

			/** 触发图元实体将要创建完成事件 */
			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kCreateElementEntityEnd);
		}
		if (createDraggingEntitys.length() > 0) {

			/** 触发拖拽实体将要创建开始事件 */
			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kCreateDraggingEntityWillStart);

			/** 锁定线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexDragging().lock();

			// 触发实体创建事件
			for (int i = 0; i < createDraggingEntitys.length(); i++) {

				for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
					GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->createEntity(createDraggingEntitys[i]);
				}
			}

			/** 解锁线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexDragging().unlock();

			/** 触发拖拽实体将要创建完成事件 */
			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kCreateDraggingEntityEnd);
		}
		if (createOtherEntitys.length() > 0) {
			for (int i = 0; i < createOtherEntitys.length(); i++) {
				for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
					GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->createEntity(createOtherEntitys[i]);
				}
			}
		}
		::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kCreateEntityAllEnd);
	}

	/* 更新实体 */
	if (updateElementEntitys.length() > 0 || updateDraggingEntitys.length() > 0 || updateOtherEntitys.length() > 0)
	{
		::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kUpdateEntityWillStart);
		if (updateElementEntitys.length() > 0) {

			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kUpdateElementEntityWillStart);

			/** 锁定线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexElement().lock();

			for (int i = 0; i < updateElementEntitys.length(); i++) {

				// 更新实体
				updateElementEntitys[i]->update();

				// 触发更新顶点事件
				for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
					GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->updateVertexs(updateElementEntitys[i]);
				}
			}

			/** 解锁线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexElement().unlock();

			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kUpdateElementEntityEnd);
		}
		if (updateDraggingEntitys.length() > 0) {

			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kUpdateDraggingEntityWillStart);

			/** 锁定线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexDragging().lock();

			for (int i = 0; i < updateDraggingEntitys.length(); i++) {

				// 更新实体
				updateDraggingEntitys[i]->update();

				// 触发更新顶点事件
				for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
					GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->updateVertexs(updateDraggingEntitys[i]);
				}
			}

			/** 解锁线程 */
			::kernel()->acgiEntityManager()->mutex()->mutexDragging().unlock();

			::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kUpdateDraggingEntityEnd);
		}
		if (updateOtherEntitys.length() > 0) {
			for (int i = 0; i < updateOtherEntitys.length(); i++) {

				// 更新实体
				updateOtherEntitys[i]->update();

				// 触发更新顶点事件
				for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
					GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->updateVertexs(updateOtherEntitys[i]);
				}
			}
		}
		::kernel()->acevEventManager()->_emit(Ev::GiEntityDraw::kUpdateEntityAllEnd);
	}

	/* 实体变换事件 */
	for (int i = 0; i < transformationEntitys.length(); i++) {

		// 触发事件
		for (int u = 0; u < GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors.length(); u++) {
			GI_IMP_ENTITYMANAGER(pointer->m_pImpl)->entityReactors[u]->transformation(transformationEntitys[i]);
		}
	}
}
void GiEntityManager::onDocumentDestroyed(GiEntityManager* param, ApDocument* doc) {

	//// 删除文档中的实体
	//param->destroyed(doc);

	//// 删除文档中的实体可见性
	//for (int i = 0; i < GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys.length(); i++) {
	//	if (GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys[i]->document == doc) {
	//		realloc_arry_at(GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys, i);
	//		break;
	//	}
	//}
}
void GiEntityManager::onDocumentToBeActivated(GiEntityManager* param, ApDocument* doc)
{
	//if (acDocManager != NULL) {
	//	GI_IMP_ENTITYMANAGER(param->m_pImpl)->activationDocument = acDocManager->curDocument();
	//}
}
void GiEntityManager::onDocumentActivated(GiEntityManager* param, ApDocument* doc) {

	//// 将当前激活的文档中的实体隐藏
	//if (GI_IMP_ENTITYMANAGER(param->m_pImpl)->activationDocument != NULL) {
	//	GiEntityDocumentVisibility* entityVisibility = NULL;
	//	for (int i = 0; i < GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys.length(); i++) {
	//		if (GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys[i]->document == GI_IMP_ENTITYMANAGER(param->m_pImpl)->activationDocument) {
	//			entityVisibility = GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys[i];
	//			break;
	//		}
	//	}
	//	if (entityVisibility == NULL) {
	//		entityVisibility = new GiEntityDocumentVisibility();
	//		entityVisibility->document = GI_IMP_ENTITYMANAGER(param->m_pImpl)->activationDocument;
	//		GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys.append(entityVisibility);
	//	}
	//	entityVisibility->entitys.removeAll();
	//	for (int i = 0; i < GI_IMP_ENTITYMANAGER(param->m_pImpl)->entitys.length(); i++) {
	//		if (GI_IMP_ENTITYMANAGER(param->m_pImpl)->entitys[i]->document() != GI_IMP_ENTITYMANAGER(param->m_pImpl)->activationDocument) {
	//			continue;
	//		}
	//		if (GI_IMP_ENTITYMANAGER(param->m_pImpl)->entitys[i]->visibility() == true) {
	//			GI_IMP_ENTITYMANAGER(param->m_pImpl)->entitys[i]->setVisibility(false);
	//			entityVisibility->entitys.append(GI_IMP_ENTITYMANAGER(param->m_pImpl)->entitys[i]);
	//		}
	//	}
	//}

	//// 将被激活的文档实体显示出来
	//for (int i = 0; i < GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys.length(); i++) {
	//	if (GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys[i]->document != doc) {
	//		continue;
	//	}
	//	GiEntityDocumentVisibility* entityVisibility = GI_IMP_ENTITYMANAGER(param->m_pImpl)->entityVisibilitys[i];
	//	for (int i = 0; i < entityVisibility->entitys.length(); i++) {
	//		entityVisibility->entitys[i]->setVisibility(true);
	//	}
	//	break;
	//}
}


void GiEntityManager::onInit() {
	std::cout << "GiEntityManager" << std::endl;
	// 监听渲染循环开始事件
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)onBeginAnimationFrame, this);

	// 监听文档被销毁事件
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentDestroyed, (void*)onDocumentDestroyed, this);

	// 监听切换文档开始事件
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentToBeActivated, (void*)onDocumentToBeActivated, this);

	// 监听切换文档完成事件
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentActivated, (void*)onDocumentActivated, this);
}
GiObjectId GiEntityManager::addEntity(GiEntity* entity)const {
	::kernel()->acgiObjectManager()->addGiObject(entity);
	GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys.append(entity);

	{
		// 获得类型索引
		GiEntityIndexPointerType* entityIndexPointerType = GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityIndexs->at(entity->type() - Gi::EntityType::kUnknownEntity - 1);

		// 获得元素索引
		GiEntityIndexPointerElement* entityIndexPointerElement = entityIndexPointerType->at(entity->element() - Gi::ElementType::kUnknownElement - 1);

		// 判断如果元素索引的材质索引长度小于材质id则填充空对象,没有则创建
		GiEntityIndexPointerMaterial* entityIndexPointerMaterial = NULL;
		if (entityIndexPointerElement->length() > entity->material()->objectId().asOldId() - 1) {
			entityIndexPointerMaterial = entityIndexPointerElement->at(entity->material()->objectId().asOldId() - 1);
		}
		if (entityIndexPointerMaterial == NULL) {
			for (int i = entityIndexPointerElement->length(); i < entity->material()->objectId().asOldId(); i++) {
				entityIndexPointerElement->append(NULL);
			}
		}

		if (entityIndexPointerMaterial == NULL) {
			entityIndexPointerMaterial = new GiEntityIndexPointerMaterial();
			entityIndexPointerMaterial->materialId = entity->material()->objectId().asOldId();
			entityIndexPointerElement->setAt(entity->material()->objectId().asOldId() - 1, entityIndexPointerMaterial);
		}

		if (entity->element() == Gi::ElementType::kDragging) {
			entityIndexPointerMaterial->entitys.append(entity);
		}
		else if (entity->element() == Gi::ElementType::kHighlight) {
			entityIndexPointerMaterial->entitys.append(entity);
		}
		else {
			entityIndexPointerMaterial->append((GiEntity*)entity);
		}
	}

	return entity->objectId();
}
void GiEntityManager::addReactor(GiEntityReactor* reactor)const {
	GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityReactors.append(reactor);
	for (int i = 0; i < GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys.length(); i++) {
		reactor->createEntity(GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys.at(i));
	}
}
void GiEntityManager::removeReactor(GiEntityReactor* reactor) const {
	for (int i = 0; i < GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityReactors.length(); i++) {
		if (GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityReactors[i] == reactor)
		{
			realloc_arry_at(GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityReactors, i);
			break;
		}
	}
}
GiEntityIterator* GiEntityManager::newIterator()const {
	GiEntityIterator* iterator = new GiEntityIterator();
	GI_IMP_ENTITYITERATOR(iterator->m_pImpl)->iterator = AcIterator<GiEntity*>((AcArray<GiEntity*>)GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys);
	return iterator;
}
GiEntity* GiEntityManager::queryEntity(const Gi::EntityType type, const Gi::ElementType element, const GiMaterial* material) {

	for (int i = 0;i < GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys.length();i++) {
		if (GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys[i]->type() == type &&
			GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys[i]->element() == element &&
			GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys[i]->material() == material) {
			return GI_IMP_ENTITYMANAGER(this->m_pImpl)->entitys[i];
		}
	}
	return NULL;


	// 获得类型索引
	GiEntityIndexPointerType* entityIndexPointerType = GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityIndexs->at(type - Gi::EntityType::kUnknownEntity - 1);

	// 获得元素索引
	GiEntityIndexPointerElement* entityIndexPointerElement = entityIndexPointerType->at(element - Gi::ElementType::kUnknownElement - 1);

	// 获得材质索引
	int materialId = material->objectId().asOldId();
	GiEntityIndexPointerMaterial* entityIndexPointerMaterial = NULL;
	if (entityIndexPointerElement->length() > materialId - 1) {
		entityIndexPointerMaterial = entityIndexPointerElement->at(materialId - 1);
	}
	if (entityIndexPointerMaterial == NULL) {
		return NULL;
	}

	if (element == Gi::ElementType::kDragging) {
		if (entityIndexPointerMaterial->entitys.length() > 0) {
			return entityIndexPointerMaterial->entitys[0];
		}
		return NULL;
	}
	else if (element == Gi::ElementType::kHighlight) {
		if (entityIndexPointerMaterial->entitys.length() > 0) {
			return entityIndexPointerMaterial->entitys[0];
		}
		return NULL;
	}

	if (entityIndexPointerMaterial->length() > 0) {
		return entityIndexPointerMaterial->at(0);
	}
	return NULL;
}
GiEntityMutex* GiEntityManager::mutex() {
	return GI_IMP_ENTITYMANAGER(this->m_pImpl)->entityMutex;
}