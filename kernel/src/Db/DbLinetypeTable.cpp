#include "Db.h"
#include "DbLinetypeTable.h"
#include "DbLinetypeTableRecord.h"
#include "DbLinetypeTableIterator.h"
#include "DbImpl.h"


DbLinetypeTable::DbLinetypeTable() {
    this->m_pImpl = new DbLinetypeTableImpl();
}
DbLinetypeTable::~DbLinetypeTable() {

}


Acad::ErrorStatus DbLinetypeTable::newIterator(DbLinetypeTableIterator*& pIterator) const {
    pIterator = new DbLinetypeTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbLinetypeTable::add(DbLinetypeTableRecord* pRecord)
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
        char* pLinetypeName = NULL;
        pRecord->getName(pLinetypeName);
        if (pLinetypeName == "")
        {
            break;
        }

        //判断是否存在
        //if (this->has(pLinetypeName) == false)
        {
            es = DbSymbolTable::add(pRecord);
        }

    } while (false);

    return es;
}
Acad::ErrorStatus DbLinetypeTable::add(DbObjectId & recordId, DbLinetypeTableRecord * pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}







Acad::ErrorStatus DbLinetypeTable::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbLinetypeTable::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}