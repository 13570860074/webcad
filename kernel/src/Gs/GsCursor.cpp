#include "GsCursor.h"
#include "ApDocument.h"
#include "ApDocManager.h"
#include "EvEventManager.h"
#include "GiMaterialManager.h"
#include "GiEntityManager.h"
#include "EdUserInputManager.h"
#include "arxHeaders.h"
#include "acedTrans.h"
#include "kernel.h"
#include "GsImpl.h"

GsCursor::GsCursor() {
	this->m_pImpl = new GsCursorImpl();
}
GsCursor::~GsCursor() {

}


void GsCursor::onInit() {

	GS_IMP_CURSOR(this->m_pImpl)->cursorEntity = new GiLineSegment();
	GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->setElement(Gi::ElementType::kCursor);
	GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->setMaterial(::kernel()->acgiMaterialManager()->lineSegments(7, 255, 0));
	::kernel()->acgiEntityManager()->addEntity(GS_IMP_CURSOR(this->m_pImpl)->cursorEntity);

	// 创建左选激活框
	GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh = new GiMesh();
	GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh->setElement(Gi::ElementType::kCursor);
	GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh->setMaterial(::kernel()->acgiMaterialManager()->meshBasic(5, 25));
	::kernel()->acgiEntityManager()->addEntity(GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh);

	// 创建右选激活框
	GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh = new GiMesh();
	GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh->setElement(Gi::ElementType::kCursor);
	GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh->setMaterial(::kernel()->acgiMaterialManager()->meshBasic(3, 25));
	::kernel()->acgiEntityManager()->addEntity(GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh);
}
void GsCursor::update() {

	// 清理实体
	GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->clear();
	GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh->clear();
	GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh->clear();
	if (this->isEnable() == false) {
		return;
	}

	// 获得视图点
	GePoint2d basePoint = trans_ncs_to_dcs(::kernel()->acedUserInputManager()->deviceLocation());

	double crossSize = ::trans_cursor_in_dcs_size(5.0);
	double pickbox = (crossSize * 0.2 * 0.2) * 5.0;

	// 获得视口矩阵
	GeMatrix3d viewportMatrix = GeMatrix3d::kIdentity;

	// 绘制
	GePoint3d startPoint;
	GePoint3d endPoint;
	if ((GS_IMP_CURSOR(this->m_pImpl)->style & GsCursor::ECursorStyle::kCrossCursor) == GsCursor::ECursorStyle::kCrossCursor)
	{
		startPoint.set(basePoint.x - crossSize / 2.0, basePoint.y, 0);
		endPoint.set(basePoint.x + crossSize / 2.0, basePoint.y, 0);
		startPoint.transformBy(viewportMatrix);
		endPoint.transformBy(viewportMatrix);
		GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(startPoint, endPoint);

		startPoint.set(basePoint.x, basePoint.y - crossSize / 2.0, 0);
		endPoint.set(basePoint.x, basePoint.y + crossSize / 2.0, 0);
		startPoint.transformBy(viewportMatrix);
		endPoint.transformBy(viewportMatrix);
		GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(startPoint, endPoint);
	}
	if ((GS_IMP_CURSOR(this->m_pImpl)->style & GsCursor::ECursorStyle::kTarget) == GsCursor::ECursorStyle::kTarget)
	{
		startPoint.set(basePoint.x - pickbox / 2.0, basePoint.y - pickbox / 2.0, 0);
		endPoint.set(basePoint.x + pickbox / 2.0, basePoint.y - pickbox / 2.0, 0);
		startPoint.transformBy(viewportMatrix);
		endPoint.transformBy(viewportMatrix);
		GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(startPoint, endPoint);

		startPoint.set(basePoint.x + pickbox / 2.0, basePoint.y - pickbox / 2.0, 0);
		endPoint.set(basePoint.x + pickbox / 2.0, basePoint.y + pickbox / 2.0, 0);
		startPoint.transformBy(viewportMatrix);
		endPoint.transformBy(viewportMatrix);
		GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(startPoint, endPoint);

		startPoint.set(basePoint.x + pickbox / 2.0, basePoint.y + pickbox / 2.0, 0);
		endPoint.set(basePoint.x - pickbox / 2.0, basePoint.y + pickbox / 2.0, 0);
		startPoint.transformBy(viewportMatrix);
		endPoint.transformBy(viewportMatrix);
		GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(startPoint, endPoint);

		startPoint.set(basePoint.x - pickbox / 2.0, basePoint.y + pickbox / 2.0, 0);
		endPoint.set(basePoint.x - pickbox / 2.0, basePoint.y - pickbox / 2.0, 0);
		startPoint.transformBy(viewportMatrix);
		endPoint.transformBy(viewportMatrix);
		GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(startPoint, endPoint);
	}
	if ((GS_IMP_CURSOR(this->m_pImpl)->style & GsCursor::ECursorStyle::kSelectionSet) == GsCursor::ECursorStyle::kSelectionSet ||
		(GS_IMP_CURSOR(this->m_pImpl)->style & GsCursor::ECursorStyle::kCorner) == GsCursor::ECursorStyle::kCorner) {

		resbuf res;
		acedGetVar("LASTPOINT", &res);

		GePoint2d dcsPoint1 = ::trans_ucs_to_dcs(GePoint3d(res.resval.rpoint[0], res.resval.rpoint[1], res.resval.rpoint[2]));
		GePoint2d dcsPoint2 = ::trans_ncs_to_dcs(::kernel()->acedUserInputManager()->deviceLocation());

		GePlane projection(GePoint3d::kOrigin, GeVector3d::kZAxis);
		DbObject* activeViewport = NULL;
		if (::acdbOpenObject(activeViewport, acedActiveViewportId()) != Acad::ErrorStatus::eOk)
		{
			if (activeViewport->isA() == DbViewportTableRecord::desc()) {
				projection.set(GePoint3d::kOrigin, ((DbViewportTableRecord*)activeViewport)->viewDirection());
			}
			else if (activeViewport->isA() == DbViewport::desc()) {
				projection.set(GePoint3d::kOrigin, ((DbViewport*)activeViewport)->viewDirection());
			}
			activeViewport->close();
		}

		GePoint3d point1, point2, point3;

		if ((GS_IMP_CURSOR(this->m_pImpl)->style & GsCursor::ECursorStyle::kSelectionSet) == GsCursor::ECursorStyle::kSelectionSet)
		{
			point1 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint1.y));
			point2 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint1.y));
			point3 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint2.y));
			point1.orthoProject(projection);
			point2.orthoProject(projection);
			point3.orthoProject(projection);
			if (dcsPoint1.x < dcsPoint2.x) {
				GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh->appendTriangleMeshGeometry(point1, point2, point3);
			}
			else {
				GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh->appendTriangleMeshGeometry(point1, point2, point3);
			}

			point1 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint2.y));
			point2 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint2.y));
			point3 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint1.y));
			point1.orthoProject(projection);
			point2.orthoProject(projection);
			point3.orthoProject(projection);
			if (dcsPoint1.x < dcsPoint2.x) {
				GS_IMP_CURSOR(this->m_pImpl)->leftSelectionSetMesh->appendTriangleMeshGeometry(point1, point2, point3);
			}
			else {
				GS_IMP_CURSOR(this->m_pImpl)->rightSelectionSetMesh->appendTriangleMeshGeometry(point1, point2, point3);
			}
		}
		if ((GS_IMP_CURSOR(this->m_pImpl)->style & GsCursor::ECursorStyle::kCorner) == GsCursor::ECursorStyle::kCorner)
		{
			point1 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint1.y));
			point2 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint1.y));
			point1.orthoProject(projection);
			point2.orthoProject(projection);
			GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(point1, point2);

			point1 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint1.y));
			point2 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint2.y));
			point1.orthoProject(projection);
			point2.orthoProject(projection);
			GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(point1, point2);

			point1 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint2.x, dcsPoint2.y));
			point2 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint2.y));
			point1.orthoProject(projection);
			point2.orthoProject(projection);
			GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(point1, point2);

			point1 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint2.y));
			point2 = ::trans_dcs_to_wcs(GePoint2d(dcsPoint1.x, dcsPoint1.y));
			point1.orthoProject(projection);
			point2.orthoProject(projection);
			GS_IMP_CURSOR(this->m_pImpl)->cursorEntity->appendLineSegment(point1, point2);
		}
	}
}
bool GsCursor::isEnable()const {
	return GS_IMP_CURSOR(this->m_pImpl)->bEnable;
}
void GsCursor::setEnable(const bool bEnable) {
	GS_IMP_CURSOR(this->m_pImpl)->bEnable = bEnable;
}
GsCursor::ECursorStyle GsCursor::cursorStyle() const {
	return GS_IMP_CURSOR(this->m_pImpl)->style;
}
void GsCursor::setCursorStyle(const GsCursor::ECursorStyle _style) {
	GS_IMP_CURSOR(this->m_pImpl)->style = _style;
}
