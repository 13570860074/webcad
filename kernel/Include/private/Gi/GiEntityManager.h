#ifndef GIENTITYMANAGER_H
#define GIENTITYMANAGER_H

#include <mutex>
#include "Gi.h"
#include "GiObject.h"
#include "RxManager.h"
#include "GiEntityMutex.h"
#include "GiEntityGeometry.h"

class DbObjectId;
class GiEntity;
class ApDocument;
class GiEntityReactor;
class GiEntityIterator;
class GiEntityManager : public RxObject, public RxManager
{
public:
	GiEntityManager();
	virtual ~GiEntityManager();
	RX_DECLARE_MEMBERS(GiEntityManager);
	RX_MANAGER_DECLARE_MEMBERS(GiEntityManager);


private:
	static void onBeginAnimationFrame(GiEntityManager* pointer);
	static void onDocumentDestroyed(GiEntityManager* pointer, ApDocument* doc);
	static void onDocumentToBeActivated(GiEntityManager* pointer, ApDocument* doc);
	static void onDocumentActivated(GiEntityManager* pointer, ApDocument* doc);

public:

	/* 初始化 */
	virtual void onInit();

	/* 添加实体 */
	GiObjectId addEntity(GiEntity* entity)const;

	/* 添加反应器 */
	void addReactor(GiEntityReactor* reactor)const;

	/* 移除反应器 */
	void removeReactor(GiEntityReactor* reactor) const;

	/* 遍历实体 */
	GiEntityIterator* newIterator()const;

	/* 查询实体 */
	GiEntity* queryEntity(const Gi::EntityType type, const Gi::ElementType element, const GiMaterial* material);

	/* 实体线程锁 */
	GiEntityMutex* mutex();
};

#endif