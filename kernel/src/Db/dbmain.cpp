
#include "dbmain.h"
#include "DbObjectManager.h"
#include "kernel.h"
#include "DbObject.h"

Acad::ErrorStatus acdbGetAdsName(ads_name& objName, DbObjectId objId) {
	DbObject* obj = ::kernel()->acdbManager()->openDbObject(objId);
	if (obj == NULL) {
		return Acad::ErrorStatus::eFail;
	}
	objName[0] = objId.asOldId();
#if EMSDK
	objName[1] = (int)obj;
#else
	objName[1] = (Adesk::UInt64)obj;
#endif
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus acdbGetObjectId(DbObjectId& objId, const ads_name objName) {
	DbObject *obj = ::kernel()->acdbManager()->openDbObject(objName[0]);
	if (obj == NULL) {
		return Acad::ErrorStatus::eFail;
	}
	objId = obj->objectId();
	return Acad::ErrorStatus::eOk;
}


