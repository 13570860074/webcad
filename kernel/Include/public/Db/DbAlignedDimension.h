#ifndef DBALIGNEDDIMENSION_H
#define DBALIGNEDDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbAlignedDimension : public DbDimension
{
public:
    DbAlignedDimension();
    DbAlignedDimension(
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& dimLinePoint);

    DbAlignedDimension(
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& dimLinePoint,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~DbAlignedDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbAlignedDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    GePoint3d         xLine1Point() const;
    Acad::ErrorStatus   setXLine1Point(const GePoint3d&);

    GePoint3d         xLine2Point() const;
    Acad::ErrorStatus   setXLine2Point(const GePoint3d&);

    GePoint3d         dimLinePoint() const;
    Acad::ErrorStatus   setDimLinePoint(const GePoint3d&);

    double              oblique() const;
    Acad::ErrorStatus   setOblique(double);

    // Aligned dimension Jog symbol
    bool jogSymbolOn() const;
    Acad::ErrorStatus setJogSymbolOn(bool value);

    GePoint3d jogSymbolPosition() const;
    Acad::ErrorStatus setJogSymbolPosition(const GePoint3d& pt);

};

#endif
