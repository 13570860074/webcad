#ifndef ACEVENTTRIGGER_H
#define ACEVENTTRIGGER_H


#include "AcEventBase.h"


class AcEventTriggerItem {
public:
	AcEventTriggerItem() {
		this->mode = 0;
		this->function = NULL;
		this->callback = NULL;
		this->lpParamter = NULL;
		this->id = 0;
		this->numParam = 0;
	}

	int mode;           //0表AcEmsdkEvent,1表AcEvent
	void* function;
	std::function<void()> callback;
	unsigned id;
	void* lpParamter;
	unsigned int numParam;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)())this->function)();
			}
		}
		else {
			if (this->function != NULL) {
				((void (*)(void*))this->function)(this->lpParamter);
			}
		}

		if (this->callback != NULL) {
			std::function<void()> callback = (std::function<void()>&)this->callback;
			callback();
		}
	}
};
template <typename T1>
class AcEventTriggerItemParam1 :public AcEventTriggerItem {
public:
	T1 t1;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1))this->function)(this->t1);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1))this->function)(this->lpParamter, this->t1);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1)> callback = (std::function<void(T1)>&)this->callback;
			callback(this->t1);
		}
	}
};
template <typename T1, typename T2>
class AcEventTriggerItemParam2 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2))this->function)(this->t1, this->t2);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2))this->function)(this->lpParamter, this->t1, this->t2);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2)> callback = (std::function<void(T1, T2)>&)this->callback;
			callback(this->t1, this->t2);
		}
	}
};
template <typename T1, typename T2, typename T3>
class AcEventTriggerItemParam3 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3))this->function)(this->t1, this->t2, this->t3);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3))this->function)(this->lpParamter, this->t1, this->t2, this->t3);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3)> callback = (std::function<void(T1, T2, T3)>&)this->callback;
			callback(this->t1, this->t2, this->t3);
		}
	}
};
template <typename T1, typename T2, typename T3, typename T4>
class AcEventTriggerItemParam4 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3, T4))this->function)(this->t1, this->t2, this->t3, this->t4);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3, T4))this->function)(this->lpParamter, this->t1, this->t2, this->t3, this->t4);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3, T4)> callback = (std::function<void(T1, T2, T3, T4)>&)this->callback;
			callback(this->t1, this->t2, this->t3, this->t4);
		}
	}
};
template <typename T1, typename T2, typename T3, typename T4, typename T5>
class AcEventTriggerItemParam5 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
	T5 t5;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3, T4, T5))this->function)(this->t1, this->t2, this->t3, this->t4, this->t5);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3, T4, T5))this->function)(this->lpParamter, this->t1, this->t2, this->t3, this->t4, this->t5);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3, T4, T5)> callback = (std::function<void(T1, T2, T3, T4, T5)>&)this->callback;
			callback(this->t1, this->t2, this->t3, this->t4, this->t5);
		}
	}
};
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class AcEventTriggerItemParam6 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
	T5 t5;
	T6 t6;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3, T4, T5, T6))this->function)(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3, T4, T5, T6))this->function)(this->lpParamter, this->t1, this->t2, this->t3, this->t4, this->t5, this->t6);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3, T4, T5, T6)> callback = (std::function<void(T1, T2, T3, T4, T5, T6)>&)this->callback;
			callback(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6);
		}
	}
};
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class AcEventTriggerItemParam7 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
	T5 t5;
	T6 t6;
	T7 t7;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3, T4, T5, T6, T7))this->function)(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3, T4, T5, T6, T7))this->function)(this->lpParamter, this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3, T4, T5, T6, T7)> callback = (std::function<void(T1, T2, T3, T4, T5, T6, T7)>&)this->callback;
			callback(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7);
		}
	}
};
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class AcEventTriggerItemParam8 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
	T5 t5;
	T6 t6;
	T7 t7;
	T8 t8;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3, T4, T5, T6, T7, T8))this->function)(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7, this->t8);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3, T4, T5, T6, T7, T8))this->function)(this->lpParamter, this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7, this->t8);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3, T4, T5, T6, T7, T8)> callback = (std::function<void(T1, T2, T3, T4, T5, T6, T7, T8)>&)this->callback;
			callback(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7, this->t8);
		}
	}
};
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class AcEventTriggerItemParam9 :public AcEventTriggerItem {
public:
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
	T5 t5;
	T6 t6;
	T7 t7;
	T8 t8;
	T9 t9;

	virtual void _emit() {
		if (this->mode == 0) {
			if (this->function != NULL) {
				((void (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9))this->function)(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7, this->t8, this->t9);
			}
		}
		else if (this->mode == 1) {
			if (this->function != NULL) {
				((void (*)(void*, T1, T2, T3, T4, T5, T6, T7, T8, T9))this->function)(this->lpParamter, this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7, this->t8, this->t9);
			}
		}

		if (this->callback != NULL) {
			std::function<void(T1, T2, T3, T4, T5, T6, T7, T8, T9)> callback = (std::function<void(T1, T2, T3, T4, T5, T6, T7, T8, T9)>&)this->callback;
			callback(this->t1, this->t2, this->t3, this->t4, this->t5, this->t6, this->t7, this->t8, this->t9);
		}
	}
};




/*

触发器:
	1.因为wasm中在使用异步(EM_ASYNC_JS)的情况下c++传递给js的值全变Promise对象了,无法直接使用
	2.采用的方法就是延迟执行,参考RxAsyncManager
	3.当触发事件的时候将回调函数和参数记录下来(AcEventTriggerItem)
	4.等程序退出EM_ASYNC_JS的情况后在执行任务,这样在EM_ASYNC_JS的情况下就能间接使用C++对象了
	5.这里又引发一个问题,因为是延迟执行,部分参数对象的声明周期只在整个函数内,延迟执行在之后要使用的时候已经被释放掉了
	6.这里采用的做法是把临时参数记录到全局中,调用js回调函数的时候直接使用全局指针,等执行回调函数后在删除指针
*/

class AcEventTrigger {
public:

private:
	static bool trigger;                                // 是否启用触发器
	static std::vector<AcEventTriggerItem*> items;      // 触发器选项
	static bool m_isPrintLog;                           // 是否打印日志
	static std::vector<int> ignoreIds;                  // 被忽略的打印日志的id

public:
	static std::vector<char*> tchar_params;          // 字符串参数

public:
	// 添加事件
	static void append(AcEventTriggerItem* item);

	// 移除所有事件
	static void removeAllEvent();

	// 触发所有事件
	static void emitAllEvent(bool _isRemoveAll = true);

	// 设置是否启用触发器
	static void setTrigger(bool _trigger);

	// 判断是否启用触发器
	static bool isTrigger();

	// 判断是否打印日志
	static bool isPrintLog();

	// 设置是否打印日志
	static void setPrintLog(bool _isPrintLog);

	// 忽略事件打印
	static void ignoreLog(int _id);

	// 打印事件开始日志
	static void printLogStart(int _eventId, int _id);

	// 打印事件结束日志
	static void printLogEnd(int _eventId, int _id);
};






#endif