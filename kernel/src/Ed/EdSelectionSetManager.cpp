#include "EdSelectionSetManager.h"
#include "EvEventManager.h"
#include "acedTrans.h"
#include "arxHeaders.h"
#include "ApDocument.h"
#include "GiEntityGeometry.h"
#include "GiLineSegmentGeometry.h"
#include "RxAsyncManager.h"
#include "GiEntityManager.h"
#include "EdUserInputManager.h"
#include "ReRendererManager.h"
#include "ReRendererReactor.h"
#include "DbObjectManager.h"
#include "RxWorkerThreadManager.h"
#include "GiGeometryExtentManager.h"
#include "GiWorldDrawManager.h"
#include "GiMaterialManager.h"
#include "GiEntityHighlander.h"
#include "GiLineSegment.h"
#include "GiMaterial.h"
#include "GiMesh.h"
#include "GiEntity.h"
#include "EdPickJig.h"
#include "EdImpl.h"
#include "DbImpl.h"
#include "kernel.h"
#include "GeMath.h"
#include "AcDebug.h"

EdSelectionSetManager::EdSelectionSetManager()
{
	this->m_pImpl = new EdSelectionSetManagerImpl();
}
EdSelectionSetManager::~EdSelectionSetManager()
{
}

void EdSelectionSetManager::onBeginDrag(EdSelectionSetManager* pointer)
{
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isFrameSelect = true;
}
void EdSelectionSetManager::onEndDrag(EdSelectionSetManager* pointer)
{
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isFrameSelect = false;
}
void EdSelectionSetManager::onCommandStackWillStart(EdSelectionSetManager* pointer, EdCommand* cmd, EdCommandEventArgs* data)
{
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand = true;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->commandFlags = cmd->commandFlags();
}
void EdSelectionSetManager::onCommandStackEnded(EdSelectionSetManager* pointer, EdCommand* cmd, EdCommandEventArgs* data)
{
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand = false;
	if (int(RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->commandFlags & ACRX_CMD_REDRAW) != ACRX_CMD_REDRAW)
	{
		pointer->removeSelectionSet();
	}
}
void EdSelectionSetManager::onCommandFailed(EdSelectionSetManager* pointer, const ACHAR* cmdStr)
{
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand = false;

	/* 取消选择集 */
	pointer->removeSelectionSet();
}
void EdSelectionSetManager::onCommandCancelled(EdSelectionSetManager* pointer, const ACHAR* cmdStr)
{
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand = false;

	/* 取消选择集 */
	pointer->removeSelectionSet();
}

/* 鼠标按下事件 */
void EdSelectionSetManager::onMousedown(EdSelectionSetManager* pointer, const EdMouseEvent& _mouseEvent)
{
	/* 如果不是鼠标左键则退出 */
	if (_mouseEvent.button != 0)
	{
		return;
	}

	/* 如果正在执行命令则退出 */
	if (RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand == true)
	{
		return;
	}

	/* 如果正在进行框选则退出 */
	if (RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isFrameSelect == true)
	{
		return;
	}

	/* 获得当前屏幕坐标 */
	GePoint2d device = ::kernel()->acedUserInputManager()->deviceLocation();

	/* 判断屏幕点是否在当前视口内 */
	GePoint2d vcs = trans_ncs_to_vcs(device);
	if (vcs.x < 0 || vcs.y < 0 || vcs.x > 1 || vcs.y > 1)
	{
		return;
	}

	/* 拾取判断 */
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentPickup = true;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentFrameSelect = true;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->downDeviceLocation = device;
}
/* 鼠标弹起事件 */
void EdSelectionSetManager::onMouseup(EdSelectionSetManager* pointer, const EdMouseEvent& _mouseEvent)
{

	bool isJudgmentPickup = RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentPickup;
	bool isJudgmentFrameSelect = RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentFrameSelect;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentPickup = false;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentFrameSelect = false;

	/* 如果正在执行命令则退出 */
	if (RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand == true)
	{
		return;
	}

	/* 获得当前ncs */
	GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();

	/* 判断光标位置是否有变化 */
	bool isCursorChange = false;
	if (deviceLocation.isEqualTo(RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->downDeviceLocation) == false)
	{
		isCursorChange = true;
	}

	/* 如果光标位置没有变化则选取光标下实体(未处于框选状态) */
	DbObjectIdArray pickupIds;
	if (isJudgmentPickup == true && isCursorChange == false && RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isFrameSelect == false)
	{


	}

	/* 如果光标位置不变且点下没有实体则进入框选状态(未处于框选状态) */
	if (isJudgmentFrameSelect == true && isCursorChange == false && pickupIds.length() == 0 &&
		RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isFrameSelect == false)
	{
		GePoint2d dcsPoint = ::trans_ncs_to_dcs(RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->downDeviceLocation);

		EdPickJig jig;
		jig.pick(dcsPoint, "\n指定对角点:");
	}
}
/* 鼠标移动事件 */
void EdSelectionSetManager::onMousemove(EdSelectionSetManager* pointer, const EdMouseEvent& _mouseEvent)
{

	/* 如果正在执行命令则退出 */
	if (RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand == true)
	{
		return;
	}

	bool isJudgmentPickup = RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentPickup;
	bool isJudgmentFrameSelect = RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentFrameSelect;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentPickup = false;
	RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isJudgmentFrameSelect = false;

	/* 进入框选状态 */
	if (isJudgmentFrameSelect == true && RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isFrameSelect == false)
	{
		GePoint2d dcsPoint = ::trans_ncs_to_dcs(RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->downDeviceLocation);

		EdPickJig jig;
		jig.pick(dcsPoint, "\n指定对角点:");
	}
}
/* 键盘按下事件 */
void EdSelectionSetManager::onKeydown(EdSelectionSetManager* pointer, const EdKeyboardEvent& keyboardEvent)
{
	std::cout << "按下键盘:"<< keyboardEvent.keyCode << std::endl;
	if (RX_IMP_SELECTIONSETMANAGER(pointer->m_pImpl)->isCommand == false && keyboardEvent.keyCode == 27)
	{
		pointer->removeSelectionSet();
	}
}

void EdSelectionSetManager::onInit()
{
	// 创建512个选择集
	RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->indexSelectionSet = 0;
	for (int i = 0; i < 512; i++)
	{
		EdSelectionSetIndex* selectionSetIndex = new EdSelectionSetIndex();
		selectionSetIndex->index = i + 1;
		RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSetIndexs.append(selectionSetIndex);
	}

	::kernel()->acevEventManager()->on(Ev::Drag::kBeginDrag, (void*)EdSelectionSetManager::onBeginDrag, this);
	::kernel()->acevEventManager()->on(Ev::Drag::kEndDrag, (void*)EdSelectionSetManager::onEndDrag, this);

	::kernel()->acevEventManager()->on(Ev::CommandStackReactor::kCommandStackWillStart, (void*)onCommandStackWillStart, this);
	::kernel()->acevEventManager()->on(Ev::CommandStackReactor::kCommandStackEnded, (void*)onCommandStackEnded, this);
	::kernel()->acevEventManager()->on(Ev::EditorReactor::kCommandFailed, (void*)onCommandFailed, this);
	::kernel()->acevEventManager()->on(Ev::EditorReactor::kCommandCancelled, (void*)onCommandCancelled, this);

	::kernel()->acevEventManager()->on(Ev::UserInput::kMousedown, (void*)EdSelectionSetManager::onMousedown, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kMousemove, (void*)EdSelectionSetManager::onMousemove, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kMouseup, (void*)EdSelectionSetManager::onMouseup, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kKeydown, (void*)EdSelectionSetManager::onKeydown, this);
}

void EdSelectionSetManager::unSelectionSetHighlight()
{
	for (int i = 0; i < RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.length(); i++)
	{
		DbEntity* entity = (DbEntity*)::kernel()->acdbObjectManager()->openDbObject(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i], false);
		if (entity == NULL)
		{
			continue;
		}

		// 去除高亮
		::kernel()->acgiWorldDrawManager()->entityHighlander()->unhighlight(entity);
	}
}
void EdSelectionSetManager::removeSelectionSet()
{
	RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->lateSelectionSet.removeAll();
	for (int i = 0; i < RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.length(); i++)
	{
		DbEntity* entity = (DbEntity*)::kernel()->acdbObjectManager()->openDbObject(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i], false);
		if (entity == NULL)
		{
			continue;
		}

		// 移除选择集标记
		DB_IMP_ENTITY(entity->m_pImpl)->mark = false;

		// 去除高亮
		::kernel()->acgiWorldDrawManager()->entityHighlander()->unhighlight(entity);

		// 添加到上一个选择集
		RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->lateSelectionSet.append(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i]);
	}

	// 移除选择集
	RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.removeAll();
}
void EdSelectionSetManager::setSelectionSet(DbObjectIdArray& _list, const char mode)
{
	// 高亮实体并对实体进行标记
	if (mode == 0)
	{
		for (int i = 0; i < _list.length(); i++) {
			DbEntity* entity = (DbEntity*)::kernel()->acdbObjectManager()->openDbObject(_list[i]);
			if (entity == NULL)
			{
				continue;
			}
			if (DB_IMP_ENTITY(entity->m_pImpl)->mark == false)
			{
				// 标记选择
				DB_IMP_ENTITY(entity->m_pImpl)->mark = true;

				// 高亮
				::kernel()->acgiWorldDrawManager()->entityHighlander()->highlight(entity);

				// 添加到列表
				RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.append(_list[i]);
			}
		}
	}
	else
	{
		// 解除标记
		for (int i = 0; i < _list.length(); i++)
		{
			DbEntity* entity = (DbEntity*)::kernel()->acdbObjectManager()->openDbObject(_list[i]);
			if (entity == NULL)
			{
				continue;
			}
			if (DB_IMP_ENTITY(entity->m_pImpl)->mark == true)
			{
				// 解除标记
				DB_IMP_ENTITY(entity->m_pImpl)->mark = false;

				// 解除高亮
				::kernel()->acgiWorldDrawManager()->entityHighlander()->unhighlight(entity);
			}
		}

		// 获得未被标记的实体
		DbObjectIdArray selectionSet;
		for (int i = 0; i < RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.length(); i++)
		{
			DbEntity* entity = (DbEntity*)::kernel()->acdbObjectManager()->openDbObject(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i]);
			if (entity == NULL)
			{
				continue;
			}
			if (DB_IMP_ENTITY(entity->m_pImpl)->mark == true)
			{
				selectionSet.append(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i]);
			}
		}

		// 重新设置选择集
		RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.removeAll();
		for (int i = 0; i < selectionSet.length(); i++)
		{
			RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.append(selectionSet[i]);
		}
	}

	// 移除已经被删除的实体
	DbObjectIdArray selectionSet;
	for (int i = 0; i < RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.length(); i++)
	{
		DbEntity* entity = (DbEntity*)::kernel()->acdbObjectManager()->openDbObject(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i]);
		if (entity == NULL)
		{
			continue;
		}
		if (entity->isErased() == true)
		{
			continue;
		}
		selectionSet.append(RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet[i]);
	}
	RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.removeAll();
	for (int i = 0; i < selectionSet.length(); i++)
	{
		RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet.append(selectionSet[i]);
	}
}

DbObjectIdArray& EdSelectionSetManager::selectionSetEnts()const
{
	return RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSet;
}
DbObjectIdArray& EdSelectionSetManager::latelySelectionSet()const
{
	return RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->lateSelectionSet;
}
DbObjectIdArray EdSelectionSetManager::selectExtents(const GePoint2d& _startPoint, const GePoint2d& _endPoint, const char mode, const int maxEntity)
{
	// 获得范围内实体
	const AcArray<DbEntity*>* entitys = this->extentEntitys(_startPoint, _endPoint);

	// 获得DbObjectId
	DbObjectIdArray list;
	if (maxEntity < 0)
	{
		for (int i = 0; i < entitys->length(); i++)
		{
			list.append(entitys->at(i)->objectId());
		}
	}
	else
	{
		for (int i = 0; i < entitys->length(); i++)
		{
			if (list.length() >= maxEntity)
			{
				break;
			}
			list.append(entitys->at(i)->objectId());
		}
	}
	delete entitys;

	// 设置选择集
	if (list.length() > 0)
	{
		this->setSelectionSet(list, mode);
	}

	return list;
}
const AcArray<DbEntity*>* EdSelectionSetManager::clickDownEntitys(const GePoint2d& _screen)
{

	do
	{

	} while (false);

	return NULL;
}
const AcArray<DbEntity*>* EdSelectionSetManager::extentEntitys(const GePoint2d& _startPoint, const GePoint2d& _endPoint)
{
	AcArray<DbEntity*>* ssEntitys = new AcArray<DbEntity*>();

	do
	{

		/** 判断是左框还是右框 */
		unsigned char mode = 0;
		if (_startPoint.x < _endPoint.x) {
			mode = 1;
		}
		else {
			mode = 2;
		}

		/** 获得几何实体 */
		AcArray<GiEntityGeometry*> geometrys;
		::kernel()->acgiGeometryExtentManager()->extentGeometryEntitys(geometrys, _startPoint, _endPoint, acedActiveViewportId());
		std::cout << "geometrys:" << geometrys.length() << std::endl;
		/** 获得实体 */
		for (int i = 0; i < geometrys.length(); i++) {
			DbEntity* entity = geometrys[i]->stake()->rootEntity();
			if (DB_IMP_ENTITY(entity->m_pImpl)->mark == true) {
				continue;
			}
			DB_IMP_ENTITY(entity->m_pImpl)->mark = true;
			ssEntitys->append(entity);
		}
		for (int i = 0; i < ssEntitys->length(); i++) {
			DbEntity* entity = ssEntitys->at(i);
			DB_IMP_ENTITY(entity->m_pImpl)->mark = false;
		}

		std::cout << "ssEntitys:" << ssEntitys->length() << std::endl;
		std::cout << "" << std::endl;
	} while (false);

	return ssEntitys;
}

EdSelectionSetIndex* EdSelectionSetManager::pushSelectionSetIndex(const int size)
{

	EdSelectionSetIndex* selectionSetIndex = NULL;

	while (true)
	{
		if (RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->indexSelectionSet >= 512)
		{
			RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->indexSelectionSet = 0;
		}
		EdSelectionSetIndex* temp = RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSetIndexs[RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->indexSelectionSet];
		if (temp->isEffective() == true)
		{
			RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->indexSelectionSet++;
			continue;
		}
		selectionSetIndex = temp;
		break;
	}

	if (selectionSetIndex == NULL)
	{
		return NULL;
	}
	selectionSetIndex->init();
	return selectionSetIndex;
}
EdSelectionSetIndex* EdSelectionSetManager::pushSelectionSetIndexAtIndex(const int index)
{
	if (index > 512 || index < 1)
	{
		return NULL;
	}
	EdSelectionSetIndex* selectionSetIndex = RX_IMP_SELECTIONSETMANAGER(this->m_pImpl)->selectionSetIndexs[index - 1];
	if (selectionSetIndex->isEffective() == false)
	{
		return NULL;
	}
	return selectionSetIndex;
}