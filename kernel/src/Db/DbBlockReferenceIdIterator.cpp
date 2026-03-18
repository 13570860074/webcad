#include "DbBlockReferenceIdIterator.h"



DbBlockReferenceIdIterator::DbBlockReferenceIdIterator(DbObjectIdArray _Ids) {
	this->mPos = 0;
	this->mAry = _Ids;
}
DbBlockReferenceIdIterator::~DbBlockReferenceIdIterator() {

}

bool DbBlockReferenceIdIterator::done() const {
	if (this->mPos >= this->mAry.length())
	{
		return true;
	}
	return false;
}

Acad::ErrorStatus DbBlockReferenceIdIterator::getBlockReferenceId(DbObjectId& id) const {
	if (this->done() == true)
	{
		return Acad::ErrorStatus::eFail;
	}

	id = this->mAry.at(this->mPos);

	return Acad::ErrorStatus::eOk;
}

Acad::ErrorStatus DbBlockReferenceIdIterator::getBlockReference(DbBlockReference*& pBlkRef, Db::OpenMode openMode, bool openErasedEntity) const
{
	return Acad::ErrorStatus::eOk;
}
