
#ifndef DBOBJECT_H
#define DBOBJECT_H


#include "Db.h"
#include "adsdef.h"
#include "RxObject.h"
#include "GiDrawable.h"
#include "DbHandle.h"
#include "DbDatabase.h"
#include "DbObjectId.h"
#include "DbDwgFiler.h"
#include "DbDeclareMethods.h"



class GsCache;
class GeMatrix3d;
class DbDwgFiler;
class DbDatabase;
class GiWorldDraw;
class GiDrawableTraits;
class GiViewportDraw;
class DbObject : public GiDrawable
{


public:
    virtual ~DbObject();
    RX_DECLARE_MEMBERS_VIRTUAL(DbObject);


    // Associated Objects
    //
    DbObjectId      objectId() const;
    DbObjectId      ownerId() const;
    virtual Acad::ErrorStatus setOwnerId(DbObjectId objId);
    void              getDbHandle(DbHandle& handle) const;
    /// <summary>This method returns a pointer to the DbDatabase object that contains
///          the AcDbObject.</summary>
///

    DbDatabase* database() const;


    // Open/Close/Cancel/Undo/Erase
    //
    virtual bool subOpen(Db::OpenMode mode);
    virtual void close();
    virtual bool subClose();
    virtual bool erase();
    virtual bool erase(bool eraseIt);
    virtual bool subErase(bool erasing);
    bool isErased() const;
    bool isReadEnabled() const;
    bool isWriteEnabled() const;
    bool isNotifyEnabled() const;
    bool isModified() const;

    virtual RxObject* clone() const;

    // Merge style for insert-style operations. Defaults to kDrcIgnore.
//
    virtual Db::DuplicateRecordCloning mergeStyle() const;

    // XData
    //
    virtual resbuf* xData(const ACHAR* regappName = nullptr) const;
    virtual Acad::ErrorStatus setXData(const resbuf* xdata);
    Acad::ErrorStatus xDataTransformBy(const GeMatrix3d& xform);

    // Access State Assertions
    //
    void assertReadEnabled() const;
    void assertWriteEnabled(Adesk::Boolean autoUndo = true, Adesk::Boolean recordModified = true);
    void assertNotifyEnabled() const;

    Acad::ErrorStatus         dwgIn(DbDwgFiler* pFiler);
    virtual Acad::ErrorStatus dwgInFields(DbDwgFiler* pFiler);
    Acad::ErrorStatus         dwgOut(DbDwgFiler* pFiler) const;
    virtual Acad::ErrorStatus dwgOutFields(DbDwgFiler* pFiler) const;

    virtual void copied(const DbObject* pObject, const DbObject* pNewObject);
    virtual void erased(const DbObject* pObject, bool erasing = true);
    virtual void goodbye(const DbObject* pObject);
    virtual void openedForModify(const DbObject* pObject);
    virtual void modified(const DbObject* pObject);
    virtual void subObjModified(const DbObject* pObject, const DbObject* pSubObj);
    virtual void modifyUndone(const DbObject* pObject);

    bool isAProxy() const;  
    virtual void objectClosed(const DbObjectId objectId);

    virtual GiDrawable* drawable();

    // GiDrawable NVI重写
    virtual unsigned int subSetAttributes(GiDrawableTraits* pTraits) const;
    virtual bool subWorldDraw(GiWorldDraw* pWd) const;
    virtual void subViewportDraw(GiViewportDraw* pVd) const;
    virtual bool isPersistent() const { return true; }
    virtual DbObjectId id() const { return objectId(); }

    void setGsNode(GsCache* pNode);
    GsCache* gsNode() const;
};


#endif


