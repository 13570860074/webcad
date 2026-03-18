#ifndef GIGEOMETRYEXTENTVIEW_H
#define GIGEOMETRYEXTENTVIEW_H


#include "AcArray.h"
#include "GePoint2d.h"
#include "DbObjectId.h"
#include "GiEntityGeometry.h"

class GiGeometryExtentView
{
public:
	GiGeometryExtentView();
	virtual ~GiGeometryExtentView();

private:
	DbObjectId m_blockId;
	DbObjectId m_viewportId;
	bool m_bRefresh;
	double m_viewHeight;
	GePoint2d m_viewCenter;
	AcArray<GiEntityGeometry*> m_geometrys;

public:

	/** 块ID */
	DbObjectId blockId()const;

	/** 设置块ID */
	void setBlockId(const DbObjectId& id);

	/** 视口ID */
	DbObjectId viewportId()const;

	/** 设置视口ID */
	void setViewportId(const DbObjectId& id);

	/** 视口中心点 */
	GePoint2d viewCenter()const;

	/** 设置视口中心点 */
	void setViewCenter(const GePoint2d& id);

	/** 视口高度 */
	double viewHeight()const;

	/** 设置视口高度 */
	void setViewHeight(const double id);

	/** 是否被标记刷新 */
	bool isRefresh()const;

	/** 标记刷新 */
	void markRefresh(const bool bRefresh = true);

	/** 获取几何体数组 */
	AcArray<GiEntityGeometry*>* geometrys();
};


#endif