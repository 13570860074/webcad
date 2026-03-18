#ifndef EDACADCONTROLLER_H
#define EDACADCONTROLLER_H


#include "EdController.h"
#include "EdMouseEvent.h"
#include "EdWheelEvent.h"
#include "EdKeyboardEvent.h"

class GePoint2d;
class EdTouchEvent;
class EdWheelEvent;
class EdMouseEvent;
class EdAcadController : public EdController {
public:
	EdAcadController();
	virtual ~EdAcadController();
	RX_DECLARE_MEMBERS(EdAcadController);

public:

	static void onBeginAnimationFrame(EdAcadController* pointer);

	static void onBeginDrag(EdAcadController* pointer);
	static void onEndDrag(EdAcadController* pointer);

	static void onMousemove(EdAcadController* pointer, const EdMouseEvent& _mouseEvent);
	static void onMousewheel(EdAcadController* pointer, const EdWheelEvent& _wheelEvent);
	static void onMouseup(EdAcadController* pointer, const EdMouseEvent& _mouseEvent);
	static void onMousedown(EdAcadController* pointer, const EdMouseEvent& _mouseEvent);
	static void onDblclick(EdAcadController* pointer, const EdMouseEvent& _mouseEvent);

	static void onInterval(EdAcadController* pointer);

private:

	// 初始化
	virtual void onInit();
};


#endif