#include "Db.h"
#include "DbDictionaryRecord.h"
#include "DbDictionaryIterator.h"
#include "DbImpl.h"


DbDictionaryIterator::DbDictionaryIterator()
{
	this->m_pImpl = new DbDictionaryIteratorImpl();
}
DbDictionaryIterator::~DbDictionaryIterator() {

}
DbObjectId DbDictionaryIterator::objectId() const {
	DbDictionaryRecord *object = DbDictionaryRecord::cast(DB_IMP_DICTIONARYITERATOR(this->m_pImpl)->iterator.record());
	return object->objectId;
}
std::string DbDictionaryIterator::name() const {
	DbDictionaryRecord* object = DbDictionaryRecord::cast(DB_IMP_DICTIONARYITERATOR(this->m_pImpl)->iterator.record());
	return object->name.constPtr();
}
bool DbDictionaryIterator::name(AcString& sName) const {
	sName = this->name();
	return true;
}
bool DbDictionaryIterator::done() const
{
	return DB_IMP_DICTIONARYITERATOR(this->m_pImpl)->iterator.done();
}
bool DbDictionaryIterator::next() {
	DB_IMP_DICTIONARYITERATOR(this->m_pImpl)->iterator.next();
	return this->done();
}