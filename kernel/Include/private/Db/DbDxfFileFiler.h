#ifndef DbDxfFileFiler_H
#define DbDxfFileFiler_H

#include "DbDxfFiler.h"



class DbDxfFileFiler : public DbDxfFiler
{
public:
    virtual Db::FilerType filerType() const;

    // working database being read or written by this filer
    virtual DbDatabase *database() const;
    virtual void *setDatabase(DbDatabase *_database) const;

    virtual Acad::ErrorStatus readResBuf(resbuf *pRb);
    virtual Acad::ErrorStatus writeResBuf(const resbuf &rb);
    virtual Acad::ErrorStatus writeObjectId(Db::DxfCode code,const DbObjectId &id) = 0;
    virtual Acad::ErrorStatus writeInt8(Db::DxfCode code, Adesk::Int8) = 0;

    virtual Acad::ErrorStatus writeString(Db::DxfCode code, const ACHAR *pVal) = 0;
    virtual Acad::ErrorStatus writeString(Db::DxfCode code, const AcString &pVal) = 0;
    virtual Acad::ErrorStatus writeBChunk(Db::DxfCode code, const ads_binary &val) = 0;
    virtual Acad::ErrorStatus writeDbHandle(Db::DxfCode code, const DbHandle &val) = 0;
    virtual Acad::ErrorStatus writeInt64(Db::DxfCode code, Adesk::Int64 val) = 0;
    virtual Acad::ErrorStatus writeInt32(Db::DxfCode code, Adesk::Int32 val) = 0;
    virtual Acad::ErrorStatus writeInt16(Db::DxfCode code, Adesk::Int16 val) = 0;
    virtual Acad::ErrorStatus writeUInt64(Db::DxfCode code, Adesk::UInt64 val) = 0;
    virtual Acad::ErrorStatus writeUInt32(Db::DxfCode code, Adesk::UInt32 val) = 0;
    virtual Acad::ErrorStatus writeUInt16(Db::DxfCode code, Adesk::UInt16 val) = 0;
    virtual Acad::ErrorStatus writeUInt8(Db::DxfCode code, Adesk::UInt8 val) = 0;
    virtual Acad::ErrorStatus writeBoolean(Db::DxfCode code, Adesk::Boolean val) = 0;
    virtual Acad::ErrorStatus writeBool(Db::DxfCode code, bool val) = 0;
    virtual Acad::ErrorStatus writeDouble(Db::DxfCode code, double val, int prec = kDfltPrec) = 0;
    virtual Acad::ErrorStatus writePoint2d(Db::DxfCode code, const GePoint2d &val, int prec = kDfltPrec) = 0;
    virtual Acad::ErrorStatus writePoint3d(Db::DxfCode code, const GePoint3d &val, int prec = kDfltPrec) = 0;
    virtual Acad::ErrorStatus writeVector2d(Db::DxfCode code, const GeVector2d &val, int prec = kDfltPrec) = 0;
    virtual Acad::ErrorStatus writeVector3d(Db::DxfCode code, const GeVector3d &val, int prec = kDfltPrec) = 0;
    virtual Acad::ErrorStatus writeScale3d(Db::DxfCode code, const GeScale3d &val, int prec = kDfltPrec) = 0;

    virtual bool includesDefaultValues() const = 0;

    DbDxfFileFiler();
    ~DbDxfFileFiler();
    DbFilerController &controller() const;

private:
    DbDxfFileFiler &operator=(const DbDxfFileFiler &); // supress C4512 as we have a reference as data member
};


#endif