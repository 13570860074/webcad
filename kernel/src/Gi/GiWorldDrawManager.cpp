#include "GiWorldDrawManager.h"
#include "EvEventManager.h"
#include "GiWorldDraw.h"
#include "GiEntDraw.h"
#include "arxHeaders.h"
#include "kernel.h"
#include "DbImpl.h"
#include "GiImpl.h"

GiWorldDrawManager::GiWorldDrawManager()
{
	this->m_pImpl = new GiWorldDrawManagerImpl();
}
GiWorldDrawManager::~GiWorldDrawManager()
{
}

void GiWorldDrawManager::onBeginAnimationFrame(GiWorldDrawManager* pointer)
{
	for (int i = 0; i < GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws.length(); i++) {
		if (((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->document() == ::curDoc()) {
			((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->draw();
			break;
		}
	}
}
void GiWorldDrawManager::onDocumentCreated(GiWorldDrawManager* pointer, ApDocument* pDocCreating) {
	GiEntDraw* draw = new GiEntDraw();
	draw->setDocument(pDocCreating);
	GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws.append(draw);
}
void GiWorldDrawManager::onDocumentDestroyed(GiWorldDrawManager* pointer, ApDocument* pDocCreating) {
	for (int i = 0; i < GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws.length(); i++) {
		if (((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->document() == pDocCreating) {
			realloc_arry_at(GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws, i);
			break;
		}
	}
}
void GiWorldDrawManager::onObjectModified(GiWorldDrawManager* pointer, const DbDatabase* database, DbObject* object)
{
	/** 如果是图层被修改 */
	bool isEntity = false;
	bool isLayerTableRecord = false;
	if (DB_IMP_OBJECT(object->m_pImpl)->isLayerTableRecord() == true) {
		if (DB_IMP_OBJECT(object->m_pImpl)->isWrite == false) {
			isLayerTableRecord = true;
			DB_IMP_OBJECT(object->m_pImpl)->isWrite = true;
		}
	}

	/** 如果是实体被修改 */
	if (DB_IMP_OBJECT(object->m_pImpl)->isEntity() == true) {
		if (DB_IMP_OBJECT(object->m_pImpl)->isWrite == false) {
			isEntity = true;
			DB_IMP_OBJECT(object->m_pImpl)->isWrite = true;
		}
	}

	/** 标记绘制 */
	if (isEntity == true) {
		for (int i = 0; i < GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws.length(); i++) {
			if (database == ((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->document()->database()) {
				((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->entityIds().append(object->objectId().asOldId());
			}
		}
	}
	else if (isLayerTableRecord == true) {
		for (int i = 0; i < GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws.length(); i++) {
			if (database == ((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->document()->database()) {
				((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(pointer->m_pImpl)->entDraws[i])->layerTableRecordIds().append(object->objectId().asOldId());
			}
		}
	}
}

void GiWorldDrawManager::onInit()
{
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)GiWorldDrawManager::onBeginAnimationFrame, this);
	::kernel()->acevEventManager()->on(Ev::DatabaseReactor::kObjectModified, (void*)GiWorldDrawManager::onObjectModified, this);
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentCreated, (void*)GiWorldDrawManager::onDocumentCreated, this);
	::kernel()->acevEventManager()->on(Ev::DocManagerReactor::kDocumentDestroyed, (void*)GiWorldDrawManager::onDocumentDestroyed, this);
}

void GiWorldDrawManager::draw(ApDocument* document)
{

	DbDatabase* database = document->database();
	if (database == NULL)
	{
		return;
	}

	DbBlockTable* pBlockTable = NULL;
	database->getBlockTable(pBlockTable);

	DbBlockTableIterator* pBlockTableIterator = NULL;
	pBlockTable->newIterator(pBlockTableIterator);
	for (pBlockTableIterator->start(); !pBlockTableIterator->done(); pBlockTableIterator->step())
	{
		DbBlockTableRecord* pBlockTableRecord = NULL;
		pBlockTableIterator->getRecord(pBlockTableRecord);
		if (pBlockTableRecord == NULL)
		{
			continue;
		}
		if (pBlockTableRecord->isLayout() == false)
		{
			pBlockTableRecord->close();
			continue;
		}
		DbBlockTableRecordIterator* pBlockTableRecordIterator = NULL;
		pBlockTableRecord->newIterator(pBlockTableRecordIterator);
		for (pBlockTableRecordIterator->start(); !pBlockTableRecordIterator->done(); pBlockTableRecordIterator->step())
		{
			DbEntity* entity = NULL;
			pBlockTableRecordIterator->getEntity(entity, Db::OpenMode::kForWrite);
			if (entity == NULL)
			{
				continue;
			}
			entity->close();
		}
		delete pBlockTableRecordIterator;

		pBlockTableRecord->close();
	}
	delete pBlockTableIterator;

	pBlockTable->close();
}
GiWorldDraw* GiWorldDrawManager::worldDraw(ApDocument* document)
{
	for (int i = 0; i < GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entDraws.length(); i++) {
		if (((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entDraws[i])->document() == document) {
			return GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entDraws[i];
		}
	}
	return NULL;
}
GiWorldDraw* GiWorldDrawManager::worldDraw(DbDatabase* database)
{
	for (int i = 0; i < GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entDraws.length(); i++) {
		if (((GiEntDraw*)GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entDraws[i])->document()->database() == database) {
			return GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entDraws[i];
		}
	}
	return NULL;
}

GiEntityHighlander* GiWorldDrawManager::entityHighlander()
{
	return GI_IMP_WORLDDRAWMANAGER(this->m_pImpl)->entityHighlander;
}