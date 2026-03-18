#include "GiObjectId.h"



bool GiObjectId::isValid() const
{
	if (this->m_Id > 0)
	{
		return true;
	}
	return false;
}
bool GiObjectId::operator < (const GiObjectId& objectId) const
{
	if (this->asOldId() < objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool GiObjectId::operator > (const GiObjectId& objectId) const
{
	if (this->asOldId() > objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool GiObjectId::operator >= (const GiObjectId& objectId) const
{
	if (this->asOldId() >= objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool GiObjectId::operator <= (const GiObjectId& objectId) const
{
	if (this->asOldId() <= objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool GiObjectId::operator == (const GiObjectId& objectId) const
{
	if (this->asOldId() == objectId.asOldId())
	{
		return true;
	}
	return false;
}
bool GiObjectId::operator != (const GiObjectId& objectId) const
{
	if (this->asOldId() != objectId.asOldId())
	{
		return true;
	}
	return false;
}
