#ifndef DBOBJECTREACTOR_H
#define DBOBJECTREACTOR_H

#include "DbObject.h"


class DbDatabase;
class DbObjectReactor: public RxObject
{
public:
    RX_DECLARE_MEMBERS(DbObjectReactor);

    virtual void cancelled(const DbObject* dbObj);
    virtual void copied(const DbObject* dbObj,const DbObject* newObj);
    virtual void erased(const DbObject* dbObj,Adesk::Boolean bErasing);
    virtual void goodbye(const DbObject* dbObj);
    virtual void openedForModify(const DbObject* dbObj);
    virtual void modified(const DbObject* dbObj);
    virtual void subObjModified(const DbObject* dbObj,const DbObject* subObj);
    virtual void modifyUndone(const DbObject* dbObj);
    virtual void modifiedXData(const DbObject* dbObj);
    virtual void unappended(const DbObject* dbObj);
    virtual void reappended(const DbObject* dbObj);
    virtual void objectClosed(const DbObjectId objId);

    //emsdk
#if EMSDK
public:
    DbObjectReactor();
    virtual ~DbObjectReactor();

    void onCancelled(int ptr);
    void onCopied(int ptr);
    void onErased(int ptr);
    void onGoodbye(int ptr);
    void onOpenedForModify(int ptr);
    void onModified(int ptr);
    void onSubObjModified(int ptr);
    void onModifyUndone(int ptr);
    void onModifiedXData(int ptr);
    void onUnappended(int ptr);
    void onReappended(int ptr);
    void onObjectClosed(int ptr);

#endif
};

#endif