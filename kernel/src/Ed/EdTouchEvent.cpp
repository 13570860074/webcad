#include "EdTouchArray.h"
#include "EdTouchEvent.h"
#include "AcJsonObject.h"


EdTouchEvent::EdTouchEvent() {
	this->altKey = false;
	this->ctrlKey = false;
	this->shiftKey = false;
	this->m_touches = new EdTouchArray();
	this->m_changedTouches = new EdTouchArray();
}
EdTouchEvent::~EdTouchEvent() {
	realloc_all(this->m_touches);
	realloc_all(this->m_changedTouches);
}


EdTouchArray* EdTouchEvent::touches() const {
	return this->m_touches;
}
EdTouchArray* EdTouchEvent::changedTouches() const {
	return this->m_changedTouches;
}
std::string EdTouchEvent::stringify(const EdTouchEvent _data) {
	AcJsonObject* obj = new AcJsonObject();
	obj->insert("altKey", _data.altKey);
	obj->insert("ctrlKey", _data.ctrlKey);
	obj->insert("shiftKey", _data.shiftKey);

	AcArray<AcJsonObject*> touches;
	for (int i = 0; i < _data.m_touches->length(); i++) {
		AcJsonObject* touchObj = new AcJsonObject();
		touchObj->insert("identifier", _data.m_touches->at(i)->identifier);
		touchObj->insert("clientX", _data.m_touches->at(i)->clientX);
		touchObj->insert("clientY", _data.m_touches->at(i)->clientY);
		touchObj->insert("pageX", _data.m_touches->at(i)->pageX);
		touchObj->insert("pageY", _data.m_touches->at(i)->pageY);
		touchObj->insert("radiusX", _data.m_touches->at(i)->radiusX);
		touchObj->insert("radiusY", _data.m_touches->at(i)->radiusY);
		touchObj->insert("rotationAngle", _data.m_touches->at(i)->rotationAngle);
		touchObj->insert("force", _data.m_touches->at(i)->force);
		touches.append(touchObj);
	}
	obj->insert("touches", touches);

	AcArray<AcJsonObject*> changedTouches;
	for (int i = 0; i < _data.m_changedTouches->length(); i++) {
		AcJsonObject* touchObj = new AcJsonObject();
		touchObj->insert("identifier", _data.m_changedTouches->at(i)->identifier);
		touchObj->insert("clientX", _data.m_changedTouches->at(i)->clientX);
		touchObj->insert("clientY", _data.m_changedTouches->at(i)->clientY);
		touchObj->insert("pageX", _data.m_changedTouches->at(i)->pageX);
		touchObj->insert("pageY", _data.m_changedTouches->at(i)->pageY);
		touchObj->insert("radiusX", _data.m_changedTouches->at(i)->radiusX);
		touchObj->insert("radiusY", _data.m_changedTouches->at(i)->radiusY);
		touchObj->insert("rotationAngle", _data.m_changedTouches->at(i)->rotationAngle);
		touchObj->insert("force", _data.m_changedTouches->at(i)->force);
		changedTouches.append(touchObj);
	}
	obj->insert("changedTouches", changedTouches);

	std::string str = AcJsonObject::stringify(obj);
	delete obj;
	return str;
}

EdTouchEvent EdTouchEvent::parse(const std::string& _data) {
	AcJsonObject* obj = AcJsonObject::parse(_data.c_str());
	EdTouchEvent touchEvent;
	touchEvent.altKey = obj->get("altKey")->toBool();
	touchEvent.ctrlKey = obj->get("ctrlKey")->toBool();
	touchEvent.shiftKey = obj->get("shiftKey")->toBool();

	AcJsonObject* touches = obj->get("touches");
	for (int i = 0; i < touches->length(); i++) {
		EdTouch *touch = new EdTouch();
		touch->pointerType = (EPointerType)touches->at(i)->get("pointerType")->toInt32();
		touch->identifier = touches->at(i)->get("identifier")->toInt32();
		touch->clientX = touches->at(i)->get("clientX")->toFloat64();
		touch->clientY = touches->at(i)->get("clientY")->toFloat64();
		touch->pageX = touches->at(i)->get("pageX")->toFloat64();
		touch->pageY = touches->at(i)->get("pageY")->toFloat64();
		touch->radiusX = touches->at(i)->get("radiusX")->toFloat64();
		touch->radiusY = touches->at(i)->get("radiusY")->toFloat64();
		touch->rotationAngle = touches->at(i)->get("rotationAngle")->toFloat64();
		touch->force = touches->at(i)->get("force")->toFloat64();
		touchEvent.m_touches->append(touch);
	}

	AcJsonObject* changedTouches = obj->get("changedTouches");
	for (int i = 0; i < changedTouches->length(); i++) {
		EdTouch* touch = new EdTouch();
		touch->pointerType = (EPointerType)changedTouches->at(i)->get("pointerType")->toInt32();
		touch->identifier = changedTouches->at(i)->get("identifier")->toInt32();
		touch->clientX = changedTouches->at(i)->get("clientX")->toFloat64();
		touch->clientY = changedTouches->at(i)->get("clientY")->toFloat64();
		touch->pageX = changedTouches->at(i)->get("pageX")->toFloat64();
		touch->pageY = changedTouches->at(i)->get("pageY")->toFloat64();
		touch->radiusX = changedTouches->at(i)->get("radiusX")->toFloat64();
		touch->radiusY = changedTouches->at(i)->get("radiusY")->toFloat64();
		touch->rotationAngle = changedTouches->at(i)->get("rotationAngle")->toFloat64();
		touch->force = changedTouches->at(i)->get("force")->toFloat64();
		touchEvent.m_changedTouches->append(touch);
	}

	delete obj;
	return touchEvent;
}

EdTouchEvent* EdTouchEvent::clone() const {
	EdTouchEvent* p = new EdTouchEvent();
	p->altKey = this->altKey;
	p->ctrlKey = this->ctrlKey;
	p->shiftKey = this->shiftKey;
	for (int i = 0; i < this->m_touches->length(); i++) {
		EdTouch *touch = EdTouch::clone(this->m_touches->at(i));
		p->touches()->append(touch);
	}
	for (int i = 0; i < this->m_changedTouches->length(); i++) {
		EdTouch* touch = EdTouch::clone(this->m_changedTouches->at(i));
		p->changedTouches()->append(touch);
	}
	return p;
}