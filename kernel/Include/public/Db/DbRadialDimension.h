#ifndef DBRADIALDIMENSION_H
#define DBRADIALDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbRadialDimension : public DbDimension
{
public:
    DbRadialDimension();
    DbRadialDimension(
        const GePoint3d& center,
        const GePoint3d& chordPoint,
        double             leaderLength);
    DbRadialDimension(
        const GePoint3d& center,
        const GePoint3d& chordPoint,
        double             leaderLength,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~DbRadialDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbRadialDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    double              leaderLength() const;
    Acad::ErrorStatus   setLeaderLength(double);

    GePoint3d         center() const;
    Acad::ErrorStatus   setCenter(const GePoint3d&);

    GePoint3d         chordPoint() const;
    Acad::ErrorStatus   setChordPoint(const GePoint3d&);

    // Radial dimension extension arc
    double extArcStartAngle() const;
    Acad::ErrorStatus setExtArcStartAngle(double newAngle);

    double extArcEndAngle() const;
    Acad::ErrorStatus setExtArcEndAngle(double newAngle);

};

#endif
