
#include "RxWorkerThread.h"
#include "RxImpl.h"
#include <thread>
#include <mutex>


class RxWorkerThreadTask {
public:
	RxWorkerThreadTask() {
		this->id = 0;
		this->funCallback = NULL;
		this->callback = NULL;
		this->lpParamter = NULL;
		this->isLock = true;
		this->isDone = false;
	}

	unsigned int id;
	void* lpParamter;
	std::function<void()> funCallback;
	void(*callback)(void*);
	bool isLock;
	bool isDone;
};



void RxWorkerThread::onCallbackThread(RxWorkerThread* param) {
	std::cout << "创建线程,进入循环开始" << std::endl;
	while (true) {

		if (RX_IMP_WORKERTHREAD(param->m_pImpl)->isClose.load() == true) {
			break;
		}

		// 获得任务
		AcArray<RxWorkerThreadTask*> tasks;
		RX_IMP_WORKERTHREAD(param->m_pImpl)->mutex.lock();
		for (int i = 0; i < RX_IMP_WORKERTHREAD(param->m_pImpl)->tasks.length(); i++) {
			tasks.append(RX_IMP_WORKERTHREAD(param->m_pImpl)->tasks.at(i));
		}
		RX_IMP_WORKERTHREAD(param->m_pImpl)->mutex.unlock();

		// 如果没有任务则等待
		if (tasks.length() == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		// 执行任务
		for (int i = 0; i < tasks.length(); i++) {
			if (tasks.at(i)->callback != NULL) {
				tasks.at(i)->callback(tasks.at(i)->lpParamter);
			}
			else {
				tasks.at(i)->funCallback();
			}
			tasks.at(i)->isDone = true;
		}

		// 移除已完成的任务
		RX_IMP_WORKERTHREAD(param->m_pImpl)->mutex.lock();
		bool isLock = false;
		for (int i = RX_IMP_WORKERTHREAD(param->m_pImpl)->tasks.length() - 1; i >= 0; i--) {
			if (tasks.at(i)->isDone == true) {
				realloc_arry_at(RX_IMP_WORKERTHREAD(param->m_pImpl)->tasks, i);
			}
			else {
				if (tasks.at(i)->isLock == true) {
					isLock = true;
				}
			}
		}
		RX_IMP_WORKERTHREAD(param->m_pImpl)->isLock.store(isLock);
		RX_IMP_WORKERTHREAD(param->m_pImpl)->mutex.unlock();
	}

	RX_IMP_WORKERTHREAD(param->m_pImpl)->isThread = false;

	std::cout << "进入循环结束" << std::endl;
}


RxWorkerThread::RxWorkerThread() {
	this->m_pImpl = new RxWorkerThreadImpl();
}
RxWorkerThread::~RxWorkerThread() {

}

unsigned int RxWorkerThread::id() {
	return RX_IMP_WORKERTHREAD(this->m_pImpl)->id;
}
unsigned int RxWorkerThread::addTask(void(*fun)(void*), const bool lock) {
	return this->addTask(fun, NULL, lock);
}
unsigned int RxWorkerThread::addTask(void(*fun)(void*), void* lpParamter, const bool lock) {

	RX_IMP_WORKERTHREAD(this->m_pImpl)->mutex.lock();

	RxWorkerThreadTask* task = new RxWorkerThreadTask();
	task->id = this->id() * 1000000 + RX_IMP_WORKERTHREAD(this->m_pImpl)->index++;
	task->callback = fun;
	task->isLock = lock;
	task->lpParamter = lpParamter;
	RX_IMP_WORKERTHREAD(this->m_pImpl)->tasks.append(task);
	if (lock == true) {
		RX_IMP_WORKERTHREAD(this->m_pImpl)->isLock.store(true);
	}
	RX_IMP_WORKERTHREAD(this->m_pImpl)->mutex.unlock();

	return task->id;
}
unsigned int RxWorkerThread::addTask(std::function<void()> fun, const bool lock) {

	RX_IMP_WORKERTHREAD(this->m_pImpl)->mutex.lock();

	RxWorkerThreadTask* task = new RxWorkerThreadTask();
	task->id = this->id() * 1000000 + RX_IMP_WORKERTHREAD(this->m_pImpl)->index++;
	task->funCallback = fun;
	task->isLock = lock;
	RX_IMP_WORKERTHREAD(this->m_pImpl)->tasks.append(task);
	if (lock == true) {
		RX_IMP_WORKERTHREAD(this->m_pImpl)->isLock.store(true);
	}
	RX_IMP_WORKERTHREAD(this->m_pImpl)->mutex.unlock();

	return task->id;
}
void RxWorkerThread::removeTask(unsigned int _taskId) {
	for (int i = 0; i < RX_IMP_WORKERTHREAD(this->m_pImpl)->tasks.length(); i++) {
		if (RX_IMP_WORKERTHREAD(this->m_pImpl)->tasks[i]->id != _taskId) {
			continue;
		}
		RX_IMP_WORKERTHREAD(this->m_pImpl)->tasks.removeAt(i);
		break;
	}
}


