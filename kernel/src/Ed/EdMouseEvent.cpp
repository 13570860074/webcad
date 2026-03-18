#include "EdMouseEvent.h"
#include "AcJsonObject.h"



EdMouseEvent::EdMouseEvent() {
	this->altKey = false;
	this->button = 0;
	this->clientX = 0;
	this->clientY = 0;
	this->ctrlKey = false;
	this->shiftKey = false;
}
EdMouseEvent::~EdMouseEvent() {}

std::string EdMouseEvent::className() const {
	return "EdMouseEvent";
}
void EdMouseEvent::clone(EdMouseEvent*& mouseEvent)const {
	mouseEvent->altKey = this->altKey;
	mouseEvent->button = this->button;
	mouseEvent->clientX = this->clientX;
	mouseEvent->clientY = this->clientY;
	mouseEvent->ctrlKey = this->ctrlKey;
	mouseEvent->shiftKey = this->shiftKey;
}

std::string EdMouseEvent::stringify(const EdMouseEvent _data) {
	AcJsonObject* obj = new AcJsonObject();
	obj->insert("altKey", _data.altKey);
	obj->insert("button", _data.button);
	obj->insert("clientX", _data.clientX);
	obj->insert("clientY", _data.clientY);
	obj->insert("ctrlKey", _data.ctrlKey);
	obj->insert("shiftKey", _data.shiftKey);
	std::string str = AcJsonObject::stringify(obj);
	delete obj;
	return str;
}

EdMouseEvent EdMouseEvent::parse(const std::string& _data) {
	AcJsonObject* obj = AcJsonObject::parse(_data.c_str());
	EdMouseEvent mouseEvent;
	mouseEvent.altKey = obj->get("altKey")->toBool();
	mouseEvent.button = obj->get("button")->toInt32();
	mouseEvent.clientX = obj->get("clientX")->toFloat64();
	mouseEvent.clientY = obj->get("clientY")->toFloat64();
	mouseEvent.ctrlKey = obj->get("ctrlKey")->toBool();
	mouseEvent.shiftKey = obj->get("shiftKey")->toBool();
	delete obj;
	return mouseEvent;
}
EdMouseEvent* EdMouseEvent::clone() const {
	EdMouseEvent* p = new EdMouseEvent();
	this->clone(p);
	return p;
}