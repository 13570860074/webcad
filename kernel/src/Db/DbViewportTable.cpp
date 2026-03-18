#include "Db.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbViewportTableIterator.h"
#include "DbImpl.h"
#include "DbDatabaseImpl.h"


DbViewportTable::DbViewportTable() {
    this->m_pImpl = new DbViewportTableImpl();
}
DbViewportTable::~DbViewportTable() {

}

Acad::ErrorStatus DbViewportTable::newIterator(DbViewportTableIterator*& pIterator) const {
    pIterator = new DbViewportTableIterator();
    return DbSymbolTable::newIterator((DbSymbolTableIterator *&)pIterator);
}

Acad::ErrorStatus DbViewportTable::add(DbViewportTableRecord* pRecord)
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

        //获得名称
        char* pViewportName = NULL;
        pRecord->getName(pViewportName);
        if (pViewportName == "")
        {
            break;
        }

        es = DbSymbolTable::add(pRecord);

    } while (false);

    return es;
}
Acad::ErrorStatus DbViewportTable::add(DbObjectId & recordId, DbViewportTableRecord * pRecord)
{
    recordId = NULL;

    Acad::ErrorStatus es = this->add(pRecord);
    if (es == Acad::ErrorStatus::eOk)
    {
        recordId = pRecord->objectId();
    }
    return es;
}




Acad::ErrorStatus DbViewportTable::dwgInFields(DbDwgFiler* pFiler) {

    Acad::ErrorStatus es = DbSymbolTable::dwgInFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbViewportTable::dwgOutFields(DbDwgFiler* pFiler) const {

    Acad::ErrorStatus es = DbSymbolTable::dwgOutFields(pFiler);
    if (es != Acad::eOk) {
        return (es);
    }


    return Acad::ErrorStatus::eOk;
}



