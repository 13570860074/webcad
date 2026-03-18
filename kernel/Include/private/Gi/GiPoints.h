#ifndef GIPOINTS_H
#define GIPOINTS_H


#include "GiEntity.h"
#include "GiPointGeometry.h"



class GiPoints : public GiEntity {
public:
	GiPoints();
	virtual ~GiPoints();
	RX_DECLARE_MEMBERS(GiPoints);

public:

	/** 获得类型 */
	virtual Gi::EntityType type()const;

	/** 刷新 */
	virtual void update();

	/** 获得顶点数据 */
#if EMSDK
	virtual emscripten::val vertexs();
#else
	virtual double* vertexs();
#endif

	/** 添加点几何数据 */
	void appendPointGeometry(GiPointGeometry* pointGeometry);
	GiPointGeometry* appendPointGeometry(const GePoint3d& _position);
};




#endif