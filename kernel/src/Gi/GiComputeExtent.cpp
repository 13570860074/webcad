#include "GiComputeExtent.h"
#include "GiEntityGeometry.h"
#include "GiLineSegmentGeometry.h"
#include "GiTriangleMeshGeometry.h"
#include "DbEntity.h"
#include "DbEntityStake.h"
#include "GeMath.h"

GiComputeExtent::GiComputeExtent()
{
	this->isDone.store(false);
	this->isMainThread = false;
	this->isModelSpace = true;
}
GiComputeExtent::~GiComputeExtent()
{
}

void GiComputeExtent::compute(GiComputeExtent* computeExtentThread)
{

	// 判断是左框还是右框
	int mode = 1;
	if (computeExtentThread->startPoint.x > computeExtentThread->endPoint.x)
	{
		mode = 2;
	}

	// 获得最小和最大点
	GePoint2d minPoint = computeExtentThread->startPoint;
	GePoint2d maxPoint = computeExtentThread->endPoint;
	if (computeExtentThread->startPoint.x < minPoint.x) {
		minPoint.x = computeExtentThread->startPoint.x;
	}
	if (computeExtentThread->endPoint.x < minPoint.x) {
		minPoint.x = computeExtentThread->endPoint.x;
	}
	if (computeExtentThread->startPoint.y < minPoint.y) {
		minPoint.y = computeExtentThread->startPoint.y;
	}
	if (computeExtentThread->endPoint.y < minPoint.y) {
		minPoint.y = computeExtentThread->endPoint.y;
	}
	if (computeExtentThread->startPoint.x > maxPoint.x) {
		maxPoint.x = computeExtentThread->startPoint.x;
	}
	if (computeExtentThread->endPoint.x > maxPoint.x) {
		maxPoint.x = computeExtentThread->endPoint.x;
	}
	if (computeExtentThread->startPoint.y > maxPoint.y) {
		maxPoint.y = computeExtentThread->startPoint.y;
	}
	if (computeExtentThread->endPoint.y > maxPoint.y) {
		maxPoint.y = computeExtentThread->endPoint.y;
	}

	GePoint2d rangeCoords[4];
	rangeCoords[0].x = minPoint.x;
	rangeCoords[0].y = minPoint.y;
	rangeCoords[1].x = maxPoint.x;
	rangeCoords[1].y = minPoint.y;
	rangeCoords[2].x = maxPoint.x;
	rangeCoords[2].y = maxPoint.y;
	rangeCoords[3].x = minPoint.x;
	rangeCoords[3].y = maxPoint.y;

	for (int i = 0; i < computeExtentThread->geometrys.length(); i++)
	{
		if (computeExtentThread->geometrys[i]->stake() == NULL) {
			continue;
		}
		DbEntity* entity = computeExtentThread->geometrys[i]->stake()->rootEntity();
		if (entity == NULL)
		{
			continue;
		}
		if (entity->objectId().asOldId() == 0)
		{
			continue;
		}

		if (computeExtentThread->geometrys[i]->type() == Gi::EntityGeometryType::kLineSegmentGeometry)
		{
			GiLineSegmentGeometry* line = (GiLineSegmentGeometry*)computeExtentThread->geometrys[i];

			GePoint2d p1;
			GePoint2d p2;
			if (computeExtentThread->isModelSpace == true)
			{
				p1 = line->displayCoord(0);
				p2 = line->displayCoord(1);
			}
			else
			{
				p1.x = line->startPoint().x;
				p1.y = line->startPoint().y;
				p2.x = line->endPoint().x;
				p2.y = line->endPoint().y;
			}

			if (mode == 1)
			{
				if (p1.x < minPoint.x)
				{
					continue;
				}
				if (p2.x < minPoint.x)
				{
					continue;
				}
				if (p1.x > maxPoint.x)
				{
					continue;
				}
				if (p2.x > maxPoint.x)
				{
					continue;
				}
				if (p1.y < minPoint.y)
				{
					continue;
				}
				if (p2.y < minPoint.y)
				{
					continue;
				}
				if (p1.y > maxPoint.y)
				{
					continue;
				}
				if (p2.y > maxPoint.y)
				{
					continue;
				}

				computeExtentThread->selectGeometrys.append(computeExtentThread->geometrys[i]);
			}
			else if (mode == 2)
			{
				if (p1.x < minPoint.x && p2.x < minPoint.x)
				{
					continue;
				}
				if (p1.x > maxPoint.x && p2.x > maxPoint.x)
				{
					continue;
				}
				if (p1.y < minPoint.y && p2.y < minPoint.y)
				{
					continue;
				}
				if (p1.y > maxPoint.y && p2.y > maxPoint.y)
				{
					continue;
				}

				bool isIntersect = false;
				int direction = 0;
				for (int j = 0; j < 4; j++)
				{
					int val = GeMath::lineDirection(rangeCoords[j], p1, p2);
					if (j == 0)
					{
						direction = val;
						continue;
					}
					if (direction != val)
					{
						isIntersect = true;
						break;
					}
				}

				if (isIntersect == true)
				{
					computeExtentThread->selectGeometrys.append(computeExtentThread->geometrys[i]);
				}
			}
		}
		else if (computeExtentThread->geometrys[i]->type() == Gi::EntityGeometryType::kTriangleMeshGeometry)
		{
			GiTriangleMeshGeometry* triangleMesh = (GiTriangleMeshGeometry*)computeExtentThread->geometrys[i];

			GePoint2d p1;
			GePoint2d p2;
			GePoint2d p3;
			if (computeExtentThread->isModelSpace == true)
			{
				p1 = triangleMesh->displayCoord(0);
				p2 = triangleMesh->displayCoord(1);
				p3 = triangleMesh->displayCoord(2);
			}
			else
			{
				GePoint3d vertex1 = triangleMesh->vertex(0);
				GePoint3d vertex2 = triangleMesh->vertex(1);
				GePoint3d vertex3 = triangleMesh->vertex(2);
				p1 = GePoint2d(vertex1.x, vertex1.y);
				p2 = GePoint2d(vertex2.x, vertex2.y);
				p3 = GePoint2d(vertex3.x, vertex3.y);
			}

			if (mode == 1)
			{
				if (p1.x < minPoint.x)
				{
					continue;
				}
				if (p2.x < minPoint.x)
				{
					continue;
				}
				if (p3.x < minPoint.x)
				{
					continue;
				}
				if (p1.x > maxPoint.x)
				{
					continue;
				}
				if (p2.x > maxPoint.x)
				{
					continue;
				}
				if (p3.x > maxPoint.x)
				{
					continue;
				}
				if (p1.y < minPoint.y)
				{
					continue;
				}
				if (p2.y < minPoint.y)
				{
					continue;
				}
				if (p3.y < minPoint.y)
				{
					continue;
				}
				if (p1.y > maxPoint.y)
				{
					continue;
				}
				if (p2.y > maxPoint.y)
				{
					continue;
				}
				if (p3.y > maxPoint.y)
				{
					continue;
				}

				computeExtentThread->selectGeometrys.append(computeExtentThread->geometrys[i]);
			}
			else if (mode == 2)
			{

				if (p1.x < minPoint.x && p2.x < minPoint.x && p3.x < minPoint.x)
				{
					continue;
				}
				if (p1.x > maxPoint.x && p2.x > maxPoint.x && p3.x > maxPoint.x)
				{
					continue;
				}
				if (p1.y < minPoint.y && p2.y < minPoint.y && p3.y < minPoint.y)
				{
					continue;
				}
				if (p1.y > maxPoint.y && p2.y > maxPoint.y && p3.y > maxPoint.y)
				{
					continue;
				}

				bool isIntersect = false;
				int direction = 0;
				for (int j = 0; j < 4; j++)
				{
					int val = GeMath::lineDirection(rangeCoords[j], p1, p2);
					if (j == 0)
					{
						direction = val;
						continue;
					}
					if (direction != val)
					{
						isIntersect = true;
						break;
					}

					val = GeMath::lineDirection(rangeCoords[j], p1, p3);
					if (direction != val)
					{
						isIntersect = true;
						break;
					}

					val = GeMath::lineDirection(rangeCoords[j], p2, p3);
					if (direction != val)
					{
						isIntersect = true;
						break;
					}
				}

				if (isIntersect == true)
				{
					computeExtentThread->selectGeometrys.append(computeExtentThread->geometrys[i]);
				}
			}
		}
	}

	computeExtentThread->isDone.store(true);
}