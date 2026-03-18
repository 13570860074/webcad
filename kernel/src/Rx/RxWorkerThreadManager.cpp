#include "RxWorkerThreadManager.h"
#include "DbObjectManager.h"
#include "GiEntityGeometry.h"
#include "AcEventTrigger.h"
#include "GiLineSegmentGeometry.h"
#include "GiTriangleMeshGeometry.h"
#include "RxWorkerThread.h"
#include "arxHeaders.h"
#include "acedTrans.h"
#include "AcArray.h"
#include "AcDebug.h"
#include "GeMath.h"
#include "kernel.h"
#include "RxImpl.h"
#include "DbImpl.h"
#include <thread>
#include <mutex>


RxWorkerThreadManager::RxWorkerThreadManager()
{
	this->m_pImpl = new RxWorkerThreadManagerImpl();
	RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->numCores = std::thread::hardware_concurrency();
	if (RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->numCores > 32) {
		RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->numCores = 32;
	}
}
RxWorkerThreadManager::~RxWorkerThreadManager()
{
}

void RxWorkerThreadManager::onInit()
{
	this->createThread();
}

int RxWorkerThreadManager::numCores()
{
	return RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->numCores;
}
int RxWorkerThreadManager::numWorkerThread() {
	if (RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->bPause == true) {
		return 0;
	}
	int num = 0;
	for (int i = 0; i < RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.length(); i++) {
		RxWorkerThread* workerThread = RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads[i];
		if (RX_IMP_WORKERTHREAD(workerThread->m_pImpl)->isLock.load() == false) {
			num++;
		}
	}
	return num;
}
void RxWorkerThreadManager::pause(const bool bPause) {
	RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->bPause = bPause;
}
unsigned int RxWorkerThreadManager::addTask(void(*fun)(void*), const bool lock) {
	return this->addTask(fun, NULL, lock);
}
unsigned int RxWorkerThreadManager::addTask(void(*fun)(void*), void* lpParamter, const bool lock) {
	AcArray<RxWorkerThread*> threads;
	this->workerThreads(threads);

	RxWorkerThread* workerThread = NULL;
	if (threads.length() > 0) {
		workerThread = threads[0];
	}
	bool isFind = false;
	for (int i = 0; i < threads.length(); i++) {
		if (isFind == true) {
			workerThread = threads[i];
			break;
		}
		if (threads[i] == RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->lastWorkerThread) {
			isFind = true;
		}
	}
	RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->lastWorkerThread = workerThread;

	unsigned int id = 0;
	if (workerThread != NULL) {
		id = workerThread->addTask(fun, lpParamter, lock);
	}
	return id;
}
unsigned int RxWorkerThreadManager::addTask(std::function<void()> fun, const bool lock) {

	RxWorkerThread* workerThread = NULL;

	int index = 0;
	bool isFind = false;
	AcArray<RxWorkerThread*> workerThreads;
	for (int i = 0; i < RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.length(); i++) {
		RxWorkerThread * worker = RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads[i];
		if (worker == NULL) {
			continue;
		}
		if (workerThread == NULL && RX_IMP_WORKERTHREAD(worker->m_pImpl)->isLock.load() == false) {
			workerThread = worker;
			index = i;
		}
		if (isFind == false && worker == RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->lastWorkerThread) {
			isFind = true;
			continue;
		}
		else if (isFind == true && RX_IMP_WORKERTHREAD(worker->m_pImpl)->isLock.load() == false) {
			workerThread = worker;
			index = i;
			break;
		}
	}
	RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->lastWorkerThread = workerThread;

	unsigned int id = 0;
	if (workerThread != NULL) {
		id = workerThread->addTask(fun, lock);
	}
	return id;
}
void RxWorkerThreadManager::workerThreads(AcArray<RxWorkerThread*>& workerThreads){
	for (int i = 0; i < RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.length(); i++) {
		RxWorkerThread* workerThread = RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads[i];
		if (RX_IMP_WORKERTHREAD(workerThread->m_pImpl)->isLock.load() == false) {
			workerThreads.append(workerThread);
		}
	}
}
void RxWorkerThreadManager::createThread() {
	if (RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.length() == 0)
	{
		unsigned int numCores = RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->numCores;
		for (int i = 0; i < numCores; i++) {
			RxWorkerThread* thread = new RxWorkerThread();
			RX_IMP_WORKERTHREAD(thread->m_pImpl)->id = i + 1;
			RX_IMP_WORKERTHREAD(thread->m_pImpl)->isThread = true;
			RX_IMP_WORKERTHREAD(thread->m_pImpl)->thread = new std::thread(RxWorkerThread::onCallbackThread, thread);
			RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.append(thread);
		}
	}
}
void RxWorkerThreadManager::closeThread() {
	for (int i = 0; i < RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.length(); i++) {
		RxWorkerThread* thread = RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.at(i);
		if (RX_IMP_WORKERTHREAD(thread->m_pImpl)->isThread == false) {
			continue;
		}
		RX_IMP_WORKERTHREAD(thread->m_pImpl)->isClose.store(true);
	}
	for (int i = 0; i < RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.length(); i++) {
		RxWorkerThread* thread = RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads.at(i);
		if (RX_IMP_WORKERTHREAD(thread->m_pImpl)->isThread == false) {
			continue;
		}
		RX_IMP_WORKERTHREAD(thread->m_pImpl)->thread->join();
	}
	realloc_arry_all(RX_IMP_WORKERTHREADMANAGER(this->m_pImpl)->workerThreads);
}
