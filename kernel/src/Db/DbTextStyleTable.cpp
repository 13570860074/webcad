#include "Db.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "DbTextStyleTableIterator.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"


DbTextStyleTable::DbTextStyleTable() {
    this->m_pImpl = new DbTextStyleTableImpl();
}
DbTextStyleTable::~DbTextStyleTable() {

}

Acad::ErrorStatus DbTextStyleTable::newIterator(DbTextStyleTableIterator*& pIterator) const {
    pIterator = new DbTextStyleTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbTextStyleTable::add(DbTextStyleTableRecord* pRecord)
{
    Acad::ErrorStatus es = Acad::ErrorStatus::eFail;

    do
    {
        if(this->database() != NULL && this->database()->pImpl->isInspect == false){
            DbSymbolTable::add(pRecord);
            es = Acad::ErrorStatus::eOk;
            break;
        }

        //判空
        if (pRecord == NULL)
        {
            break;
        }

        //获得图层名称
        char* pTextStyleName = NULL;
        pRecord->getName(pTextStyleName);
        if (pTextStyleName == "")
        {
            break;
        }

        //判断是否存在
        es = DbSymbolTable::add(pRecord);

    } while (false);

    return es;
}
Acad::ErrorStatus DbTextStyleTable::add(DbObjectId & recordId, DbTextStyleTableRecord * pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}





Acad::ErrorStatus DbTextStyleTable::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbTextStyleTable::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}