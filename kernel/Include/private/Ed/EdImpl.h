#ifndef EDIMPL_H
#define EDIMPL_H

#include "AcArray.h"
#include "RxImpl.h"
#include "RxEventReactor.h"
#include "AcEmsdkEvent.h"
#include "EdJig.h"
#include "EdTouch.h"
#include "EdWheelEvent.h"
#include "EdController.h"
#include "EdUserInputEvent.h"
#include "EdCommand.h"
#include "EdCommandReactor.h"
#include "EdCommandStack.h"
#include "EdCommandStackReactor.h"
#include "EdSelectionSetIndex.h"
#include "DbObjectIdArray.h"
#include "GiMaterial.h"
#include "GiEntity.h"
#include "GePoint2d.h"
#include "AcEvent.h"
#include "AcMap.h"
#include <thread>
#include <mutex>
#include <atomic>

#define RX_IMP_COMMAND(x) ((EdCommandImpl *)x)
#define RX_IMP_REGISTRYCOMMAND(x) ((EdRegistryCommandImpl *)x)

#define RX_IMP_ITERATOR(x) ((EdIteratorImpl *)x)
#define RX_IMP_COMMANDREACTOR(x) ((EdCommandReactorImpl *)x)
#define RX_IMP_COMMANDITERATOR(x) ((EdCommandIteratorImpl *)x)
#define RX_IMP_REGISTRYCOMMANDITERATOR(x) ((EdRegistryCommandIteratorImpl *)x)

#define RX_IMP_COMMANDSTACK(x) ((EdCommandStackImpl *)x)
#define RX_IMP_COMMANDSTACKREACTOR(x) ((EdCommandStackReactorImpl *)x)
#define RX_IMP_REGISTRYCOMMANDSTACK(x) ((EdRegistryCommandStackImpl *)x)

#define RX_IMP_SELECTIONSETMANAGER(x) ((EdSelectionSetManagerImpl *)x)

#define RX_IMP_USERINPUTMANAGER(x) ((EdUserInputManagerImpl *)x)

#define ED_IMP_CONTROLLER(x) ((EdControllerImpl *)x)
#define ED_IMP_ACADCONTROLLER(x) ((EdAcadControllerImpl *)x)
#define ED_IMP_ACADCONTROLLER(x) ((EdAcadControllerImpl *)x)
#define ED_IMP_CONTROLLERMANAGER(x) ((EdControllerManagerImpl *)x)

#define ED_IMP_TOUCHCONTROLLER(x) ((EdTouchControllerImpl *)x)

#define ED_IMP_DRAGREACTOR(x) ((EdDragReactorImpl *)x)
#define ED_IMP_DRAGMANAGER(x) ((EdDragManagerImpl *)x)

#define ED_IMP_JIG(x) ((EdJigImpl *)x)
#define ED_IMP_JIGSETTER(x) ((EdJigSetterImpl *)x)

#define ED_IMP_EDITTORREACTOR(x) ((EditorReactorImpl *)x)
#define ED_IMP_EDITTORREACTORMANAGER(x) ((EditorReactorManagerImpl *)x)

#define ED_IMP_DRAGCOMPUTEMANAGER(x) ((EdDragComputeManagerImpl *)x)

#define ED_IMP_CONTEXTINPUTPOINTMANAGER(x) ((EdContextInputPointManagerImpl *)x)

#define ED_IMP_OSNAPCOMPUTEMANAGER(x) ((EdOsnapComputeManagerImpl *)x)


class EdUserInputManagerImpl : public RxObjectImpl
{
public:
	EdUserInputManagerImpl()
	{
		this->isLock = false;
		this->pEvent = new AcEvent();

		this->fps = 60;
		this->devicePixelRatio = 1;
		this->rangTop = 0;
		this->rangBottom = 0;
		this->rangLeft = 0;
		this->rangRight = 0;
		this->mouse[0] = 0.0;
		this->mouse[1] = 0.0;

		this->shift = false;
		this->ctrl = false;
	}
	virtual ~EdUserInputManagerImpl()
	{
		delete this->pEvent;
	}

	bool isLock;
	int fps;
	AcEvent *pEvent;
	double rangTop;
	double rangBottom;
	double rangLeft;
	double rangRight;
	double screenWidth;
	double screenHeight;
	double devicePixelRatio;
	double mouse[2];

	bool shift;
	bool ctrl;

	AcArray<EdUserInputEvent *> userInputEvent;
};


class EdControllerImpl : public RxObjectImpl
{
public:
	EdControllerImpl()
	{
	}
	virtual ~EdControllerImpl()
	{
	}
};
class EdAcadControllerImpl : public EdControllerImpl
{
public:
	EdAcadControllerImpl()
	{
		this->isDrag = false;

		this->isViewZoom = false;
		this->viewTimeCount = 0;

		this->isTwist = false;
	}
	virtual ~EdAcadControllerImpl()
	{
	}

	bool isDrag;

	bool isMoveView;
	GePoint2d dragStartPoint;
	GePoint2d dragEndPoint;

	bool isViewZoom;
	unsigned int viewTimeCount;

	bool isTwist;
	GePoint2d TwistBasePoint;
	GePoint2d TwistStartPoint;
	GePoint2d TwistEndPoint;
	GePoint3d geometryCenter;
	GePoint3d wcsViewCenter;
};
class EdTouchControllerImpl : public EdControllerImpl
{
public:
	EdTouchControllerImpl()
	{
		this->touches = new AcArray<EdTouch*>();
	}
	virtual ~EdTouchControllerImpl()
	{
	}

	AcArray<EdTouch*>* touches;
	GePoint2d initDeviceLocation1;
	GePoint2d initDeviceLocation2;
	GePoint2d startDeviceLocation1;
	GePoint2d startDeviceLocation2;
	GePoint2d endDeviceLocation1;
	GePoint2d endDeviceLocation2;
};
class EdControllerManagerImpl : public RxObjectImpl
{
public:
	EdControllerManagerImpl()
	{
		this->controllers = new AcArray<EdController*>();
	}
	virtual ~EdControllerManagerImpl()
	{
		realloc_all(this->controllers);
	}

	AcArray<EdController*>* controllers;
};


class EdCommandImpl : public RxObjectImpl
{
public:
	EdCommandImpl()
	{
		this->function = NULL;
		this->functionCode = 0;
		this->commandFlags = 0;
		this->flags = 0;
		this->app = NULL;
	}
	virtual ~EdCommandImpl()
	{
	}

	RxFunctionPtr function;
	AcString groupName;
	AcString globalName;
	AcString localName;
	int flags;
	int commandFlags;
	int functionCode;
	void* app;

	AcArray<EdCommandReactor*> reactor;
};
class EdRegistryCommandImpl : public EdCommandImpl
{
public:
	EdRegistryCommandImpl()
	{
	}
	virtual ~EdRegistryCommandImpl()
	{
	}
};


class EdIteratorImpl : public RxObjectImpl
{
public:
	EdIteratorImpl()
	{
	}
	virtual ~EdIteratorImpl()
	{
	}
};
class EdCommandReactorImpl :public RxObjectReactorImpl {
public:
	EdCommandReactorImpl() {
	}
	virtual ~EdCommandReactorImpl() {

	}
};
class EdCommandIteratorImpl : public EdIteratorImpl
{
public:
	EdCommandIteratorImpl()
	{
		this->index = NULL;
		this->pRecords = NULL;
	}
	virtual ~EdCommandIteratorImpl()
	{
	}

	int index;
	AcArray<EdCommand*>* pRecords;

	bool done() const
	{
		if (this->index < 0 || this->index > this->pRecords->length() - 1 || this->pRecords->length() <= 0)
		{
			return true;
		}
		return false;
	}
	bool next()
	{
		this->index = this->index + 1;
		return this->done();
	}
	EdCommand* command() const
	{
		if (this->done() == true)
		{
			return NULL;
		}
		return this->pRecords->at(this->index);
	}
	const ACHAR* key() const
	{
		if (this->done() == true)
		{
			return NULL;
		}
		return this->pRecords->at(this->index)->globalName();
	}
};
class EdRegistryCommandIteratorImpl : public EdCommandIteratorImpl
{
public:
	EdRegistryCommandIteratorImpl()
	{
	}
	virtual ~EdRegistryCommandIteratorImpl()
	{
	}
};


class EdCommandStackImpl : public RxObjectImpl
{
public:
	EdCommandStackImpl()
	{
		this->pCommands = new AcArray<EdCommand*>();
	}
	virtual ~EdCommandStackImpl()
	{
		realloc_all(this->pCommands);
	}

	AcArray<EdCommand*>* pCommands;
	AcArray<EdCommandStackReactor*> reactors;
};
class EdCommandStackReactorImpl :public RxObjectReactorImpl {
public:
	EdCommandStackReactorImpl() {
	}
	virtual ~EdCommandStackReactorImpl() {

	}
};
class EdRegistryCommandStackImpl : public EdCommandStackImpl
{
public:
	EdRegistryCommandStackImpl()
	{
		this->isCommand = false;
	}
	virtual ~EdRegistryCommandStackImpl()
	{
	}

	AcString command;
	bool isCommand;
};


class EditorReactorImpl : public RxObjectReactorImpl
{
public:
	EditorReactorImpl()
	{
	}
	virtual ~EditorReactorImpl()
	{
	}
};
class EditorReactorManagerImpl : public RxObjectImpl
{

public:
	EditorReactorManagerImpl()
	{
	}
	virtual ~EditorReactorManagerImpl()
	{
		realloc_arry_all(this->reactors);
	}

	AcArray<RxEventReactor*> reactors;
};


class EdJigImpl : public RxObjectImpl
{
public:
	EdJigImpl()
	{
		this->dynMode = 0;
		this->bConfirm = true;
		this->bSpace = false;
		this->bKeyword = false;
		this->bDrag = false;
		this->bAsyncDone = false;
		this->dimDatas = new DbDimDataPtrArray();
		this->dragStatus = EdJig::DragStatus::kNormal;
		this->cursorType = EdJig::CursorType::kCrosshair;
		this->userInputControls = EdJig::UserInputControls::kUseBasePointElevation;
	}
	virtual ~EdJigImpl()
	{
		delete this->dimDatas;
	}

	int dynMode;
	bool bConfirm;									// 是否允许输入确认
	bool bSpace;									// 是否允许输入空格
	bool bKeyword;									// 是否是快捷键
	bool bDrag;
	bool bAsyncDone;
	EdJig::DragStatus dragStatus;
	GePoint3d position;
	EdJig::UserInputControls userInputControls;
	EdJig::CursorType cursorType;
	AcString keywordList;
	AcString dispPrompt;
	AcString inputString;
	DbDimDataPtrArray* dimDatas;
	DbDimDataPtrArray dynDimDatas;

	GePoint3d m_ucsOrigin;
	GeVector3d m_ucsXAxis;
	GeVector3d m_ucsYAxis;
	GeVector3d m_ucsZAxis;
	GeVector3d m_referXAxis;
};


class EdSelectionSetManagerImpl : public RxObjectImpl
{
public:
	EdSelectionSetManagerImpl()
	{
		this->isJudgmentPickup = false;
		this->isJudgmentFrameSelect = false;
		this->isFrameSelect = false;
		this->isCommand = false;
		this->indexSelectionSet = 0;

		this->lastMaterialId = 0;
		this->lastEntity = NULL;
	}
	virtual ~EdSelectionSetManagerImpl()
	{
	}

	int commandFlags;					// 命令标志
	bool isJudgmentPickup;				// 拾取判断
	GePoint2d downDeviceLocation;		// 鼠标按下位置
	bool isJudgmentFrameSelect;			// 框选判断
	bool isFrameSelect;					// 是否处于框选状态
	bool isCommand;						// 是否正在执行命令
	DbObjectIdArray selectionSet;		// 当前选择集
	DbObjectIdArray lateSelectionSet;	// 上一个选择集

	unsigned int lastMaterialId;
	GiEntity* lastEntity;

	int indexSelectionSet;
	AcArray<EdSelectionSetIndex*> selectionSetIndexs;
};



#endif
