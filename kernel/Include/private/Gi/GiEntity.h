#ifndef GIENTITY_H
#define GIENTITY_H

#include "Gi.h"
#include "AcArray.h"
#include "GiObject.h"
#include "GeMatrix3d.h"
#include "GeQuaternion.h"
#include "GiEntityGeometry.h"

/*
#define maxLineSegment 10000
#define maxLineSegmentVertex maxLineSegment * 2
#define maxLineSegmentVertexBuffer maxLineSegmentVertex * 3

#define maxTriangleMesh 2000
#define maxTriangleMeshVertex maxTriangleMesh * 3
#define maxTriangleMeshVertexBuffer maxTriangleMeshVertex * 3
*/

class ApDocument;
class GiEntity : public GiObject
{
protected:
	GiEntity();

public:
	virtual ~GiEntity();
	RX_DECLARE_MEMBERS(GiEntity);

public:

	/** 获取实体的类型 */
	virtual Gi::EntityType type() const;

	/** 清理实体 */
	virtual void clear();

	/** 更新实体 */
	virtual void update();

	/** 几何对象数量 */
	virtual unsigned int numGeometrys() const;

	/** 获得几何对象 */
	virtual const AcArray<GiEntityGeometry *> &geometrys();

	/** 顶点数据float64长度(调用update方法后生效) */
	virtual unsigned int vertexLength() const;

	/** 获取实体的顶点数组 */
#if EMSDK
	virtual emscripten::val vertexs();
#else
	virtual double *vertexs();
#endif

	/** 获取实体的元素类型 */
	Gi::ElementType element() const;

	/** 设置实体的元素类型 */
	void setElement(const Gi::ElementType _style);

	/** 获取实体的可见性 */
	bool visibility() const;

	/** 设置实体的可见性 */
	void setVisibility(const bool _isVisibility);

	/** 获取实体的材质 */
	GiMaterial *material() const;

	/** 设置实体的材质 */
	void setMaterial(const GiMaterial* material);

	/** 获取实体的矩阵 */
	GeMatrix3d matrix() const;

	/** 设置实体的矩阵 */
	void setMatrix(const GeMatrix3d &_matrix);

	/** 矩阵相加 */
	GeMatrix3d &applyMatrix(const GeMatrix3d &_matrix) const;

	/** 获取实体的位置 */
	GePoint3d position() const;

	/** 设置实体的位置 */
	void setPosition(const GePoint3d &point);

	/** 获得四元数 */
	GeQuaternion quaternion() const;
};

#endif