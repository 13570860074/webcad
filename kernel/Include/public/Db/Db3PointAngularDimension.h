#ifndef DB3POINTANGULARDIMENSION_H
#define DB3POINTANGULARDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class Db3PointAngularDimension : public DbDimension
{
public:
    Db3PointAngularDimension();
    Db3PointAngularDimension(
        const GePoint3d& centerPoint,
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& arcPoint);
    Db3PointAngularDimension(
        const GePoint3d& centerPoint,
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& arcPoint,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~Db3PointAngularDimension();
    DB_ENTITY_DECLARE_MEMBERS(Db3PointAngularDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    GePoint3d         arcPoint() const;
    Acad::ErrorStatus   setArcPoint(const GePoint3d&);

    GePoint3d         xLine1Point() const;
    Acad::ErrorStatus   setXLine1Point(const GePoint3d&);

    GePoint3d         xLine2Point() const;
    Acad::ErrorStatus   setXLine2Point(const GePoint3d&);

    GePoint3d         centerPoint() const;
    Acad::ErrorStatus   setCenterPoint(const GePoint3d&);

    // Angular dimension extension arc for quadrant
    bool extArcOn() const;
    Acad::ErrorStatus setExtArcOn(bool value);

};

#endif
