#ifndef DBLAYOUT_H
#define DBLAYOUT_H


#include "DbPlotSettings.h"
#include "DbObjectIdArray.h"

class DbLayout : public DbPlotSettings
{
public:
    DbLayout();
    virtual ~DbLayout();
    DB_DECLARE_MEMBERS(DbLayout);

    DbObjectId getBlockTableRecordId() const;

    virtual Acad::ErrorStatus setBlockTableRecordId(DbObjectId BlockTableRecordId);

    virtual Acad::ErrorStatus addToLayoutDict(DbDatabase* towhichDb, DbObjectId BlockTableRecordId);

    Acad::ErrorStatus getName(AcString& sName) const;
    Acad::ErrorStatus getLayoutName(ACHAR*& layoutName) const; // deprecated
    virtual Acad::ErrorStatus setLayoutName(const ACHAR* layoutName);

#if EMSDK
    emstring emsdk_getName() const;
    emstring emsdk_getLayoutName() const;
    virtual Acad::ErrorStatus emsdk_setLayoutName(const emstring& layoutName);
#endif

    int getTabOrder() const;
    virtual void setTabOrder(int newOrder);

    bool getTabSelected() const;
    virtual void setTabSelected(bool tabSelected);

    DbObjectIdArray getViewportArray() const;


    virtual void close();
};



#endif