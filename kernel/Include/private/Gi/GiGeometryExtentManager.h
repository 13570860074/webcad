#ifndef GIGEOMETRYSELECTORMANAGER_H
#define GIGEOMETRYSELECTORMANAGER_H

#include "RxManager.h"
#include "AcArray.h"
#include "GePoint3d.h"
#include "DbObjectId.h"

class DbEntity;
class GeMatrix3d;
class DbObject;
class ApDocument;
class GiEntityGeometry;
class GiGeometryExtentManager : public RxObject, public RxManager
{
public:
	GiGeometryExtentManager();
	virtual ~GiGeometryExtentManager();
	RX_DECLARE_MEMBERS(GiGeometryExtentManager);
	RX_MANAGER_DECLARE_MEMBERS(GiGeometryExtentManager);

private:

	static void onDocumentCreated(GiGeometryExtentManager* pointer, ApDocument* document);
	static void onDocumentToBeDestroyed(GiGeometryExtentManager* pointer, ApDocument* document);
	static void onBeginAnimationFrame(GiGeometryExtentManager* pointer);

	static void onMoveViewBegin(GiGeometryExtentManager* pointer, GePoint2d _mouse);
	static void onMoveViewEnd(GiGeometryExtentManager* pointer, GePoint2d _mouse);
	static void onZoomViewBegin(GiGeometryExtentManager* pointer);
	static void onZoomViewEnd(GiGeometryExtentManager* pointer);
	static void onTwistBegin(GiGeometryExtentManager* pointer);
	static void onTwistEnd(GiGeometryExtentManager* pointer);

	static void onEntityDrawEnd(GiGeometryExtentManager* pointer, DbEntity* pEntity);

	/** 获得所有需要计算的几何对象 */
	void allGeometrys(AcArray<GiEntityGeometry*>& geometrys, const DbObjectId _viewportId) const;

	/** 更新几何实体的显示坐标 */
	void updateDisplayMatrix(const DbObjectId& viewportId);

	/** 更新几何实体的显示坐标(异步) */
	void asyncUpdateDisplayMatrix(const DbObjectId& viewportId);

	/** 更新几何实体的显示范围 */
	void updateDisplayExtent(const DbObjectId& viewportId);

	/** 更新几何实体的显示范围(异步) */
	void asyncUpdateDisplayExtent(const DbObjectId& viewportId);

	/** 更新几何实体的显示矩阵和范围 */
	void updateDisplayMatrixAddExtent(const DbObjectId& viewportId);

	/** 更新几何实体的显示矩阵和范围(异步) */
	void asyncUpdateDisplayMatrixAddExtent(const DbObjectId& viewportId);

public:
	/** 初始化 */
	virtual void onInit();

	/** 是否处于锁定状态 */
	bool isLocked() const;

	/** 获得显示范围内的几何实体 */
	void extentGeometryEntitys(
		AcArray<GiEntityGeometry*>& geometrys,							// 返回的范围内几何对象
		const GePoint2d& startPoint,									// 开始坐标
		const GePoint2d& endPoint,										// 结束坐标
		const DbObjectId viewportId = NULL);							// 视口

	void extentGeometryEntitys(
		AcArray<GiEntityGeometry*>& geometrys,							// 返回的范围内几何对象
		const AcArray<GiEntityGeometry*>& targetGeometrys,				// 目标几何对象
		const GePoint2d& startPoint,									// 开始坐标
		const GePoint2d& endPoint,										// 结束坐标
		const DbObjectId viewportId = NULL);							// 视口

	/** 获得显示范围内的几何实体(异步)*/
	void asyncExtentGeometryEntitys(
		void (*fun)(AcArray<GiEntityGeometry*>&),
		const GePoint2d& startPoint, 
		const GePoint2d& endPoint,
		const DbObjectId viewportId = NULL);

	void asyncExtentGeometryEntitys(
		void (*fun)(AcArray<GiEntityGeometry*>&),
		const AcArray<GiEntityGeometry*>& targetGeometrys,
		const GePoint2d& startPoint, 
		const GePoint2d& endPoint,
		const DbObjectId viewportId = NULL);
};

#endif