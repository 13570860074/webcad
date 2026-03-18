#ifndef RERENDERER_H
#define RERENDERER_H

#include "RxObject.h"
#include "AcArray.h"

class ReCamera;
class GiEntity;
class DbObjectId;
class ReRenderer : public RxObject {
public:
	ReRenderer();
	virtual ~ReRenderer();
	RX_DECLARE_MEMBERS(ReRenderer);


public:

	// 是否是工作区
	bool isWorkArea();

	// 视口id
	DbObjectId viewport()const;

	// 设置视口
	void setViewport(const DbObjectId& id);

	// 获得相机
	ReCamera* camera();

	// 设置相机
	void setCamera(ReCamera* camera);

	// 获取实体数量
	virtual unsigned int numEntity();

#if EMSDK
	// 获得需要显示的实体
	virtual emscripten::val entityIds();
#else
	// 获得需要显示的实体
	virtual unsigned int* entityIds();
#endif
	// 设置实体
	virtual void setEntitys(AcArray<GiEntity*>* entitys);
};




#endif