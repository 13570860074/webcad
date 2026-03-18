#include "Db.h"
#include "DbRegAppTable.h"
#include "DbRegAppTableRecord.h"
#include "DbRegAppTableIterator.h"
#include "DbImpl.h"

DbRegAppTable::DbRegAppTable()
{
    this->m_pImpl = new DbRegAppTableImpl();
}
DbRegAppTable::~DbRegAppTable()
{
}

Acad::ErrorStatus DbRegAppTable::newIterator(DbRegAppTableIterator *&pIterator) const
{
    pIterator = new DbRegAppTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbRegAppTable::add(DbRegAppTableRecord *pRecord)
{
    Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

    do
    {
        // 判空
        if (pRecord == NULL)
        {
            break;
        }

        // 获得图层名称
        char *pRegAppName = NULL;
        pRecord->getName(pRegAppName);
        if (pRegAppName == "")
        {
            break;
        }

        // 判断是否存在
        if (this->has(pRegAppName) == false)
        {
            es = DbSymbolTable::add(pRecord);
        }

    } while (false);

    return es;
}
Acad::ErrorStatus DbRegAppTable::add(DbObjectId &recordId, DbRegAppTableRecord *pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}

Acad::ErrorStatus DbRegAppTable::dwgInFields(DbDwgFiler *pFiler)
{

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk)
    {
        return (es);
    }

    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbRegAppTable::dwgOutFields(DbDwgFiler *pFiler) const
{

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk)
    {
        return (es);
    }

    return Acad::ErrorStatus::eOk;
}