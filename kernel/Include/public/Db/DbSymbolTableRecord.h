#ifndef DBSYMBOLTABLERECORD_H
#define DBSYMBOLTABLERECORD_H

#include "AcString.h"
#include "DbObject.h"


class DbSymbolTableRecord : public DbObject
{
public:
    virtual ~DbSymbolTableRecord();
    DB_DECLARE_MEMBERS_VIRTUAL(DbSymbolTableRecord);

    Acad::ErrorStatus getName(ACHAR*& pName) const;     // deprecated
    Acad::ErrorStatus getName(AcString& sName) const;
    Acad::ErrorStatus getName(const ACHAR*& pName) const;
    Acad::ErrorStatus setName(const ACHAR* pName);
#if EMSDK
    emstring emsdk_getName() const;
    Acad::ErrorStatus emsdk_setName(const emstring& pName);
#endif


protected:
    DbSymbolTableRecord();
};


#endif