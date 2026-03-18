#ifndef EDTOUCH_H
#define EDTOUCH_H

enum EPointerType {
	kUnknown,
	kMouse,
	kTouch,
	kPen,
};

class EdTouch {
public:
	EdTouch() {
		this->clientX = 0.0;
		this->clientY = 0.0;
		this->force = 0.0;
		this->identifier = 0;
		this->pageX = 0.0;
		this->pageY = 0.0;
		this->radiusX = 0.0;
		this->radiusY = 0.0;
		this->rotationAngle = 0.0;
		this->pointerType = EPointerType::kUnknown;
	}
	virtual ~EdTouch() {}

	bool operator == (const EdTouch& touch) const {
		if (this->clientX == touch.clientX &&
			this->clientY == touch.clientY &&
			this->force == touch.force &&
			this->identifier == touch.identifier &&
			this->pageX == touch.pageX &&
			this->pageY == touch.pageY &&
			this->radiusX == touch.radiusX &&
			this->radiusY == touch.radiusY &&
			this->rotationAngle == touch.rotationAngle &&
			this->pointerType == touch.pointerType) {
			return true;
		}
		return false;
	}

	double clientX;
	double clientY;
	double force;
	int identifier;
	double pageX;
	double pageY;
	double radiusX;
	double radiusY;
	double rotationAngle;
	EPointerType pointerType;

	static EdTouch* clone(const EdTouch* touch) {
		EdTouch *tempTouch = new EdTouch();
		tempTouch->clientX = touch->clientX;
		tempTouch->clientY = touch->clientY;
		tempTouch->force = touch->force;
		tempTouch->identifier = touch->identifier;
		tempTouch->pageX = touch->pageX;
		tempTouch->pageY = touch->pageY;
		tempTouch->radiusX = touch->radiusX;
		tempTouch->radiusY = touch->radiusY;
		tempTouch->rotationAngle = touch->rotationAngle;
		tempTouch->pointerType = touch->pointerType;
		return tempTouch;
	}
};

#endif