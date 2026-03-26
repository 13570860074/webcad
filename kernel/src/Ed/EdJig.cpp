#include "kernel.h"
#include "arxHeaders.h"
#include "RxAsyncManager.h"
#include "DbOsnapData.h"
#include "DbObjectManager.h"
#include "EvEventManager.h"
#include "EdTouchEvent.h"
#include "EdKeyboardEvent.h"
#include "EdUserInputManager.h"
#include "acedTrans.h"
#include "AcDebug.h"
#include "EdImpl.h"


EdJig::EdJig()
{
	this->m_pImpl = new EdJigImpl();

	DbObject* viewport = NULL;
	ED_IMP_JIG(this->m_pImpl)->m_ucsOrigin = GePoint3d::kOrigin;
	ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis = GeVector3d::kXAxis;
	ED_IMP_JIG(this->m_pImpl)->m_ucsYAxis = GeVector3d::kYAxis;
	ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis = GeVector3d::kZAxis;
	if (::acdbOpenObject(viewport, acedActiveViewportId()) == Acad::ErrorStatus::eOk)
	{
		if (viewport->isA() == DbViewportTableRecord::desc())
		{
			DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)viewport;
			pViewportTableRecord->getUcs(ED_IMP_JIG(this->m_pImpl)->m_ucsOrigin, ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis, ED_IMP_JIG(this->m_pImpl)->m_ucsYAxis);
		}
		else if (viewport->isA() == DbViewport::desc())
		{
			DbViewport* pViewport = (DbViewport*)viewport;
			pViewport->getUcs(ED_IMP_JIG(this->m_pImpl)->m_ucsOrigin, ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis, ED_IMP_JIG(this->m_pImpl)->m_ucsYAxis);
		}
		viewport->close();
		ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis = ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis.crossProduct(ED_IMP_JIG(this->m_pImpl)->m_ucsYAxis);
	}

	GeMatrix3d rotateMat;
	rotateMat.setToPlaneToWorld(GePlane(GePoint3d::kOrigin, ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis));
	ED_IMP_JIG(this->m_pImpl)->m_referXAxis = GeVector3d::kXAxis;
	ED_IMP_JIG(this->m_pImpl)->m_referXAxis.transformBy(rotateMat);
}
EdJig::~EdJig()
{

}


static void onEdJigSysVarChanged(EdJig* jig, const ACHAR* varName, bool success)
{

}
static void onEdJigMousedown(EdJig* pointer, const EdMouseEvent& _mouseEvent)
{
	if (_mouseEvent.button == 0) {
		ED_IMP_JIG(pointer->m_pImpl)->bAsyncDone = true;
	}
}
static void onEdJigKeydown(EdJig* pointer, const EdKeyboardEvent& keyboardEvent)
{

	// F8(正交)
	if (keyboardEvent.keyCode == 119) {
		resbuf result;
		acedGetVar("ORTHO", &result);
		bool isOrtho = false;
		if (result.resval.rint == 1) {
			isOrtho = true;
		}
		if (isOrtho == false) {
			result.resval.rint = 1;
			acedSetVar("ORTHO", &result);
		}
		else {
			result.resval.rint = 0;
			acedSetVar("ORTHO", &result);
		}
	}
	// F12(动态尺寸)
	else if (keyboardEvent.keyCode == 123) {

		resbuf result;
		acedGetVar("DYNMODE", &result);
		if (result.resval.rint == 0) {
			result.resval.rint = 3;
			acedSetVar("DYNMODE", &result);
		}
		else {
			result.resval.rint = 0;
			acedSetVar("DYNMODE", &result);
		}
	}

	// ESC键(触发用户取消输入事件)
	if (keyboardEvent.keyCode == 27)
	{
		ED_IMP_JIG(pointer->m_pImpl)->bAsyncDone = true;
		ED_IMP_JIG(pointer->m_pImpl)->dragStatus = EdJig::DragStatus::kCancel;
	}
}
static void onEdJigMousemove(EdJig* pointer, const EdMouseEvent& _mouseEvent)
{

}
static void onEdJigTouchend(EdJig* pointer, const EdTouchEvent& _touchEven) {


}
static void onEdJigTouchmove(EdJig* pointer, const EdTouchEvent& _touchEven) {

}
static void onEdJigViewChanged(EdJig* pointer)
{

}
static void onEdJigSwitchViewport(EdJig* pointer, DbObjectId viewportId)
{

}
static void onEdJigBeginAnimationFrame(EdJig* pointer)
{
	GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();
	GePoint3d wcsPoint = ::trans_ncs_to_wcs(deviceLocation);
	ED_IMP_JIG(pointer->m_pImpl)->position = ::wcs_to_viewport_projection(wcsPoint);

	pointer->sampler();
	pointer->update();
}
static void onEdJigMoveViewBegin(EdJig* pointer, GePoint2d _mouse)
{

}
static void onEdJigMoveViewEnd(EdJig* pointer, GePoint2d _mouse)
{

}
static void onEdJigCommandWillStart(EdJig* pointer, const ACHAR* cmdStr) {

}


EdJig::DragStatus EdJig::drag()
{
	ED_IMP_JIG(this->m_pImpl)->bDrag = true;
	ED_IMP_JIG(this->m_pImpl)->dragStatus = EdJig::DragStatus::kNormal;


	// 设置光标样式
	resbuf res;
	res.restype = Db::DwgDataType::kDwgInt16;
	res.resval.rint = this->specialCursorType();
	acedSetVar("CORSORSTYLE", &res);

	// 触发拖拽开始事件
	::kernel()->acevEventManager()->_emit(Ev::Drag::kBeginDrag);

	// 监听事件
	AcArray<int> eventIds;
	eventIds.append(::kernel()->acevEventManager()->on(Ev::UserInput::kTouchend, (void*)onEdJigTouchend, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::UserInput::kTouchmove, (void*)onEdJigTouchmove, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::UserInput::kMousedown, (void*)onEdJigMousedown, this));
	//eventIds.append(::kernel()->acevEventManager()->on(Ev::UserInput::kMousemove, (void*)onEdJigMousemove, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::UserInput::kKeydown, (void*)onEdJigKeydown, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::Controller::kMoveViewBegin, (void*)onEdJigMoveViewBegin, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::Controller::kMoveViewEnd, (void*)onEdJigMoveViewEnd, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::Viewport::kSwitchViewport, (void*)onEdJigSwitchViewport, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::EditorReactor::kViewChanged, (void*)onEdJigViewChanged, this));
	eventIds.append(::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)onEdJigBeginAnimationFrame, this));

	/** 异步等待 */
	ED_IMP_JIG(this->m_pImpl)->bAsyncDone = false;
	::kernel()->acrxAsyncManager()->async([this]() -> bool
		{
			return ED_IMP_JIG(this->m_pImpl)->bAsyncDone;
		});

	// 触发拖拽完成事件
	::kernel()->acevEventManager()->_emit(Ev::Drag::kEndDrag);

	// 移除事件
	for (int i = 0; i < eventIds.length(); i++) {
		::kernel()->acevEventManager()->removeEventListenerId(eventIds[i]);
	}

	ED_IMP_JIG(this->m_pImpl)->bDrag = false;

	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::drag(const EdDragStyle& style)
{
	return this->drag();
}
DbObjectId EdJig::append()
{

	if (this->entity() == NULL)
	{
		return NULL;
	}

	DbObjectId id = NULL;

	DbDatabase* database = acdbHostApplicationServices()->workingDatabase();
	if (database != NULL)
	{

		DbObjectId currentSpaceId = database->currentSpaceId();

		DbBlockTableRecord* pBlockTableRecord = NULL;
		if (acdbOpenObject(pBlockTableRecord, currentSpaceId, Db::OpenMode::kForWrite) == 0)
		{
			pBlockTableRecord->appendDbEntity(id, this->entity());
			this->entity()->close();
			pBlockTableRecord->close();
		}
	}

	return id;
}

const ACHAR* EdJig::keywordList()
{
	return ED_IMP_JIG(this->m_pImpl)->keywordList.constPtr();
}
void EdJig::setKeywordList(const ACHAR* _v)
{
	ED_IMP_JIG(this->m_pImpl)->keywordList = _v;
}

const ACHAR* EdJig::dispPrompt()
{
	return ED_IMP_JIG(this->m_pImpl)->dispPrompt.constPtr();
}
void EdJig::setDispPrompt(const ACHAR* args, ...)
{

	va_list list;
	va_start(list, args);
	AcString prompt = AcString::_format(args, list);
	va_end(list);

	AcString dispPrompt = this->dispPrompt();
	dispPrompt += prompt;

	AcString str;
	const unsigned int dispPromptLength = dispPrompt.length();
	for (unsigned int i = 0; i < dispPromptLength; i++)
	{
		if (dispPrompt.at(i) == '\n')
		{
			str += dispPrompt.at(i);
			acedPrompt(str.constPtr());
			str.clear();
		}
		else
		{
			str += dispPrompt.at(i);
		}
	}

	// 设置显示文字
	ED_IMP_JIG(this->m_pImpl)->dispPrompt = str;
}

EdJig::DragStatus EdJig::acquireString(AcString& s)
{
	s.set(ED_IMP_JIG(this->m_pImpl)->inputString.constPtr());
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquireString(ACHAR(&str)[2049])
{
	const unsigned int inputStringLength = ED_IMP_JIG(this->m_pImpl)->inputString.length();
	for (unsigned int i = 0; i < inputStringLength; i++)
	{
		str[i] = ED_IMP_JIG(this->m_pImpl)->inputString.at(i);
		if (i >= 2049 - 1)
		{
			str[i + 1] = '\0';
		}
	}
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquireAngle(double& ang)
{
	if (ED_IMP_JIG(this->m_pImpl)->bDrag == true)
	{
		GeVector3d dir = ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis;
		GePoint3d referPoint = ED_IMP_JIG(this->m_pImpl)->m_ucsOrigin;
		GePoint3d position = ED_IMP_JIG(this->m_pImpl)->position;
		position = position.orthoProject(GePlane(referPoint, ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis));
		if (position.isEqualTo(referPoint) == false)
		{
			dir = position - referPoint;
			dir.normal();
		}
		ang = ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis.angleToCCW(dir, ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis);
		RadianNormalize(ang);
		return ED_IMP_JIG(this->m_pImpl)->dragStatus;
	}

	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquireAngle(double& ang, const GePoint3d& basePnt)
{
	if (ED_IMP_JIG(this->m_pImpl)->bDrag == true)
	{
		::kernel()->acevEventManager()->_emit(Ev::Drag::kJigUpdateDragline, basePnt, ED_IMP_JIG(this->m_pImpl)->position);
		GeVector3d dir = ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis;
		GePoint3d referPoint = basePnt;
		GePoint3d position = ED_IMP_JIG(this->m_pImpl)->position;
		position = position.orthoProject(GePlane(referPoint, ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis));
		if (position.isEqualTo(referPoint) == false)
		{
			dir = position - referPoint;
			dir.normal();
		}
		ang = ED_IMP_JIG(this->m_pImpl)->m_ucsXAxis.angleToCCW(dir, ED_IMP_JIG(this->m_pImpl)->m_ucsZAxis);
		return ED_IMP_JIG(this->m_pImpl)->dragStatus;
	}

	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquireDist(double& dist)
{
	if (ED_IMP_JIG(this->m_pImpl)->bDrag == true)
	{
		dist = ED_IMP_JIG(this->m_pImpl)->position.distanceTo(ED_IMP_JIG(this->m_pImpl)->m_ucsOrigin);
		return ED_IMP_JIG(this->m_pImpl)->dragStatus;
	}
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquireDist(double& dist, const GePoint3d& basePnt)
{
	if (ED_IMP_JIG(this->m_pImpl)->bDrag == true)
	{
		::kernel()->acevEventManager()->_emit(Ev::Drag::kJigUpdateDragline, basePnt, ED_IMP_JIG(this->m_pImpl)->position);
		dist = ED_IMP_JIG(this->m_pImpl)->position.distanceTo(basePnt);
		return ED_IMP_JIG(this->m_pImpl)->dragStatus;
	}
	while (true)
	{
		ED_IMP_JIG(this->m_pImpl)->dragStatus = this->drag();
		if (ED_IMP_JIG(this->m_pImpl)->dragStatus != EdJig::DragStatus::kOther)
		{
			dist = ED_IMP_JIG(this->m_pImpl)->position.distanceTo(basePnt);
			break;
		}
		acedPrompt("\n需要数值距离、第二点或选项关键字。");
		continue;
	};
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquirePoint(GePoint3d& pt)
{
	if (ED_IMP_JIG(this->m_pImpl)->bDrag == true)
	{
		pt = ED_IMP_JIG(this->m_pImpl)->position;
		return ED_IMP_JIG(this->m_pImpl)->dragStatus;
	}
	while (true)
	{
		ED_IMP_JIG(this->m_pImpl)->dragStatus = this->drag();
		if (ED_IMP_JIG(this->m_pImpl)->dragStatus != EdJig::DragStatus::kOther)
		{
			pt = ED_IMP_JIG(this->m_pImpl)->position;
			break;
		}
		acedPrompt("\n需要点或者关键字。");
		continue;
	};
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
EdJig::DragStatus EdJig::acquirePoint(GePoint3d& pt, const GePoint3d& basePnt)
{
	if (ED_IMP_JIG(this->m_pImpl)->bDrag == true)
	{
		::kernel()->acevEventManager()->_emit(Ev::Drag::kJigUpdateDragline, basePnt, ED_IMP_JIG(this->m_pImpl)->position);
		pt = ED_IMP_JIG(this->m_pImpl)->position;
		return ED_IMP_JIG(this->m_pImpl)->dragStatus;
	}

	while (true)
	{
		ED_IMP_JIG(this->m_pImpl)->dragStatus = this->drag();
		if (ED_IMP_JIG(this->m_pImpl)->dragStatus != EdJig::DragStatus::kOther)
		{
			pt = ED_IMP_JIG(this->m_pImpl)->position;
			break;
		}
		acedPrompt("\n需要点或者关键字。");
		continue;
	};
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}

EdJig::CursorType EdJig::specialCursorType()
{
	return ED_IMP_JIG(this->m_pImpl)->cursorType;
}
void EdJig::setSpecialCursorType(CursorType _type)
{
	ED_IMP_JIG(this->m_pImpl)->cursorType = _type;
}

EdJig::UserInputControls EdJig::userInputControls()
{
	return ED_IMP_JIG(this->m_pImpl)->userInputControls;
}
void EdJig::setUserInputControls(EdJig::UserInputControls _v)
{
	ED_IMP_JIG(this->m_pImpl)->userInputControls = _v;
}

DbEntity* EdJig::entity() const
{
	return NULL;
}
DbDimDataPtrArray* EdJig::dimData(const double dimScale)
{
	return NULL;
}
Acad::ErrorStatus EdJig::setDimValue(const DbDimData* dimData, const double dimValue)
{
	return Acad::ErrorStatus::eOk;
}
EdJig::DragStatus EdJig::sampler()
{
	return ED_IMP_JIG(this->m_pImpl)->dragStatus;
}
Adesk::Boolean EdJig::update()
{
	return true;
}
#if EMSDK
const std::string EdJig::emsdk_keywordList()
{
	return ED_IMP_JIG(this->m_pImpl)->keywordList.constPtr();
}
const std::string EdJig::emsdk_dispPrompt()
{
	return ED_IMP_JIG(this->m_pImpl)->dispPrompt.constPtr();
}
#endif