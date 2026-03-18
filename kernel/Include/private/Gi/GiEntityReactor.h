#ifndef GIENTITYREACTOR_H
#define GIENTITYREACTOR_H


#include "RxObject.h"


class GiEntity;
class GiEntityReactor : public RxObject {
public:
	RX_DECLARE_MEMBERS(GiEntityReactor);

	virtual void createEntity(GiEntity* pEntity);
	virtual void updateVertexs(GiEntity* pEntity);
	virtual void transformation(GiEntity* pEntity);
	virtual void setVisibility(GiEntity* pEntity, bool isVisibility);

#if EMSDK
public:
	GiEntityReactor();
	virtual ~GiEntityReactor();

	void onCreateEntity(int ptr);
	void onUpdateVertexs(int ptr);
	void onTransformation(int ptr);
	void onVisibility(int ptr);
#endif

};



#endif