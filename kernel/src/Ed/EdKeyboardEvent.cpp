#include "EdKeyboardEvent.h"
#include "AcJsonObject.h"



EdKeyboardEvent::EdKeyboardEvent() {
	this->altKey = false;
	this->ctrlKey = false;
	this->shiftKey = false;
	this->keyCode = 0;
}
EdKeyboardEvent::~EdKeyboardEvent() {}


std::string EdKeyboardEvent::stringify(const EdKeyboardEvent _data) {
	AcJsonObject*obj = new AcJsonObject();
	obj->insert("altKey", _data.altKey);
	obj->insert("ctrlKey", _data.ctrlKey);
	obj->insert("shiftKey", _data.shiftKey);
	obj->insert("keyCode", _data.keyCode);
	std::string str = AcJsonObject::stringify(obj);
	delete obj;
	return str;
}

EdKeyboardEvent EdKeyboardEvent::parse(const std::string& _data) {
	AcJsonObject* obj = AcJsonObject::parse(_data.c_str());
	EdKeyboardEvent keyboardEvent;
	keyboardEvent.altKey = obj->get("altKey")->toBool();
	keyboardEvent.ctrlKey = obj->get("ctrlKey")->toBool();
	keyboardEvent.shiftKey = obj->get("shiftKey")->toBool();
	keyboardEvent.keyCode = obj->get("keyCode")->toInt32();
	delete obj;
	return keyboardEvent;
}
EdKeyboardEvent* EdKeyboardEvent::clone() const {
	EdKeyboardEvent* p = new EdKeyboardEvent();
	p->altKey = this->altKey;
	p->ctrlKey = this->ctrlKey;
	p->shiftKey = this->shiftKey;
	p->keyCode = this->keyCode;
	return p;
}