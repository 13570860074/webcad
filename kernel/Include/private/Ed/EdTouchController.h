#ifndef EDTOUCHCONTROLLER_H
#define EDTOUCHCONTROLLER_H

#include "EdController.h"

class EdTouchEvent;
class EdTouchController :public EdController {
public:
	EdTouchController();
	virtual ~EdTouchController();

public:

	static void onBeginAnimationFrame(EdTouchController* pointer);

	static void onTouchstart(EdTouchController* pointer, const EdTouchEvent& _touchEvent);
	static void onTouchmove(EdTouchController* pointer, const EdTouchEvent& _touchEvent);
	static void onTouchend(EdTouchController* pointer, const EdTouchEvent& _touchEvent);

public:
	virtual void onInit();
};



#endif