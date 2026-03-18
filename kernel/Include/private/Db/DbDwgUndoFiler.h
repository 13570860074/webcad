#ifndef DBDWGUNDOFILER_H
#define DBDWGUNDOFILER_H

#include "AcBinary.h"
#include "DbDwgFiler.h"


class DbHandle;
class AcBinaryFiler;
class DbDwgUndoFiler : public DbDwgFiler
{
public:
    DbDwgUndoFiler();
    virtual ~DbDwgUndoFiler();
    RX_DECLARE_MEMBERS(DbDwgUndoFiler);

public:

    AcBinaryFiler* binary();

    virtual  Acad::ErrorStatus filerStatus() const;
    virtual  Db::FilerType   filerType() const;
    virtual void               setFilerStatus(Acad::ErrorStatus es);
    virtual void               resetFilerStatus();


    //        readXxx() and writeXxx() functions
    //

    virtual Acad::ErrorStatus   readInt8(Adesk::Int8* pVal);
    virtual Acad::ErrorStatus   writeInt8(Adesk::Int8 val);

    // Note: use of readString(ACHAR **) is discouraged, because
    // caller has to free the returned string.  It may be phased
    // out in a future release.
    virtual Acad::ErrorStatus   readString(ACHAR** pVal);
    virtual Acad::ErrorStatus   writeString(const ACHAR* pVal);

    virtual Acad::ErrorStatus   readString(AcString& val);
    virtual Acad::ErrorStatus   writeString(const AcString& pVal);

    virtual Acad::ErrorStatus   readBChunk(ads_binary* pVal);
    virtual Acad::ErrorStatus   writeBChunk(const ads_binary&);

    virtual Acad::ErrorStatus   readDbHandle(DbHandle* pVal);
    virtual Acad::ErrorStatus   writeDbHandle(const DbHandle& val);

    virtual Acad::ErrorStatus   readInt64(Adesk::Int64* pVal);
    virtual Acad::ErrorStatus   writeInt64(Adesk::Int64 val);

    virtual Acad::ErrorStatus   readInt32(Adesk::Int32* pVal);
    virtual Acad::ErrorStatus   writeInt32(Adesk::Int32 val);

    virtual Acad::ErrorStatus   readInt16(Adesk::Int16* pVal);
    virtual Acad::ErrorStatus   writeInt16(Adesk::Int16 val);

    virtual Acad::ErrorStatus   readUInt64(Adesk::UInt64* pVal);
    virtual Acad::ErrorStatus   writeUInt64(Adesk::UInt64 val);

    virtual Acad::ErrorStatus   readUInt32(Adesk::UInt32* pVal);
    virtual Acad::ErrorStatus   writeUInt32(Adesk::UInt32 val);

    virtual Acad::ErrorStatus   readUInt16(Adesk::UInt16* pVal);
    virtual Acad::ErrorStatus   writeUInt16(Adesk::UInt16 val);

    virtual Acad::ErrorStatus   readUInt8(Adesk::UInt8* pVal);
    virtual Acad::ErrorStatus   writeUInt8(Adesk::UInt8 val);

    virtual Acad::ErrorStatus   readBoolean(Adesk::Boolean* pVal);
    virtual Acad::ErrorStatus   writeBoolean(Adesk::Boolean val);

    virtual Acad::ErrorStatus   readBool(bool* pVal);
    virtual Acad::ErrorStatus   writeBool(bool val);

    virtual Acad::ErrorStatus   readDouble(double* pVal);
    virtual Acad::ErrorStatus   writeDouble(double val);

    virtual Acad::ErrorStatus   readPoint2d(GePoint2d* pVal);
    virtual Acad::ErrorStatus   writePoint2d(const GePoint2d& val);

    virtual Acad::ErrorStatus   readPoint3d(GePoint3d* pVal);
    virtual Acad::ErrorStatus   writePoint3d(const GePoint3d& val);

    virtual Acad::ErrorStatus   readVector2d(GeVector2d* pVal);
    virtual Acad::ErrorStatus   writeVector2d(const GeVector2d& val);

    virtual Acad::ErrorStatus   readVector3d(GeVector3d* pVal);
    virtual Acad::ErrorStatus   writeVector3d(const GeVector3d& val);

    virtual Acad::ErrorStatus   readScale3d(GeScale3d* pVal);
    virtual Acad::ErrorStatus   writeScale3d(const GeScale3d& val);
};


#endif