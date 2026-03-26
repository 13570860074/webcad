#ifndef DBHATCH_H
#define DBHATCH_H


#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbEntity.h"
#include "GeDoubleArray.h"
#include "GePoint2dArray.h"


class DbHatch : public DbEntity
{
public:

    enum HatchPatternType
    {
        kUserDefined  = 0,
        kPreDefined   = 1,
        kCustomDefined = 2
    };

    enum HatchStyle
    {
        kNormal = 0,
        kOuter  = 1,
        kIgnore = 2
    };

    enum HatchLoopType
    {
        kDefault          = 0,
        kExternal         = 1,
        kPolyline         = 2,
        kDerived          = 4,
        kTextbox          = 8,
        kOutermost        = 0x10
    };

    DbHatch();
    virtual ~DbHatch();
    DB_ENTITY_DECLARE_MEMBERS(DbHatch);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    double elevation() const;
    Acad::ErrorStatus setElevation(double elevation);

    GeVector3d normal() const;
    Acad::ErrorStatus setNormal(const GeVector3d& normal);

    HatchPatternType patternType() const;

    AcString patternName() const;
    Acad::ErrorStatus setPattern(HatchPatternType patType, const char* patName);

    double patternAngle() const;
    Acad::ErrorStatus setPatternAngle(double angle);

    double patternScale() const;
    Acad::ErrorStatus setPatternScale(double scale);

    double patternSpace() const;
    Acad::ErrorStatus setPatternSpace(double space);

    bool patternDouble() const;
    Acad::ErrorStatus setPatternDouble(bool isDouble);

    HatchStyle hatchStyle() const;
    Acad::ErrorStatus setHatchStyle(HatchStyle style);

    bool associative() const;
    Acad::ErrorStatus setAssociative(bool isAssociative);

    bool isSolidFill() const;

    int numLoops() const;
    int loopTypeAt(int loopIndex) const;
    Acad::ErrorStatus getLoopAt(int loopIndex, GePoint2dArray& vertices, GeDoubleArray& bulges) const;
    Acad::ErrorStatus appendLoop(int loopType, const GePoint2dArray& vertices, const GeDoubleArray& bulges);
    Acad::ErrorStatus removeLoopAt(int loopIndex);

    int numSeedPoints() const;
    GePoint2d getSeedPointAt(int index) const;
    Acad::ErrorStatus appendSeedPoint(const GePoint2d& point);
};



#endif
