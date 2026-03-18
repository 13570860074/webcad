#ifndef EDSELECTIONSETMANAGER_H
#define EDSELECTIONSETMANAGER_H

#include "AcArray.h"
#include "RxManager.h"
#include "GePoint2d.h"
#include "DbEntity.h"
#include "DbObjectIdArray.h"
#include "EdMouseEvent.h"
#include "EdKeyboardEvent.h"
#include "EdSelectionSetIndex.h"

class ApDocument;
class GiEntityGeometry;
class EdUserInputManager;
class EdCommand;
class EdCommandEventArgs;
class EdSelectionSetManager : public RxObject, public RxManager
{
public:
	EdSelectionSetManager();
	virtual ~EdSelectionSetManager();
	RX_DECLARE_MEMBERS(EdSelectionSetManager);
	RX_MANAGER_DECLARE_MEMBERS(EdSelectionSetManager);

public:
	/* 初始化 */
	virtual void onInit();

	static void onBeginDrag(EdSelectionSetManager *pointer);
	static void onEndDrag(EdSelectionSetManager *pointer);

	static void onCommandStackWillStart(EdSelectionSetManager *pointer, EdCommand *cmd, EdCommandEventArgs *data);
	static void onCommandStackEnded(EdSelectionSetManager *pointer, EdCommand *cmd, EdCommandEventArgs *data);
	static void onCommandFailed(EdSelectionSetManager *pointer, const ACHAR *cmdStr);
	static void onCommandCancelled(EdSelectionSetManager *pointer, const ACHAR *cmdStr);

	static void onMousedown(EdSelectionSetManager *pointer, const EdMouseEvent &_mouseEvent);
	static void onMouseup(EdSelectionSetManager *pointer, const EdMouseEvent &_mouseEvent);
	static void onMousemove(EdSelectionSetManager *pointer, const EdMouseEvent &_mouseEvent);
	static void onKeydown(EdSelectionSetManager *pointer, const EdKeyboardEvent &keyboardEvent);

public:
	/* 移除选择集 */
	void removeSelectionSet();

	/* 解除选择集高亮(依旧处于选择状态) */
	void unSelectionSetHighlight();

	/*
	 * _list:对象列表
	 * mode:模式0表正选,1表反选
	 */
	/* 设置选择集 */
	void setSelectionSet(DbObjectIdArray &_list, const char mode = 0);

	/*
	 * _startPoint:起始点dcs
	 * _endPoint:终止点dcs
	 * mode:模式0表正选,1表反选
	 * maxEntity:需要选择的数量,-1表不限制数量
	 */
	/* 选择范围内的实体 */
	DbObjectIdArray selectExtents(const GePoint2d &_startPoint, const GePoint2d &_endPoint, const char mode, const int maxEntity);

	/* 获得范围内实体 */
	const AcArray<DbEntity *> *extentEntitys(const GePoint2d &_startPoint, const GePoint2d &_endPoint);

	/* 获得当前选择集实体 */
	DbObjectIdArray& selectionSetEnts() const;

	/* 获得上一个选择集 */
	DbObjectIdArray& latelySelectionSet() const;

	/* 获得点下实体 */
	const AcArray<DbEntity *> *clickDownEntitys(const GePoint2d &_screen);

	/* 获取一个选择集 */
	EdSelectionSetIndex *pushSelectionSetIndex(const int size = 0);

	/* 通过index获得选择集 */
	EdSelectionSetIndex *pushSelectionSetIndexAtIndex(const int index);
};

#endif