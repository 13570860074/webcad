#include "EvEventManager.h"
#include "EvRendererReactor.h"
#include "ReRendererManager.h"
#include "EvDocManagerReactor.h"
#include "EvDatabaseReactor.h"
#include "ApDocument.h"
#include "ApDocManager.h"
#include "kernel.h"

EvEventManager::EvEventManager()
{
}
EvEventManager::~EvEventManager()
{
}

void EvEventManager::onDocumentCreated(EvEventManager *pointer, ApDocument *pDocCreating)
{
	if (pDocCreating->database() != NULL)
	{
		EvDatabaseReactor* pDatabaseReactor = new EvDatabaseReactor();
		pDocCreating->database()->addReactor(pDatabaseReactor);
	}
}

void EvEventManager::onInit()
{
	EvRendererReactor *pRendererReactor = new EvRendererReactor();
	::kernel()->acreRendererManager()->addReactor(pRendererReactor);

	EvDocManagerReactor *pDocManagerReactor = new EvDocManagerReactor();
	acDocManager->addReactor(pDocManagerReactor);

	/** 监听创建文档事件 */
	this->on(Ev::DocManagerReactor::kDocumentCreated, (void *)onDocumentCreated, this);
}
