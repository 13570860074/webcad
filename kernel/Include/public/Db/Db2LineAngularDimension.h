#ifndef DB2LINEANGULARDIMENSION_H
#define DB2LINEANGULARDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class Db2LineAngularDimension : public DbDimension
{
public:
    Db2LineAngularDimension();
    Db2LineAngularDimension(
        const GePoint3d& xLine1Start,
        const GePoint3d& xLine1End,
        const GePoint3d& xLine2Start,
        const GePoint3d& xLine2End,
        const GePoint3d& arcPoint);
    Db2LineAngularDimension(
        const GePoint3d& xLine1Start,
        const GePoint3d& xLine1End,
        const GePoint3d& xLine2Start,
        const GePoint3d& xLine2End,
        const GePoint3d& arcPoint,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~Db2LineAngularDimension();
    DB_ENTITY_DECLARE_MEMBERS(Db2LineAngularDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    GePoint3d         arcPoint() const;
    Acad::ErrorStatus   setArcPoint(const GePoint3d&);

    GePoint3d         xLine1Start() const;
    Acad::ErrorStatus   setXLine1Start(const GePoint3d&);

    GePoint3d         xLine1End() const;
    Acad::ErrorStatus   setXLine1End(const GePoint3d&);

    GePoint3d         xLine2Start() const;
    Acad::ErrorStatus   setXLine2Start(const GePoint3d&);

    GePoint3d         xLine2End() const;
    Acad::ErrorStatus   setXLine2End(const GePoint3d&);

    // Angular dimension extension arc for quadrant
    bool extArcOn() const;
    Acad::ErrorStatus setExtArcOn(bool value);

};

#endif
