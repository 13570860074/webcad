#include "Db.h"
#include "DbDatabaseImpl.h"
#include "DbObjectManager.h"
#include "DbLayout.h"
#include "DbObjectId.h"
#include "DbObject.h"
#include "DbEntity.h"
#include "DbViewport.h"
#include "DbSymbolTable.h"
#include "DbViewportTable.h"
#include "DbViewTable.h"
#include "DbSymbolTableRecord.h"
#include "DbUCSTable.h"
#include "DbUCSTableRecord.h"
#include "DbLinetypeTable.h"
#include "DbLinetypeTableRecord.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbDimStyleTable.h"
#include "DbDimStyleTableRecord.h"
#include "DbDatabaseReactor.h"
#include "kernel.h"
#include "AcArray.h"
#include "DbImpl.h"

Acad::ErrorStatus acdbOpenObject(DbObject *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus error = Acad::ErrorStatus::eOk;

	do
	{
		pObj = ::kernel()->acdbObjectManager()->openDbObject(id);
		if (pObj == NULL)
		{
			pObj = NULL;
			error = Acad::ErrorStatus::eFail;
			break;
		}

		if (pObj->isErased() == true && openErased == false)
		{
			pObj = NULL;
			error = Acad::ErrorStatus::eFail;
			break;
		}

		pObj->subOpen(mode);

		error = Acad::ErrorStatus::eOk;

	} while (false);

	return error;
}
Acad::ErrorStatus acdbOpenObject(DbSymbolTable *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject *pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbSymbolTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbBlockTable*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbBlockTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbLayerTable*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbLayerTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbLinetypeTable*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbLinetypeTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbUCSTable*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbUCSTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbViewportTable *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject *pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbViewportTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbViewTable *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject *pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbViewTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbSymbolTableRecord *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject *pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbSymbolTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbBlockTableRecord*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbBlockTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbLayerTableRecord*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbLayerTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbLinetypeTableRecord*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbLinetypeTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbUCSTableRecord*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbUCSTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbViewportTableRecord *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject *pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbViewportTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbViewTableRecord *&pObj, DbObjectId id, Db::OpenMode mode, bool openErased)
{
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject *pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbViewTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbTextStyleTable*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbTextStyleTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbTextStyleTableRecord*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbTextStyleTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbDimStyleTable*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbDimStyleTable::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbDimStyleTableRecord*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbDimStyleTableRecord::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}

Acad::ErrorStatus acdbOpenObject(DbLayout*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbLayout::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}

Acad::ErrorStatus acdbOpenObject(DbEntity*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbEntity::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}
Acad::ErrorStatus acdbOpenObject(DbViewport*& pObj, DbObjectId id, Db::OpenMode mode, bool openErased) {
	Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

	do
	{
		DbObject* pObject = NULL;
		acdbOpenObject(pObject, id, mode, openErased);
		if (pObject == NULL)
		{
			break;
		}

		pObj = DbViewport::cast(pObject);
		if (pObj != NULL)
		{
			es = Acad::ErrorStatus::eOk;
		}

	} while (false);

	return es;
}