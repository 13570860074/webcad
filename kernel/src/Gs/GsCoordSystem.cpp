#include "GsCoordSystem.h"
#include "ApDocument.h"
#include "ApDocManager.h"
#include "DbObject.h"
#include "DbViewport.h"
#include "DbViewportTableRecord.h"
#include "EvEventManager.h"
#include "GiMaterialManager.h"
#include "GiEntityManager.h"
#include "EdUserInputManager.h"
#include "acedTrans.h"
#include "kernel.h"
#include "aced.h"
#include "DbImpl.h"
#include "GsImpl.h"

GsCoordSystem::GsCoordSystem() {
	this->m_pImpl = new GsCoordSystemImpl();
}
GsCoordSystem::~GsCoordSystem() {

}


void GsCoordSystem::onInit() {


	// 创建模型空间中的坐标系
	GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity = new GiLineSegment();
	GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->setElement(Gi::ElementType::kCoordSystem);
	GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->setMaterial(::kernel()->acgiMaterialManager()->lineSegments(7, 255, 0));
	::kernel()->acgiEntityManager()->addEntity(GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity);

	// 创建图纸空间中的坐标系
	GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity = new GiLineSegment();
	GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->setElement(Gi::ElementType::kCoordSystem);
	GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->setMaterial(::kernel()->acgiMaterialManager()->lineSegments(160, 255, 0));
	::kernel()->acgiEntityManager()->addEntity(GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity);
}
bool GsCoordSystem::extents(const DbObject* viewport) const {

	//// 获得视口的坐标系数据
	//GePoint3d origin;
	//GeVector3d xAxis = GeVector3d::kXAxis;
	//GeVector3d yAxis = GeVector3d::kYAxis;
	//GeVector3d zAxis = GeVector3d::kZAxis;
	//if (viewport->isA() == DbViewportTableRecord::desc()) {
	//	DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)viewport;
	//	pViewportTableRecord->getUcs(origin, xAxis, yAxis);
	//	zAxis = xAxis.crossProduct(yAxis);
	//}
	//else if (viewport->isA() == DbViewport::desc()) {
	//	DbViewport* pViewport = (DbViewport*)viewport;
	//	pViewport->getUcs(origin, xAxis, yAxis);
	//	zAxis = xAxis.crossProduct(yAxis);
	//}

	//// 获得1个像素尺寸
	//double pixelSize = ::trans_pixel_in_dcs_size(1);

	//GePoint3d p1, p2, p3;
	//GePoint3d endPoint;
	//p1 = (origin + xAxis * GS_IMP_COORDSYSTEM(this->m_pImpl)->size * pixelSize);
	//p2 = (origin + yAxis * GS_IMP_COORDSYSTEM(this->m_pImpl)->size * pixelSize);
	//p3 = (origin + yAxis * GS_IMP_COORDSYSTEM(this->m_pImpl)->size * pixelSize);

	//GePoint2d dcsP1 = ::trans_wcs_to_dcs(p1);
	//GePoint2d dcsP2 = ::trans_wcs_to_dcs(p2);
	//GePoint2d dcsP3 = ::trans_wcs_to_dcs(p3);
	//GePoint2d minPoint = dcsP1;
	//GePoint2d maxPoint = dcsP2;
	//if (dcsP2.x < minPoint.x) {
	//	minPoint.x = dcsP2.x;
	//}
	//if (dcsP3.x < minPoint.x) {
	//	minPoint.x = dcsP3.x;
	//}
	//if (dcsP2.y < minPoint.y) {
	//	minPoint.x = dcsP2.x;
	//}
	//if (dcsP3.y < minPoint.y) {
	//	minPoint.y = dcsP3.y;
	//}
	//if (dcsP2.x > maxPoint.x) {
	//	maxPoint.x = dcsP2.x;
	//}
	//if (dcsP3.x > maxPoint.x) {
	//	maxPoint.x = dcsP3.x;
	//}
	//if (dcsP2.y > maxPoint.y) {
	//	maxPoint.x = dcsP2.x;
	//}
	//if (dcsP3.y > maxPoint.y) {
	//	maxPoint.y = dcsP3.y;
	//}

	//// 获得当前可视范围(dcs)
	//GePoint2d dcsLowerLeftCorner = ::trans_vcs_to_dcs(GePoint2d(0.0, 0.0), viewport->objectId());
	//GePoint2d dcsUpperRightCorner = ::trans_vcs_to_dcs(GePoint2d(1.0, 1.0), viewport->objectId());
	//double determineVal = (dcsUpperRightCorner.y - dcsLowerLeftCorner.y) / 20.0;

	//if (minPoint.x < dcsLowerLeftCorner.x + determineVal) {
	//	return false;
	//}
	//if (maxPoint.x > dcsUpperRightCorner.x - determineVal) {
	//	return false;
	//}
	//if (minPoint.y < dcsLowerLeftCorner.y + determineVal) {
	//	return false;
	//}
	//if (maxPoint.y > dcsUpperRightCorner.y - determineVal) {
	//	return false;
	//}
	return true;
}
void GsCoordSystem::update() {

	// 获得当前被激活视口
	DbObjectId activeViewportId = acedActiveViewportId();

	// 打开视口
	DbObject* pActiveViewport = NULL;
	if (::acdbOpenObject(pActiveViewport, activeViewportId) != Acad::ErrorStatus::eOk)
	{
		return;
	}

	// 清理实体
	GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->clear();
	GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->clear();

	// 获得坐标系数据
	GePoint3d origin;
	GeVector3d xAxis = GeVector3d::kXAxis;
	GeVector3d yAxis = GeVector3d::kYAxis;
	GeVector3d zAxis = GeVector3d::kZAxis;
	if (GS_IMP_COORDSYSTEM(this->m_pImpl)->coordSys == NULL) {

		DbObject* viewport = NULL;
		if (::acdbOpenObject(viewport, acedActiveViewportId()) == Acad::ErrorStatus::eOk) {
			if (viewport->isA() == DbViewportTableRecord::desc()) {
				DbViewportTableRecord* pViewportTableRecord = (DbViewportTableRecord*)viewport;
				pViewportTableRecord->getUcs(origin, xAxis, yAxis);
				zAxis = xAxis.crossProduct(yAxis);
			}
			else if (viewport->isA() == DbViewport::desc()) {
				DbViewport* pViewport = (DbViewport*)viewport;
				pViewport->getUcs(origin, xAxis, yAxis);
				zAxis = xAxis.crossProduct(yAxis);
			}
			viewport->close();
		}
	}
	else {
		origin = GS_IMP_COORDSYSTEM(this->m_pImpl)->coordSys->origin();
		xAxis = GS_IMP_COORDSYSTEM(this->m_pImpl)->coordSys->xAxis();
		yAxis = GS_IMP_COORDSYSTEM(this->m_pImpl)->coordSys->yAxis();
		zAxis = xAxis.crossProduct(yAxis);
	}

	// 获得视口高度
	double viewHeight = 0.0;
	if (DB_IMP_OBJECT(pActiveViewport->m_pImpl)->isViewportTableRecord() == true) {
		viewHeight = ((DbViewportTableRecord*)pActiveViewport)->height();
	}
	else if (DB_IMP_OBJECT(pActiveViewport->m_pImpl)->isViewport() == true) {
		viewHeight = ((DbViewport*)pActiveViewport)->viewHeight();
	}

	// 获得视图方向
	GeVector3d viewDir = GeVector3d::kZAxis;
	GeMatrix3d viewMatrix = ::trans_dcs_mat();
	viewDir.transformBy(viewMatrix);

	// 判断是否是模型空间
	bool isModelSpace = false;
	if (DB_IMP_OBJECT(pActiveViewport->m_pImpl)->isViewportTableRecord() == true) {
		isModelSpace = true;
	}
	else if (DB_IMP_OBJECT(pActiveViewport->m_pImpl)->isViewport() == true) {
		DbViewport* pViewport = (DbViewport*)pActiveViewport;
		if (pViewport->number() == 1) {
			isModelSpace = false;
		}
		else {
			isModelSpace = true;
		}
	}

	GePoint3d startPoint;
	GePoint3d endPoint;

	// 创建模型空间中的坐标系
	if (isModelSpace == true) {

		// 获得尺寸
		double axialLength = viewHeight * 0.045;
		double targetSize = viewHeight * 0.01;
		double textSize = viewHeight * 0.012;

		// 如果坐标系在范围外且开启了固定左下角则设置原点为左下角
		if (this->extents(pActiveViewport) == false && GS_IMP_COORDSYSTEM(this->m_pImpl)->isFixedLowerLeftCorner == true) {
			GePoint2d dcsOrigin = ::trans_vcs_to_dcs(GePoint2d(1.0 / 20.0 / ::kernel()->acedUserInputManager()->screenAspect(), 1.0 / 20.0), pActiveViewport->objectId());
			origin = ::wcs_to_viewport_projection(::trans_dcs_to_wcs(dcsOrigin));
		}

		// 创建原点方形框
		if (this->isDrag() == false) {

			GeVector3d relativeXAxis = xAxis;
			GeVector3d relativeYAxis = yAxis;

			startPoint = GePoint3d(origin.x, origin.y, origin.z);
			startPoint -= relativeXAxis * targetSize / 2.0;
			startPoint -= relativeYAxis * targetSize / 2.0;
			endPoint = GePoint3d(origin.x, origin.y, origin.z);
			endPoint += relativeXAxis * targetSize / 2.0;
			endPoint -= relativeYAxis * targetSize / 2.0;
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = GePoint3d(origin.x, origin.y, origin.z);
			startPoint += relativeXAxis * targetSize / 2.0;
			startPoint -= relativeYAxis * targetSize / 2.0;
			endPoint = GePoint3d(origin.x, origin.y, origin.z);
			endPoint += relativeXAxis * targetSize / 2.0;
			endPoint += relativeYAxis * targetSize / 2.0;
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = GePoint3d(origin.x, origin.y, origin.z);
			startPoint += relativeXAxis * targetSize / 2.0;
			startPoint += relativeYAxis * targetSize / 2.0;
			endPoint = GePoint3d(origin.x, origin.y, origin.z);
			endPoint -= relativeXAxis * targetSize / 2.0;
			endPoint += relativeYAxis * targetSize / 2.0;
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = GePoint3d(origin.x, origin.y, origin.z);
			startPoint -= relativeXAxis * targetSize / 2.0;
			startPoint += relativeYAxis * targetSize / 2.0;
			endPoint = GePoint3d(origin.x, origin.y, origin.z);
			endPoint -= relativeXAxis * targetSize / 2.0;
			endPoint -= relativeYAxis * targetSize / 2.0;
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);
		}

		// 创建文字'X'
		if (viewDir.isParallelTo(xAxis) == false)
		{
			GePoint3d tempPoint = GePoint3d(origin.x, origin.y, origin.z);
			tempPoint += xAxis * axialLength;

			GePoint2d dcsPoint = ::trans_wcs_to_dcs(tempPoint);
			dcsPoint.x += textSize / 2.0;
			dcsPoint.y += textSize / 5.0;

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x, dcsPoint.y));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize, dcsPoint.y + textSize));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x, dcsPoint.y + textSize));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize, dcsPoint.y));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);
		}

		// 创建文字'Y'
		if (viewDir.isParallelTo(yAxis) == false) {

			GePoint3d tempPoint = GePoint3d(origin.x, origin.y, origin.z);
			tempPoint += yAxis * axialLength;

			GePoint2d dcsPoint = ::trans_wcs_to_dcs(tempPoint);

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize / 2.0, dcsPoint.y));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize / 2.0, dcsPoint.y + textSize / 2.0));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize / 2.0, dcsPoint.y + textSize / 2.0));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x, dcsPoint.y + textSize));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize / 2.0, dcsPoint.y + textSize / 2.0));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize, dcsPoint.y + textSize));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);
		}

		// 创建文字'Z'
		if (viewDir.isParallelTo(zAxis, GeTol(0.0001)) == false) {

			GePoint3d tempPoint = GePoint3d(origin.x, origin.y, origin.z);
			tempPoint += zAxis * axialLength;

			GePoint2d dcsPoint = ::trans_wcs_to_dcs(tempPoint);
			dcsPoint.x += textSize * 0.2;

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x, dcsPoint.y));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize, dcsPoint.y));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x, dcsPoint.y));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize, dcsPoint.y + textSize));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

			startPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x, dcsPoint.y + textSize));
			endPoint = ::trans_dcs_to_wcs(GePoint2d(dcsPoint.x + textSize, dcsPoint.y + textSize));
			GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);
		}

		// 创建X轴
		startPoint = GePoint3d(origin.x, origin.y, origin.z);
		endPoint = startPoint;
		endPoint += xAxis * axialLength;
		GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

		// 创建Y轴
		startPoint = GePoint3d(origin.x, origin.y, origin.z);
		endPoint = startPoint;
		endPoint += yAxis * axialLength;
		GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);

		// 创建Z轴
		startPoint = GePoint3d(origin.x, origin.y, origin.z);
		endPoint = startPoint;
		endPoint += zAxis * axialLength;
		GS_IMP_COORDSYSTEM(this->m_pImpl)->ordinaryCoorSysEntity->appendLineSegment(startPoint, endPoint);
	}
	// 创建图纸空间中的坐标系
	else if (isModelSpace == false) {

		// 获得尺寸
		double axialLengthX = viewHeight * 0.045;
		double axialLengthY = viewHeight * 0.075;
		double targetSize = viewHeight * 0.01;
		double textSize = viewHeight * 0.012;

		GePoint2d dcsPoint = ::trans_ncs_to_dcs(GePoint2d(0, 0));
		origin = GePoint3d(dcsPoint.x, dcsPoint.y, 0);
		origin.x += viewHeight * 0.02;
		origin.y += viewHeight * 0.02;

		startPoint = GePoint3d(origin.x, origin.y, origin.z);
		endPoint = GePoint3d(origin.x + axialLengthX, origin.y, origin.z);
		GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->appendLineSegment(startPoint, endPoint);

		startPoint = GePoint3d(origin.x, origin.y, origin.z);
		endPoint = GePoint3d(origin.x, origin.y + axialLengthY, origin.z);
		GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->appendLineSegment(startPoint, endPoint);

		startPoint = GePoint3d(origin.x + axialLengthX, origin.y, origin.z);
		endPoint = GePoint3d(origin.x, origin.y + axialLengthY, origin.z);
		GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->appendLineSegment(startPoint, endPoint);

		startPoint = GePoint3d(origin.x + targetSize, origin.y, origin.z);
		endPoint = GePoint3d(origin.x + targetSize, origin.y + targetSize, origin.z);
		GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->appendLineSegment(startPoint, endPoint);

		startPoint = GePoint3d(origin.x, origin.y + targetSize, origin.z);
		endPoint = GePoint3d(origin.x + targetSize, origin.y + targetSize, origin.z);
		GS_IMP_COORDSYSTEM(this->m_pImpl)->layoutCoorSysEntity->appendLineSegment(startPoint, endPoint);
	}

	// 关闭视口
	pActiveViewport->close();
}
bool GsCoordSystem::isDrag() const {
	return GS_IMP_COORDSYSTEM(this->m_pImpl)->isDrag;
}
void GsCoordSystem::setDrag(const bool isDrag) {
	GS_IMP_COORDSYSTEM(this->m_pImpl)->isDrag = isDrag;
}
GsCoordSystem::ECoordSystemStyle GsCoordSystem::coordSystemStyle() const {
	return GS_IMP_COORDSYSTEM(this->m_pImpl)->style;
}
void GsCoordSystem::setCoordSystemStyle(const GsCoordSystem::ECoordSystemStyle _style) {
	GS_IMP_COORDSYSTEM(this->m_pImpl)->style = _style;
}
