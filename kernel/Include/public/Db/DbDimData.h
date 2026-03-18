#ifndef DBDIMDATA_H
#define DBDIMDATA_H

#include "AcArray.h"
#include "DbDimension.h"

class DbDimData;

#if !EMSDK
typedef AcArray<DbDimData*> DbDimDataPtrArray;
#endif
// emsdk
#if EMSDK
class DbDimDataPtrArray
{
public:
    DbDimDataPtrArray() {}
    DbDimDataPtrArray(unsigned int count) {}
    ~DbDimDataPtrArray() {}

public:
    ACARRAY_PTR_METHODS(DbDimData*);
};
#endif
typedef GeVector3d (*DimDataSetValueFuncPtr) (DbDimData* pThis,
                                        DbEntity* pEnt, 
                                        double newValue,
                                        const GeVector3d& offset);
// DimDataSetCustomStringFuncPtr is the callback function signature for DbDimData::setCustomStringFunc
typedef GeVector3d (*DimDataSetCustomStringFuncPtr) (DbDimData* pThis,
                                        DbEntity* pEnt, 
                                        const ACHAR* pCustomString,
                                        const GeVector3d& offset);
class DbDimData
{
public:
    enum DimDataFlags {
        kDimEditable           = 0x1,
        kDimInvisible          = 0x2,
        kDimFocal              = 0x4,
        kDimHideIfValueIsZero  = 0x8,
        kDimEmptyData          = 0x10, //For constraint-unlock support
        kDimResultantLength    = 0x20,
        kDimDeltaLength        = 0x40,
        kDimResultantAngle     = 0x80,
        kDimDeltaAngle         = 0x100,
        kDimRadius             = 0x200,
        kDimCustomValue        = 0x400,
        kDimConstrained        = 0x800,
        kDimCustomString       = 0x1000,
	};

    DbDimData();
    DbDimData(DbDimension* pDim, 
                DimDataSetValueFuncPtr setDimFunc = NULL,
                unsigned int bitFlags = 0,
                void* appData = NULL,
                DimDataSetCustomStringFuncPtr setCustomStringFunc = NULL);

    ~DbDimData();

    DbDimData(const DbDimData&);
    DbDimData&  operator = (const DbDimData&);
    bool operator == (const DbDimData&) const;
    DbDimension* dimension() const;
    Acad::ErrorStatus setDimension(DbDimension* pDim);
    DbObjectId ownerId() const;
    Acad::ErrorStatus setOwnerId(const DbObjectId& objId);
    unsigned int bitFlags() const;
    Acad::ErrorStatus setBitFlags(unsigned int flags);
    bool isDimFocal() const;
    Acad::ErrorStatus setDimFocal(bool focal);
    bool isDimEditable() const;
    Acad::ErrorStatus setDimEditable(bool editable);
    bool isDimInvisible() const;
    Acad::ErrorStatus setDimInvisible(bool invisible);
    bool isDimHideIfValueIsZero() const;
    Acad::ErrorStatus setDimHideIfValueIsZero(bool hide);
    void *appData() const;
    Acad::ErrorStatus setAppData(void* appData);
    DimDataSetValueFuncPtr dimValueFunc() const;
    Acad::ErrorStatus setDimValueFunc(DimDataSetValueFuncPtr funcPtr);
    DimDataSetCustomStringFuncPtr customStringFunc() const;
    Acad::ErrorStatus setCustomStringFunc(DimDataSetCustomStringFuncPtr funcPtr);
    bool isDimResultantLength() const;
    Acad::ErrorStatus setDimResultantLength(bool bValue);
    bool isDimDeltaLength() const;
    Acad::ErrorStatus setDimDeltaLength(bool bValue);
    bool isDimResultantAngle() const;
    Acad::ErrorStatus setDimResultantAngle(bool bValue);
    bool isDimDeltaAngle() const;
    Acad::ErrorStatus setDimDeltaAngle(bool bValue);
    bool isDimRadius() const;
    Acad::ErrorStatus setDimRadius(bool bValue);
    bool isCustomDimValue() const;
    Acad::ErrorStatus setCustomDimValue(bool custom);
    bool isConstrained() const;
    Acad::ErrorStatus setConstrain(bool bValue);
    bool isCustomString() const;
    Acad::ErrorStatus setCustomString(bool bValue);

private:
    DbDimension*      mpDim;
    DbObjectId        mOwnerId;
    unsigned int        mBitFlags;
    DimDataSetValueFuncPtr  mpSetDimValueFunc;
    void*               mpAppData;
    DimDataSetCustomStringFuncPtr  mpSetCustomStringFunc;
};

#if !EMSDK
typedef AcArray<DbDimData> DbDimDataArray;
#endif
// emsdk
#if EMSDK
class DbDimDataArray
{
public:
    DbDimDataArray() {}
    DbDimDataArray(unsigned int count) {}
    ~DbDimDataArray() {}

public:
    ACARRAY_METHODS(DbDimData);
};
#endif

inline DbDimData::DbDimData() : 
    mpDim(NULL), 
    mBitFlags(0),
    mpSetDimValueFunc(NULL),
    mpAppData (NULL),
    mpSetCustomStringFunc(NULL)
{
}

inline DbDimData::DbDimData(DbDimension* pDim, 
                   DimDataSetValueFuncPtr setValueFunc,
                   unsigned int bitFlags,
                   void* appData,
                   DimDataSetCustomStringFuncPtr setCustomStringFunc) :
    mpDim(pDim),
    mBitFlags(bitFlags),
    mpSetDimValueFunc(setValueFunc),
    mpAppData(appData),
    mpSetCustomStringFunc(setCustomStringFunc)
{
}

inline DbDimData::DbDimData(const DbDimData& src)
{
    *this = src;
}

inline DbDimData::~DbDimData()
{
    if (mpDim) 
        delete mpDim;
}

inline DbDimData& DbDimData::operator = (const DbDimData& src)
{
    if (this == &src)
        return *this;

    mpDim = (DbDimension*)src.dimension()->clone();
    mOwnerId = src.ownerId();
    mpSetDimValueFunc = src.dimValueFunc();
    mpSetCustomStringFunc = src.customStringFunc();
    mBitFlags = src.bitFlags();
    mpAppData = src.appData();
    return *this;
}
inline bool DbDimData::operator == (const DbDimData& v) const {
    return this->bitFlags() == v.bitFlags();
}

inline DbDimension*
DbDimData::dimension() const
{
    return mpDim;
}

inline Acad::ErrorStatus
DbDimData::setDimension(DbDimension* pDim)
{
    mpDim = pDim;
    return Acad::eOk;
}

inline DbObjectId
DbDimData::ownerId() const
{
    return mOwnerId;
}

inline Acad::ErrorStatus
DbDimData::setOwnerId(const DbObjectId& objId)
{
    mOwnerId = objId;
    return Acad::eOk;
}

inline unsigned int 
DbDimData::bitFlags() const
{
    return mBitFlags;
}

inline Acad::ErrorStatus
DbDimData::setBitFlags(unsigned int flags)
{
    mBitFlags = flags;
    return Acad::eOk;
}

inline bool
DbDimData::isDimFocal() const
{
    return !!(mBitFlags & kDimFocal);
}

inline Acad::ErrorStatus
DbDimData::setDimFocal(bool focal)
{
    if (focal)
        mBitFlags |= kDimFocal;
    else
        mBitFlags &= ~kDimFocal;
    return Acad::eOk;
}

inline bool
DbDimData::isDimEditable() const
{
    return !!(mBitFlags & kDimEditable);
}

inline Acad::ErrorStatus
DbDimData::setDimEditable(bool editable)
{
    if (editable)
        mBitFlags |= kDimEditable;
    else
        mBitFlags &= ~kDimEditable;
    return Acad::eOk;
}

inline bool
DbDimData::isDimInvisible() const
{
    return !!(mBitFlags & kDimInvisible);
}

inline Acad::ErrorStatus
DbDimData::setDimInvisible(bool invisible)
{
    if (invisible)
        mBitFlags |= kDimInvisible;
    else
        mBitFlags &= ~kDimInvisible;
    return Acad::eOk;
}

inline bool
DbDimData::isDimHideIfValueIsZero() const
{
    return !!(mBitFlags & kDimHideIfValueIsZero);
}

inline Acad::ErrorStatus
DbDimData::setDimHideIfValueIsZero(bool hide)
{
    if (hide)
        mBitFlags |= kDimHideIfValueIsZero;
    else
        mBitFlags &= ~kDimHideIfValueIsZero;
    return Acad::eOk;
}

inline bool
DbDimData::isDimResultantLength() const
{
    return !!(mBitFlags & kDimResultantLength);
}

inline Acad::ErrorStatus
DbDimData::setDimResultantLength(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimResultantLength;
    else
        mBitFlags &= ~kDimResultantLength;
    return Acad::eOk;
}
inline bool
DbDimData::isDimDeltaLength() const
{
    return !!(mBitFlags & kDimDeltaLength);
}

inline Acad::ErrorStatus
DbDimData::setDimDeltaLength(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimDeltaLength;
    else
        mBitFlags &= ~kDimDeltaLength;
    return Acad::eOk;
}
inline bool
DbDimData::isDimResultantAngle() const
{
    return !!(mBitFlags & kDimResultantAngle);
}

inline Acad::ErrorStatus
DbDimData::setDimResultantAngle(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimResultantAngle;
    else
        mBitFlags &= ~kDimResultantAngle;
    return Acad::eOk;
}
inline bool
DbDimData::isDimDeltaAngle() const
{
    return !!(mBitFlags & kDimDeltaAngle);
}

inline Acad::ErrorStatus
DbDimData::setDimDeltaAngle(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimDeltaAngle;
    else
        mBitFlags &= ~kDimDeltaAngle;
    return Acad::eOk;
}
inline bool
DbDimData::isDimRadius() const
{
    return !!(mBitFlags & kDimRadius);
}

inline Acad::ErrorStatus
DbDimData::setDimRadius(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimRadius;
    else
        mBitFlags &= ~kDimRadius;
    return Acad::eOk;
}

inline bool
DbDimData::isCustomDimValue() const
{
    return !!(mBitFlags & kDimCustomValue);
}

inline Acad::ErrorStatus
DbDimData::setCustomDimValue(bool custom)
{
    if (custom)
        mBitFlags |= kDimCustomValue;
    else
        mBitFlags &= ~kDimCustomValue;
    return Acad::eOk;
}

inline bool
DbDimData::isConstrained() const
{
    return !!(mBitFlags & kDimConstrained);
}

inline Acad::ErrorStatus
DbDimData::setConstrain(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimConstrained;
    else
        mBitFlags &= ~kDimConstrained;
    return Acad::eOk;
}

inline bool
DbDimData::isCustomString() const
{
    return !!(mBitFlags & kDimCustomString);
}

inline Acad::ErrorStatus
DbDimData::setCustomString(bool bValue)
{
    if (bValue)
        mBitFlags |= kDimCustomString;
    else
        mBitFlags &= ~kDimCustomString;
    return Acad::eOk;
}

inline void*
DbDimData::appData() const
{
    return mpAppData;
}

inline Acad::ErrorStatus
DbDimData::setAppData(void* appData)
{
    mpAppData = appData;
    return Acad::eOk;
}

inline DimDataSetValueFuncPtr
DbDimData::dimValueFunc() const
{
    return mpSetDimValueFunc;
}

inline Acad::ErrorStatus
DbDimData::setDimValueFunc(DimDataSetValueFuncPtr funcPtr)
{
    mpSetDimValueFunc = funcPtr;
    return Acad::eOk;
}

inline DimDataSetCustomStringFuncPtr
DbDimData::customStringFunc() const
{
    return mpSetCustomStringFunc;
}

inline Acad::ErrorStatus
DbDimData::setCustomStringFunc(DimDataSetCustomStringFuncPtr funcPtr)
{
    mpSetCustomStringFunc = funcPtr;
    return Acad::eOk;
}

#endif



