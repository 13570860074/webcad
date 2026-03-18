

#include "EdUserInputManager.h"
#include "ReRendererManager.h"
#include "EvEventManager.h"
#include "GsElementManager.h"
#include "kernel.h"
#include "GsCursor.h"
#include "EdImpl.h"

#define EDUSERINPUTMANAGER_LOCK_DETERMINE\
	if (RX_IMP_USERINPUTMANAGER(this->m_pImpl)->isLock == true) \
	{\
		return;\
	}\

#define EDUSERINPUTMANAGER_PRELOAD 1


void EdUserInputManager::onBeginAnimationFrame(EdUserInputManager * pointer) {

	if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent.length() <= 0) {
		return;
	}

	for (int i = 0; i < RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent.length() - 1; i++) {
		if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kMousemove &&
			RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i + 1]->eventType() == EdUserInputEventType::kMousemove) {
			realloc_arry_at(RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent, i);
			i--;
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kTouchmove &&
			RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i + 1]->eventType() == EdUserInputEventType::kTouchmove) {
			realloc_arry_at(RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent, i);
			i--;
		}
	}
	for (int i = 0; i < RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent.length(); i++) {
		if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kMousemove) {

			EdMouseEvent& mouseEvent = (EdMouseEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];

			double x = mouseEvent.clientX - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
			double y = mouseEvent.clientY - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;

			x = (x - 0) / pointer->rangWidth();
			y = (y - 0) / pointer->rangHeight();

			y = 1.0 - y;
			RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[0] = x;
			RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[1] = y;

			::kernel()->acevEventManager()->_emit(Ev::UserInput::kMousemove, &mouseEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kMousewheel) {
			EdWheelEvent& wheelEvent = (EdWheelEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];
			wheelEvent.clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
			wheelEvent.clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kMousewheel, &wheelEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kMouseup) {
			EdMouseEvent& mouseEvent = (EdMouseEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];
			mouseEvent.clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
			mouseEvent.clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kMouseup, &mouseEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kMousedown) {
			EdMouseEvent& mouseEvent = (EdMouseEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];

			double x = mouseEvent.clientX - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
			double y = mouseEvent.clientY - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;

			x = (x - 0) / pointer->rangWidth();
			y = (y - 0) / pointer->rangHeight();

			y = 1.0 - y;
			RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[0] = x;
			RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[1] = y;

			mouseEvent.clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
			mouseEvent.clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kMousedown, &mouseEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kDblclick) {
			EdMouseEvent& mouseEvent = (EdMouseEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];
			mouseEvent.clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
			mouseEvent.clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kDblclick, &mouseEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kKeydown) {
			EdKeyboardEvent& keyboardEvent = (EdKeyboardEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];
			if (keyboardEvent.keyCode == 16) {
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->shift = true;
			}
			if (keyboardEvent.keyCode == 17) {
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->ctrl = true;
			}
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kKeydown, &keyboardEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kKeyup) {
			EdKeyboardEvent &keyboardEvent = (EdKeyboardEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];
			if (keyboardEvent.keyCode == 16) {
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->shift = false;
			}
			if (keyboardEvent.keyCode == 17) {
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->ctrl = false;
			}
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kKeyup, &keyboardEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kTouchstart) {
			EdTouchEvent &touchEvent = (EdTouchEvent&)(*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]);

			if (touchEvent.changedTouches()->length() > 0) {
				double x = touchEvent.changedTouches()->at(0)->clientX - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
				double y = touchEvent.changedTouches()->at(0)->clientY - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;

				x = (x - 0) / pointer->rangWidth();
				y = (y - 0) / pointer->rangHeight();

				y = 1.0 - y;
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[0] = x;
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[1] = y;
			}
			for (int i = 0; i < touchEvent.touches()->length(); i++) {
				touchEvent.touches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
				touchEvent.touches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			}
			for (int i = 0; i < touchEvent.changedTouches()->length(); i++) {
				touchEvent.changedTouches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
				touchEvent.changedTouches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			}

			::kernel()->acevEventManager()->_emit(Ev::UserInput::kTouchstart, &touchEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kTouchend) {
			EdTouchEvent& touchEvent = (EdTouchEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];
			::kernel()->acevEventManager()->_emit(Ev::UserInput::kTouchend, &touchEvent);
		}
		else if (RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i]->eventType() == EdUserInputEventType::kTouchmove) {
			EdTouchEvent& touchEvent = (EdTouchEvent&)*RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent[i];

			if (touchEvent.changedTouches()->length() > 0) {
				double x = touchEvent.changedTouches()->at(0)->clientX - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
				double y = touchEvent.changedTouches()->at(0)->clientY - RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;

				x = (x - 0) / pointer->rangWidth();
				y = (y - 0) / pointer->rangHeight();

				y = 1.0 - y;
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[0] = x;
				RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->mouse[1] = y;
			}
			for (int i = 0; i < touchEvent.touches()->length(); i++) {
				touchEvent.touches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
				touchEvent.touches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			}
			for (int i = 0; i < touchEvent.changedTouches()->length(); i++) {
				touchEvent.changedTouches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangLeft;
				touchEvent.changedTouches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->rangTop;
			}

			::kernel()->acevEventManager()->_emit(Ev::UserInput::kTouchmove, &touchEvent);
		}
	}

	realloc_arry_all(RX_IMP_USERINPUTMANAGER(pointer->m_pImpl)->userInputEvent);

}

EdUserInputManager::EdUserInputManager()
{
	this->m_pImpl = new EdUserInputManagerImpl();
}
EdUserInputManager::~EdUserInputManager()
{
}
void EdUserInputManager::onInit() {
	std::cout << "EdUserInputManager" << std::endl;
	::kernel()->acevEventManager()->on(Ev::RendererReactor::kBeginAnimationFrame, (void*)onBeginAnimationFrame, this);
}
bool EdUserInputManager::shift() const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->shift;
}
bool EdUserInputManager::ctrl() const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->ctrl;
}
void EdUserInputManager::userInput(const std::string& input, const std::string& data) {

	if (input == "mousemove") {
		EdMouseEvent mouseEvent = EdMouseEvent::parse(data);
		this->mousemove(mouseEvent);
	}
	else if (input == "mousedown") {
		EdMouseEvent mouseEvent = EdMouseEvent::parse(data);
		this->mousedown(mouseEvent);
	}
	else if (input == "mouseup") {
		EdMouseEvent mouseEvent = EdMouseEvent::parse(data);
		this->mouseup(mouseEvent);
	}
	else if (input == "mousewheel") {
		EdWheelEvent wheelEvent = EdWheelEvent::parse(data);
		this->mousewheel(wheelEvent);
	}
	else if (input == "keydown") {
		EdKeyboardEvent keyboardEvent = EdKeyboardEvent::parse(data);
		this->keydown(keyboardEvent);
	}
	else if (input == "keyup") {
		EdKeyboardEvent keyboardEvent = EdKeyboardEvent::parse(data);
		this->keyup(keyboardEvent);
	}
	else if (input == "touchstart") {
		EdTouchEvent touchEvent = EdTouchEvent::parse(data);
		this->touchstart(touchEvent);
	}
	else if (input == "touchmove") {
		EdTouchEvent touchEvent = EdTouchEvent::parse(data);
		this->touchmove(touchEvent);
	}
	else if (input == "touchend") {
		EdTouchEvent touchEvent = EdTouchEvent::parse(data);
		this->touchend(touchEvent);
	}
}
void EdUserInputManager::setRangRect(const double _top, const double _bottom, const double _left, const double _right)
{
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft = _left;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop = _top;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangRight = _right;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangBottom = _bottom;
	if (RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenWidth < 0.02) {
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenWidth = this->rangWidth();
	}
	if (RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenHeight < 0.02) {
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenHeight = this->rangHeight();
	}
}
void EdUserInputManager::setScreenSize(double _width, double _height) {
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenWidth = _width;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenHeight = _height;
}
void EdUserInputManager::setDevicePixelRatio(double _devicePixelRatio) {
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->devicePixelRatio = _devicePixelRatio;
}
int EdUserInputManager::fps() const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->fps;
}
void EdUserInputManager::setFps(const int v) {
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->fps = v;
}
int EdUserInputManager::ms()const {
	return int(1000.0 / RX_IMP_USERINPUTMANAGER(this->m_pImpl)->fps);
}
GePoint2d EdUserInputManager::deviceLocation() const
{
	GePoint2d position;
	position.x = RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[0];
	position.y = RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[1];
	return position;
}
void EdUserInputManager::setDeviceLocation(const GePoint2d& _point) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[0] = _point.x;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[1] = _point.y;

}
double EdUserInputManager::devicePixelRatio()const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->devicePixelRatio;
}
double EdUserInputManager::screenAspect() const
{
	return this->rangWidth() / this->rangHeight();
}
double EdUserInputManager::screenWidth()const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenWidth;
}
double EdUserInputManager::screenHeight()const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->screenHeight;
}
void EdUserInputManager::rangRect(double& _left, double& _right, double& _top, double& _bottom) {
	_left = RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	_right = RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangRight;
	_top = RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	_bottom = RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangBottom;
}
double EdUserInputManager::rangWidth()const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangRight - RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
}
double EdUserInputManager::rangHeight()const {
	return RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangBottom - RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
}

/**
* 鼠标滚轮滚动
*/
void EdUserInputManager::mousewheel(const EdWheelEvent& _wheelEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD
	EdWheelEvent* wheelEvent = _wheelEvent.clone();
	wheelEvent->setEventType(EdUserInputEventType::kMousewheel);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(wheelEvent);
#else
	EdWheelEvent wheelEvent = _wheelEvent;
	wheelEvent.clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	wheelEvent.clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kMousewheel, wheelEvent);
#endif
}
void EdUserInputManager::mousewheel(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey, int wheelDelta) {
	EdWheelEvent wheelEvent;
	wheelEvent.button = button;
	wheelEvent.clientX = clientX;
	wheelEvent.clientY = clientY;
	wheelEvent.altKey = altKey;
	wheelEvent.ctrlKey = ctrlKey;
	wheelEvent.shiftKey = shiftKey;
	wheelEvent.wheelDelta = wheelDelta;
	this->mousewheel(wheelEvent);
}

/**
* 鼠标移动
*/
void EdUserInputManager::mousemove(const EdMouseEvent& _mouseEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD

	double x = _mouseEvent.clientX - RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	double y = _mouseEvent.clientY - RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;

	x = (x - 0) / this->rangWidth();
	y = (y - 0) / this->rangHeight();

	y = 1.0 - y;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[0] = x;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[1] = y;

	EdMouseEvent* mouseEvent = _mouseEvent.clone();
	mouseEvent->setEventType(EdUserInputEventType::kMousemove);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(mouseEvent);
#else

	double x = _mouseEvent.clientX - RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	double y = _mouseEvent.clientY - RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;

	x = (x - 0) / this->rangWidth();
	y = (y - 0) / this->rangHeight();

	y = 1.0 - y;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[0] = x;
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[1] = y;

	::kernel()->acevEventManager()->_emit(Ev::UserInput::kMousemove, _mouseEvent);
#endif
}
void EdUserInputManager::mousemove(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey) {
	EdMouseEvent mouseEvent;
	mouseEvent.button = button;
	mouseEvent.clientX = clientX;
	mouseEvent.clientY = clientY;
	mouseEvent.altKey = altKey;
	mouseEvent.ctrlKey = ctrlKey;
	mouseEvent.shiftKey = shiftKey;
	this->mousemove(mouseEvent);
}

/**
* 鼠标抬起
*/
void EdUserInputManager::mouseup(const EdMouseEvent& _mouseEvent) {

#if EDUSERINPUTMANAGER_PRELOAD
	EDUSERINPUTMANAGER_LOCK_DETERMINE;
	EdMouseEvent* mouseEvent = _mouseEvent.clone();
	mouseEvent->setEventType(EdUserInputEventType::kMouseup);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(mouseEvent);
#else
	EdMouseEvent mouseEvent = _mouseEvent;
	mouseEvent.clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	mouseEvent.clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kMouseup, mouseEvent);
#endif
}
void EdUserInputManager::mouseup(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey) {
	EdMouseEvent mouseEvent;
	mouseEvent.button = button;
	mouseEvent.clientX = clientX;
	mouseEvent.clientY = clientY;
	mouseEvent.altKey = altKey;
	mouseEvent.ctrlKey = ctrlKey;
	mouseEvent.shiftKey = shiftKey;
	this->mouseup(mouseEvent);
}

/**
* 鼠标按下
*/
void EdUserInputManager::mousedown(const EdMouseEvent& _mouseEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD
	EdMouseEvent* mouseEvent = _mouseEvent.clone();
	mouseEvent->setEventType(EdUserInputEventType::kMousedown);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(mouseEvent);
#else
	EdMouseEvent mouseEvent = _mouseEvent;
	mouseEvent.clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	mouseEvent.clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kMousedown, mouseEvent);
#endif
}
void EdUserInputManager::mousedown(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey) {
	EdMouseEvent mouseEvent;
	mouseEvent.button = button;
	mouseEvent.clientX = clientX;
	mouseEvent.clientY = clientY;
	mouseEvent.altKey = altKey;
	mouseEvent.ctrlKey = ctrlKey;
	mouseEvent.shiftKey = shiftKey;
	this->mousedown(mouseEvent);
}

/**
* 双击事件
*/
void EdUserInputManager::dblclick(const EdMouseEvent& _mouseEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD
	EdMouseEvent* mouseEvent = _mouseEvent.clone();
	mouseEvent->setEventType(EdUserInputEventType::kDblclick);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(mouseEvent);
#else
	EdMouseEvent mouseEvent = _mouseEvent;
	mouseEvent.clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
	mouseEvent.clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kDblclick, mouseEvent);
#endif
}
void EdUserInputManager::dblclick(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey) {
	EdMouseEvent mouseEvent;
	mouseEvent.button = button;
	mouseEvent.clientX = clientX;
	mouseEvent.clientY = clientY;
	mouseEvent.altKey = altKey;
	mouseEvent.ctrlKey = ctrlKey;
	mouseEvent.shiftKey = shiftKey;
	this->dblclick(mouseEvent);
}

/**
* 键盘按下
*/
void EdUserInputManager::keydown(const EdKeyboardEvent& _keyboardEvent) {

	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD
	EdKeyboardEvent* keyboardEvent = _keyboardEvent.clone();
	keyboardEvent->setEventType(EdUserInputEventType::kKeydown);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(keyboardEvent);
#else
	if (_keyboardEvent.keyCode == 16) {
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->shift = true;
	}
	if (_keyboardEvent.keyCode == 17) {
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->ctrl = true;
	}
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kKeydown, _keyboardEvent);
#endif

}

/**
* 按键抬起
*/
void EdUserInputManager::keyup(const EdKeyboardEvent& _keyboardEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;


#if EDUSERINPUTMANAGER_PRELOAD
	EdKeyboardEvent* keyboardEvent = _keyboardEvent.clone();
	keyboardEvent->setEventType(EdUserInputEventType::kKeyup);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(keyboardEvent);
#else
	if (_keyboardEvent.keyCode == 16) {
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->shift = false;
	}
	if (_keyboardEvent.keyCode == 17) {
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->ctrl = false;
	}

	::kernel()->acevEventManager()->_emit(Ev::UserInput::kKeyup, _keyboardEvent);
#endif
}


void EdUserInputManager::touchstart(const EdTouchEvent& _touchEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;


#if EDUSERINPUTMANAGER_PRELOAD
	EdTouchEvent* touchEvent = _touchEvent.clone();
	touchEvent->setEventType(EdUserInputEventType::kTouchstart);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(touchEvent);
#else
	EdTouchEvent mouseEvent = _touchEvent;
	for (int i = 0; i < _touchEvent.touches()->length(); i++) {
		_touchEvent.touches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
		_touchEvent.touches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	}
	for (int i = 0; i < _touchEvent.changedTouches()->length(); i++) {
		_touchEvent.changedTouches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
		_touchEvent.changedTouches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	}
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kMouseup, _touchEvent);
#endif

}
void EdUserInputManager::touchmove(const EdTouchEvent& _touchEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD
	EdTouchEvent* touchEvent = _touchEvent.clone();
	touchEvent->setEventType(EdUserInputEventType::kTouchmove);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(touchEvent);
#else
	if (_touchEvent.changedTouches()->length() > 0)
	{
		double x = _touchEvent.changedTouches()->at(0)->clientX;
		double y = _touchEvent.changedTouches()->at(0)->clientY;

		x = (x - 0) / this->rangWidth();
		y = (y - 0) / this->rangHeight();

		y = 1.0 - y;
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[0] = x;
		RX_IMP_USERINPUTMANAGER(this->m_pImpl)->mouse[1] = y;
	}
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kTouchmove, _touchEvent);
#endif
}
void EdUserInputManager::touchend(const EdTouchEvent& _touchEvent) {
	EDUSERINPUTMANAGER_LOCK_DETERMINE;

#if EDUSERINPUTMANAGER_PRELOAD
	EdTouchEvent* touchEvent = _touchEvent.clone();
	touchEvent->setEventType(EdUserInputEventType::kTouchend);
	RX_IMP_USERINPUTMANAGER(this->m_pImpl)->userInputEvent.append(touchEvent);
#else
	EdTouchEvent mouseEvent = _touchEvent;
	for (int i = 0; i < _touchEvent.touches()->length(); i++) {
		_touchEvent.touches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
		_touchEvent.touches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	}
	for (int i = 0; i < _touchEvent.changedTouches()->length(); i++) {
		_touchEvent.changedTouches()->at(i)->clientX -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangLeft;
		_touchEvent.changedTouches()->at(i)->clientY -= RX_IMP_USERINPUTMANAGER(this->m_pImpl)->rangTop;
	}
	::kernel()->acevEventManager()->_emit(Ev::UserInput::kTouchend, _touchEvent);
#endif
}


