#include "Db.h"
#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbLayerTableIterator.h"
#include "DbLinetypeTableRecord.h"
#include "DbLinetypeTable.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"

DbLayerTable::DbLayerTable()
{
    this->m_pImpl = new DbLayerTableImpl();
}
DbLayerTable::~DbLayerTable()
{
}

#if EMSDK

DbLayerTableRecord *DbLayerTable::emsdk_getAt(const emstring &entryName) const
{
    return this->emsdk_getAt(entryName, Db::kForRead, false);
}
DbLayerTableRecord *DbLayerTable::emsdk_getAt(const emstring &entryName, Db::OpenMode openMode) const
{
    return this->emsdk_getAt(entryName, openMode, false);
}
DbLayerTableRecord *DbLayerTable::emsdk_getAt(const emstring &entryName, Db::OpenMode openMode, bool openErasedRec) const
{
    DbLayerTableRecord *pLayerTableRecord = NULL;
    this->getAt(entryName.c_str(), pLayerTableRecord, openMode, openErasedRec);
    return pLayerTableRecord;
}
DbLayerTableIterator *DbLayerTable::emsdk_newIterator() const
{
    DbLayerTableIterator *pLayerTableIterator = NULL;
    this->newIterator(pLayerTableIterator);
    return pLayerTableIterator;
}

#endif

Acad::ErrorStatus DbLayerTable::newIterator(DbLayerTableIterator*& pIterator) const
{
    pIterator = new DbLayerTableIterator();
    DbSymbolTable::newIterator((DbSymbolTableIterator*&)pIterator);
    return Acad::ErrorStatus::eOk;
}


Acad::ErrorStatus DbLayerTable::add(DbLayerTableRecord *pRecord)
{
    Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

    do
    {
        if (this->database() != NULL && this->database()->pImpl->isInspect == false)
        {
            DbSymbolTable::add(pRecord);
            es = Acad::ErrorStatus::eOk;
            break;
        }

        // 判空
        if (pRecord == NULL)
        {
            break;
        }

        // 获得图层名称
        char *pLayerName = NULL;
        pRecord->getName(pLayerName);
        if (pLayerName == "")
        {
            break;
        }

        // 判断是否存在
        if (this->has(pLayerName) == true)
        {
            break;
        }

        // 判断是否需要设置默认线型
        bool isDefaultLinetype = false;
        DbObjectId linetypeObjectId = pRecord->linetypeObjectId();
        if (linetypeObjectId.isNull() == false)
        {
            isDefaultLinetype = true;
        }
        if (isDefaultLinetype == false)
        {
            DbLinetypeTableRecord *linetypeTableRecord = NULL;
            if (::acdbOpenObject(linetypeTableRecord, linetypeObjectId) == Acad::ErrorStatus::eOk)
            {
                if (linetypeTableRecord->database() == this->database())
                {
                    isDefaultLinetype = true;
                }
                linetypeTableRecord->close();
            }
        }

        // 设置默认线型
        if (isDefaultLinetype == false)
        {

            DbLinetypeTable *linetypeTable = NULL;
            this->database()->getLinetypeTable(linetypeTable);

            linetypeObjectId = NULL;
            linetypeTable->getAt("Continuous", linetypeObjectId);
            if (linetypeObjectId.isNull() == false)
            {
                pRecord->setLinetypeObjectId(linetypeObjectId);
            }

            linetypeTable->close();
        }

        es = DbSymbolTable::add(pRecord);

    } while (false);

    return es;
}
Acad::ErrorStatus DbLayerTable::add(DbObjectId &recordId, DbLayerTableRecord *pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}

Acad::ErrorStatus DbLayerTable::dwgInFields(DbDwgFiler *pFiler)
{

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk)
    {
        return (es);
    }

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLayerTable::dwgOutFields(DbDwgFiler *pFiler) const
{

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk)
    {
        return (es);
    }

    return Acad::ErrorStatus::eOk;
}
