#include "Db.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbViewTableIterator.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"


DbViewTable::DbViewTable() {
    this->m_pImpl = new DbViewTableImpl();
}
DbViewTable::~DbViewTable() {

}

Acad::ErrorStatus DbViewTable::newIterator(DbViewTableIterator*& pIterator) const {
    pIterator = new DbViewTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbViewTable::add(DbViewTableRecord* pRecord)
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

        //设置名称
        ACHAR* pName = NULL;
        pRecord->getName(pName);
        if (pName == "") {
            pRecord->setName("*Active");
        }

        //添加对象
        es = DbSymbolTable::add(pRecord);

    } while (false);

    return es;
}
Acad::ErrorStatus DbViewTable::add(DbObjectId & recordId, DbViewTableRecord * pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}




Acad::ErrorStatus DbViewTable::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewTable::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}


