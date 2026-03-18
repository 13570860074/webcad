#include "EdTouchController.h"
#include "EdControllerManager.h"
#include "EdUserInputManager.h"
#include "EvEventManager.h"
#include "EdTouchEvent.h"
#include "acedTrans.h"
#include "kernel.h"
#include "EdImpl.h"
#include "DbImpl.h"


EdTouchController::EdTouchController()
{
	this->m_pImpl = new EdTouchControllerImpl();
}
EdTouchController::~EdTouchController()
{
}


void EdTouchController::onTouchstart(EdTouchController* pointer, const EdTouchEvent& _touchEvent) {
	// 设置触摸点
	realloc_all(ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches);
	ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches = new AcArray<EdTouch*>();
	for (int i = 0; i < _touchEvent.touches()->length(); i++) {
		EdTouch* touch = EdTouch::clone(_touchEvent.touches()->at(i));
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches->append(touch);
	}
	std::cout << "测试A" << std::endl;
	// 判断缩放初始点
	if (_touchEvent.touches()->length() == 2) {
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation1 = ::trans_client_to_ncs(_touchEvent.touches()->at(0)->clientX, _touchEvent.touches()->at(0)->clientY);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation2 = ::trans_client_to_ncs(_touchEvent.touches()->at(1)->clientX, _touchEvent.touches()->at(1)->clientY);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation1 = ::trans_ncs_to_dcs(ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation1);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation2 = ::trans_ncs_to_dcs(ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation2);

		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->initDeviceLocation1 = ::trans_client_to_ncs(_touchEvent.touches()->at(0)->clientX, _touchEvent.touches()->at(0)->clientY);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->initDeviceLocation2 = ::trans_client_to_ncs(_touchEvent.touches()->at(1)->clientX, _touchEvent.touches()->at(1)->clientY);
	}
}
void EdTouchController::onTouchmove(EdTouchController* pointer, const EdTouchEvent& _touchEvent) {

	bool isDrag = false;
	bool isZoom = false;

	/** 当前手指触摸点为一时,且为手指时进行拖动 */
	if (_touchEvent.touches()->length() == 1 && _touchEvent.changedTouches()->length() == 1) {
		/** 触摸点为手指时,启用拖动 */
		if (_touchEvent.changedTouches()->at(0)->pointerType == EPointerType::kTouch) {
			isZoom = false;
			isDrag = true;
		}
	}

	/** 判断缩放 */
	if (_touchEvent.touches()->length() == 2 && isDrag == false) {
		isDrag = false;
		isZoom = true;
	}

	/** 拖动 */
	if (isDrag == true) {

		GePoint2d startPoint = GePoint2d::kOrigin;
		GePoint2d endPoint = GePoint2d::kOrigin;

		for (int u = 0; u < ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches->length(); u++) {
			if (ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches->at(u)->identifier == _touchEvent.changedTouches()->at(0)->identifier) {
				startPoint.x += ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches->at(u)->clientX;
				startPoint.y += ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches->at(u)->clientY;
				break;
			}
		}

		for (int i = 0; i < _touchEvent.changedTouches()->length(); i++) {
			endPoint.x += _touchEvent.changedTouches()->at(i)->clientX;
			endPoint.y += _touchEvent.changedTouches()->at(i)->clientY;
			break;
		}

		startPoint = ::trans_client_to_ncs(startPoint.x, startPoint.y);
		endPoint = ::trans_client_to_ncs(endPoint.x, endPoint.y);
		startPoint = ::trans_ncs_to_dcs(startPoint);
		endPoint = ::trans_ncs_to_dcs(endPoint);

		EdControllerManager::viewportMove(startPoint, endPoint);
	}

	/** 缩放 */
	if (isZoom == true) {

		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation1 = ::trans_client_to_ncs(_touchEvent.touches()->at(0)->clientX, _touchEvent.touches()->at(0)->clientY);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation2 = ::trans_client_to_ncs(_touchEvent.touches()->at(1)->clientX, _touchEvent.touches()->at(1)->clientY);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation1 = ::trans_ncs_to_dcs(ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation1);
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation2 = ::trans_ncs_to_dcs(ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation2);

		double a = (ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation2.y - ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation1.y);
		double b = (ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation2.y - ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation1.y);
		double v = a / b;
		if (v < 0.9 || v >1.1) {
			v = 1.0;
		}

		GePoint2d zoomCenter = ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->initDeviceLocation1;
		zoomCenter = zoomCenter + (ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->initDeviceLocation2 - ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->initDeviceLocation1) / 2.0;
		EdControllerManager::viewportZoom(zoomCenter, v);

		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation1 = ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation1;
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->endDeviceLocation2 = ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->startDeviceLocation2;
	}

	/** 设置触摸点 */
	realloc_all(ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches);
	ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches = new AcArray<EdTouch*>();
	for (int i = 0; i < _touchEvent.touches()->length(); i++) {
		EdTouch* touch = EdTouch::clone(_touchEvent.touches()->at(i));
		ED_IMP_TOUCHCONTROLLER(pointer->m_pImpl)->touches->append(touch);
	}
}
void EdTouchController::onTouchend(EdTouchController* pointer, const EdTouchEvent& _touchEvent) {

}


void EdTouchController::onInit() {

	::kernel()->acevEventManager()->on(Ev::UserInput::kTouchstart, (void*)EdTouchController::onTouchstart, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kTouchmove, (void*)EdTouchController::onTouchmove, this);
	::kernel()->acevEventManager()->on(Ev::UserInput::kTouchend, (void*)EdTouchController::onTouchend, this);
}