#include "export.h"
#include "RxObject.h"
#include "RxExport.h"
#include "RxManager.h"
#include "EdUserInputManager.h"
#include "EdTouchArray.h"
#include "EdTouchEvent.h"
#include "EdKeyboardEvent.h"
#include "EdUserInputEvent.h"
#include "AcArrayExport.h"



#if EMSDK



EMSCRIPTEN_BINDINGS(Ed) {


	enum_<EPointerType>("EPointerType")
		.value("kUnknown", EPointerType::kUnknown)
		.value("kMouse", EPointerType::kMouse)
		.value("kTouch", EPointerType::kTouch)
		.value("kPen", EPointerType::kPen);

	// EdTouch
	emscripten::class_<EdTouch>("EdTouch")
		.property("clientX", &EdTouch::clientX)
		.property("clientY", &EdTouch::clientY)
		.property("force", &EdTouch::force)
		.property("identifier", &EdTouch::identifier)
		.property("pageX", &EdTouch::pageX)
		.property("pageY", &EdTouch::pageY)
		.property("radiusX", &EdTouch::radiusX)
		.property("radiusY", &EdTouch::radiusY)
		.property("rotationAngle", &EdTouch::rotationAngle)
		.property("pointerType", &EdTouch::pointerType)
		.constructor();


	enum_<EdUserInputEventType>("EdUserInputEventType")
		.value("kNull", EdUserInputEventType::kNull)
		.value("kMousemove", EdUserInputEventType::kMousemove)
		.value("kMousewheel", EdUserInputEventType::kMousewheel)
		.value("kMouseup", EdUserInputEventType::kMouseup)
		.value("kMousedown", EdUserInputEventType::kMousedown)
		.value("kDblclick", EdUserInputEventType::kDblclick)
		.value("kKeydown", EdUserInputEventType::kKeydown)
		.value("kKeyup", EdUserInputEventType::kKeyup)
		.value("kTouchstart", EdUserInputEventType::kTouchstart)
		.value("kTouchend", EdUserInputEventType::kTouchend)
		.value("kTouchmove", EdUserInputEventType::kTouchmove);

	// EdUserInputEvent
	emscripten::class_<EdUserInputEvent>("EdUserInputEvent")
		.function("eventType", &EdUserInputEvent::eventType)
		.function("setEventType", &EdUserInputEvent::setEventType);

	// EdTouchArray
	emscripten::class_<EdTouchArray>("EdTouchArray")
		.constructor()
		ACARRAY_PTR_EMSDK_EXPORT_METHODS(EdTouchArray, EdTouch*);

	// EdTouchEvent
	emscripten::class_<EdTouchEvent, base<EdUserInputEvent>>("EdTouchEvent")
		.constructor()
		.property("altKey", &EdTouchEvent::altKey)
		.property("ctrlKey", &EdTouchEvent::ctrlKey)
		.property("shiftKey", &EdTouchEvent::shiftKey)
		.function("touches", &EdTouchEvent::touches, allow_raw_pointers())
		.function("changedTouches", &EdTouchEvent::changedTouches, allow_raw_pointers());

	// EdMouseEvent
	emscripten::class_<EdMouseEvent, base<EdUserInputEvent>>("EdMouseEvent")
		.property("button", &EdMouseEvent::button)
		.property("clientX", &EdMouseEvent::clientX)
		.property("clientY", &EdMouseEvent::clientY)
		.property("altKey", &EdMouseEvent::altKey)
		.property("ctrlKey", &EdMouseEvent::ctrlKey)
		.property("shiftKey", &EdMouseEvent::shiftKey)
		.constructor<>()
		.class_function("stringify", &EdMouseEvent::stringify)
		.class_function("parse", &EdMouseEvent::parse);

	// EdWheelEvent
	emscripten::class_<EdWheelEvent, base<EdMouseEvent>>("EdWheelEvent")
		.property("wheelDelta", &EdWheelEvent::wheelDelta)
		.constructor()
		.class_function("stringify", &EdWheelEvent::stringify)
		.class_function("parse", &EdWheelEvent::parse);

	// EdKeyboardEvent
	emscripten::class_<EdKeyboardEvent, base<EdUserInputEvent>>("EdKeyboardEvent")
		.property("altKey", &EdKeyboardEvent::altKey)
		.property("ctrlKey", &EdKeyboardEvent::ctrlKey)
		.property("shiftKey", &EdKeyboardEvent::shiftKey)
		.property("keyCode", &EdKeyboardEvent::keyCode)
		.constructor<>()
		.class_function("stringify", &EdKeyboardEvent::stringify)
		.class_function("parse", &EdKeyboardEvent::parse);

	// EdUserInputManager
	emscripten::class_<EdUserInputManager, base<RxObject>>("EdUserInputManager")
		.function("setDevicePixelRatio", &EdUserInputManager::setDevicePixelRatio)
		.function("fps", &EdUserInputManager::fps)
		.function("setFps", &EdUserInputManager::setFps)
		.function("setRangRect", &EdUserInputManager::setRangRect)
		.function("setScreenSize", &EdUserInputManager::setScreenSize)
		.function("mousewheel", select_overload<void(const EdWheelEvent&)>(&EdUserInputManager::mousewheel))
		.function("mousewheel", select_overload<void(int, double, double, bool, bool, bool, int)>(&EdUserInputManager::mousewheel))
		.function("mousemove", select_overload<void(const EdMouseEvent&)>(&EdUserInputManager::mousemove))
		.function("mousemove", select_overload<void(int, double, double, bool, bool, bool)>(&EdUserInputManager::mousemove))
		.function("mouseup", select_overload<void(const EdMouseEvent&)>(&EdUserInputManager::mouseup))
		.function("mouseup", select_overload<void(int, double, double, bool, bool, bool)>(&EdUserInputManager::mouseup))
		.function("mousedown", select_overload<void(const EdMouseEvent&)>(&EdUserInputManager::mousedown))
		.function("mousedown", select_overload<void(int, double, double, bool, bool, bool)>(&EdUserInputManager::mousedown))
		.function("dblclick", select_overload<void(const EdMouseEvent&)>(&EdUserInputManager::dblclick))
		.function("dblclick", select_overload<void(int, double, double, bool, bool, bool)>(&EdUserInputManager::dblclick))
		.function("touchstart", &EdUserInputManager::touchstart)
		.function("touchmove", &EdUserInputManager::touchmove)
		.function("touchend", &EdUserInputManager::touchend)
		.function("userInput", &EdUserInputManager::userInput);
}



#endif
