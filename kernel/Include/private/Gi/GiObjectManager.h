#ifndef GIMGD_H
#define GIMGD_H


#include "AcArray.h"
#include "RxManager.h"

class GiObject;
class GiObjectId;
class GiObjectManager : public RxObject, public RxManager
{
public:
	GiObjectManager();
	virtual ~GiObjectManager();
	RX_DECLARE_MEMBERS(GiObjectManager);
	RX_MANAGER_DECLARE_MEMBERS(GiObjectManager);

public:
	/* 添加对象 */
	GiObjectId addGiObject(GiObject* _pObject);

	/* 打开对象 */
	GiObject* openGiObject(const unsigned id);
	GiObject* openGiObject(const GiObjectId& objectId);


private:
	unsigned int OverallId;
	AcArray<GiObject*>* pObjects;
};



#endif

