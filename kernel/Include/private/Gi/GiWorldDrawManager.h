#ifndef GIWORLDDRAWMANAGER_H
#define GIWORLDDRAWMANAGER_H

#include "RxManager.h"

class DbObject;
class ApDocument;
class DbDatabase;
class GiWorldDraw;
class GiEntityHighlander;
class GiWorldDrawManagerImpl;

class GiWorldDrawManager : public RxObject, public RxManager
{
public:
	GiWorldDrawManager();
	virtual ~GiWorldDrawManager();
	RX_DECLARE_MEMBERS(GiWorldDrawManager);
	RX_MANAGER_DECLARE_MEMBERS(GiWorldDrawManager);

private:
	virtual void onInit();

	static void onBeginAnimationFrame(GiWorldDrawManager *pointer);
	static void onDocumentCreated(GiWorldDrawManager *pointer, ApDocument *pDocCreating);
	static void onDocumentDestroyed(GiWorldDrawManager *pointer, ApDocument *pDocCreating);
	static void onObjectModified(GiWorldDrawManager *pointer, const DbDatabase *database, DbObject *object);

public:
	/* 重绘 */
	void draw(ApDocument *document);

	/* 获得世界绘制器 */
	GiWorldDraw *worldDraw(ApDocument *document);
	GiWorldDraw *worldDraw(DbDatabase *database);

	/* 实体高亮器 */
	GiEntityHighlander *entityHighlander();
};

#endif