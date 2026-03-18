#include "EdWheelEvent.h"
#include "AcJsonObject.h"



EdWheelEvent::EdWheelEvent() {
	this->altKey = false;
	this->button = 0;
	this->clientX = 0;
	this->clientY = 0;
	this->ctrlKey = false;
	this->shiftKey = false;
	this->wheelDelta = 0;
}
EdWheelEvent::~EdWheelEvent() {}

std::string EdWheelEvent::className() const {
	return "EdWheelEvent";
}
void EdWheelEvent::clone(EdWheelEvent*& wheelEvent) const {
	EdMouseEvent::clone((EdMouseEvent*&)wheelEvent);
	wheelEvent->wheelDelta = this->wheelDelta;
}

std::string EdWheelEvent::stringify(const EdWheelEvent _data) {
	AcJsonObject*obj = new AcJsonObject();
	obj->insert("altKey", _data.altKey);
	obj->insert("button", _data.button);
	obj->insert("clientX", _data.clientX);
	obj->insert("clientY", _data.clientY);
	obj->insert("ctrlKey", _data.ctrlKey);
	obj->insert("shiftKey", _data.shiftKey);
	obj->insert("wheelDelta", _data.wheelDelta);
	std::string str = AcJsonObject::stringify(obj);
	delete obj;
	return str;
}

EdWheelEvent EdWheelEvent::parse(const std::string& _data) {
	AcJsonObject* obj = AcJsonObject::parse(_data.c_str());
	EdWheelEvent wheelEvent;
	wheelEvent.altKey = obj->get("altKey")->toBool();
	wheelEvent.button = obj->get("button")->toInt32();
	wheelEvent.clientX = obj->get("clientX")->toFloat64();
	wheelEvent.clientY = obj->get("clientY")->toFloat64();
	wheelEvent.ctrlKey = obj->get("ctrlKey")->toBool();
	wheelEvent.shiftKey = obj->get("shiftKey")->toBool();
	wheelEvent.wheelDelta = obj->get("wheelDelta")->toInt32();
	delete obj;
	return wheelEvent;
}
EdWheelEvent* EdWheelEvent::clone() const {
	EdWheelEvent* p = new EdWheelEvent();
	this->clone(p);
	return p;
}