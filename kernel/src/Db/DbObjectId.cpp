#include "DbHandle.h"
#include "DbObject.h"
//#include "DbDatabase.h"
#include "DbObjectId.h"



const DbObjectId DbObjectId::kNull = DbObjectId();

bool DbObjectId::isValid() const
{
	DbObject* pObject = NULL;
	//::acdbOpenObject(pObject, *this, Db::OpenMode::kForRead, true);
	if (pObject != NULL)
	{
		if (pObject->isErased() == false)
		{
			return true;
		}
	}
	return false;
}
bool DbObjectId::operator < (const DbObjectId& objectId) const
{
	if (this->asOldId() < objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool DbObjectId::operator > (const DbObjectId& objectId) const
{
	if (this->asOldId() > objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool DbObjectId::operator >= (const DbObjectId& objectId) const
{
	if (this->asOldId() >= objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool DbObjectId::operator <= (const DbObjectId& objectId) const
{
	if (this->asOldId() <= objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool DbObjectId::operator == (const DbObjectId& objectId) const
{
	if (this->asOldId() == objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool DbObjectId::operator != (const DbObjectId& objectId) const
{
	if (this->asOldId() != objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool DbObjectId::isErased() const
{
	DbObject* pObject = NULL;
	//::acdbOpenObject(pObject, *this, Db::OpenMode::kForRead, true);
	if (pObject != NULL)
	{
		return pObject->isErased();
	}
	return false;
}
DbHandle DbObjectId::handle() const
{
	DbObject* pObject = NULL;
	//::acdbOpenObject(pObject, *this, Db::OpenMode::kForRead, true);
	if (pObject != NULL)
	{
		DbHandle handle;
		pObject->getDbHandle(handle);
		return handle;
	}
	return DbHandle();
}