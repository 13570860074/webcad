#include "Db.h"
#include "AcIterator.h"
#include "DbSymbolTableRecord.h"
#include "DbSymbolTableIterator.h"
#include "DbImpl.h"



DbSymbolTableIterator::~DbSymbolTableIterator()
{

}
void DbSymbolTableIterator::start() {
	return this->start(true, true);
}
void DbSymbolTableIterator::start(bool atBeginning) {
	return this->start(true, true);
}
void DbSymbolTableIterator::start(bool atBeginning, bool skipDeleted)
{
	if (atBeginning == true)
	{
		DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.start();
	}
	for (; !DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.done(); DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.next())
	{
		DbSymbolTableRecord* pSymbolTableRecord = DbSymbolTableRecord::cast(DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.record());
		if (pSymbolTableRecord != NULL)
		{
			if (pSymbolTableRecord->isErased() == false)
			{
				break;
			}
			if (pSymbolTableRecord->isErased() == true && skipDeleted == false)
			{
				break;
			}
		}
	}
}
bool DbSymbolTableIterator::done() const
{
	return DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.done();
}



DbObjectId DbSymbolTableIterator::getRecordId() const {
	DbObjectId id;
	this->getRecordId(id);
	return id;
}
DbSymbolTableRecord* DbSymbolTableIterator::getRecord() const {
	return this->getRecord(Db::OpenMode::kForRead, false);
}
DbSymbolTableRecord* DbSymbolTableIterator::getRecord(bool openErasedRec) const {
	return this->getRecord(Db::OpenMode::kForRead, openErasedRec);
}
DbSymbolTableRecord* DbSymbolTableIterator::getRecord(Db::OpenMode openMode, bool openErasedRec) const {
	DbSymbolTableRecord* pSymbolTableRecord = NULL;
	this->getRecord(pSymbolTableRecord, openMode, openErasedRec);
	return pSymbolTableRecord;
}



Acad::ErrorStatus DbSymbolTableIterator::getRecordId(DbObjectId& id) const
{
	DbObject *pObject = DbObject::cast(DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.record());
	if (pObject != NULL)
	{
		id = pObject->objectId();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbSymbolTableIterator::getRecord(DbSymbolTableRecord*& pRecord, Db::OpenMode openMode, bool openErasedRec) const
{
	Acad::ErrorStatus Error = Acad::ErrorStatus::eFail;

	do
	{
		pRecord = DbSymbolTableRecord::cast(DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.record());
		if (pRecord != NULL)
		{
			if (pRecord->isErased() == true && openErasedRec == false)
			{
				pRecord = NULL;
			}
			else
			{
				pRecord->subOpen(openMode);
				Error = Acad::ErrorStatus::eOk;
			}
		}
	} while (false);

	return Error;
}

void DbSymbolTableIterator::step() {
	return this->step(true, true);
}
void DbSymbolTableIterator::step(bool forward) {
	return this->step(true, true);
}
void DbSymbolTableIterator::step(bool forward, bool skipDeleted) {
	if (forward == true)
	{
		DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.next();
	}
	for (; !DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.done(); DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.next())
	{
		DbSymbolTableRecord* pSymbolTableRecord = DbSymbolTableRecord::cast(DB_IMP_SYMBOLTABLEITERATOR(this->m_pImpl)->iterator.record());
		if (pSymbolTableRecord != NULL)
		{
			if (pSymbolTableRecord->isErased() == false)
			{
				break;
			}
			if (pSymbolTableRecord->isErased() == true && skipDeleted == false)
			{
				break;
			}
		}
	}
}
