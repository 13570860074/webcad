#ifndef DBVIEWPORTTABLERECORD_H
#define DBVIEWPORTTABLERECORD_H

#include "Db.h"
#include "GeMatrix3d.h"
#include "DbAbstractViewTableRecord.h"

class DbViewportTableRecord : public DbAbstractViewTableRecord
{
public:
    DbViewportTableRecord();
    virtual ~DbViewportTableRecord();
    DB_DECLARE_MEMBERS(DbViewportTableRecord);

public:
    typedef DbViewportTable TableType;

    virtual void close();

    Int16 number() const;

    GePoint2d lowerLeftCorner() const;
    void setLowerLeftCorner(const GePoint2d &pt);

    GePoint2d upperRightCorner() const;
    void setUpperRightCorner(const GePoint2d &pt);

    bool ucsFollowMode() const;
    void setUcsFollowMode(bool enabled);

    UInt16 circleSides() const;
    void setCircleSides(UInt16 circleSides);

    bool fastZoomsEnabled() const;
    void setFastZoomsEnabled(bool enabled);

    bool iconEnabled() const;
    void setIconEnabled(bool enabled);

    bool iconAtOrigin() const;
    void setIconAtOrigin(bool atOrigin);

    bool gridEnabled() const;
    void setGridEnabled(bool enabled);

    GePoint2d gridIncrements() const;
    void setGridIncrements(const GePoint2d &base);

    bool snapEnabled() const;
    void setSnapEnabled(bool enabled);

    bool isometricSnapEnabled() const;
    void setIsometricSnapEnabled(bool enabled);

    Int16 snapPair() const;
    void setSnapPair(Int16 pairType);

    double snapAngle() const;
    void setSnapAngle(double angle);

    GePoint2d snapBase() const;
    void setSnapBase(const GePoint2d &base);

    GePoint2d snapIncrements() const;
    void setSnapIncrements(const GePoint2d &base);

    bool isUcsSavedWithViewport() const;
    void setUcsPerViewport(bool ucsvp);
};

#endif