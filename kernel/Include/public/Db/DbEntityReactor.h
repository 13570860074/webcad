#ifndef DBENTITYREACTOR_H
#define DBENTITYREACTOR_H

#include "DbObjectReactor.h"

class DbEntity;
class DbEntityReactor : public DbObjectReactor
{
public:
    RX_DECLARE_MEMBERS(DbEntityReactor);

    virtual void modifiedGraphics(const DbEntity* dbObj);
    virtual void dragCloneToBeDeleted(const DbEntity* pOriginalObj, const DbEntity* pClone);

#if EMSDK
public:
    DbEntityReactor();
    virtual ~DbEntityReactor();

    void onModifiedGraphics(int ptr);
    void onDragCloneToBeDeleted(int ptr);

#endif

};
#endif