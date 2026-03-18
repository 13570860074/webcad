#ifndef DBARCDIMENSION_H
#define DBARCDIMENSION_H

#include "Db.h"
#include "GeTol.h"
#include "GeGbl.h"
#include "DbDimension.h"


class DbArcDimension : public DbDimension
{
public:
    // Default constructor
    DbArcDimension();
    // Primary constructor
    DbArcDimension(
        const GePoint3d& centerPoint,
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& arcPoint);
    DbArcDimension(
        const GePoint3d& centerPoint,
        const GePoint3d& xLine1Point,
        const GePoint3d& xLine2Point,
        const GePoint3d& arcPoint,
        const ACHAR* dimText,
        DbObjectId dimStyle = DbObjectId::kNull);
    // Default destructor
    ~DbArcDimension();
    DB_ENTITY_DECLARE_MEMBERS(DbArcDimension);
    DB_ENTITY_DECLARE_OVERRIDE_METHOD;

    // Query method which returns a point on the arc length dimension's dimension arc.
    GePoint3d         arcPoint() const;
    // Set method to set the point which the arc length dimension's dimension arc passes through.
    Acad::ErrorStatus   setArcPoint(const GePoint3d& arcPt);

    // Query method which returns the start point for the arc length dimension's first extension line.
    GePoint3d         xLine1Point() const;
    // Set method which sets the start point for the arc length dimension's first extension line.        
    Acad::ErrorStatus   setXLine1Point(const GePoint3d& xLine1Pt);

    // Query method which returns the start point for the arc length dimension's second extension line.
    GePoint3d         xLine2Point() const;
    // Set method which sets the start point for the arc length dimension's second extension line.    
    Acad::ErrorStatus   setXLine2Point(const GePoint3d& xLine2Pt);

    // Query method which returns the center point of the arc dimensioned by the arc length dimension.
    GePoint3d         centerPoint() const;
    // Set method which sets the center point of the arc dimensioned by the arc length dimension.    
    Acad::ErrorStatus   setCenterPoint(const GePoint3d& ctrPt);

    // Query method which returns true if this arc length dimension was specified using two points along the arc, 
    // false if the arc length dimension dimensions the entire arc segment.
    bool                isPartial() const;
    // Set method if passed true, the arc length dimension was specified using the entire arc segment, 
    // if passed false the arc length dimension arc length dimension was specified using two points along the arc.
    Acad::ErrorStatus   setIsPartial(const bool& partial);

    // Query method which returns the parameter of the arc being dimensioned cooresponding to the arc length dimension's first definiton point.
    double              arcStartParam() const;
    // Set method which sets the parameter of the arc being dimensioned cooresponding to the arc length dimension's first definiton point.        
    Acad::ErrorStatus   setArcStartParam(const double& arcParam);

    // Query method which returns the parameter of the arc being dimensioned cooresponding to the arc length dimension's second definiton point.
    double              arcEndParam() const;
    // Set method which sets the parameter of the arc being dimensioned cooresponding to the arc length dimension's second definiton point.    
    Acad::ErrorStatus   setArcEndParam(const double& arcParam);

    // Query method which returns true if this arc length dimension has an extra leader drawn to resolve ambiguity, false if the arc length dimension has no extra leader drawn.
    bool                hasLeader() const;
    // Set method if passed true, the arc length dimension has an extra leader drawn to resolve ambiguity, if passed false the arc length dimension has no extra leader drawn.    
    Acad::ErrorStatus   setHasLeader(const bool& leaderVal);

    // Query method which returns the start point for the arc length dimension's extra leader, if drawn.
    GePoint3d         leader1Point() const;
    // Set method which sets the start point for the arc length dimension's extra leader, if drawn.        
    Acad::ErrorStatus   setLeader1Point(const GePoint3d& ldr1Pt);

    // Query method which returns the end point for the arc length dimension's extra leader, if drawn.
    GePoint3d         leader2Point() const;
    // Set method which sets the end point for the arc length dimension's extra leader, if drawn.    
    Acad::ErrorStatus   setLeader2Point(const GePoint3d& ldr2Pt);

    // Query method which returns the type of symbol used within the arc length dimension's text string:
    // 0 - Arc symbol precedes text, 1 - Arc symbol is above text, 2 - No arc symbol is used.
    int arcSymbolType() const;
    // Set method which sets the type of symbol to use within the arc length dimension's text string:
    // 0 - Arc symbol precedes text, 1 - Arc symbol is above text, 2 - No arc symbol is used.
    // This overrides the setting of this value in the dimension's style.
    Acad::ErrorStatus setArcSymbolType(const int symbol);

};

#endif
