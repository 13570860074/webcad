#ifndef GIGEOMETRYEXTENT_H
#define GIGEOMETRYEXTENT_H


#include "AcArray.h"
#include "RxObject.h"
#include "DbObjectId.h"
#include "GiGeometryExtentView.h"

/*
* 1. 用于管理一个文档中的几何对象范围
* 2. 几何对象中存储了模型空间中的dcs坐标
* 3. GiGeometryExtentView存储的是一个视口中被显示的几何对象,当视口发生变化时,都需要更新视口中的几何对象
* 4. 获得dcs范围内的几何对象
*
* */

class GePoint2d;
class GeMatrix3d;
class ApDocument;
class DbEntity;
class GiEntityGeometry;
class GiGeometryExtent : public RxObject
{
public:
	GiGeometryExtent();
	virtual ~GiGeometryExtent();
	RX_DECLARE_MEMBERS(GiGeometryExtent);


public:

	/** 获得文档对象 */
	ApDocument* document() const;

	/** 设置文档对象 */
	void setDocument(ApDocument* doc);

	/** 模型空间 */
	DbObjectId modelSpace() const;

	/** 设置模型空间 */
	void setModelSpace(const DbObjectId& modelSpace);

	/** 获得显示矩阵 */
	GeMatrix3d displayMatrix() const;

	/** 设置显示矩阵 */
	void setDisplayMatrix(const GeMatrix3d& mat);

	/** 标记刷新 */
	void refreshMatrix(const bool bRefresh = true);

	/** 判断是否被标记 */
	bool isRefreshMatrix() const;

	/** 视口表 */
	AcArray<GiGeometryExtentView*>* views();

	/** 获得视口中被显示的几何实体 */
	AcArray<GiEntityGeometry*>* geometrys(const DbObjectId& viewportId) const;

	/** 更新视口显示坐标 */
	void updateDisplayMatrix(const AcArray<GiEntityGeometry*>& targetGeometrys, const DbObjectId& _viewportId);

	/** 更新显示范围 */
	void updateDisplayExtent(const AcArray<GiEntityGeometry*>& targetGeometrys, const DbObjectId& _viewportId);

	/** 计算范围内的几何实体(0表自动,1表左选,2表右选,3表所有) */
	void extentGeometrys(
		AcArray<GiEntityGeometry*>& geometrys,						// 获得的几何对象
		const GePoint2d& startPoint,								// 显示范围开始点
		const GePoint2d& endPoint,									// 显示范围
		const DbObjectId& viewportId);								// 视口
	void extentGeometrys(
		AcArray<GiEntityGeometry*>& geometrys,						// 获得的几何对象
		const AcArray<GiEntityGeometry*>& targetGeometrys,			// 目标几何对象
		const GePoint2d& startPoint,								// 显示范围开始点
		const GePoint2d& endPoint,									// 显示范围结束点
		const DbObjectId& viewportId);								// 视口
};

#endif
