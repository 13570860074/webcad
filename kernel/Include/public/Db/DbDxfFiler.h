#ifndef DBDXFFILER_H
#define DBDXFFILER_H

#include "Db.h"
#include "RxObject.h"
#include "GePoint2d.h"
#include "GePoint3d.h"
#include "GeVector2d.h"
#include "GeVector3d.h"
#include "AcString.h"
#include "acutads.h"

class DbDatabase;
class DbHandle;
class DbFilerController;
class DbDxfFiler : public RxObject
{
public:
    virtual Db::FilerType filerType() const = 0;

    // working database being read or written by this filer
    virtual DbDatabase *database() const = 0;

    // version of the drawing file being read or written by this filer

    virtual Acad::ErrorStatus dwgVersion(Db::DbDwgVersion &ver, Db::MaintenanceReleaseVersion &maintVer) const;

    virtual Acad::ErrorStatus extendedDwgMaintenanceReleaseVersion(Db::MaintenanceReleaseVersion &maintVer) const;

    virtual int precision() const;
    virtual void setPrecision(int prec);

    // Number of decimal digits printed in ASCII DXF file
    //
    enum
    {
        kDfltPrec = -1,
        kMaxPrec = 16
    };

    //        readXxx and writeXxx functions
    //
    virtual Acad::ErrorStatus readResBuf(resbuf *pRb);
    virtual Acad::ErrorStatus writeResBuf(const resbuf &rb);
    virtual Acad::ErrorStatus writeObjectId(Db::DxfCode code,
                                            const DbObjectId &id) = 0;
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

    //        readItem and writeItem functions
    //
    Acad::ErrorStatus readItem(resbuf *pItem);
    Acad::ErrorStatus writeItem(const resbuf &pItem);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const DbObjectId &id);

    Acad::ErrorStatus writeItem(Db::DxfCode dc, const ACHAR *val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const AcString &val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const ads_binary &val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const DbHandle &val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::Int32 val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::Int16 val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::Int8 val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::UInt32 val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::UInt16 val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::UInt8 val);

#ifndef Adesk_Boolean_is_bool
    // CAUTION: "int" parameters go to writeBoolean when Adesk_Boolean_is_bool
    // is not defined. When it is defined, the bool overload handles this
    // case.
    Acad::ErrorStatus writeItem(Db::DxfCode dc, Adesk::Boolean val);
#endif

    Acad::ErrorStatus writeItem(Db::DxfCode dc, bool val);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, double val, int = kDfltPrec);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const GePoint2d &val, int prec = kDfltPrec);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const GePoint3d &val, int prec = kDfltPrec);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const GeVector2d &val, int prec = kDfltPrec);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const GeVector3d &val, int prec = kDfltPrec);
    Acad::ErrorStatus writeItem(Db::DxfCode dc, const GeScale3d &val, int prec = kDfltPrec);
    virtual Acad::ErrorStatus writeXDataStart();
    virtual bool includesDefaultValues() const = 0;

    virtual Acad::ErrorStatus pushBackItem();
    virtual bool atEOF();
    virtual bool atSubclassData(const ACHAR *subClassName);
    virtual bool atExtendedData();
    virtual bool atEndOfObject();
    virtual void haltAtClassBoundaries(bool halt);

    virtual Acad::ErrorStatus writeEmbeddedObjectStart();
    virtual bool atEmbeddedObjectStart();

    virtual double elevation() const;
    virtual double thickness() const;

    virtual bool isModifyingExistingObject() const;

    DbDxfFiler();
    ~DbDxfFiler();
    DbFilerController &controller() const;

private:
    DbDxfFiler &operator=(const DbDxfFiler &); // supress C4512 as we have a reference as data member
};

inline Acad::ErrorStatus
DbDxfFiler::readItem(resbuf *pVal)
{
    return readResBuf(pVal);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(const resbuf &val)
{
    return writeResBuf(val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const DbObjectId &id)
{
    return writeObjectId(dc, id);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const ACHAR *val)
{
    return writeString(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const ads_binary &val)
{
    return writeBChunk(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const DbHandle &val)
{
    return writeDbHandle(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::Int32 val)
{
    return writeInt32(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::Int16 val)
{
    return writeInt16(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::Int8 val)
{
    return this->writeInt8(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::UInt32 val)
{
    return writeUInt32(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::UInt16 val)
{
    return writeUInt16(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::UInt8 val)
{
    return writeUInt8(dc, val);
}

#if !defined(Adesk_Boolean_is_bool)
inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, Adesk::Boolean val)
{
    return writeBoolean(dc, val);
}
#endif

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, bool val)
{
    return writeBool(dc, val);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, double val, int prec)
{
    return writeDouble(dc, val, prec);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const GePoint2d &val, int prec)
{
    return writePoint2d(dc, val, prec);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const GePoint3d &val, int prec)
{
    return writePoint3d(dc, val, prec);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const GeVector2d &val, int prec)
{
    return writeVector2d(dc, val, prec);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const GeVector3d &val, int prec)
{
    return writeVector3d(dc, val, prec);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const GeScale3d &val, int prec)
{
    return writeScale3d(dc, val, prec);
}

inline Acad::ErrorStatus
DbDxfFiler::writeItem(Db::DxfCode dc, const AcString &val)
{
    return writeString(dc, val);
}

#endif