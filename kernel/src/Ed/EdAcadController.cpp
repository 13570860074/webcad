#include "EdAcadController.h"
#include "EdMouseEvent.h"
#include "EdWheelEvent.h"
#include "EvEventManager.h"
#include "ReRendererManager.h"
#include "ApDocumentIterator.h"
#include "EdUserInputManager.h"
#include "EdControllerManager.h"
#include "GiLineSegmentGeometry.h"
#include "EvEventManager.h"
#include "DbObjectManager.h"
#include "acedTrans.h"
#include "AcDebug.h"
#include "GeMath.h"
#include "kernel.h"
#include "EdImpl.h"
#include "DbImpl.h"



EdAcadController::EdAcadController()
{
	this->m_pImpl = new EdAcadControllerImpl();
}
EdAcadController::~EdAcadController()
{
}

void EdAcadController::onBeginAnimationFrame(EdAcadController* pointer)
{

}

void EdAcadController::onBeginDrag(EdAcadController* pointer) {

}
void EdAcadController::onEndDrag(EdAcadController* pointer) {

}

void EdAcadController::onMousemove(EdAcadController* pointer, const EdMouseEvent& _mouseEvent) {

	// 处理拖动
	while (ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isMoveView)
	{
		// 判断鼠标位置是否发生变化
		GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();
		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragEndPoint.set(deviceLocation.x, deviceLocation.y);
		if (ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragStartPoint.isEqualTo(ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragEndPoint) == true)
		{
			break;
		}

		// 获得屏幕点
		GePoint2d dragStartPoint = ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragStartPoint;
		GePoint2d dragEndPoint = ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragEndPoint;

		// 将屏幕坐标转显示坐标
		dragStartPoint = ::trans_ncs_to_dcs(dragStartPoint);
		dragEndPoint = ::trans_ncs_to_dcs(dragEndPoint);

		// 设置视口移动
		::kernel()->acedControllerManager()->viewportMove(dragStartPoint, dragEndPoint);

		// 重新设置拖动开始点
		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragStartPoint = ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragEndPoint;
		break;
	}
}
void EdAcadController::onMousewheel(EdAcadController* pointer, const EdWheelEvent& _wheelEvent)
{
	GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();
	GePoint2d vcsDeviceLocation = ::trans_ncs_to_vcs(deviceLocation);
	if (vcsDeviceLocation.x < 0.0 || vcsDeviceLocation.y > 1.0 || vcsDeviceLocation.y < 0.0 || vcsDeviceLocation.y > 1.0) {
		return;
	}

	if (ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isViewZoom == false) {
		::kernel()->acevEventManager()->_emit(Ev::Controller::kZoomViewBegin);
	}
	ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isViewZoom = true;
	ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->viewTimeCount = 0;

	if (_wheelEvent.wheelDelta > 0)
	{
		EdControllerManager::viewportZoom(::kernel()->acedUserInputManager()->deviceLocation(), 0.85);
	}
	else
	{
		EdControllerManager::viewportZoom(::kernel()->acedUserInputManager()->deviceLocation(), 1.15);
	}
}
void EdAcadController::onMouseup(EdAcadController* pointer, const EdMouseEvent& _mouseEvent)
{
	if (_mouseEvent.button == 1)
	{

		// 设置拖动完成
		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isMoveView = false;

		// 设置触发拖动结束事件
		GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();
		::kernel()->acevEventManager()->_emit(Ev::Controller::kMoveViewEnd, deviceLocation);
	}
}
void EdAcadController::onMousedown(EdAcadController* pointer, const EdMouseEvent& _mouseEvent) {
	if (_mouseEvent.button == 1) {

		// 获得当前屏幕位置
		GePoint2d deviceLocation = ::kernel()->acedUserInputManager()->deviceLocation();
		GePoint2d vcsDeviceLocation = ::trans_ncs_to_vcs(deviceLocation);
		if (vcsDeviceLocation.x < 0.0 || vcsDeviceLocation.y > 1.0 || vcsDeviceLocation.y < 0.0 || vcsDeviceLocation.y > 1.0) {
			return;
		}

		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->dragStartPoint.set(deviceLocation.x, deviceLocation.y);

		// 设置正在拖动
		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isMoveView = true;

		// 设置触发拖动开始事件
		::kernel()->acevEventManager()->_emit(Ev::Controller::kMoveViewBegin, deviceLocation);
	}
}
void EdAcadController::onDblclick(EdAcadController* pointer, const EdMouseEvent& _mouseEvent)
{

}
void EdAcadController::onInterval(EdAcadController* pointer) {
	if (ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isViewZoom == true)
	{
		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->viewTimeCount++;
	}
	if (ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isViewZoom == true && ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->viewTimeCount > 30)
	{
		ED_IMP_ACADCONTROLLER(pointer->m_pImpl)->isViewZoom = false;
		::kernel()->acevEventManager()->_emit(Ev::Controller::kZoomViewEnd);
	}
}

void EdAcadController::onInit()
{
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)EdAcadController::onBeginAnimationFrame, this);

	::kernel()->acevEventManager()->on(Ev::Drag::kBeginDrag, (void*)EdAcadController::onBeginDrag, this);
	::kernel()->acevEventManager()->on(Ev::Drag::kEndDrag, (void*)EdAcadController::onEndDrag, this);

	::kernel()->acevEventManager()->on(Ev::UserInput::kMousemove, (void*)EdAcadController::onMousemove, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kMousewheel, (void*)EdAcadController::onMousewheel, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kMouseup, (void*)EdAcadController::onMouseup, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kMousedown, (void*)EdAcadController::onMousedown, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kDblclick, (void*)EdAcadController::onDblclick, this);

	::kernel()->acevEventManager()->on(Ev::Interval::kInterval,(void*)EdAcadController::onInterval, this);
}
