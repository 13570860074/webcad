#include "Db.h"
#include "DbDimStyleTable.h"
#include "DbDimStyleTableRecord.h"
#include "DbDimStyleTableIterator.h"
#include "DbImpl.h"


DbDimStyleTable::DbDimStyleTable() {
    this->m_pImpl = new DbDimStyleTableImpl();
}
DbDimStyleTable::~DbDimStyleTable() {

}

Acad::ErrorStatus DbDimStyleTable::newIterator(DbDimStyleTableIterator*& pIterator) const {
    pIterator = new DbDimStyleTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbDimStyleTable::add(DbDimStyleTableRecord* pRecord)
{
    Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

    do
    {
        //判空
        if (pRecord == NULL)
        {
            break;
        }

        //获得图层名称
        char* pDimStyleName = NULL;
        pRecord->getName(pDimStyleName);
        if (pDimStyleName == "")
        {
            break;
        }

        //判断是否存在
        if (this->has(pDimStyleName) == false)
        {
            es = DbSymbolTable::add(pRecord);
        }

    } while (false);

    return es;
}
Acad::ErrorStatus DbDimStyleTable::add(DbObjectId & recordId, DbDimStyleTableRecord * pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}




Acad::ErrorStatus DbDimStyleTable::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimStyleTable::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
