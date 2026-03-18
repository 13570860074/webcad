#include "kernel.h"
#include "GiObject.h"
#include "GiImpl.h"




GiObject::~GiObject()
{

}


GiObjectId GiObject::objectId() const {
	return GI_IMP_OBJECT(this->m_pImpl)->m_id;
}
