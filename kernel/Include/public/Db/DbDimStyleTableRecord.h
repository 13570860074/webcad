#ifndef DBDIMSTYLETABLERECORD_H
#define DBDIMSTYLETABLERECORD_H

#include "Db.h"
#include "DbSymbolTableRecord.h"


class CmColor;
class DbDimStyleTableRecord : public  DbSymbolTableRecord
{
public:
    DbDimStyleTableRecord();
    virtual ~DbDimStyleTableRecord();
    DB_DECLARE_MEMBERS(DbDimStyleTableRecord);

    typedef DbDimStyleTable TableType;

    // Dimension variable api:
    //
#undef DBDIMVAR_H         // force the file to get read again
#include "dbdimvar.h"


    // DEPRECATED METHODS!
    // These are supported but will be removed in future releases:
    //
    Acad::ErrorStatus getDimpost(ACHAR*& pOutput) const;
    Acad::ErrorStatus getDimapost(ACHAR*& pOutput) const;
    Acad::ErrorStatus getDimblk(AcString& sOutput) const;
    Acad::ErrorStatus getDimblk(ACHAR*& pOutput) const;
    Acad::ErrorStatus getDimblk1(AcString& sOutput) const;
    Acad::ErrorStatus getDimblk1(ACHAR*& pOutput) const;
    Acad::ErrorStatus getDimblk2(AcString& sOutput) const;
    Acad::ErrorStatus getDimblk2(ACHAR*& pOutput) const;
#if EMSDK
    const emstring emsdk_getDimpost()const;
    const emstring emsdk_getDimapost()const;
    const emstring emsdk_getDimblk()const;
    const emstring emsdk_getDimblk1()const;
    const emstring emsdk_getDimblk2()const;
#endif

    int dimfit() const;
    int dimunit() const;

    Acad::ErrorStatus setDimfit(int fit);
    Acad::ErrorStatus setDimunit(int unit);
    //
    // end DEPRECATED METHODS!

    bool isModifiedForRecompute() const;
};


#endif