#ifndef EV_H
#define EV_H

struct Ev
{
private:
	enum EventHead
	{
		kEditorReactorEventHead = 10000,
		kDocManagerReactorEventHead = 11000,
		kInputContextReactorEventHead = 12000,
		kLayerReactorEventHead = 13000,
		kRendererReactorEventHead = 14000,
		kDatabaseReactorEventHead = 15000,
		kCommandStackReactorEventHead = 16000,
		kObjectStackReactorEventHead = 17000,

		kGiEntityDrawEventHead = 20000,
		kDbEntityDrawEventHead = 21000,
		kDbObjectStackEventHead = 22000,
		kEdSelectionSetEventHead = 23000,
		kEdOsnapEventHead = 24000,

		kUserInputEventHead = 30000,
		kViewportEventHead = 31000,
		kDragEventHead = 32000,
		kControllerEventHead = 33000,
		kFileSystemEventHead = 34000,
		kAsyncEventHead = 35000,

		kIntervalEventHead = 40000,

		kEventPriorityEventHead = 100000,

	};

public:
	/** 反应器事件:由反应器间接触发 **/
	/** --------------------------------------------------------------- */
	// 编辑反应器事件
	enum EditorReactor
	{

		kUnknownCommand = kEditorReactorEventHead + 1,
		kCommandWillStart,
		kCommandEnded,
		kCommandCancelled,
		kCommandFailed,

		kLispWillStart,
		kLispEnded,
		kLispCancelled,

		kBeginDwgOpen,
		kEndDwgOpen,

		kSysVarChanged,
		kSysVarWillChange,

		kUndoSubcommandAuto,
		kUndoSubcommandControl,
		kUndoSubcommandBegin,
		kUndoSubcommandEnd,
		kUndoSubcommandMark,
		kUndoSubcommandBack,
		kUndoSubcommandNumber,

		kPickfirstModified,
		kLayoutSwitched,
		kLayoutToBeSwitched,
		kDwgViewResized,

		kDocFrameMovedOrResized,
		kMainFrameMovedOrResized,

		kBeginDoubleClick,
		kBeginRightClick,

		kToolbarBitmapSizeWillChange,
		kToolbarBitmapSizeChanged,

		kBeginDocClose,
		kDocCloseAborted,

		kDocCloseWillStart,

		kBeginClose,

		kBeginCloseAll,
		kBeginQuit,
		kQuitAborted,
		kQuitWillStart,

		kViewChanged,
	};

	// 文档反应器事件
	enum DocManagerReactor
	{
		kDocumentCreateStarted = kDocManagerReactorEventHead + 1,
		kDocumentCreated,
		kDocumentToBeDestroyed,
		kDocumentDestroyed,
		kDocumentCreateCanceled,
		kDocumentLockModeWillChange,
		kDocumentLockModeChangeVetoed,
		kDocumentLockModeChanged,
		kDocumentBecameCurrent,
		kDocumentToBeActivated,
		kDocumentToBeDeactivated,
		kDocumentActivationModified,
		kDocumentActivated,
		kDocumentTitleUpdated,
	};

	// 上下文输入事件
	enum InputContextReactor
	{
		kBeginGetPoint = kInputContextReactorEventHead + 1,
		kEndGetPoint,
		kBeginGetAngle,
		kEndGetAngle,
		kBeginGetDistance,
		kEndGetDistance,
		kBeginGetOrientation,
		kEndGetOrientation,
		kBeginGetCorner,
		kEndGetCorner,
		kBeginGetScaleFactor,
		kEndGetScaleFactor,
		kBeginGetString,
		kEndGetString,
		kBeginGetKeyword,
		kEndGetKeyword,
		kBeginGetInteger,
		kEndGetInteger,
		kBeginGetColor,
		kEndGetColor,
		kBeginGetReal,
		kEndGetReal,
		kBeginEntsel,
		kEndEntsel,
		kBeginNentsel,
		kEndNentsel,
		kBeginSSGet,
		kEndSSGet,
		kBeginDragSequence,
		kEndDragSequence,
		kBeginQuiescentState,
		kEndQuiescentState,
	};

	// 命令反应器
	enum CommandStackReactor
	{
		kCommandStackAdded = kCommandStackReactorEventHead + 1,
		kCommandStackRemoved,
		kCommandStackWillStart,
		kCommandStackEnded,
	};

	// 图层反应器事件
	enum LayerReactor
	{
		kSwitchLayer = kLayerReactorEventHead + 1,
	};

	// 渲染反应器事件
	enum RendererReactor
	{
		kBeginAnimationFrame = kRendererReactorEventHead + 1,
		kEndAnimationFrame,
		kBeginRenderer,
		kEndRenderer,
		kBeginRendererViewport,
		kEndRendererViewport,
		kRendererViewport,
		kBeginCallbackRenderer,
		kEndCallbackRenderer,
	};

	// 数据库反应器
	enum DatabaseReactor
	{
		kObjectAppended = EventHead::kDatabaseReactorEventHead + 1,
		kObjectUnAppended,
		kObjectReAppended,
		kObjectOpenedForModify,
		kObjectModified,
		kObjectErased,
		kHeaderSysVarWillChange,
		kHeaderSysVarChanged,
		kProxyResurrectionCompleted,
		kGoodbye,
	};

	// Db对象堆栈反应器
	enum ObjectStackReactor{
		kCreateUndoStackWillStart = EventHead::kObjectStackReactorEventHead + 1,
		kCreateUndoStackEnd,
		kAppendRedoStack,
		kRemoveRedoStacks,
	};

	/** --------------------------------------------------------------- */

	// Gi实体绘制事件
	enum GiEntityDraw
	{
		kCreateEntityWillStart = kGiEntityDrawEventHead + 1, // 创建实体将要开始
		kCreateEntityAllEnd,								 // 创建实体完成

		kCreateElementEntityWillStart, // 创建图元实体将要开始
		kCreateElementEntityEnd,	   // 创建图元实体完成

		kCreateDraggingEntityWillStart, // 创建拖拽实体将要开始
		kCreateDraggingEntityEnd,		// 创建拖拽实体完成

		kUpdateEntityWillStart, // 更新实体将要开始
		kUpdateEntityAllEnd,	// 更新实体完成

		kUpdateElementEntityWillStart, // 更新图元实体将要开始
		kUpdateElementEntityEnd,	   // 更新图元实体完成

		kUpdateDraggingEntityWillStart, // 更新拖拽实体将要开始
		kUpdateDraggingEntityEnd,		// 更新拖拽实体完成
	};

	// Db实体绘制事件
	enum DbEntityDraw
	{
		kEntityDrawBegin = kDbEntityDrawEventHead + 1, // 绘制实体开始
		kEntityDrawEnd,								   // 绘制实体完成
	};

	// Db对象堆栈事件
	enum DbObjectStack
	{
		kObjectToDb = kDbObjectStackEventHead + 1, // 对象加入到数据库
		kObjectOpen,							   // 对象被打开事件
		kObjectClose,							   // 对象被关闭
	};

	// Ed选择集事件
	enum EdSelectionSet
	{
		kSelectionChanged = kEdSelectionSetEventHead + 1,
	};

	// Ed对象捕捉事件
	enum EdOsnap
	{
		kUpdateOsnap = kEdOsnapEventHead + 1,
	};

	// 用户输入事件
	enum UserInput
	{
		kMousemove = kUserInputEventHead + 1, // 鼠标移动
		kMousewheel,						  // 滚轮滚动
		kMouseup,							  // 鼠标抬起
		kMousedown,							  // 鼠标按下
		kDblclick,							  // 双击事件

		kKeydown, // 键盘按下
		kKeyup,	  // 按键弹起

		kTouchstart, // 触摸开始
		kTouchend,	 // 触摸解除
		kTouchmove,	 // 触摸移动
	};

	// 视口事件
	enum Viewport
	{
		kSwitchViewport = kViewportEventHead + 1,
	};

	// 拖拽事件
	enum Drag
	{
		kBeginDrag = kDragEventHead + 1, // 开始拖拽
		kEndDrag,						 // 结束拖拽
		kUserInput,						 // 用户输入
		kBeginUpdate,					 // 开始更新
		kEndUpdate,						 // 结束更新

		kJigBeginDrag,		// EdJig开始拖拽
		kJigUpdateDynDim,	// EdJig更新动态尺寸
		kJigUpdateDragline, // EdJig更新拖拽线
		kJigSetupDimValue,	// EdJig设置动态值
		kJigEndDrag,		// EdJig结束拖拽
	};

	// 控制器事件
	enum Controller
	{
		kMoveViewBegin = kControllerEventHead + 1, // 移动视口开始
		kMoveViewProgress,						   // 移动视口进行中
		kMoveViewEnd,							   // 移动视口结束

		kZoomViewBegin, // 缩放视口开始
		kZoomViewEnd,	// 缩放视口结束

		kTwistBegin, // 转动开始
		kTwistEnd,	 // 转动结束
	};

	// 文件系统事件
	enum FileSystem
	{
		kInitFileSystem = kFileSystemEventHead + 1, // 初始化文件系统
	};

	// 异步系统事件
	enum Async
	{
		kAsyncBegin = kAsyncEventHead + 1, // 进入异步状态
		kAsyncEnd,						   // 结束异步状态

		kAsyncAwaitBegin, // 异步等待开始
		kAsyncAwaitEnd,	  // 异步等待结束
	};

	// 事件优先级
	enum EventPriority
	{
		kKeydown_AppDrawing = kEventPriorityEventHead + 1, // AppDrawing键盘按下
		kKeydown_GripStretch,							   // GripStretch键盘按下(夹点拖动)
	};

	enum Interval {
		kInterval = kIntervalEventHead + 1,
	};
};

#endif
