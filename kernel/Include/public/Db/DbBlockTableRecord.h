#ifndef DBBLOCKTABLERECORD_H
#define DBBLOCKTABLERECORD_H

#include "Db.h"
#include "DbObjectIdArray.h"
#include "GePoint3d.h"
#include "DbSymbolTableRecord.h"


class DbBlockBegin;
class DbBlockEnd;
class DbBlockTableRecordIterator;
class DbBlockReferenceIdIterator;
class DbBlockTableRecord : public  DbSymbolTableRecord
{
public:
    DbBlockTableRecord();
    virtual ~DbBlockTableRecord();
    DB_DECLARE_MEMBERS(DbBlockTableRecord);


    Acad::ErrorStatus appendDbEntity(DbEntity* pEntity)
    {
        DbObjectId id;
        return this->appendDbEntity(id, pEntity);
    }

    // Note: does not close the entity. Caller must close it
    // after entity is successfully appended.
    Acad::ErrorStatus appendDbEntity(DbObjectId& pOutputId, DbEntity* pEntity);

    Acad::ErrorStatus newIterator(DbBlockTableRecordIterator*& pIterator, bool atBeginning = true, bool skipDeleted = true) const;

    Acad::ErrorStatus comments(ACHAR*& pString) const;  // deprecated
    Acad::ErrorStatus comments(const ACHAR*& pString) const;
    Acad::ErrorStatus comments(AcString& sComments) const;
    Acad::ErrorStatus setComments(const ACHAR* pString);
#if EMSDK
    const emstring emsdk_comments()const;
    Acad::ErrorStatus emsdk_setComments(const emstring& pString);
#endif

    Acad::ErrorStatus pathName(ACHAR*& pPath) const;    // deprecated
    Acad::ErrorStatus pathName(const ACHAR*& pPath) const;
    Acad::ErrorStatus pathName(AcString& sPath) const;
    Acad::ErrorStatus setPathName(const ACHAR* pathStr);
#if EMSDK
    const emstring emsdk_pathName()const;
    Acad::ErrorStatus emsdk_setPathName(const emstring& pathStr);
#endif

    GePoint3d origin() const;
    Acad::ErrorStatus setOrigin(const GePoint3d& pt);

    Acad::ErrorStatus openBlockBegin(DbBlockBegin*& pBlockBegin, Db::OpenMode openMode = Db::kForRead);

    Acad::ErrorStatus openBlockEnd(DbBlockEnd*& pBlockEnd, Db::OpenMode openMode = Db::kForRead);

    bool hasAttributeDefinitions() const;

    bool isAnonymous() const;

    bool isFromExternalReference() const;

    bool isFromOverlayReference() const;
    Acad::ErrorStatus setIsFromOverlayReference(bool bIsOverlay);

    bool isLayout() const;

    DbObjectId getLayoutId() const;
    Acad::ErrorStatus setLayoutId(DbObjectId);


    Acad::ErrorStatus newBlockReferenceIdIterator(
        DbBlockReferenceIdIterator*& pIter) const;

    // DbBlockReferenceIdIterator will be going away. 
    // Use getBlockReferenceIds() instead.
    //
    Acad::ErrorStatus getBlockReferenceIds(DbObjectIdArray& ids, bool bDirectOnly = true, bool bForceValidity = false) const;

    Acad::ErrorStatus getErasedBlockReferenceIds(DbObjectIdArray&);


    bool isUnloaded() const;
    Acad::ErrorStatus setIsUnloaded(bool isUnloaded);

    virtual Acad::ErrorStatus assumeOwnershipOf(DbObjectIdArray& entitiesToMove);

    // Block scaling and exploding control

    enum BlockScaling {
        kAny,
        kUniform
    };


    DbBlockTableRecord::BlockScaling blockScaling() const;
    Acad::ErrorStatus  setBlockScaling(DbBlockTableRecord::BlockScaling blockScaling);

    Acad::ErrorStatus setExplodable(bool bExplodable);
    bool explodable() const;

    Acad::ErrorStatus setBlockInsertUnits(Db::UnitsValue insunits);
    Db::UnitsValue blockInsertUnits() const;
};


#endif