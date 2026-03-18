#ifndef DBRADIALDIMENSIONLARGE_H
#define DBRADIALDIMENSIONLARGE_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbRadialDimensionLarge : public DbDimension
{
public:
    // Default constructor
    DbRadialDimensionLarge();
    // Primary constructor
    DbRadialDimensionLarge(
        const GePoint3d& center,
        const GePoint3d& chordPoint,
        const GePoint3d& overrideCenter,
        const GePoint3d& jogPoint,
        double             jogAngle);
    DbRadialDimensionLarge(
        const GePoint3d& center,
        const GePoint3d& chordPoint,
        const GePoint3d& overrideCenter,
        const GePoint3d& jogPoint,
        double             jogAngle,
        const ACHAR* dimText,
        DbObjectId       dimStyle = DbObjectId::kNull);
    // Default destructor
    ~DbRadialDimensionLarge();
    DB_ENTITY_DECLARE_MEMBERS(DbRadialDimensionLarge);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;


    // Query method which returns the center point of the arc dimensioned by the jogged radius dimension.    
    GePoint3d         center() const;
    // Set method which sets the center point of the arc dimensioned by the jogged radius dimension.        
    Acad::ErrorStatus   setCenter(const GePoint3d& centerPoint);

    // Query method which returns the chord point on the arc dimensioned by the jogged radius dimension.    
    GePoint3d         chordPoint() const;
    // Set method which sets the chord point on the arc dimensioned by the jogged radius dimension.        
    Acad::ErrorStatus   setChordPoint(const GePoint3d& chordPoint);

    // Query method which returns the overidden center point used by the jogged radius dimension.    
    GePoint3d         overrideCenter() const;
    // Set method which sets the overidden center point to use for the jogged radius dimension.        
    Acad::ErrorStatus   setOverrideCenter(const GePoint3d& overrideCenterPoint);

    // Query method which returns the jog point used by the jogged radius dimension.    
    GePoint3d         jogPoint() const;
    // Set method which sets the jog point used by the jogged radius dimension.        
    Acad::ErrorStatus   setJogPoint(const GePoint3d& jogPoint);

    // Query method which returns the jog angle used by the jogged radius dimension.    
    double              jogAngle() const;
    // Set method which sets the jog angle used by the jogged radius dimension.        
    Acad::ErrorStatus   setJogAngle(double jogAngle);

    // Used exclusively by property palette
    // This function is required because once the jogged radius dimension's defining points are updated, all the other defining points need to be updated too.
    Acad::ErrorStatus   setOverrideCenterPP(const GePoint3d& overrideCenterPointPP);
    // Used exclusively by property palette
    // This function is required because once the jogged radius dimension's defining points are updated, all the other defining points need to be updated too.    
    Acad::ErrorStatus   setJogPointPP(const GePoint3d& jogPointPP);
    // Used exclusively by property palette
    // This function is required because once the jogged radius dimension's defining points are updated, all the other defining points need to be updated too.    
    Acad::ErrorStatus   setTextPositionPP(const GePoint3d& textPointPP);

    // Radial large dimension extension arc
    double extArcStartAngle() const;
    Acad::ErrorStatus setExtArcStartAngle(double newAngle);

    double extArcEndAngle() const;
    Acad::ErrorStatus setExtArcEndAngle(double newAngle);

};

#endif
