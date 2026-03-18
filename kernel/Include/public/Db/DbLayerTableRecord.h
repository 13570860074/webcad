#ifndef DBLAYERTABLERECORD_H
#define DBLAYERTABLERECORD_H

#include "Db.h"
#include "CmColor.h"
#include "CmTransparency.h"
#include "DbSymbolTableRecord.h"


class GiDrawable;
class DbLayerTableRecord : public  DbSymbolTableRecord
{
public:
    DbLayerTableRecord();
    virtual ~DbLayerTableRecord();
    DB_DECLARE_MEMBERS(DbLayerTableRecord);


    bool isFrozen() const;
    Acad::ErrorStatus setIsFrozen(bool frozen);

    bool isOff() const;
    void setIsOff(bool off);

    bool VPDFLT() const;
    void setVPDFLT(bool frozen);

    bool isLocked() const;
    void setIsLocked(bool locked);

    CmColor color() const;
    void setColor(const CmColor& color);
    CmEntityColor entityColor(void) const;

    CmTransparency transparency(void) const;
    Acad::ErrorStatus setTransparency(const CmTransparency& trans);

    DbObjectId linetypeObjectId() const;
    Acad::ErrorStatus setLinetypeObjectId(DbObjectId id);

    bool isPlottable() const;
    Acad::ErrorStatus setIsPlottable(bool plot);

    Db::LineWeight lineWeight() const;
    Acad::ErrorStatus setLineWeight(Db::LineWeight weight);


    bool isInUse() const;
    ACHAR* description() const;      // deprecated
    Acad::ErrorStatus description(AcString&) const;
    Acad::ErrorStatus setDescription(const ACHAR* description);
#if EMSDK
    emstring emsdk_description() const;
    Acad::ErrorStatus emsdk_setDescription(const emstring& description);
#endif

    bool isHidden() const;
    Acad::ErrorStatus setIsHidden(bool on);
    static bool isHidden(DbObjectId);
};


#endif