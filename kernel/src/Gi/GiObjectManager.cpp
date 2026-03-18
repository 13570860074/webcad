#include "GiObjectManager.h"
#include "GiObject.h"
#include "GiObjectId.h"
#include "GiImpl.h"



GiObjectManager::GiObjectManager() {
	this->OverallId = 0;
	this->pObjects = new AcArray<GiObject*>();
}
GiObjectManager::~GiObjectManager() {
	delete this->pObjects;
}


GiObjectId GiObjectManager::addGiObject(GiObject* _pObject) {

	if (_pObject == NULL) {
		return NULL;
	}

	this->OverallId++;
	GI_IMP_OBJECT(_pObject->m_pImpl)->m_id.setFromOldId(this->OverallId);

	this->pObjects->append(_pObject);

	return _pObject->objectId();
}
GiObject* GiObjectManager::openGiObject(const GiObjectId& objectId)
{
	return this->openGiObject(objectId.asOldId());
}
GiObject* GiObjectManager::openGiObject(const unsigned id) {
	if (id == NULL) {
		return NULL;
	}
	if (id > this->pObjects->length()) {
		return NULL;
	}
	return this->pObjects->at(id - 1);
}