#ifndef GIENTITYGEOMETRY_H
#define GIENTITYGEOMETRY_H

#include "Gi.h"
#include "RxObject.h"
#include "GePoint3d.h"
#include "GePoint2d.h"
#include "GiEntityGeometryStake.h"

class GiEntity;
class DbEntity;
class GiMaterial;
class DbEntityStake;
class GiEntityGeometry : public RxObject
{
protected:
	GiEntityGeometry();

public:
	virtual ~GiEntityGeometry();

protected:
	virtual void clone(GiEntityGeometry*& p) const;

public:
	/** 类型 */
	virtual Gi::EntityGeometryType type() = 0;

	/** clone */
	virtual GiEntityGeometry* clone() const;

	/** 归还对象池前重置基类状态 */
	void poolReset();

	/** stake */
	DbEntityStake* stake() const;

	/** 设置stake */
	void setStake(DbEntityStake* stake);

	/** 获得可见性 */
	bool visibility() const;

	/** 设置可见性 */
	void setVisibility(const bool _isVisibility);

	/** 判断是否被删除 */
	bool isErased();

	/** 删除实体 */
	void erased();

	/** 获得GiEntity */
	GiEntity* giEntity();

	/** 获得显示坐标 */
	virtual GePoint2d displayCoord(unsigned char index) const;

	/** 设置显示坐标 */
	virtual void setDisplayCoord(unsigned char index, const GePoint2d& pt) const;
};

#endif