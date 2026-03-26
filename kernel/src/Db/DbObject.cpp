#include "DbObject.h"
#include "DbHandle.h"
#include "ApDocManager.h"
#include "ReRendererManager.h"
#include "RxObject.h"
#include "kernel.h"
#include "DbDatabaseImpl.h"
#include "DbImpl.h"
#include "EdImpl.h"

DbObject::~DbObject()
{
}

DbObjectId DbObject::objectId() const
{
	return DB_IMP_OBJECT(this->m_pImpl)->objectId;
}

DbObjectId DbObject::ownerId() const
{
	return DB_IMP_OBJECT(this->m_pImpl)->ownerId;
}
Acad::ErrorStatus DbObject::setOwnerId(DbObjectId objId) {
	DB_IMP_OBJECT(this->m_pImpl)->ownerId = objId;
	return Acad::ErrorStatus::eOk;
}
void DbObject::getDbHandle(DbHandle& handle) const {
	handle = DB_IMP_OBJECT(this->m_pImpl)->handle;
}
DbDatabase* DbObject::database() const {
	return DB_IMP_OBJECT(this->m_pImpl)->database;
}



bool DbObject::subOpen(Db::OpenMode mode)
{
	bool isDatabaseReactor = false;
	if (DB_IMP_OBJECT(this->m_pImpl)->mode == Db::OpenMode::kNotOpen ||
		DB_IMP_OBJECT(this->m_pImpl)->mode == Db::OpenMode::kForRead)
	{
		if (mode == Db::OpenMode::kForWrite || mode == Db::OpenMode::kForNotify)
		{
			isDatabaseReactor = true;
		}
	}
	DB_IMP_OBJECT(this->m_pImpl)->mode = mode;
	if (isDatabaseReactor == true && this->database() != NULL)
	{
		if (mode == Db::OpenMode::kForWrite || mode == Db::OpenMode::kForNotify)
		{
			const AcArray<DbDatabaseReactor*>& reactors = this->database()->pImpl->reactors;
			for (int i = 0; i < reactors.length(); i++)
			{
				reactors.at(i)->objectOpenedForModify(this->database(), this);
			}
		}
	}

	return true;
}

void DbObject::close()
{
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		/* 触发数据库反应器修改对象事件 */
		if (this->database() != NULL) {
			const AcArray<DbDatabaseReactor*>& reactors = ((DbDatabaseImpl*)this->database()->pImpl)->reactors;
			for (int i = 0; i < reactors.length(); i++) {
				reactors.at(i)->objectModified(this->database(), this);
			}
		}
	}

	DB_IMP_OBJECT(this->m_pImpl)->mode = Db::OpenMode::kNotOpen;
}
bool DbObject::subClose()
{
	DB_IMP_OBJECT(this->m_pImpl)->mode = Db::OpenMode::kNotOpen;
	return true;
}
bool DbObject::erase()
{
	return this->erase(true);
}
bool DbObject::erase(bool eraseIt)
{
	this->subErase(eraseIt);
	return true;
}
bool DbObject::subErase(bool erasing)
{
	DB_IMP_OBJECT(this->m_pImpl)->isErase = erasing;
	return false;
}
bool DbObject::isErased() const
{
	return DB_IMP_OBJECT(this->m_pImpl)->isErase;
}
bool DbObject::isReadEnabled() const
{
	Db::OpenMode mode = DB_IMP_OBJECT(this->m_pImpl)->mode;
	if (mode == Db::OpenMode::kForRead || mode == Db::OpenMode::kForWrite || mode == Db::OpenMode::kForNotify)
	{
		return true;
	}
	return false;
}
bool DbObject::isWriteEnabled() const
{
	if (DB_IMP_OBJECT(this->m_pImpl)->mode == Db::OpenMode::kForWrite)
	{
		return true;
	}
	return false;
}
bool DbObject::isNotifyEnabled() const
{
	if (DB_IMP_OBJECT(this->m_pImpl)->mode == Db::OpenMode::kForNotify)
	{
		return true;
	}
	return false;
}
bool DbObject::isModified() const
{
	return false;
}
RxObject *DbObject::clone() const
{
	return NULL;
}

Db::DuplicateRecordCloning DbObject::mergeStyle() const
{
	return Db::DuplicateRecordCloning::kDrcIgnore;
}

resbuf *DbObject::xData(const ACHAR *regappName) const
{
	resbuf *pRes = NULL;
	return pRes;
}
Acad::ErrorStatus DbObject::setXData(const resbuf *xdata)
{
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbObject::xDataTransformBy(const GeMatrix3d &xform)
{
	return Acad::ErrorStatus::eOk;
}

// Access State Assertions
//
void DbObject::assertReadEnabled() const
{
	DB_IMP_OBJECT(this->m_pImpl)->mode = Db::OpenMode::kForRead;
}
void DbObject::assertWriteEnabled(Adesk::Boolean autoUndo, Adesk::Boolean recordModified)
{
	bool needReactor = (DB_IMP_OBJECT(this->m_pImpl)->mode == Db::OpenMode::kNotOpen ||
		DB_IMP_OBJECT(this->m_pImpl)->mode == Db::OpenMode::kForRead);
	DB_IMP_OBJECT(this->m_pImpl)->mode = Db::OpenMode::kForWrite;
	if (needReactor && this->database() != NULL) {
		const AcArray<DbDatabaseReactor*>& reactors = this->database()->pImpl->reactors;
		for (int i = 0; i < reactors.length(); i++) {
			reactors.at(i)->objectOpenedForModify(this->database(), this);
		}
	}
}
void DbObject::assertNotifyEnabled() const
{
	DB_IMP_OBJECT(this->m_pImpl)->mode = Db::OpenMode::kForNotify;
}
Acad::ErrorStatus DbObject::dwgIn(DbDwgFiler *pFiler)
{
	return this->dwgInFields(pFiler);
}
Acad::ErrorStatus DbObject::dwgInFields(DbDwgFiler *pFiler)
{

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbObject::dwgOut(DbDwgFiler *pFiler) const
{
	return this->dwgOutFields(pFiler);
}
Acad::ErrorStatus DbObject::dwgOutFields(DbDwgFiler *pFiler) const
{
	return Acad::ErrorStatus::eOk;
}

void DbObject::copied(const DbObject *pObject, const DbObject *pNewObject)
{
}
void DbObject::erased(const DbObject *pObject, bool erasing)
{
}
void DbObject::goodbye(const DbObject *pObject)
{
}
void DbObject::openedForModify(const DbObject *pObject)
{
}
void DbObject::modified(const DbObject *pObject)
{
}
void DbObject::subObjModified(const DbObject *pObject, const DbObject *pSubObj)
{
}
void DbObject::modifyUndone(const DbObject *pObject)
{
}
bool DbObject::isAProxy() const
{
	return false;
}
void DbObject::objectClosed(const DbObjectId objectId)
{
}
GiDrawable *DbObject::drawable()
{
	return NULL;
}
unsigned int DbObject::subSetAttributes(GiDrawableTraits *pTraits) const
{
	return 0;
}
bool DbObject::subWorldDraw(GiWorldDraw *pWd) const
{
	return false;
}
void DbObject::subViewportDraw(GiViewportDraw *pVd) const
{
}
void DbObject::setGsNode(GsCache *pNode)
{
}
GsCache *DbObject::gsNode() const
{
	return NULL;
}
