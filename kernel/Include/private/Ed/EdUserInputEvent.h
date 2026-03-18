#ifndef EDUSERINPUTEVENT_H
#define EDUSERINPUTEVENT_H

#include <string>

enum EdUserInputEventType {
	kNull,

	kMousemove,									// 鼠标移动
	kMousewheel,								// 滚轮滚动
	kMouseup,									// 鼠标抬起
	kMousedown,									// 鼠标按下
	kDblclick,									// 双击事件

	kKeydown,									// 键盘按下
	kKeyup,										// 按键弹起

	kTouchstart,								// 触摸开始
	kTouchend,									// 触摸解除
	kTouchmove,									// 触摸移动
};



class EdUserInputEvent {

protected:
	EdUserInputEvent() {
		this->m_type = EdUserInputEventType::kNull;
	}

public:
	virtual ~EdUserInputEvent() {}


private:
	EdUserInputEventType m_type;

public:

	/** 事件模式 */
	virtual EdUserInputEventType eventType()const {
		return this->m_type;
	}

	/** 设置模式 */
	virtual void setEventType(const EdUserInputEventType _mode) {
		this->m_type = _mode;
	}
};

#endif