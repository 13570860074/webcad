#ifndef RXPARALLELCOMPUTEMANAGER_H
#define RXPARALLELCOMPUTEMANAGER_H


#include "RxManager.h"
#include "DbOsnapData.h"
#include "GeMatrix3d.h"
#include "RxWorkerThread.h"


class GiEntityGeometry;
class RxWorkerThreadManager : public RxObject, public RxManager {
public:
	RxWorkerThreadManager();
	virtual ~RxWorkerThreadManager();
	RX_DECLARE_MEMBERS(RxWorkerThreadManager);
	RX_MANAGER_DECLARE_MEMBERS(RxWorkerThreadManager);

	/** 创建线程 */
	virtual void createThread();

	/** 关闭线程 */
	virtual void closeThread();

public:

	/** 初始化 */
	virtual void onInit();

	/** 获得逻辑处理器数量(最少2线程,最多32线程) */
	virtual int numCores();

	/** 获得可用的线程数量 */
	virtual int numWorkerThread();

	/** 禁用线程 */
	virtual void pause(const bool bPause = true);

	/** 获得可用于工作的线程对象 */
	virtual void workerThreads(AcArray<RxWorkerThread*>& workerThreads);

	/*
		fun:回调函数
		lpParamter:参数
		lock:是否锁定线程,如果锁定线程则表示该线程为不可用状态不在分配任务
	*/
	/** 添加任务 */
	virtual unsigned int addTask(void(*fun)(void*), const bool lock);
	virtual unsigned int addTask(void(*fun)(void*), void* lpParamter, const bool lock);
	virtual unsigned int addTask(std::function<void()> fun, const bool lock);
};




#endif




