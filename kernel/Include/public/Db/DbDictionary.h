#ifndef DBDICTIONARY_H
#define DBDICTIONARY_H

#include "DbObject.h"


class DbDictionaryRecord;
class DbDictionaryIterator;
class DbDictionary :public DbObject {
public:
	DbDictionary();
	virtual ~DbDictionary();
    DB_DECLARE_MEMBERS(DbDictionary);


    // Needed for cases where a smart ptr is passed in
    //Acad::ErrorStatus getAt(const ACHAR* entryName, DbObjectId& entryId) const;
    //Acad::ErrorStatus getAt(const ACHAR* entryName, DbObject*& entryObj, Db::OpenMode mode = Db::kForRead);
        // Get an entry by name.
    //
    template<class ObjType> Acad::ErrorStatus getAt(const ACHAR* entryName,
        ObjType*& entryObj,
        Db::OpenMode mode = Db::kForRead) const
    {
        entryObj = nullptr;
        DbObjectId id;
        Acad::ErrorStatus es = this->getAt(entryName, id);
        if (es == Acad::eOk)
            es = ::acdbOpenObject(entryObj, id, mode, /*openErased*/false);
        return es;
    }

    // Needed for cases where a smart ptr is passed in
    Acad::ErrorStatus getAt(const ACHAR* entryName,
        DbObject*& entryObj,
        Db::OpenMode mode = Db::kForRead)
    {
        return this->getAt<DbObject>(entryName, entryObj, mode);
    }

    Acad::ErrorStatus getAt(const ACHAR* entryName, DbObjectId& entryId) const;
#if EMSDK
    DbObjectId emsdk_getAt(const emstring& entryName);
    DbObject* emsdk_getAt(const emstring& entryName, Db::OpenMode mode);
#endif

    // Find name corresponding to object id.
    //
    Acad::ErrorStatus nameAt(DbObjectId objId, ACHAR*& name) const;  // deprecated
    Acad::ErrorStatus nameAt(DbObjectId objId, AcString& sName) const;
#if EMSDK
    const emstring emsdk_nameAt(DbObjectId objId);
#endif

    // Query contents of dictionary
    //
    bool has(const ACHAR* entryName) const;
    bool has(DbObjectId objId) const;
    UInt32 numEntries() const;
#if EMSDK
    bool emsdk_has(const emstring& entryName) const;
#endif

    // Remove entries.
    //
    Acad::ErrorStatus remove(const ACHAR* key);
    Acad::ErrorStatus remove(const ACHAR* key, DbObjectId& returnId);
    Acad::ErrorStatus remove(DbObjectId  objId);
#if EMSDK
    DbObjectId emsdk_remove(const emstring& key);
#endif

    // Reset an entry.
    //
    bool setName(const ACHAR* oldName, const ACHAR* newName);
#if EMSDK
    bool emsdk_setName(const emstring& oldName, const emstring& newName);
#endif

    Acad::ErrorStatus setAt(const ACHAR* srchKey, DbObject* newValue, DbObjectId& retObjId);
#if EMSDK
    DbObjectId emsdk_setAt(const emstring& srchKey, DbObject* newValue);
#endif

    // Get an iterator for this dictionary.
    //
    DbDictionaryIterator* newIterator() const;
};




#endif