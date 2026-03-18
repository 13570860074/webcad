#ifndef DBROTATEDDIMENSION_H
#define DBROTATEDDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbRotatedDimension : public DbDimension
{
public:
    DbRotatedDimension();
    DbRotatedDimension(
        double             rotation,
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& dimLinePoint);
    DbRotatedDimension(
        double             rotation,
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& dimLinePoint,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~DbRotatedDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbRotatedDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;


    GePoint3d         xLine1Point() const;
    Acad::ErrorStatus   setXLine1Point(const GePoint3d&);

    GePoint3d         xLine2Point() const;
    Acad::ErrorStatus   setXLine2Point(const GePoint3d&);

    GePoint3d         dimLinePoint() const;
    Acad::ErrorStatus   setDimLinePoint(const GePoint3d&);

    double              oblique() const;
    Acad::ErrorStatus   setOblique(double);

    double              rotation() const;
    Acad::ErrorStatus   setRotation(double);

    // Rotated dimension Jog symbol
    bool jogSymbolOn() const;
    Acad::ErrorStatus setJogSymbolOn(bool value);

    GePoint3d jogSymbolPosition() const;
    Acad::ErrorStatus setJogSymbolPosition(const GePoint3d& pt);

    virtual void close();

};

#endif
