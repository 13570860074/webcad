#ifndef DBDATABASE_H
#define DBDATABASE_H

#include "Db.h"
#include "DbObject.h"
#include "CmColor.h"
#include "CmTransparency.h"


class DbHandle;
class DbDatabaseReactor;
class DbDatabaseImpl;
class DbDatabase INHERIT_EM_OBJECT {
public:
    DbDatabase(bool buildDefaultDrawing = true);
    virtual ~DbDatabase();

public:

    // 内部使用
    Acad::ErrorStatus addDbObject(DbObjectId& id, DbObject* pObj);
    Acad::ErrorStatus addDbObject(DbObject* pObj);

    // Symbol Tables
    Acad::ErrorStatus getBlockTable(DbBlockTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getLayerTable(DbLayerTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getTextStyleTable(DbTextStyleTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getLinetypeTable(DbLinetypeTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getViewTable(DbViewTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getUCSTable(DbUCSTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getViewportTable(DbViewportTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getRegAppTable(DbRegAppTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getDimStyleTable(DbDimStyleTable*& pTable, Db::OpenMode mode = Db::kForRead);

    Acad::ErrorStatus getSymbolTable(DbBlockTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbLayerTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbLinetypeTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbViewTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbUCSTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbViewportTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbRegAppTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbTextStyleTable*& pTable, Db::OpenMode mode = Db::kForRead);
    Acad::ErrorStatus getSymbolTable(DbDimStyleTable*& pTable, Db::OpenMode mode = Db::kForRead);

#if EMSDK
    DbBlockTable* emsdk_getBlockTable();
    DbLayerTable* emsdk_getLayerTable();
    DbLinetypeTable* emsdk_getLinetypeTable();
    DbViewTable* emsdk_getViewTable();
    DbUCSTable* emsdk_getUCSTable();
    DbViewportTable* emsdk_getViewportTable();
    DbRegAppTable* emsdk_getRegAppTable();
    DbBlockTable* emsdk_getBlockTable(Db::OpenMode mode);
    DbLayerTable* emsdk_getLayerTable(Db::OpenMode mode);
    DbLinetypeTable* emsdk_getLinetypeTable(Db::OpenMode mode);
    DbViewTable* emsdk_getViewTable(Db::OpenMode mode);
    DbUCSTable* emsdk_getUCSTable(Db::OpenMode mode);
    DbViewportTable* emsdk_getViewportTable(Db::OpenMode mode);
    DbRegAppTable* emsdk_getRegAppTable(Db::OpenMode mode);
    DbTextStyleTable* emsdk_getTextStyleTable(Db::OpenMode mode);
    DbDimStyleTable* emsdk_getDimStyleTable(Db::OpenMode mode);
#endif

    DbObjectId  blockTableId() const;
    DbObjectId  layerTableId() const;
    DbObjectId  linetypeTableId() const;
    DbObjectId  viewTableId() const;
    DbObjectId  UCSTableId() const;
    DbObjectId  viewportTableId() const;
    DbObjectId  regAppTableId() const;
    DbObjectId  textStyleTableId() const;
    DbObjectId  dimStyleTableId() const;
    DbObjectId  layoutDictionaryId() const;

    DbObjectId layerZero() const;
    DbObjectId continuousLinetype() const;
    DbObjectId byLayerLinetype() const;
    DbObjectId byBlockLinetype() const;
    DbObjectId paperSpaceVportId() const;
    DbObjectId currentSpaceId() const;

    DbHandle handseed() const;
    Acad::ErrorStatus setHandseed(const DbHandle& handle);

    DbDictionary* getLayoutDictionary() const;
    DbDictionary* getLayoutDictionary(Db::OpenMode mode) const;
    Acad::ErrorStatus getLayoutDictionary(DbDictionary*& pDict, Db::OpenMode mode = Db::kForRead) const;

    double plinewid()const;
    Acad::ErrorStatus setPlinewid(double width);

    CmColor cecolor() const;
    Acad::ErrorStatus setCecolor(const CmColor& color);

    CmTransparency cetransparency() const;
    Acad::ErrorStatus setCetransparency(const CmTransparency& transparency);

    DbObjectId clayer()const;
    Acad::ErrorStatus setClayer(DbObjectId _layerId);

    DbObjectId textstyle() const;
    Acad::ErrorStatus setTextstyle(DbObjectId objId);

    Acad::ErrorStatus getDbObjectId(DbObjectId& retId, bool createIfNotFound, const DbHandle& objHandle);
    Acad::ErrorStatus getDbObjectId(DbObjectId& retId, bool createIfNotFound, const DbHandle& objHandle, Adesk::UInt32 xRefId);

    // Can return eInvalidInput, eObjectToBeDeleted, eDuplicateKey or eOk
    Acad::ErrorStatus addReactor(DbDatabaseReactor* pReactor) const;

    // Can return eInvalidInput, eKeyNotFound or eOk
    Acad::ErrorStatus removeReactor(DbDatabaseReactor* pReactor) const;

public:
    DbDatabaseImpl* pImpl;
};



extern void repairDatabase(DbDatabase *database);

#endif

