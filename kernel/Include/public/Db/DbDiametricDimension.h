#ifndef DBDIAMETRICDIMENSION_H
#define DBDIAMETRICDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbDiametricDimension : public DbDimension
{
public:
    DbDiametricDimension();
    DbDiametricDimension(
        const GePoint3d& chordPoint,
        const GePoint3d& farChordPoint,
        double             leaderLength,
        const ACHAR* dimText = NULL,
        DbObjectId       dimStyle = DbObjectId::kNull);
    ~DbDiametricDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbDiametricDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    double              leaderLength() const;
    Acad::ErrorStatus   setLeaderLength(double);

    GePoint3d         chordPoint() const;
    Acad::ErrorStatus   setChordPoint(const GePoint3d&);

    GePoint3d         farChordPoint() const;
    Acad::ErrorStatus   setFarChordPoint(const GePoint3d&);

    // Diametric dimension extension arc
    double extArcStartAngle() const;
    Acad::ErrorStatus setExtArcStartAngle(double newAngle);

    double extArcEndAngle() const;
    Acad::ErrorStatus setExtArcEndAngle(double newAngle);
};

#endif
