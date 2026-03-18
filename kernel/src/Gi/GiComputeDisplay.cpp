#include "GiGeometryExtent.h"
#include "GiEntityManager.h"
#include "GiEntityIterator.h"
#include "GiPointGeometry.h"
#include "GiComputeDisplay.h"
#include "GiLineSegmentGeometry.h"
#include "RxWorkerThread.h"
#include "RxWorkerThreadManager.h"
#include "DbObjectManager.h"
#include "ApDocument.h"
#include "arxHeaders.h"
#include "acedTrans.h"
#include "AcDebug.h"
#include "kernel.h"
#include "GeMath.h"
#include "GiImpl.h"
#include "DbImpl.h"
#include <thread>
#include <mutex>

GiComputeDisplay::GiComputeDisplay() {
	this->isDone.store(false);
	this->isMainThread = false;
	this->defaultMatrix = false;
}
GiComputeDisplay::~GiComputeDisplay()
{
}
void GiComputeDisplay::compute(GiComputeDisplay* computeDisplay)
{
	for (int i = 0; i < computeDisplay->allGeometrys.length(); i++)
	{
		if (computeDisplay->allGeometrys[i]->isErased() == true)
		{
			continue;
		}
		if (computeDisplay->allGeometrys[i]->type() == Gi::EntityGeometryType::kLineSegmentGeometry)
		{
			GiLineSegmentGeometry* line = (GiLineSegmentGeometry*)computeDisplay->allGeometrys[i];

			GePoint3d pos = line->startPoint();
			pos.transformBy(computeDisplay->matrix);
			line->setDisplayCoord(0, GePoint2d(pos.x, pos.y));

			pos = line->endPoint();
			pos.transformBy(computeDisplay->matrix);
			line->setDisplayCoord(1, GePoint2d(pos.x, pos.y));
		}
		else if (computeDisplay->allGeometrys[i]->type() == Gi::EntityGeometryType::kTriangleMeshGeometry)
		{
			GiTriangleMeshGeometry* triangleMesh = (GiTriangleMeshGeometry*)computeDisplay->allGeometrys[i];

			GePoint3d p1 = triangleMesh->vertex(0);
			GePoint3d p2 = triangleMesh->vertex(1);
			GePoint3d p3 = triangleMesh->vertex(2);

			p1.transformBy(computeDisplay->matrix);
			p2.transformBy(computeDisplay->matrix);
			p3.transformBy(computeDisplay->matrix);

			triangleMesh->setDisplayCoord(0, GePoint2d(p1.x, p1.y));
			triangleMesh->setDisplayCoord(1, GePoint2d(p2.x, p2.y));
			triangleMesh->setDisplayCoord(2, GePoint2d(p3.x, p3.y));
		}
		else if (computeDisplay->allGeometrys[i]->type() == Gi::EntityGeometryType::kPointGeometry) {
			
			GiPointGeometry* pointGeometry = (GiPointGeometry*)computeDisplay->allGeometrys[i];

			GePoint3d position = pointGeometry->position();

			position.transformBy(computeDisplay->matrix);

			pointGeometry->setDisplayCoord(0, GePoint2d(position.x, position.y));
		}
	}
	computeDisplay->isDone.store(true);
}
