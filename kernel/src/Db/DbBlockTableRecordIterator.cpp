#include "DbObjectId.h"
#include "DbEntity.h"
#include "DbBlockTableRecordIterator.h"
#include "DbImpl.h"


DbBlockTableRecordIterator::DbBlockTableRecordIterator() {
	this->m_pImpl = new DbBlockTableRecordIteratorImpl();
}
DbBlockTableRecordIterator::~DbBlockTableRecordIterator() {

}

void DbBlockTableRecordIterator::start(bool atBeginning, bool skipDeleted) {
	if (atBeginning == true)
	{
		DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.start();
	}
	for (; !DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.done(); DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.next())
	{
		DbEntity* pEntity = DbEntity::cast(DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.record());
		if (pEntity != NULL)
		{
			if (pEntity->isErased() == false)
			{
				break;
			}
			if (pEntity->isErased() == true && skipDeleted == false)
			{
				break;
			}
		}
	}
}

bool DbBlockTableRecordIterator::done() const {
	return DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.done();
}

Acad::ErrorStatus DbBlockTableRecordIterator::getEntityId(DbObjectId& entityId) const {
	DbEntity* pObject = DbEntity::cast(DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.record());
	if (pObject != NULL)
	{
		entityId = pObject->objectId();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbBlockTableRecordIterator::getEntity(DbEntity*& pEntity, Db::OpenMode openMode, bool openErasedEntity) const {
	Acad::ErrorStatus Error = Acad::ErrorStatus::eFail;

	do
	{
		pEntity = DbEntity::cast(DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.record());
		if (pEntity != NULL)
		{
			if (pEntity->isErased() == true && openErasedEntity == false)
			{
				pEntity = NULL;
			}
			else
			{
				pEntity->subOpen(openMode);
				Error = Acad::ErrorStatus::eOk;
			}
		}
	} while (false);

	return Error;
}

DbBlockTableRecord* DbBlockTableRecordIterator::blockTableRecord() const {
	return DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->blockTableRecord;
}

void DbBlockTableRecordIterator::step(bool forward, bool skipDeleted) {
	if (forward == true)
	{
		DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.next();
	}
	for (; !DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.done(); DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.next())
	{
		DbEntity* pEntity = DbEntity::cast(DB_IMP_BLOCKTABLERECORDITERATOR(this->m_pImpl)->iterator.record());
		if (pEntity != NULL)
		{
			if (pEntity->isErased() == false)
			{
				break;
			}
			if (pEntity->isErased() == true && skipDeleted == false)
			{
				break;
			}
		}
	}
}
