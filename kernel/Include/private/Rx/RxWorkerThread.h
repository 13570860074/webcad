#ifndef RXWORKERTHREAD_H
#define RXWORKERTHREAD_H

#include "RxObject.h"
#include <functional>


class RxWorkerThread :public RxObject{
public:
	RxWorkerThread();
	virtual ~RxWorkerThread();
	RX_DECLARE_MEMBERS(RxWorkerThread);

	static void onCallbackThread(RxWorkerThread*param);

public:
	/** 获得id */
	unsigned int id();

	/** 添加任务 */
	virtual unsigned int addTask(void(*fun)(void*), const bool lock);
	virtual unsigned int addTask(void(*fun)(void*), void* lpParamter, const bool lock);
	unsigned int addTask(std::function<void()> fun, const bool lock);

	/** 移除任务 */
	void removeTask(unsigned int _taskId);
};


#endif




