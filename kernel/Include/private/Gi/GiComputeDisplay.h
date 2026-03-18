#ifndef GICOMPUTEDISPLAY_H
#define GICOMPUTEDISPLAY_H

#include <atomic>
#include <functional>
#include "AcArray.h"
#include "GeMatrix3d.h"

/*
* 
* // 同步计算逻辑
*	1.一个逻辑处理器对应一个GiComputeDisplay
*	2.矩阵计算完成后设置isDone为true
* 
* // 异步计算逻辑
*	1.单个线程处理逻辑同GiAsyncComputeDisplay
*	2.GiAsyncComputeDisplayTask用于一个计算任务,一个计算任务包含多个GiAsyncComputeDisplay
* 		在启动计算任务时设置isWorking为true,计算完成后设置isWorking为false
* 
*/

class GiEntityGeometry;
class GiComputeDisplay
{
public:
	GiComputeDisplay();
	virtual ~GiComputeDisplay();

public:
	bool isMainThread;
	GeMatrix3d matrix;
	bool defaultMatrix;
	std::atomic<bool> isDone;
	AcArray<GiEntityGeometry *> allGeometrys;

public:
	static void compute(GiComputeDisplay* computeDisplay);
};

#endif

