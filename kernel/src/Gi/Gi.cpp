#include "Gi.h"
#include "kernel.h"
#include "GiObjectManager.h"
#include "GiObject.h"
#include "GiMaterial.h"


bool acgiOpenObject(GiObject*& pObj, const GiObjectId& id)
{
	return acgiOpenObject(pObj, id.asOldId());
}
bool acgiOpenObject(GiObject*& pObj, const unsigned id) {

	bool es = true;

	pObj = ::kernel()->acgiObjectManager()->openGiObject(id);
	if (pObj == NULL)
	{
		pObj = NULL;
		es = false;
	}
	return es;
}

bool acgiOpenObject(GiMaterial*& pObj, const GiObjectId& id)
{
	return acgiOpenObject(pObj, id.asOldId());
}
bool acgiOpenObject(GiMaterial*& pObj, const unsigned id) {
	bool es = false;
	GiObject* pObject = NULL;
	acgiOpenObject(pObject, id);
	if (pObject == NULL) {
		return es;
	}
	pObj = GiMaterial::cast(pObject);
	if (pObj != NULL) {
		es = true;
	}
	return es;
}
