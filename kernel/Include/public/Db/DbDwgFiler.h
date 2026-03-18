#ifndef DbDwgFiler_H
#define DbDwgFiler_H


#include "adesk.h"
#include "adsdef.h"
#include "acadstrc.h"
#include "RxObject.h"
#include "Db.h"
#include "GePoint2d.h"
#include "GeVector3d.h"
#include "GeScale3d.h"
#include "RxDeclareMembers.h"


class DbHandle;
class DbDwgFiler : public RxObject
{
public:
    RX_DECLARE_MEMBERS_VIRTUAL(DbDwgFiler);
protected:
    DbDwgFiler();
public:
    virtual ~DbDwgFiler();

    virtual  Acad::ErrorStatus filerStatus() const = 0;
    virtual  Db::FilerType   filerType() const = 0;
    virtual void               setFilerStatus(Acad::ErrorStatus es) = 0;
    virtual void               resetFilerStatus() = 0;

    // version of the drawing file being read or written by this filer

    virtual  Acad::ErrorStatus  dwgVersion(unsigned int& ver) const;

    //        readXxx() and writeXxx() functions
    //

    virtual Acad::ErrorStatus   readInt8(Adesk::Int8* pVal) = 0;
    virtual Acad::ErrorStatus   writeInt8(Adesk::Int8 val) = 0;

    // Note: use of readString(ACHAR **) is discouraged, because
    // caller has to free the returned string.  It may be phased
    // out in a future release.
    virtual Acad::ErrorStatus   readString(ACHAR** pVal) = 0;
    virtual Acad::ErrorStatus   writeString(const ACHAR* pVal) = 0;

    virtual Acad::ErrorStatus   readString(AcString& val) = 0;
    virtual Acad::ErrorStatus   writeString(const AcString& pVal) = 0;

    virtual Acad::ErrorStatus   readBChunk(ads_binary* pVal) = 0;
    virtual Acad::ErrorStatus   writeBChunk(const ads_binary&) = 0;

    virtual Acad::ErrorStatus   readDbHandle(DbHandle* pVal) = 0;
    virtual Acad::ErrorStatus   writeDbHandle(const DbHandle& val) = 0;

    virtual Acad::ErrorStatus   readInt64(Adesk::Int64* pVal) = 0;
    virtual Acad::ErrorStatus   writeInt64(Adesk::Int64 val) = 0;

    virtual Acad::ErrorStatus   readInt32(Adesk::Int32* pVal) = 0;
    virtual Acad::ErrorStatus   writeInt32(Adesk::Int32 val) = 0;

    virtual Acad::ErrorStatus   readInt16(Adesk::Int16* pVal) = 0;
    virtual Acad::ErrorStatus   writeInt16(Adesk::Int16 val) = 0;

    virtual Acad::ErrorStatus   readUInt64(Adesk::UInt64* pVal) = 0;
    virtual Acad::ErrorStatus   writeUInt64(Adesk::UInt64 val) = 0;

    virtual Acad::ErrorStatus   readUInt32(Adesk::UInt32* pVal) = 0;
    virtual Acad::ErrorStatus   writeUInt32(Adesk::UInt32 val) = 0;

    virtual Acad::ErrorStatus   readUInt16(Adesk::UInt16* pVal) = 0;
    virtual Acad::ErrorStatus   writeUInt16(Adesk::UInt16 val) = 0;

    virtual Acad::ErrorStatus   readUInt8(Adesk::UInt8* pVal) = 0;
    virtual Acad::ErrorStatus   writeUInt8(Adesk::UInt8 val) = 0;

    virtual Acad::ErrorStatus   readBoolean(Adesk::Boolean* pVal) = 0;
    virtual Acad::ErrorStatus   writeBoolean(Adesk::Boolean val) = 0;

    virtual Acad::ErrorStatus   readBool(bool* pVal) = 0;
    virtual Acad::ErrorStatus   writeBool(bool val) = 0;

    virtual Acad::ErrorStatus   readDouble(double* pVal) = 0;
    virtual Acad::ErrorStatus   writeDouble(double val) = 0;

    virtual Acad::ErrorStatus   readPoint2d(GePoint2d* pVal) = 0;
    virtual Acad::ErrorStatus   writePoint2d(const GePoint2d& val) = 0;

    virtual Acad::ErrorStatus   readPoint3d(GePoint3d* pVal) = 0;
    virtual Acad::ErrorStatus   writePoint3d(const GePoint3d& val) = 0;

    virtual Acad::ErrorStatus   readVector2d(GeVector2d* pVal) = 0;
    virtual Acad::ErrorStatus   writeVector2d(const GeVector2d& val) = 0;

    virtual Acad::ErrorStatus   readVector3d(GeVector3d* pVal) = 0;
    virtual Acad::ErrorStatus   writeVector3d(const GeVector3d& val) = 0;

    virtual Acad::ErrorStatus   readScale3d(GeScale3d* pVal) = 0;
    virtual Acad::ErrorStatus   writeScale3d(const GeScale3d& val) = 0;

    virtual Acad::ErrorStatus   readAddress(void** pVal);
    virtual Acad::ErrorStatus   writeAddress(const void* pVal);


    // Note: there are no filer methods for explicitly reading or
    // writing a single text character.
    //
    virtual Acad::ErrorStatus   readItem(ACHAR** pVal);
    virtual Acad::ErrorStatus   writeItem(const ACHAR* val);
    virtual Acad::ErrorStatus   writeItem(const AcString& val);

    virtual Acad::ErrorStatus   readItem(ads_binary* pVal);
    virtual Acad::ErrorStatus   writeItem(const ads_binary& val);

    virtual Acad::ErrorStatus   readItem(DbHandle* pVal);
    virtual Acad::ErrorStatus   writeItem(const DbHandle& val);

    virtual Acad::ErrorStatus   readItem(Adesk::Int64* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::Int64 val);

    virtual Acad::ErrorStatus   readItem(Adesk::Int32* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::Int32 val);

    virtual Acad::ErrorStatus   readItem(Adesk::Int16* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::Int16 val);

    virtual Acad::ErrorStatus   readItem(Adesk::Int8* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::Int8 val);

    virtual Acad::ErrorStatus   readItem(Adesk::UInt64* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::UInt64 val);

    virtual Acad::ErrorStatus   readItem(Adesk::UInt32* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::UInt32 val);

    virtual Acad::ErrorStatus   readItem(Adesk::UInt16* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::UInt16 val);

    virtual Acad::ErrorStatus   readItem(Adesk::UInt8* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::UInt8 val);

    virtual Acad::ErrorStatus   readItem(Adesk::Boolean* pVal);
    virtual Acad::ErrorStatus   writeItem(Adesk::Boolean val);

    virtual Acad::ErrorStatus   readItem(double* pVal);
    virtual Acad::ErrorStatus   writeItem(double val);

    virtual Acad::ErrorStatus   readItem(GePoint2d* pVal);
    virtual Acad::ErrorStatus   writeItem(const GePoint2d& val);

    virtual Acad::ErrorStatus   readItem(GePoint3d* pVal);
    virtual Acad::ErrorStatus   writeItem(const GePoint3d& val);

    virtual Acad::ErrorStatus   readItem(GeVector2d* pVal);
    virtual Acad::ErrorStatus   writeItem(const GeVector2d& val);

    virtual Acad::ErrorStatus   readItem(GeVector3d* pVal);
    virtual Acad::ErrorStatus   writeItem(const GeVector3d& val);

    virtual Acad::ErrorStatus   readItem(GeScale3d* pVal);
    virtual Acad::ErrorStatus   writeItem(const GeScale3d& val);

    virtual Acad::ErrorStatus   readItem(void** pp);
    virtual Acad::ErrorStatus   writeItem(const void* p);


private:
    DbDwgFiler& operator = (const DbDwgFiler&); 
};


#endif