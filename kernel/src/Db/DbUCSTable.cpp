#include "Db.h"
#include "DbUCSTable.h"
#include "DbUCSTableRecord.h"
#include "DbUCSTableIterator.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"


DbUCSTable::DbUCSTable() {
    this->m_pImpl = new DbUCSTableImpl();
}
DbUCSTable::~DbUCSTable() {

}


Acad::ErrorStatus DbUCSTable::newIterator(DbUCSTableIterator*& pIterator) const {
    pIterator = new DbUCSTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbUCSTable::add(DbUCSTableRecord* pRecord)
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
        char* pUCSName = NULL;
        pRecord->getName(pUCSName);
        if (pUCSName == "")
        {
            break;
        }

        //判断是否存在
        if (this->has(pUCSName) == false)
        {
            es = DbSymbolTable::add(pRecord);
        }

    } while (false);

    return es;
}
Acad::ErrorStatus DbUCSTable::add(DbObjectId & recordId, DbUCSTableRecord * pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}



Acad::ErrorStatus DbUCSTable::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbUCSTable::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}

