#ifndef DBDICTIONARYITERATOR_H
#define DBDICTIONARYITERATOR_H

#include "Db.h"
#include "AcArray.h"
#include "DbDictionary.h"


class DbDictionaryIterator : public RxObject
{
public:
    DbDictionaryIterator();
    virtual ~DbDictionaryIterator();
    RX_DECLARE_MEMBERS(DbDictionaryIterator);

    std::string name() const;
    bool name(AcString& sName) const;

    template<class ObjType> 
    Acad::ErrorStatus getObject(ObjType*& pObject, Db::OpenMode mode = Db::kForRead)
    {
        const DbObjectId id = this->objectId();
        return ::acdbOpenObject(pObject, id, mode, /*openErased*/false);
    }

    // Needed for cases where a smart ptr is passed in
    DbObject* getObject(Db::OpenMode mode) {
        DbObject* object = NULL;
        this->getObject(object, mode);
        return object;
    }
    Acad::ErrorStatus getObject(DbObject*& pObject, Db::OpenMode mode = Db::kForRead) {
        const DbObjectId id = this->objectId();
        return ::acdbOpenObject(pObject, id, mode, false);
    }

    DbObject* object() {
        return this->getObject(Db::OpenMode::kForRead);
    }
    DbObject* object(Db::OpenMode mode) {
        return this->getObject(mode);
    }
    DbObjectId objectId() const;

    bool done() const;
    bool next();
};


#endif