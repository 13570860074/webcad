#ifndef EDUSERINPUTMANAGER_H
#define EDUSERINPUTMANAGER_H

#include "export.h"
#include "AcEvent.h"
#include "RxManager.h"
#include "GePoint2d.h"
#include "EdTouchEvent.h"
#include "EdWheelEvent.h"
#include "EdMouseEvent.h"
#include "EdKeyboardEvent.h"

class EdUserInputManager : public RxObject, public RxManager
{
public:
	EdUserInputManager();
	virtual ~EdUserInputManager();
	RX_DECLARE_MEMBERS(EdUserInputManager);
	RX_MANAGER_DECLARE_MEMBERS(EdUserInputManager);

	static void onBeginAnimationFrame(EdUserInputManager * pointer);

public:

	/* 初始化 */
	virtual void onInit();

	/**
	* 用户输入
	* @param input 输入类型
	*	mousewheel,
	*	mousemove,
	*	mouseup,
	*	mousedown,
	*	dblclick,
	*	keydown,
	*	keyup,
	*	touchstart,
	*	touchmove,
	*	touchend
	* @param data 输入数据
	*/
	virtual void userInput(const std::string& input, const std::string& data);

	/**
	* 设置绘图区范围
	*/
	virtual void setRangRect(const double _top, const double _bottom, const double _left, const double _right);

	/**
	* 设置屏幕尺寸
	*/
	virtual void setScreenSize(double _width, double _height);

	/**
	* 设置屏幕像素比例
	*/
	virtual void setDevicePixelRatio(double _devicePixelRatio);

	/**
	* 鼠标滚轮滚动
	*/
	virtual void mousewheel(const EdWheelEvent& _wheelEvent);
	virtual void mousewheel(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey, int wheelDelta);

	/**
	* 鼠标移动
	*/
	virtual void mousemove(const EdMouseEvent& _mouseEvent);
	virtual void mousemove(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey);

	/**
	* 鼠标抬起
	*/
	virtual void mouseup(const EdMouseEvent& _mouseEvent);
	virtual void mouseup(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey);

	/**
	* 鼠标按下
	*/
	virtual void mousedown(const EdMouseEvent& _mouseEvent);
	virtual void mousedown(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey);

	/**
	* 双击事件
	*/
	virtual void dblclick(const EdMouseEvent& _mouseEvent);
	virtual void dblclick(int button, double clientX, double clientY, bool altKey, bool ctrlKey, bool shiftKey);

	/**
	* 键盘按下
	*/
	virtual void keydown(const EdKeyboardEvent& keyboardEvent);

	/**
	* 按键抬起
	*/
	virtual void keyup(const EdKeyboardEvent& keyboardEvent);


	/**
	* 触摸(移动端)
	*/
	virtual void touchstart(const EdTouchEvent& _touchEvent);

	/**
	* 触摸移动(移动端)
	*/
	virtual void touchmove(const EdTouchEvent& _touchEvent);

	/**
	* 触摸松开(移动端)
	*/
	virtual void touchend(const EdTouchEvent& _touchEvent);


	/** 是否处于按下shift状态 */
	bool shift() const;

	/** 是否处于按下ctrl状态 */
	bool ctrl() const;


	/** 获得帧率 */
	int fps() const;

	/** 设置帧率 */
	void setFps(const int v);

	/** 获得一帧时间ms */
	int ms()const;

	/** 获得光标所在的设备区位置 */
	GePoint2d deviceLocation() const;

	/** 设置光标所在的设备区位置 */
	void setDeviceLocation(const GePoint2d& _point);

	/** 获得设备像素比例 */
	double devicePixelRatio()const;

	/** 获得屏幕宽度 */
	double screenWidth()const;

	/** 获得屏幕高度 */
	double screenHeight()const;

	/** 获得绘图区宽高比 */
	double screenAspect() const;

	/** 获得绘图区范围 */
	void rangRect(double& _left, double& _right, double& _top, double& _bottom);

	/** 获得绘图区宽度 */
	double rangWidth()const;

	/** 获得绘图区高度 */
	double rangHeight()const;
};

#endif