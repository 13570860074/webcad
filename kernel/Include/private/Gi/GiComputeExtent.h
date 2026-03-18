#ifndef GICOMPUTEEXTENTTHREAD_H
#define GICOMPUTEEXTENTTHREAD_H


#include <thread>
#include <mutex>
#include <atomic>
#include "GePoint2d.h"
#include "AcArray.h"
#include <functional>


class GiEntityGeometry;
class GiComputeExtent
{
public:
	GiComputeExtent();
	virtual ~GiComputeExtent();

	bool isMainThread;
	bool isModelSpace;
	GePoint2d startPoint;
	GePoint2d endPoint;
	AcArray<GiEntityGeometry*> geometrys;
	AcArray<GiEntityGeometry*> selectGeometrys;
	std::atomic<bool> isDone;

	static void compute(GiComputeExtent* computeExtentThread);
};


#endif